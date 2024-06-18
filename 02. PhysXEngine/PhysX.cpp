#include "pch.h"
#include "PhysX.h"

#include "iostream"
#include "PhysicsSimulationEventCallback.h"
#include "ActorUserData.h"
#include "CharactorController.h"
#include "SoftBody.h"
#include "MeshGenerator.h"

#include <cassert>
//#include <physx/foundation/PxPreprocessor.h>		// ��ó���� 
//#include <cudamanager/PxCudaContext.h>
#include <extensions/PxParticleExt.h>
//#include <cudamanager/PxCudaContextManager.h>
//#include <geometry/PxTetrahedronMesh.h>
//#include <gpu/PxGpu.h>
//#include <extensions/PxTetMakerExt.h>
#include <extensions/PxSoftBodyExt.h>
#include <extensions/PxRemeshingExt.h>
#include "ClothPhysics.h"


namespace PhysicsEngine
{
	enum ObjectType
	{
		OBJECT_TYPE_A = (1 << 0),
		OBJECT_TYPE_B = (1 << 1),
		OBJECT_TYPE_C = (1 << 2),
	};

	physx::PxFilterFlags CustomSimulationFilterShader(
		physx::PxFilterObjectAttributes attributes0, physx::PxFilterData filterData0,
		physx::PxFilterObjectAttributes attributes1, physx::PxFilterData filterData1,
		physx::PxPairFlags& pairFlags, const void* constantBlock, physx::PxU32 constantBlockSize)
	{
		//
		// �ֿ� ���� CCD�� Ȱ��ȭ�ϰ� �ʱ� �� CCD ����ó�� ���� ����ó ������ ��û�մϴ�.
		// ���� ������ ������ �����ϰ� �����ڿ��� ������ �����մϴ�
		// ������ �� ��� ���մϴ�.
		//

		// ���� ���̴� ���� ( Ʈ���� )
		if (physx::PxFilterObjectIsTrigger(attributes0) || physx::PxFilterObjectIsTrigger(attributes1))
		{
			pairFlags = physx::PxPairFlag::eTRIGGER_DEFAULT
				| physx::PxPairFlag::eNOTIFY_TOUCH_CCD
				| physx::PxPairFlag::eNOTIFY_THRESHOLD_FORCE_FOUND
				| physx::PxPairFlag::eNOTIFY_THRESHOLD_FORCE_LOST
				| physx::PxPairFlag::eNOTIFY_THRESHOLD_FORCE_PERSISTS;

			return physx::PxFilterFlag::eDEFAULT;
		}

		// ���� ������ �浹 üũ ( �ùķ��̼� )
		if (((filterData0.word1 & filterData1.word0) > 0) && ((filterData1.word1 & filterData0.word0) > 0))
		{
			pairFlags = physx::PxPairFlag::eCONTACT_DEFAULT
				| physx::PxPairFlag::eDETECT_CCD_CONTACT
				| physx::PxPairFlag::eNOTIFY_TOUCH_CCD
				| physx::PxPairFlag::eNOTIFY_THRESHOLD_FORCE_FOUND
				| physx::PxPairFlag::eNOTIFY_THRESHOLD_FORCE_LOST
				| physx::PxPairFlag::eNOTIFY_THRESHOLD_FORCE_PERSISTS
				| physx::PxPairFlag::eNOTIFY_CONTACT_POINTS
				| physx::PxPairFlag::eCONTACT_EVENT_POSE;
			return physx::PxFilterFlag::eDEFAULT;
		}
		else
		{
			pairFlags = physx::PxPairFlag::eCONTACT_DEFAULT
				| physx::PxPairFlag::eDETECT_CCD_CONTACT
				| physx::PxPairFlag::eNOTIFY_TOUCH_CCD
				| physx::PxPairFlag::eNOTIFY_THRESHOLD_FORCE_FOUND
				| physx::PxPairFlag::eNOTIFY_THRESHOLD_FORCE_LOST
				| physx::PxPairFlag::eNOTIFY_THRESHOLD_FORCE_PERSISTS
				| physx::PxPairFlag::eNOTIFY_CONTACT_POINTS
				| physx::PxPairFlag::eCONTACT_EVENT_POSE;
			return physx::PxFilterFlag::eDEFAULT;

			//pairFlags &= ~physx::PxPairFlag::eCONTACT_DEFAULT; // �浹 �ൿ �����
			//return physx::PxFilterFlag::eSUPPRESS;
		}
	}

	PhysX::PhysX()
	{

	}

	PhysX::~PhysX()
	{

	}

	physx::PxFilterFlags myFilterShader(
		physx::PxFilterObjectAttributes attributes0, physx::PxFilterData filterData0,
		physx::PxFilterObjectAttributes attributes1, physx::PxFilterData filterData1,
		physx::PxPairFlags& pairFlags, const void* constantBlock, physx::PxU32 constantBlockSize)
	{
		// ���� ���̴� ����
		if (physx::PxFilterObjectIsTrigger(attributes0) || physx::PxFilterObjectIsTrigger(attributes1))
		{
			pairFlags = physx::PxPairFlag::eTRIGGER_DEFAULT;
			return physx::PxFilterFlag::eDEFAULT;
		}

		pairFlags = physx::PxPairFlag::eCONTACT_DEFAULT;
		pairFlags |= physx::PxPairFlag::eNOTIFY_TOUCH_FOUND | physx::PxPairFlag::eNOTIFY_TOUCH_LOST;

		return physx::PxFilterFlag::eDEFAULT;
	}


	void PhysX::Init(ID3D11Device* device)
	{
		// PhysX Foundation�� �����ϰ� �ʱ�ȭ�մϴ�.
		m_Foundation = PxCreateFoundation(PX_PHYSICS_VERSION, m_DefaultAllocatorCallback, m_DefaultErrorCallback);

		// Foundation�� ���������� �����Ǿ����� Ȯ���մϴ�.
		if (!m_Foundation)
		{
			throw("PxCreateFoundation failed!"); // Foundation ������ ������ ��� ���ܸ� throw�մϴ�.
		}

		// PhysX Visual Debugger (PVD)�� �����ϰ� �����մϴ�.
		m_Pvd = physx::PxCreatePvd(*m_Foundation); // Foundation�� ����Ͽ� PVD�� �����մϴ�.
		physx::PxPvdTransport* transport = physx::PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10); // PVD�� ����� Ʈ������Ʈ�� �����մϴ�.
		m_Pvd->connect(*transport, physx::PxPvdInstrumentationFlag::eALL); // PVD�� Ʈ������Ʈ�� �����մϴ�.

		// ���� �ùķ��̼��� ��� ���� �������� �����մϴ�.
		m_ToleranceScale.length = 1; // ���� ��� ���� �������� �����մϴ�.
		m_ToleranceScale.speed = 1000; // �ӵ� ��� ���� �������� �����մϴ�.

		// PhysX Physics�� �����ϰ� �ʱ�ȭ�մϴ�.
		m_Physics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_Foundation, m_ToleranceScale, true, m_Pvd); // Physics�� �����մϴ�..

		if (PxGetSuggestedCudaDeviceOrdinal(m_Foundation->getErrorCallback()) >= 0)
		{
			// initialize CUDA
			physx::PxCudaContextManagerDesc cudaContextManagerDesc;
			m_CudaContextManager = PxCreateCudaContextManager(*m_Foundation, cudaContextManagerDesc, PxGetProfilerCallback());
			if (m_CudaContextManager && !m_CudaContextManager->contextIsValid())
			{
				m_CudaContextManager->release();
				m_CudaContextManager = NULL;
			}
		}
		if (m_CudaContextManager == NULL)
		{
			PxGetFoundation().error(physx::PxErrorCode::eINVALID_OPERATION, PX_FL, "Failed to initialize CUDA!\n");
		}

		// PhysX �ùķ��̼��� ���� Scene�� �����մϴ�.
		physx::PxSceneDesc sceneDesc(m_Physics->getTolerancesScale()); // Scene�� ������ �� �������� ��� ���� �������� �����մϴ�.

		sceneDesc.gravity = physx::PxVec3(0.f, -10.f, 0.f); // �߷��� �����մϴ�.

		// CPU ����ó�� �����ϰ� �����մϴ�.
		m_Dispatcher = physx::PxDefaultCpuDispatcherCreate(2); // CPU ����ó�� �����մϴ�.

		m_MyEventCallback = new PhysicsSimulationEventCallback;
		physx::PxPairFlags pairFlags = physx::PxPairFlags();

		// Scene �����ڿ� CPU ����ó�� ���� ���̴��� �����մϴ�.
		// �߷��� �����մϴ�.
		sceneDesc.cpuDispatcher = m_Dispatcher; // Scene �����ڿ� CPU ����ó�� �����մϴ�.
		sceneDesc.filterShader = CustomSimulationFilterShader;
		sceneDesc.simulationEventCallback = m_MyEventCallback;		// Ŭ���� ���
		sceneDesc.cudaContextManager = m_CudaContextManager;
		sceneDesc.staticStructure = physx::PxPruningStructureType::eDYNAMIC_AABB_TREE;
		sceneDesc.flags |= physx::PxSceneFlag::eENABLE_PCM;
		sceneDesc.flags |= physx::PxSceneFlag::eENABLE_GPU_DYNAMICS;
		sceneDesc.broadPhaseType = physx::PxBroadPhaseType::eGPU;
		sceneDesc.solverType = physx::PxSolverType::eTGS;

		// PhysX Physics���� Scene�� �����մϴ�.
		m_Scene = m_Physics->createScene(sceneDesc); // Scene�� �����մϴ�.
		assert(m_Scene);

		m_Material = m_Physics->createMaterial(5.f, 5.f, 20.f);

		// 
		m_pvdClient = m_Scene->getScenePvdClient();
		if (m_pvdClient)
		{
			m_pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
			m_pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
			m_pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
		}

		CreateActor();
		CreateCharactorController();
		//CreateSoftBodies();
		//CreateArticulation();

		// Setup Cloth
		//const physx::PxReal totalClothMass = 2.f;

		//physx::PxU32 numPointsX = 30;
		//physx::PxU32 numPointsZ = 30;
		//physx::PxReal particleSpacing = 3.f;

		//CreateCloth(numPointsX, numPointsZ, physx::PxVec3(-0.5f * numPointsX * particleSpacing, 550.f, -0.5f * numPointsZ * particleSpacing), particleSpacing, totalClothMass);

		CreateCloth();
		//CreateCudaCloth();
	}

	void PhysX::Update(float elapsedTime)
	{
		m_CharactorController->Update(elapsedTime);

		m_Scene->simulate(elapsedTime);
		m_Scene->fetchResults(true);

		for (physx::PxU32 i = 0; i < m_SoftBodies.size(); i++)
		{
			SoftBody* sb = &m_SoftBodies[i];
			sb->copyDeformedVerticesFromGPU();
		}

		//std::cout << m_Scene->getTimestamp() << std::endl;

		//m_Scene->getVisualizationParameter(physx::PxVisualizationParameter::eCOLLISION_EDGES);

		//physx::PxRaycastBuffer hitbuffer;
		//bool isBlock = m_Scene->raycast(physx::PxVec3(0.f, 0.f, 10.f), physx::PxVec3(0.f, 1.f, 0.f), 100.f, hitbuffer);
		//if (isBlock)
		//{
		//	physx::PxRaycastHit& hit = hitbuffer.block;
		//	
		//	physx::PxVec3 hitPosition = hit.position;

		//	float distance = hit.distance;

		//	std::cout << "rayCast" << std::endl;
		//}

		if (m_ClothPhysics)
		{
			m_ClothPhysics->GetClothBuffer();
		}
	}

#pragma region Actor
	void PhysX::CreateActor()
	{
		// ������Ʈ Ÿ�Կ� ���� ���� ������ ����
		physx::PxFilterData filterDataA;
		filterDataA.word0 = OBJECT_TYPE_A;
		filterDataA.word1 = OBJECT_TYPE_A | OBJECT_TYPE_B | OBJECT_TYPE_C;

		physx::PxFilterData filterDataB;
		filterDataB.word0 = OBJECT_TYPE_B;
		filterDataB.word1 = OBJECT_TYPE_A | OBJECT_TYPE_B | OBJECT_TYPE_C;

		physx::PxFilterData filterDataC;
		filterDataC.word0 = OBJECT_TYPE_C;
		filterDataC.word1 = OBJECT_TYPE_A | OBJECT_TYPE_B | OBJECT_TYPE_C;

		// �ùķ��̼� ����
		ActorUserData* data1 = new ActorUserData(ActorType::TILE);
		ActorUserData* data2 = new ActorUserData(ActorType::MONSTER);
		m_groundPlane = physx::PxCreatePlane(*m_Physics, physx::PxPlane(0, 1, 0, 1), *m_Material);
		physx::PxShape* shape;
		m_groundPlane->getShapes(&shape, sizeof(physx::PxShape));
		shape->setSimulationFilterData(filterDataC);
		m_groundPlane->userData = data1;
		m_Scene->addActor(*m_groundPlane);

		// ������ �޽� ����
		physx::PxConvexMeshDesc convexdesc;
		convexdesc.points.count = m_ModelVertices.size();
		convexdesc.points.stride = sizeof(physx::PxVec3);
		convexdesc.vertexLimit = 255;
		convexdesc.polygonLimit = 10;
		convexdesc.points.data = m_ModelVertices.data();
		convexdesc.flags = physx::PxConvexFlag::eCOMPUTE_CONVEX;

		physx::PxTolerancesScale scale;
		physx::PxCookingParams params(scale);
		params.buildGPUData = true;

		physx::PxDefaultMemoryOutputStream buf;
		physx::PxConvexMeshCookingResult::Enum result;
		assert(PxCookConvexMesh(params, convexdesc, buf, &result));
		physx::PxDefaultMemoryInputData input(buf.getData(), buf.getSize());
		physx::PxConvexMesh* convexMesh = m_Physics->createConvexMesh(input);


		float halfExtent = 5.f;
		physx::PxU32 size = 1;

		const physx::PxTransform t(physx::PxVec3(0));
		for (physx::PxU32 i = 0; i < size; i++)
		{
			for (physx::PxU32 j = 0; j < size - i; j++)
			{
				ActorUserData* data = new ActorUserData(ActorType::PLAYER);
				physx::PxShape* Shape = m_Physics->createShape(physx::PxBoxGeometry(physx::PxVec3(halfExtent / 2, halfExtent / 2, halfExtent / 2)), *m_Material);
				physx::PxTransform localTm(physx::PxVec3(physx::PxReal(j * 2) - physx::PxReal(size - i), physx::PxReal(i * 2 + 1), 0) * halfExtent);
				physx::PxRigidDynamic* body = m_Physics->createRigidDynamic(t.transform(localTm));
				Shape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, true);
				Shape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, false);
				body->userData = data;
				Shape->setContactOffset(0.02f);
				Shape->setRestOffset(0.f);
				Shape->setSimulationFilterData(filterDataB);
				body->attachShape(*Shape);
				assert(physx::PxRigidBodyExt::updateMassAndInertia(*body, 100.f));

				m_Scene->addActor(*body);
				m_Shapes.push_back(Shape);
				m_Bodies.push_back(body);

			}
		}

		physx::PxShape* Shape = m_Physics->createShape(physx::PxConvexMeshGeometry(convexMesh), *m_Material);
		physx::PxTransform localTm(physx::PxVec3(physx::PxReal(3), physx::PxReal(250), 0), physx::PxQuat(1.f, 0.1f, 0.f, 0.f));
		physx::PxRigidDynamic* body = m_Physics->createRigidDynamic(t.transform(localTm));
		Shape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, true);
		Shape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, false);
		body->userData = data2;
		Shape->setContactOffset(0.02f);
		Shape->setRestOffset(0.f);
		Shape->setSimulationFilterData(filterDataB);
		body->attachShape(*Shape);
		Shape->release();
		body->detachShape(*Shape);

		physx::PxShape* newShape = m_Physics->createShape(physx::PxConvexMeshGeometry(convexMesh), *m_Material);
		newShape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, true);
		newShape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, false);
		newShape->setContactOffset(1.1f);
		newShape->setSimulationFilterData(filterDataB);
		body->attachShape(*newShape);
		m_Shapes.push_back(newShape);
		m_Bodies.push_back(body);

		body->setMassSpaceInertiaTensor(physx::PxVec3(
			1.f / (body->getCMassLocalPose().p.x * body->getCMassLocalPose().p.x),
			1.f / (body->getCMassLocalPose().p.y * body->getCMassLocalPose().p.y),
			1.f / (body->getCMassLocalPose().p.z * body->getCMassLocalPose().p.z)
		));

		physx::PxRigidBodyExt::updateMassAndInertia(*body, 100.f);
		m_Scene->addActor(*body);

		physx::PxRaycastBuffer hitbuffer;
		physx::PxU32 hitCount = m_Scene->raycast(physx::PxVec3(0.f, 0.f, 0.f), physx::PxVec3(0.f, 1.f, 0.f), 10.f, hitbuffer);
		hitbuffer.hasAnyHits();
	}
#pragma endregion

#pragma region Controller
	void PhysX::CreateCharactorController()
	{
		m_ControllerManager = PxCreateControllerManager(*m_Scene);
		m_CharactorController = std::make_shared<CharactorController>();
		m_CharactorController->Initialzie(m_Material, m_ControllerManager);
		m_ControllerManager->setDebugRenderingFlags(physx::PxControllerDebugRenderFlag::eALL); \
	}
#pragma endregion

#pragma region Articulation
	void PhysX::CreateArticulation()
	{
		physx::PxArticulationReducedCoordinate* articulation = m_Physics->createArticulationReducedCoordinate();

		articulation->setArticulationFlag(physx::PxArticulationFlag::eFIX_BASE, true);
		articulation->setArticulationFlag(physx::PxArticulationFlag::eDISABLE_SELF_COLLISION, true);
		articulation->setSolverIterationCounts(4);
		articulation->setMaxCOMLinearVelocity(10.f);

		physx::PxArticulationLink* rootLink = articulation->createLink(nullptr, physx::PxTransform(0.f, 50.f, 0.f));
		//physx::PxRigidActorExt::createExclusiveShape(*rootLink, physx::PxSphereGeometry(5.f), *m_Material);
		//physx::PxRigidBodyExt::updateMassAndInertia(*rootLink, 10.f);

		physx::PxArticulationLink* link = articulation->createLink(rootLink, physx::PxTransform(physx::PxVec3(0.f, 0.f, 0.f)));
		if (!physx::PxRigidActorExt::createExclusiveShape(*link, physx::PxSphereGeometry(5.f), *m_Material))
			std::cout << "false" << std::endl;
		physx::PxRigidBodyExt::updateMassAndInertia(*link, 10.f);

		physx::PxArticulationLink* link1 = articulation->createLink(link, physx::PxTransform(physx::PxVec3(5.f, 0.f, 0.f)));
		if (!physx::PxRigidActorExt::createExclusiveShape(*link1, physx::PxSphereGeometry(2.f), *m_Material))
			std::cout << "false" << std::endl;
		physx::PxRigidBodyExt::updateMassAndInertia(*link1, 10.f);

		physx::PxArticulationLink* link2 = articulation->createLink(link1, physx::PxTransform(physx::PxVec3(0.f, 0.f, 0.f)));
		if (!physx::PxRigidActorExt::createExclusiveShape(*link2, physx::PxSphereGeometry(2.f), *m_Material))
			std::cout << "false" << std::endl;
		physx::PxRigidBodyExt::updateMassAndInertia(*link2, 10.f);

		//m_Bodies.push_back(rootLink);
		m_Bodies.push_back(link);
		m_Bodies.push_back(link1);
		m_Bodies.push_back(link2);

		physx::PxFilterData filterData;
		filterData.word0 = OBJECT_TYPE_A;
		filterData.word1 = OBJECT_TYPE_A | OBJECT_TYPE_B | OBJECT_TYPE_C;

		physx::PxShape* shape;
		link->getShapes(&shape, 1);
		shape->setQueryFilterData(filterData);
		shape->setSimulationFilterData(filterData);
		link1->getShapes(&shape, 1);
		shape->setQueryFilterData(filterData);
		shape->setSimulationFilterData(filterData);
		link2->getShapes(&shape, 1);
		shape->setQueryFilterData(filterData);
		shape->setSimulationFilterData(filterData);

		physx::PxArticulationJointReducedCoordinate* joint = link->getInboundJoint();
		joint->setParentPose(physx::PxTransform(physx::PxVec3(0.f, 0.f, 0.f)));
		joint->setChildPose(physx::PxTransform(physx::PxVec3(0.f, 0.f, 0.f)));

		joint->setJointType(physx::PxArticulationJointType::eSPHERICAL);
		joint->setMotion(physx::PxArticulationAxis::eSWING1, physx::PxArticulationMotion::eLOCKED);
		joint->setMotion(physx::PxArticulationAxis::eSWING2, physx::PxArticulationMotion::eLOCKED);
		joint->setMotion(physx::PxArticulationAxis::eTWIST, physx::PxArticulationMotion::eLOCKED);


		physx::PxArticulationJointReducedCoordinate* joint1 = link1->getInboundJoint();
		joint1->setParentPose(physx::PxTransform(physx::PxVec3(5.f, 0.f, 0.f)));
		joint1->setChildPose(physx::PxTransform(physx::PxVec3(-5.f, 0.f, 0.f)));

		joint1->setJointType(physx::PxArticulationJointType::eSPHERICAL);
		joint1->setMotion(physx::PxArticulationAxis::eSWING1, physx::PxArticulationMotion::eFREE);
		joint1->setMotion(physx::PxArticulationAxis::eSWING2, physx::PxArticulationMotion::eFREE);
		joint1->setMotion(physx::PxArticulationAxis::eTWIST, physx::PxArticulationMotion::eFREE);

		physx::PxArticulationJointReducedCoordinate* joint2 = link2->getInboundJoint();
		joint2->setParentPose(physx::PxTransform(physx::PxVec3(10.f, 0.f, 0.f)));
		joint2->setChildPose(physx::PxTransform(physx::PxVec3(-10.f, 0.f, 0.f)));

		joint2->setJointType(physx::PxArticulationJointType::eSPHERICAL);
		joint2->setMotion(physx::PxArticulationAxis::eSWING1, physx::PxArticulationMotion::eFREE);
		joint2->setMotion(physx::PxArticulationAxis::eSWING2, physx::PxArticulationMotion::eFREE);
		joint2->setMotion(physx::PxArticulationAxis::eTWIST, physx::PxArticulationMotion::eFREE);

		physx::PxArticulationLimit limits;
		//limits.low = -physx::PxPiDivFour;    // in rad for a rotational motion
		//limits.high = physx::PxPiDivFour;
		//limits.low = -1.f;    // in rad for a rotational motion
		//limits.high = 1.f;
		//joint->setLimitParams(physx::PxArticulationAxis::eSWING2, limits);
		//joint->setLimitParams(physx::PxArticulationAxis::eTWIST, limits);

		physx::PxArticulationDrive posDrive;
		posDrive.stiffness = 100.f;
		posDrive.damping = 0.f;
		posDrive.maxForce = 1000.f;
		posDrive.driveType = physx::PxArticulationDriveType::eFORCE;
		joint->setDriveParams(physx::PxArticulationAxis::eSWING2, posDrive);
		joint1->setDriveParams(physx::PxArticulationAxis::eSWING2, posDrive);
		joint2->setDriveParams(physx::PxArticulationAxis::eSWING2, posDrive);

		// Create fixed tendon if needed

		m_Scene->addArticulation(*articulation);
	}
#pragma endregion

#pragma region Cloth
	// �޼� ��ǥ��(DirectX11)���� ������ ��ǥ��(PhysX)�� ��ȯ�ϱ�
	void CopyDirectXMatrixToPxTransform(const DirectX::SimpleMath::Matrix& dxMatrix, physx::PxTransform& pxTransform)
	{
		DirectX::XMFLOAT4X4 dxMatrixData;
		DirectX::XMStoreFloat4x4(&dxMatrixData, dxMatrix);

		pxTransform.p.x = dxMatrixData._41;
		pxTransform.p.y = dxMatrixData._42;
		pxTransform.p.z = -dxMatrixData._43; // z�� ���� ����

		// ȸ�� �������� z�� ���� ���� ����
		DirectX::XMVECTOR quaternion = DirectX::XMQuaternionRotationMatrix(dxMatrix);
		DirectX::XMVECTOR flippedZ = DirectX::XMQuaternionRotationAxis(DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), DirectX::XM_PI);
		quaternion = DirectX::XMQuaternionMultiply(quaternion, flippedZ);

		pxTransform.q.x = DirectX::XMVectorGetX(quaternion);
		pxTransform.q.y = DirectX::XMVectorGetY(quaternion);
		pxTransform.q.z = DirectX::XMVectorGetZ(quaternion);
		pxTransform.q.w = DirectX::XMVectorGetW(quaternion);
	}

	physx::PxVec4 multiply(const physx::PxMat44& mat, const physx::PxVec4& vec)  // 4x4 ��İ� PxVec4�� ���ϴ� �Լ�
	{
		physx::PxVec4 result;
		result.x = mat(0, 0) * vec.x + mat(1, 0) * vec.y + mat(2, 0) * vec.z + mat(3, 0) * vec.w;
		result.y = mat(0, 1) * vec.x + mat(1, 1) * vec.y + mat(2, 1) * vec.z + mat(3, 1) * vec.w;
		result.z = mat(0, 2) * vec.x + mat(1, 2) * vec.y + mat(2, 2) * vec.z + mat(3, 2) * vec.w;
		result.w = mat(0, 3) * vec.x + mat(1, 3) * vec.y + mat(2, 3) * vec.z + mat(3, 3) * vec.w;
		return result;
	}

	// �� ���� 2���� �ε����� 1���� �ε����� ��ȯ�ϴ� �Լ�
	// x, y: 2���� �ε���
	// numY: �� ��° ������ ũ��
	PX_FORCE_INLINE physx::PxU32 id(physx::PxU32 x, physx::PxU32 y, physx::PxU32 numY)
	{
		return x * numY + y;
	}

	void PhysX::CreateCloth()
	{
		PhysicsClothInfo info;
		std::vector<DirectX::SimpleMath::Vector3> vertices;
		std::vector<DirectX::SimpleMath::Vector2> uv;
		vertices.resize(m_ModelVertices.size());
		uv.resize(m_ModelVertices.size());
		for (int i = 0; i < m_ModelVertices.size(); i++)
		{
			vertices[i].x = m_ModelVertices[i].x;
			vertices[i].y = m_ModelVertices[i].y;
			vertices[i].z = m_ModelVertices[i].z;
			uv[i].x = m_ModelUV[i].x;
			uv[i].y = m_ModelUV[i].y;
		}
		info.vertices = vertices.data();
		info.uv = uv.data();
		info.vertexSize = vertices.size();
		info.indices = m_ModelIndices.data();
		info.indexSize = m_ModelIndices.size();
		info.id = 100;
		info.layerNumber = 1;
		info.worldTransform = DirectX::SimpleMath::Matrix::CreateTranslation(0.f, 100.f, 0.f);

		m_ClothPhysics = std::make_shared<ClothPhysics>(info.id, info.layerNumber);
		m_ClothPhysics->Initialize(info, m_Physics, m_Scene, m_CudaContextManager);
	}

	void PhysX::CreateCudaCloth()
	{
		PhysicsClothInfo info;
		std::vector<DirectX::SimpleMath::Vector3> vertices;
		std::vector<DirectX::SimpleMath::Vector2> uv;
		vertices.resize(m_ModelVertices.size());
		uv.resize(m_ModelVertices.size());
		for (int i = 0; i < m_ModelVertices.size(); i++)
		{
			vertices[i].x = m_ModelVertices[i].x;
			vertices[i].y = m_ModelVertices[i].y;
			vertices[i].z = m_ModelVertices[i].z;
			uv[i].x = m_ModelUV[i].x;
			uv[i].y = m_ModelUV[i].y;
		}
		info.vertices = vertices.data();
		info.uv = uv.data();
		info.vertexSize = vertices.size();
		info.indices = m_ModelIndices.data();
		info.indexSize = m_ModelIndices.size();
		info.id = 100;
		info.layerNumber = 1;
		info.worldTransform = DirectX::SimpleMath::Matrix::CreateTranslation(0.f, 100.f, 0.f);

		m_ClothPhysics = std::make_shared<ClothPhysics>(info.id, info.layerNumber);
		m_ClothPhysics->Initialize(info, m_Physics, m_Scene, m_CudaContextManager);
	}

	// õ��(cloth)�� �����ϴ� �Լ�
	// numX, numZ: õ���� ���� �� ���ο� �ش��ϴ� ����(particle)�� ����
	// position: õ���� ���� ��ġ
	// particleSpacing: ���� ���� ����
	// totalClothMass: õ���� �� ����
	void PhysX::CreateCloth(const physx::PxU32 numX, const physx::PxU32 numZ, const physx::PxVec3& position, const physx::PxReal particleSpacing, const physx::PxReal totalClothMass)
	{
		// CUDA ���ؽ�Ʈ �Ŵ��� ��������
		m_CudaContextManager = m_Scene->getCudaContextManager();
		if (m_CudaContextManager == nullptr)
			return;

		// ���� �� ������, �ﰢ���� ���� ���
		const physx::PxU32 numParticles = numX * numZ;	// ���� ����
		const physx::PxU32 numSprings = (numX - 1) * (numZ - 1) * 3 + (numX - 1) + (numZ - 1);	// ���� �ϳ��� �̿��ϴ� ���ڵ鿡 ������ ���� �����µ�, �� ������ ����
		const physx::PxU32 numTriangles = (numX - 1) * (numZ - 1) * 2;	// �ﰢ�� ����

		const physx::PxReal restOffset = particleSpacing - 0.5f;

		const physx::PxReal stretchStiffness = 100.f;
		const physx::PxReal shearStiffness = 100.f;
		const physx::PxReal springDamping = 0.1f;

		// ���(Material) ����
		physx::PxPBDMaterial* defaultMat = m_Physics->createPBDMaterial(0.8f, 0.001f, 1e+7f, 0.001f, 0.5f, 0.005f, 0.05f, 0.f, 0.f, 1.f, 2.f);

		// ���� �ý��� ����
		m_ParticleSystem = m_Physics->createPBDParticleSystem(*m_CudaContextManager);

		// ���� �ý����� ����
		const physx::PxReal particleMass = totalClothMass / numParticles;
		m_ParticleSystem->setRestOffset(restOffset);
		m_ParticleSystem->setContactOffset(restOffset + 0.02f);
		m_ParticleSystem->setParticleContactOffset(restOffset + 0.02f);
		m_ParticleSystem->setSolidRestOffset(restOffset);
		m_ParticleSystem->setFluidRestOffset(0.0f);

		// ���� ���� �ý��� �߰�
		m_Scene->addActor(*m_ParticleSystem);

		// ������ ���¸� �����ϴ� ���� ����
		const physx::PxU32 particlePhase = m_ParticleSystem->createPhase(defaultMat,
			physx::PxParticlePhaseFlags(physx::PxParticlePhaseFlag::eParticlePhaseSelfCollideFilter | physx::PxParticlePhaseFlag::eParticlePhaseSelfCollide));

		physx::ExtGpu::PxParticleClothBufferHelper* clothBuffers = physx::ExtGpu::PxCreateParticleClothBufferHelper(1, numTriangles, numSprings, numParticles, m_CudaContextManager);
		// ����, ������, �ﰢ���� ���¸� �����ϱ� ���� ���� �Ҵ�
		physx::PxU32* phase = m_CudaContextManager->allocPinnedHostBuffer<physx::PxU32>(numParticles);
		physx::PxVec4* positionInvMass = m_CudaContextManager->allocPinnedHostBuffer<physx::PxVec4>(numParticles);
		physx::PxVec4* velocity = m_CudaContextManager->allocPinnedHostBuffer<physx::PxVec4>(numParticles);

		physx::PxReal x = position.x;
		physx::PxReal y = position.y;
		physx::PxReal z = position.z;

		// ������ �� �ﰢ�� ����
		physx::PxArray<physx::PxParticleSpring> springs;
		springs.reserve(numSprings);
		physx::PxArray<physx::PxU32> triangles;
		triangles.reserve(numTriangles * 3);

		for (physx::PxU32 i = 0; i < numX; ++i)
		{
			for (physx::PxU32 j = 0; j < numZ; ++j)
			{
				const physx::PxU32 index = i * numZ + j;

				// ������ ��ġ�� ���� ����
				physx::PxVec4 pos(x, y, z, 1.0f / particleMass);
				phase[index] = particlePhase;
				positionInvMass[index] = pos;
				velocity[index] = physx::PxVec4(0.0f);

				// ������ �߰�
				if (i > 0)
				{
					physx::PxParticleSpring spring = {id(i - 1, j, numZ), id(i, j, numZ), particleSpacing, stretchStiffness, springDamping, 0 };
					springs.pushBack(spring);
				}
				if (j > 0)
				{
					physx::PxParticleSpring spring = { id(i, j - 1, numZ), id(i, j, numZ), particleSpacing, stretchStiffness, springDamping, 0 };
					springs.pushBack(spring);
				}

				// �ﰢ�� �߰�
				if (i > 0 && j > 0)
				{
					physx::PxParticleSpring spring0 = { id(i - 1, j - 1, numZ), id(i, j, numZ), physx::PxSqrt(2.0f) * particleSpacing, shearStiffness, springDamping, 0 };
					//springs.pushBack(spring0);
					physx::PxParticleSpring spring1 = { id(i - 1, j, numZ), id(i, j - 1, numZ), physx::PxSqrt(2.0f) * particleSpacing, shearStiffness, springDamping, 0 };
					springs.pushBack(spring1);

					// �ﰢ���� õ���� �Ʒ��� ������ �� �ٻ����� ���� ���׷� ��꿡 ���˴ϴ�.
					triangles.pushBack(id(i - 1, j - 1, numZ));
					triangles.pushBack(id(i - 1, j, numZ));
					triangles.pushBack(id(i, j - 1, numZ));

					triangles.pushBack(id(i - 1, j, numZ));
					triangles.pushBack(id(i, j - 1, numZ));
					triangles.pushBack(id(i, j, numZ));
				}

				z += particleSpacing;
			}
			z = position.z;
			x += particleSpacing;
		}

		// ������ ������ �� �ﰢ�� ���� ������ �����Ǿ����� Ȯ��
		PX_ASSERT(numSprings == springs.size());
		PX_ASSERT(numTriangles == triangles.size() / 3);

		// õ���� ���ۿ� ������ �߰�
		clothBuffers->addCloth(0.0f, 0.0f, 0.0f, triangles.begin(), numTriangles, springs.begin(), numSprings, positionInvMass, numParticles);

		// ������ ���¸� ��Ÿ���� ���� ����
		physx::ExtGpu::PxParticleBufferDesc bufferDesc;
		bufferDesc.maxParticles = numParticles;
		bufferDesc.numActiveParticles = numParticles;
		bufferDesc.positions = positionInvMass;
		bufferDesc.velocities = velocity;
		bufferDesc.phases = phase;

		// õ���� ���� ��������
		const physx::PxParticleClothDesc& clothDesc = clothBuffers->getParticleClothDesc();

		// ���� õ���� ��ó���� ����
		physx::PxParticleClothPreProcessor* clothPreProcessor = PxCreateParticleClothPreProcessor(m_CudaContextManager);

		// ���� õ�� ���� �� ó��
		physx::PxPartitionedParticleCloth output;
		clothPreProcessor->partitionSprings(clothDesc, output);
		clothPreProcessor->release();

		// õ�� ���� ����
		m_ClothBuffer = physx::ExtGpu::PxCreateAndPopulateParticleClothBuffer(bufferDesc, clothDesc, output, m_CudaContextManager);
		m_ParticleSystem->addParticleBuffer(m_ClothBuffer);

		// ���� ����
		clothBuffers->release();

		// �Ҵ�� �޸� ����
		m_CudaContextManager->freePinnedHostBuffer(positionInvMass);
		m_CudaContextManager->freePinnedHostBuffer(velocity);
		m_CudaContextManager->freePinnedHostBuffer(phase);

		//int paticleSize = m_ClothBuffer->getNbActiveParticles();
		//physx::PxVec4* particle = m_ClothBuffer->getPositionInvMasses();

		//physx::PxCudaContextManager* cudaContextManager = m_CudaContextManager;
		//m_CudaContextManager->acquireContext();

		//physx::PxCudaContext* cudaContext = cudaContextManager->getCudaContext();
		//std::vector<physx::PxVec4> vertex;
		//vertex.resize(paticleSize);

		//cudaContext->memcpyDtoH(vertex.data(), CUdeviceptr(particle), sizeof(physx::PxVec4) * paticleSize);

		//physx::PxTransform pxCurrentTrnasform;

		//DirectX::SimpleMath::Quaternion rotation = DirectX::SimpleMath::Quaternion::CreateFromYawPitchRoll(DirectX::SimpleMath::Vector3(-1.8f, 0.3f, 0.f));
		//DirectX::SimpleMath::Matrix TrnaslationTransform = DirectX::SimpleMath::Matrix::CreateTranslation(DirectX::SimpleMath::Vector3(x, 10000.f, -z));
		//DirectX::SimpleMath::Matrix rotationTransform = DirectX::SimpleMath::Matrix::CreateFromQuaternion(rotation);
		//DirectX::SimpleMath::Matrix worldTransform = rotationTransform * TrnaslationTransform;
		//CopyDirectXMatrixToPxTransform(worldTransform, pxCurrentTrnasform);

		//for (int i = 0; i < paticleSize; i++)
		//{
		//	vertex[i] = multiply(pxCurrentTrnasform, vertex[i]);		// ���� ���� Ʈ�������� ��ġ�� ��ȯ�ϰ�
		//}

		//cudaContext->memcpyHtoD(CUdeviceptr(particle), vertex.data(), sizeof(physx::PxVec4)* paticleSize);
	}
#pragma endregion

#pragma region SoftBody
	void PhysX::addSoftBody(physx::PxSoftBody* softBody, const physx::PxFEMParameters& femParams, const physx::PxTransform& transform, const physx::PxReal density, const physx::PxReal scale, const physx::PxU32 iterCount)
	{
		physx::PxVec4* simPositionInvMassPinned;
		physx::PxVec4* simVelocityPinned;
		physx::PxVec4* collPositionInvMassPinned;
		physx::PxVec4* restPositionPinned;

		physx::PxSoftBodyExt::allocateAndInitializeHostMirror(*softBody, m_CudaContextManager, simPositionInvMassPinned, simVelocityPinned, collPositionInvMassPinned, restPositionPinned);

		const physx::PxReal maxInvMassRatio = 50.f;

		softBody->setParameter(femParams);
		softBody->setSolverIterationCounts(iterCount);

		physx::PxSoftBodyExt::transform(*softBody, transform, scale, simPositionInvMassPinned, simVelocityPinned, collPositionInvMassPinned, restPositionPinned);
		physx::PxSoftBodyExt::updateMass(*softBody, density, maxInvMassRatio, simPositionInvMassPinned);
		physx::PxSoftBodyExt::copyToDevice(*softBody, physx::PxSoftBodyDataFlag::eALL, simPositionInvMassPinned, simVelocityPinned, collPositionInvMassPinned, restPositionPinned);

		SoftBody sBody(softBody, m_CudaContextManager);

		m_SoftBodies.push_back(sBody);

		m_CudaContextManager->freePinnedHostBuffer(simPositionInvMassPinned);
		m_CudaContextManager->freePinnedHostBuffer(simVelocityPinned);
		m_CudaContextManager->freePinnedHostBuffer(collPositionInvMassPinned);
		m_CudaContextManager->freePinnedHostBuffer(restPositionPinned);
	}

	physx::PxSoftBody* PhysX::CreateSoftBody(const physx::PxCookingParams& params, const physx::PxArray<physx::PxVec3>& triVerts, const physx::PxArray<physx::PxU32>& triIndices, bool useCollisionMeshForSimulation)
	{
		physx::PxSoftBodyMesh* softBodyMesh;

		physx::PxU32 numVoxelsAlongLongestAABBAxis = 8;

		physx::PxSimpleTriangleMesh surfaceMesh;
		surfaceMesh.points.count = triVerts.size();
		surfaceMesh.points.data = triVerts.begin();
		surfaceMesh.triangles.count = triIndices.size() / 3;
		surfaceMesh.triangles.data = triIndices.begin();

		if (useCollisionMeshForSimulation)
		{
			softBodyMesh = physx::PxSoftBodyExt::createSoftBodyMeshNoVoxels(params, surfaceMesh, m_Physics->getPhysicsInsertionCallback());
		}
		else
		{
			softBodyMesh = physx::PxSoftBodyExt::createSoftBodyMesh(params, surfaceMesh, numVoxelsAlongLongestAABBAxis, m_Physics->getPhysicsInsertionCallback());
		}

		PX_ASSERT(softBodyMesh);

		if (!m_CudaContextManager)
			return NULL;

		physx::PxSoftBody* softBody = m_Physics->createSoftBody(*m_CudaContextManager);
		if (softBody)
		{
			physx::PxShapeFlags shapeFlags = physx::PxShapeFlag::eVISUALIZATION | physx::PxShapeFlag::eSCENE_QUERY_SHAPE | physx::PxShapeFlag::eSIMULATION_SHAPE;

			physx::PxFEMSoftBodyMaterial* materialPtr = PxGetPhysics().createFEMSoftBodyMaterial(1e+6f, 0.45f, 0.5f);
			physx::PxTetrahedronMeshGeometry geometry(softBodyMesh->getCollisionMesh());
			physx::PxShape* shape = m_Physics->createShape(geometry, &materialPtr, 1, true, shapeFlags);

			if (shape)
			{
				physx::PxFilterData filterDataB;
				filterDataB.word0 = OBJECT_TYPE_B;
				filterDataB.word1 = OBJECT_TYPE_A | OBJECT_TYPE_B | OBJECT_TYPE_C;

				softBody->attachShape(*shape);
				shape->setSimulationFilterData(filterDataB);
			}
			softBody->attachSimulationMesh(*softBodyMesh->getSimulationMesh(), *softBodyMesh->getSoftBodyAuxData());

			m_Scene->addActor(*softBody);

			physx::PxFEMParameters femParams;
			addSoftBody(softBody, femParams, physx::PxTransform(physx::PxVec3(0.f, 100.f, 0.f), physx::PxQuat(physx::PxIdentity)), 1.f, 1.f, 30);
			softBody->setSoftBodyFlag(physx::PxSoftBodyFlag::eDISABLE_SELF_COLLISION, true);
		}

		return softBody;
	}

	void PhysX::CreateSoftBodies()
	{
		physx::PxTolerancesScale scale;
		m_Physics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_Foundation, scale, true, m_Pvd);
		PxInitExtensions(*m_Physics, m_Pvd);

		physx::PxCookingParams params(scale);
		params.meshWeldTolerance = 0.001f;
		params.meshPreprocessParams = physx::PxMeshPreprocessingFlags(physx::PxMeshPreprocessingFlag::eWELD_VERTICES);
		params.buildTriangleAdjacencies = false;
		params.buildGPUData = true;

		physx::PxArray<physx::PxVec3> triVerts;
		physx::PxArray<physx::PxU32> triIndices;

		physx::PxReal maxEdgeLength = 4;

		createCube(triVerts, triIndices, physx::PxVec3(0, 0, 0), 20.0);
		physx::PxRemeshingExt::limitMaxEdgeLength(triIndices, triVerts, maxEdgeLength);
		CreateSoftBody(params, triVerts, triIndices, true);
	}
#pragma endregion

	void PhysX::move(DirectX::SimpleMath::Vector3& direction)
	{
		m_CharactorController->AddDirection(direction);
	}

	void PhysX::Jump(DirectX::SimpleMath::Vector3& direction)
	{
		m_CharactorController->AddDirection(direction);
	}

	PhysicsClothGetData PhysX::GetPhysicsClothGetData()
	{
		PhysicsClothGetData getData;

		physx::PxCudaContext* cudaContext = m_CudaContextManager->getCudaContext();
		m_ClothPhysics->GetPhysicsCloth(m_CudaContextManager, cudaContext, getData);

		return getData;
	}

	//PhysicsClothGetData PhysX::GetCudaPhysicsClothGetData()
	//{
	//	PhysicsClothGetData getData;

	//	physx::PxCudaContext* cudaContext = m_CudaContextManager->getCudaContext();
	//	m_ClothPhysics->GetPhysicsCloth(m_CudaContextManager, cudaContext, getData);

	//	return getData;
	//}

	//bool PhysX::SetClothBuffer(ID3D11Buffer* buffer)
	//{
	//	if (buffer == nullptr)
	//		return false;

	//	return m_CudaClothPhysics->RegisterD3D11BufferWithCUDA(buffer);
	//}

	//const unsigned int& PhysX::GetPhysicsVertexSize()
	//{
	//	return m_CudaClothPhysics->GetVertexSize();
	//}
	//const unsigned int& PhysX::GetPhysicsIndexSize()
	//{
	//	return m_CudaClothPhysics->GetIndexSize();
	//}
}