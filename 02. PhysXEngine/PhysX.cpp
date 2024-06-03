#include "pch.h"
#include "PhysX.h"

#include "iostream"
#include "PhysicsSimulationEventCallback.h"
#include "ActorUserData.h"
#include "CharactorController.h"
#include "SoftBody.h"
#include "MeshGenerator.h"

#include <cassert>
//#include <physx/foundation/PxPreprocessor.h>		// 전처리기 
#include <physx/cudamanager/PxCudaContext.h>
#include <physx/extensions/PxParticleExt.h>
#include <physx/cudamanager/PxCudaContextManager.h>
#include <physx/geometry/PxTetrahedronMesh.h>
#include <physx/gpu/PxGpu.h>
#include <physx/extensions/PxTetMakerExt.h>
#include <physx/extensions/PxSoftBodyExt.h>
#include <physx/extensions/PxRemeshingExt.h>


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
		// 쌍에 대해 CCD를 활성화하고 초기 및 CCD 연락처에 대한 연락처 보고서를 요청합니다.
		// 또한 접점별 정보를 제공하고 행위자에게 정보를 제공합니다
		// 접촉할 때 포즈를 취합니다.
		//

		// 필터 셰이더 로직 ( 트리거 )
		if (physx::PxFilterObjectIsTrigger(attributes0) || physx::PxFilterObjectIsTrigger(attributes1))
		{
			pairFlags = physx::PxPairFlag::eTRIGGER_DEFAULT
				| physx::PxPairFlag::eNOTIFY_TOUCH_FOUND
				| physx::PxPairFlag::eNOTIFY_TOUCH_LOST
				| physx::PxPairFlag::eNOTIFY_TOUCH_PERSISTS;

			return physx::PxFilterFlag::eDEFAULT;
		}

		// 필터 데이터 충돌 체크 ( 시뮬레이션 )
		if (((filterData0.word1 & filterData1.word0) > 0) && ((filterData1.word1 & filterData0.word0) > 0))
		{
			pairFlags = physx::PxPairFlag::eCONTACT_DEFAULT
				| physx::PxPairFlag::eDETECT_CCD_CONTACT
				| physx::PxPairFlag::eNOTIFY_TOUCH_CCD
				| physx::PxPairFlag::eNOTIFY_TOUCH_FOUND
				| physx::PxPairFlag::eNOTIFY_TOUCH_LOST
				| physx::PxPairFlag::eNOTIFY_CONTACT_POINTS
				| physx::PxPairFlag::eCONTACT_EVENT_POSE
				| physx::PxPairFlag::eNOTIFY_TOUCH_PERSISTS;
			return physx::PxFilterFlag::eDEFAULT;
		}
		else
		{
			pairFlags = physx::PxPairFlag::eCONTACT_DEFAULT
				| physx::PxPairFlag::eDETECT_CCD_CONTACT
				| physx::PxPairFlag::eNOTIFY_TOUCH_CCD
				| physx::PxPairFlag::eNOTIFY_TOUCH_FOUND
				| physx::PxPairFlag::eNOTIFY_TOUCH_LOST
				| physx::PxPairFlag::eNOTIFY_CONTACT_POINTS
				| physx::PxPairFlag::eCONTACT_EVENT_POSE
				| physx::PxPairFlag::eNOTIFY_TOUCH_PERSISTS;
			return physx::PxFilterFlag::eDEFAULT;

			//pairFlags &= ~physx::PxPairFlag::eCONTACT_DEFAULT; // 충돌 행동 비허용
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
		// 필터 셰이더 로직
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
		// PhysX Foundation을 생성하고 초기화합니다.
		m_Foundation = PxCreateFoundation(PX_PHYSICS_VERSION, m_DefaultAllocatorCallback, m_DefaultErrorCallback);

		// Foundation이 성공적으로 생성되었는지 확인합니다.
		if (!m_Foundation)
		{
			throw("PxCreateFoundation failed!"); // Foundation 생성에 실패한 경우 예외를 throw합니다.
		}

		// PhysX Visual Debugger (PVD)를 생성하고 설정합니다.
		m_Pvd = physx::PxCreatePvd(*m_Foundation); // Foundation을 사용하여 PVD를 생성합니다.
		physx::PxPvdTransport* transport = physx::PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10); // PVD에 사용할 트랜스포트를 생성합니다.
		m_Pvd->connect(*transport, physx::PxPvdInstrumentationFlag::eALL); // PVD를 트랜스포트에 연결합니다.

		// 물리 시뮬레이션의 허용 오차 스케일을 설정합니다.
		m_ToleranceScale.length = 100; // 길이 허용 오차 스케일을 설정합니다.
		m_ToleranceScale.speed = 1000; // 속도 허용 오차 스케일을 설정합니다.

		// PhysX Physics를 생성하고 초기화합니다.
		m_Physics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_Foundation, m_ToleranceScale, true, m_Pvd); // Physics를 생성합니다..

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

		// PhysX 시뮬레이션을 위한 Scene을 설정합니다.
		physx::PxSceneDesc sceneDesc(m_Physics->getTolerancesScale()); // Scene을 생성할 때 물리적인 허용 오차 스케일을 설정합니다.

		sceneDesc.gravity = physx::PxVec3(0.f, -10.f, 0.f); // 중력을 설정합니다.

		// CPU 디스패처를 생성하고 설정합니다.
		m_Dispatcher = physx::PxDefaultCpuDispatcherCreate(2); // CPU 디스패처를 생성합니다.

		m_MyEventCallback = new PhysicsSimulationEventCallback;
		physx::PxPairFlags pairFlags = physx::PxPairFlags();

		// Scene 설명자에 CPU 디스패처와 필터 셰이더를 설정합니다.
		// 중력을 설정합니다.
		sceneDesc.cpuDispatcher = m_Dispatcher; // Scene 설명자에 CPU 디스패처를 설정합니다.
		sceneDesc.filterShader = CustomSimulationFilterShader;
		sceneDesc.simulationEventCallback = m_MyEventCallback;		// 클래스 등록
		sceneDesc.cudaContextManager = m_CudaContextManager;
		sceneDesc.staticStructure = physx::PxPruningStructureType::eDYNAMIC_AABB_TREE;
		sceneDesc.flags |= physx::PxSceneFlag::eENABLE_PCM;
		sceneDesc.flags |= physx::PxSceneFlag::eENABLE_GPU_DYNAMICS;
		sceneDesc.broadPhaseType = physx::PxBroadPhaseType::eGPU;
		sceneDesc.solverType = physx::PxSolverType::eTGS;

		// PhysX Physics에서 Scene을 생성합니다.
		m_Scene = m_Physics->createScene(sceneDesc); // Scene을 생성합니다.
		assert(m_Scene);

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
		CreateSoftBodies();
		CreateArticulation();

		// Setup Cloth
		//const physx::PxReal totalClothMass = 10.0f;

		//physx::PxU32 numPointsX = 25;
		//physx::PxU32 numPointsZ = 25;
		//physx::PxReal particleSpacing = 0.05f;

		//CreateCloth(numPointsX, numPointsZ, physx::PxVec3(-0.5f * numPointsX * particleSpacing, 8.f, -0.5f * numPointsZ * particleSpacing), particleSpacing, totalClothMass);
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
	}

#pragma region Actor
	void PhysX::CreateActor()
	{
		// 오브젝트 타입에 대한 필터 데이터 정의
		physx::PxFilterData filterDataA;
		filterDataA.word0 = OBJECT_TYPE_A;
		filterDataA.word1 = OBJECT_TYPE_A | OBJECT_TYPE_B | OBJECT_TYPE_C;

		physx::PxFilterData filterDataB;
		filterDataB.word0 = OBJECT_TYPE_B;
		filterDataB.word1 = OBJECT_TYPE_B | OBJECT_TYPE_C;

		physx::PxFilterData filterDataC;
		filterDataC.word0 = OBJECT_TYPE_C;
		filterDataC.word1 = OBJECT_TYPE_A | OBJECT_TYPE_B;

		// 시뮬레이션 생성
		ActorUserData* data1 = new ActorUserData(ActorType::TILE);
		ActorUserData* data2 = new ActorUserData(ActorType::MONSTER);
		m_Material = m_Physics->createMaterial(1.f, 1.f, 10.f);
		m_groundPlane = physx::PxCreatePlane(*m_Physics, physx::PxPlane(0, 1, 0, 1), *m_Material);
		physx::PxShape* shape;
		m_groundPlane->getShapes(&shape, sizeof(physx::PxShape));
		shape->setSimulationFilterData(filterDataC);
		m_groundPlane->userData = data1;
		m_Scene->addActor(*m_groundPlane);

		// 컨벡스 메시 생성
		physx::PxConvexMeshDesc convexdesc;
		convexdesc.points.count = m_ModelVertices.size();
		convexdesc.points.stride = sizeof(physx::PxVec3);
		convexdesc.vertexLimit = 255;
		convexdesc.polygonLimit = 10;
		convexdesc.points.data = m_ModelVertices.data();
		convexdesc.flags = physx::PxConvexFlag::eCOMPUTE_CONVEX;

		physx::PxTolerancesScale scale;
		physx::PxCookingParams params(scale);

		physx::PxDefaultMemoryOutputStream buf;
		physx::PxConvexMeshCookingResult::Enum result;
		assert(PxCookConvexMesh(params, convexdesc, buf, &result));
		physx::PxDefaultMemoryInputData input(buf.getData(), buf.getSize());
		physx::PxConvexMesh* convexMesh = m_Physics->createConvexMesh(input);


		float halfExtent = 5.f;
		physx::PxU32 size = 0;

		const physx::PxTransform t(physx::PxVec3(0));
		for (physx::PxU32 i = 0; i < size; i++)
		{
			for (physx::PxU32 j = 0; j < size - i; j++)
			{
				ActorUserData* data = new ActorUserData(ActorType::PLAYER);
				physx::PxShape* Shape = m_Physics->createShape(physx::PxSphereGeometry(halfExtent), *m_Material);
				physx::PxTransform localTm(physx::PxVec3(physx::PxReal(j * 2) - physx::PxReal(size - i), physx::PxReal(i * 2 + 1) + 100, 0) * halfExtent);
				physx::PxRigidDynamic* body = m_Physics->createRigidDynamic(t.transform(localTm));
				Shape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, true);
				Shape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, false);
				body->userData = data;
				Shape->setSimulationFilterData(filterDataB);
				body->attachShape(*Shape);
				assert(physx::PxRigidBodyExt::updateMassAndInertia(*body, 1000.f));
				Shape->setContactOffset(0.001f);

				m_Scene->addActor(*body);
				m_Shapes.push_back(Shape);
				m_Bodies.push_back(body);

			}
		}

		//physx::PxShape* Shape = m_Physics->createShape(physx::PxConvexMeshGeometry(convexMesh), *m_Material);
		//physx::PxTransform localTm(physx::PxVec3(physx::PxReal(3), physx::PxReal(150), 0), physx::PxQuat(1.f, 0.1f, 0.f, 0.f));
		//physx::PxRigidDynamic* body = m_Physics->createRigidDynamic(t.transform(localTm));
		//Shape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, true);
		//Shape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, false);
		//body->userData = data2;
		//Shape->setContactOffset(0.001f);
		//Shape->setSimulationFilterData(filterDataB);
		//body->attachShape(*Shape);

		//physx::PxRigidBodyExt::updateMassAndInertia(*body, 1000.f);
		//m_Scene->addActor(*body);
		//m_Shapes.push_back(Shape);
		//m_Bodies.push_back(body);

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
		articulation->setArticulationFlag(physx::PxArticulationFlag::eDISABLE_SELF_COLLISION, false);
		articulation->setSolverIterationCounts(4);
		articulation->setMaxCOMLinearVelocity(10000000.f);

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
	PX_FORCE_INLINE physx::PxU32 id(physx::PxU32 x, physx::PxU32 y, physx::PxU32 numY)
	{
		return x * numY + y;
	}

	void PhysX::CreateCloth(const physx::PxU32 numX, const physx::PxU32 numZ, const physx::PxVec3& position, const physx::PxReal particleSpacing, const physx::PxReal totalClothMass)
	{
		m_CudaContextManager = m_Scene->getCudaContextManager();

		const physx::PxU32 numParticles = numX * numZ;
		const physx::PxU32 numSprings = (numX - 1) * (numZ - 1) * 4 + (numX - 1) + (numZ - 1);
		const physx::PxU32 numTriangles = (numX - 1) * (numZ - 1) * 2;

		const physx::PxReal restOffset = particleSpacing;

		const physx::PxReal stretchStiffness = 10000.f;
		const physx::PxReal shearStiffness = 100.f;
		const physx::PxReal springDamping = 0.001f;

		// Material setup
		physx::PxPBDMaterial* defaultMat = m_Physics->createPBDMaterial(0.8f, 0.05f, 1e+6f, 0.001f, 0.5f, 0.005f, 0.05f, 0.f, 0.f);

		physx::PxPBDParticleSystem* particleSystem = m_Physics->createPBDParticleSystem(*m_CudaContextManager);
		m_ParticleSystem = particleSystem;

		// General particle system setting

		const physx::PxReal particleMass = totalClothMass / numParticles;
		particleSystem->setRestOffset(restOffset);
		particleSystem->setContactOffset(restOffset + 0.02f);
		particleSystem->setParticleContactOffset(restOffset + 0.02f);
		particleSystem->setSolidRestOffset(restOffset);
		particleSystem->setFluidRestOffset(0.0f);

		m_Scene->addActor(*particleSystem);

		// Create particles and add them to the particle system
		const physx::PxU32 particlePhase = particleSystem->createPhase(defaultMat, physx::PxParticlePhaseFlags
		(physx::PxParticlePhaseFlag::eParticlePhaseSelfCollideFilter | physx::PxParticlePhaseFlag::eParticlePhaseSelfCollide));

		physx::ExtGpu::PxParticleClothBufferHelper* clothBuffers = physx::ExtGpu::PxCreateParticleClothBufferHelper(1, numTriangles, numSprings, numParticles, m_CudaContextManager);

		physx::PxU32* phase = m_CudaContextManager->allocPinnedHostBuffer<physx::PxU32>(numParticles);
		physx::PxVec4* positionInvMass = m_CudaContextManager->allocPinnedHostBuffer<physx::PxVec4>(numParticles);
		physx::PxVec4* velocity = m_CudaContextManager->allocPinnedHostBuffer<physx::PxVec4>(numParticles);

		physx::PxReal x = position.x;
		physx::PxReal y = position.y;
		physx::PxReal z = position.z;

		// Define springs and triangles
		physx::PxArray<physx::PxParticleSpring> springs;
		springs.reserve(numSprings);
		physx::PxArray<physx::PxU32> triangles;
		triangles.reserve(numTriangles * 3);

		for (physx::PxU32 i = 0; i < numX; ++i)
		{
			for (physx::PxU32 j = 0; j < numZ; ++j)
			{
				const physx::PxU32 index = i * numZ + j;

				physx::PxVec4 pos(x, y, z, 1.0f / particleMass);
				phase[index] = particlePhase;
				positionInvMass[index] = pos;
				velocity[index] = physx::PxVec4(0.0f);

				if (i > 0)
				{
					physx::PxParticleSpring spring = { id(i - 1, j, numZ), id(i, j, numZ), particleSpacing, stretchStiffness, springDamping, 0 };
					springs.pushBack(spring);
				}
				if (j > 0)
				{
					physx::PxParticleSpring spring = { id(i, j - 1, numZ), id(i, j, numZ), particleSpacing, stretchStiffness, springDamping, 0 };
					springs.pushBack(spring);
				}

				if (i > 0 && j > 0)
				{
					physx::PxParticleSpring spring0 = { id(i - 1, j - 1, numZ), id(i, j, numZ), physx::PxSqrt(2.0f) * particleSpacing, shearStiffness, springDamping, 0 };
					springs.pushBack(spring0);
					physx::PxParticleSpring spring1 = { id(i - 1, j, numZ), id(i, j - 1, numZ), physx::PxSqrt(2.0f) * particleSpacing, shearStiffness, springDamping, 0 };
					springs.pushBack(spring1);

					//Triangles are used to compute approximated aerodynamic forces for cloth falling down
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

		PX_ASSERT(numSprings == springs.size());
		PX_ASSERT(numTriangles == triangles.size() / 3);

		clothBuffers->addCloth(0.0f, 0.0f, 0.0f, triangles.begin(), numTriangles, springs.begin(), numSprings, positionInvMass, numParticles);

		physx::ExtGpu::PxParticleBufferDesc bufferDesc;
		bufferDesc.maxParticles = numParticles;
		bufferDesc.numActiveParticles = numParticles;
		bufferDesc.positions = positionInvMass;
		bufferDesc.velocities = velocity;
		bufferDesc.phases = phase;

		const physx::PxParticleClothDesc& clothDesc = clothBuffers->getParticleClothDesc();
		physx::PxParticleClothPreProcessor* clothPreProcessor = PxCreateParticleClothPreProcessor(m_CudaContextManager);

		physx::PxPartitionedParticleCloth output;
		clothPreProcessor->partitionSprings(clothDesc, output);
		clothPreProcessor->release();

		m_ClothBuffer = physx::ExtGpu::PxCreateAndPopulateParticleClothBuffer(bufferDesc, clothDesc, output, m_CudaContextManager);
		m_ParticleSystem->addParticleBuffer(m_ClothBuffer);



		clothBuffers->release();

		m_CudaContextManager->freePinnedHostBuffer(positionInvMass);
		m_CudaContextManager->freePinnedHostBuffer(velocity);
		m_CudaContextManager->freePinnedHostBuffer(phase);
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
				softBody->attachShape(*shape);
				shape->setSimulationFilterData(physx::PxFilterData(0, UINT_MAX, 0, 0));
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

		physx::PxReal maxEdgeLength = 2;

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
}