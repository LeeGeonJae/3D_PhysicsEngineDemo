#include "pch.h"
#include "PhysX.h"

#include "iostream"
#include "PhysicsSimulationEventCallback.h"
#include "ActorUserData.h"
#include "CharactorController.h"
#include <cassert>

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
				| physx::PxPairFlag::eNOTIFY_TOUCH_FOUND
				| physx::PxPairFlag::eNOTIFY_TOUCH_LOST
				| physx::PxPairFlag::eNOTIFY_TOUCH_PERSISTS;

			return physx::PxFilterFlag::eDEFAULT;
		}

		// ���� ������ �浹 üũ ( �ùķ��̼� )
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
			pairFlags &= ~physx::PxPairFlag::eCONTACT_DEFAULT; // �浹 �ൿ �����
			return physx::PxFilterFlag::eSUPPRESS;
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


	void PhysX::Init()
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
		m_ToleranceScale.length = 100; // ���� ��� ���� �������� �����մϴ�.
		m_ToleranceScale.speed = 2000; // �ӵ� ��� ���� �������� �����մϴ�.

		// PhysX Physics�� �����ϰ� �ʱ�ȭ�մϴ�.
		m_Physics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_Foundation, m_ToleranceScale, true, m_Pvd); // Physics�� �����մϴ�.

		// PhysX �ùķ��̼��� ���� Scene�� �����մϴ�.
		physx::PxSceneDesc sceneDesc(m_Physics->getTolerancesScale()); // Scene�� ������ �� �������� ��� ���� �������� �����մϴ�.

		// �߷��� �����մϴ�.
		sceneDesc.gravity = physx::PxVec3(0.f, -20.f, 0.f); // �߷��� �����մϴ�.

		// CPU ����ó�� �����ϰ� �����մϴ�.
		m_Dispatcher = physx::PxDefaultCpuDispatcherCreate(2); // CPU ����ó�� �����մϴ�.

		
		m_MyEventCallback = new PhysicsSimulationEventCallback;
		physx::PxPairFlags pairFlags = physx::PxPairFlags();

		// Scene �����ڿ� CPU ����ó�� ���� ���̴��� �����մϴ�.
		sceneDesc.cpuDispatcher = m_Dispatcher; // Scene �����ڿ� CPU ����ó�� �����մϴ�.
		sceneDesc.filterShader = CustomSimulationFilterShader;
		sceneDesc.simulationEventCallback = m_MyEventCallback;		// Ŭ���� ���


		// PhysX Physics���� Scene�� �����մϴ�.
		m_Scene = m_Physics->createScene(sceneDesc); // Scene�� �����մϴ�.
		assert(m_Scene);

		// 
		m_pvdClient = m_Scene->getScenePvdClient();
		if (m_pvdClient)
		{
			m_pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
			m_pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
			m_pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
		}
		m_Scene->setVisualizationParameter(physx::PxVisualizationParameter::eJOINT_LOCAL_FRAMES, 1.f);
		m_Scene->setVisualizationParameter(physx::PxVisualizationParameter::eJOINT_LIMITS, 1.f);

		CreateActor();
		CreateCharactorController();
		CreateArticulation();
	}

	void PhysX::Update(float elapsedTime)
	{
		m_CharactorController->Update(elapsedTime);

		m_Scene->simulate(elapsedTime);
		m_Scene->fetchResults(true);
		//m_Scene->getVisualizationParameter(physx::PxVisualizationParameter::eCOLLISION_EDGES);
	}

	void PhysX::CreateActor()
	{
		// ������Ʈ Ÿ�Կ� ���� ���� ������ ����
		physx::PxFilterData filterDataA;
		filterDataA.word0 = OBJECT_TYPE_A;
		filterDataA.word1 = OBJECT_TYPE_A | OBJECT_TYPE_B | OBJECT_TYPE_C;

		physx::PxFilterData filterDataB;
		filterDataB.word0 = OBJECT_TYPE_B;
		filterDataB.word1 = OBJECT_TYPE_B | OBJECT_TYPE_C;

		physx::PxFilterData filterDataC;
		filterDataC.word0 = OBJECT_TYPE_C;
		filterDataC.word1 = OBJECT_TYPE_A | OBJECT_TYPE_B;

		// �ùķ��̼� ����
		ActorUserData* data1 = new ActorUserData(ActorType::TILE);
		ActorUserData* data2 = new ActorUserData(ActorType::MONSTER);
		m_Material = m_Physics->createMaterial(1.f, 1.f, 1.f);
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

		physx::PxDefaultMemoryOutputStream buf;
		physx::PxConvexMeshCookingResult::Enum result;
		assert(PxCookConvexMesh(params, convexdesc, buf, &result));
		physx::PxDefaultMemoryInputData input(buf.getData(), buf.getSize());
		physx::PxConvexMesh* convexMesh = m_Physics->createConvexMesh(input);


		float halfExtent = 5.f;
		physx::PxU32 size = 2;

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
	}

	void PhysX::CreateCharactorController()
	{
		m_ControllerManager = PxCreateControllerManager(*m_Scene);
		m_CharactorController = std::make_shared<CharactorController>();
		m_CharactorController->Initialzie(m_Material, m_ControllerManager);
		m_ControllerManager->setDebugRenderingFlags(physx::PxControllerDebugRenderFlag::eALL);\
	}

	void PhysX::CreateArticulation()
	{
		physx::PxTransform transform1 = physx::PxTransform(physx::PxVec3{ -2.5, 100.f, 0.f });
		physx::PxTransform transform2 = physx::PxTransform(physx::PxVec3{ 2.5f, 100.f, 0.f });

		physx::PxArticulationReducedCoordinate* articulation = m_Physics->createArticulationReducedCoordinate();

		articulation->setArticulationFlag(physx::PxArticulationFlag::eFIX_BASE, true);
		articulation->setSolverIterationCounts(4);
		articulation->setMaxCOMLinearVelocity(1000000);

		physx::PxArticulationLink* rootLink = articulation->createLink(NULL, physx::PxTransform(0.f, 0.f, 0.f));
		physx::PxRigidActorExt::createExclusiveShape(*rootLink, physx::PxSphereGeometry(1.f), *m_Material);
		physx::PxRigidBodyExt::updateMassAndInertia(*rootLink, 1.f);

		physx::PxArticulationLink* link = articulation->createLink(rootLink, physx::PxTransform(physx::PxVec3(15.f, 0.f, 0.f)));
		physx::PxRigidActorExt::createExclusiveShape(*link, physx::PxSphereGeometry(3.f), *m_Material);
		physx::PxRigidBodyExt::updateMassAndInertia(*link, 1.f);

		physx::PxArticulationJointReducedCoordinate* joint = link->getInboundJoint();
		joint->setParentPose(m_Bodies[1]->getGlobalPose());
		joint->setChildPose(m_Bodies[2]->getGlobalPose());

		joint->setJointType(physx::PxArticulationJointType::eREVOLUTE);
		// ����Ʈ �������� Z��(eSWING2)�� �߽����� ȸ���ϴ� ȸ�� ����Ʈ
		joint->setMotion(physx::PxArticulationAxis::eSWING2, physx::PxArticulationMotion::eLIMITED);
		joint->setMotion(physx::PxArticulationAxis::eTWIST, physx::PxArticulationMotion::eLIMITED);
		joint->setMotion(physx::PxArticulationAxis::eX, physx::PxArticulationMotion::eLOCKED);
		joint->setMotion(physx::PxArticulationAxis::eY, physx::PxArticulationMotion::eLOCKED);
		joint->setMotion(physx::PxArticulationAxis::eZ, physx::PxArticulationMotion::eFREE);

		physx::PxArticulationLimit limits;
		limits.low = -physx::PxPiDivFour;	// in rad for a rotational motion
		limits.high = physx::PxPiDivFour;
		joint->setLimitParams(physx::PxArticulationAxis::eSWING2, limits);
		joint->setLimitParams(physx::PxArticulationAxis::eTWIST, limits);

		physx::PxArticulationDrive posDrive;
		posDrive.stiffness = 10.f;										// ����Ʈ�� ��ǥ ��ġ�� �����ϴ� ������ ���
		posDrive.damping = 5.f;											// ����Ʈ�� ��ǥ �ӵ��� �����ϴ� ���� ���
		posDrive.maxForce = 1000.f;										// ����̺꿡 ���� �� ����
		posDrive.driveType = physx::PxArticulationDriveType::eFORCE;	// ����̺� ����� ��/torque�� �ǵ��� �մϴ�.(�⺻��)
		// ���� �� ��ǥ ���� (�ϰ��� ���� �ٽ� ���)
		joint->setDriveParams(physx::PxArticulationAxis::eSWING2, posDrive);
		joint->setDriveVelocity(physx::PxArticulationAxis::eSWING2, 0.f);
		joint->setDriveTarget(physx::PxArticulationAxis::eSWING2, 1.5f);
		joint->setDriveTarget(physx::PxArticulationAxis::eTWIST, 0.5f);

		physx::PxArticulationFixedTendon* fixedTendon = articulation->createFixedTendon();

		m_Scene->addArticulation(*articulation);
	}

	void PhysX::move(DirectX::SimpleMath::Vector3& direction)
	{
		m_CharactorController->AddDirection(direction);
	}

	void PhysX::Jump(DirectX::SimpleMath::Vector3& direction)
	{
		m_CharactorController->AddDirection(direction);
	}
}