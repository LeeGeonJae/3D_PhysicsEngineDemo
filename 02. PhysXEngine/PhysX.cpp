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
			pairFlags &= ~physx::PxPairFlag::eCONTACT_DEFAULT; // 충돌 행동 비허용
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


	void PhysX::Init()
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
		m_Physics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_Foundation, m_ToleranceScale, true, m_Pvd); // Physics를 생성합니다.

		// PhysX 시뮬레이션을 위한 Scene을 설정합니다.
		physx::PxSceneDesc sceneDesc(m_Physics->getTolerancesScale()); // Scene을 생성할 때 물리적인 허용 오차 스케일을 설정합니다.

		// 중력을 설정합니다.
		sceneDesc.gravity = physx::PxVec3(0.f, -10.f, 0.f); // 중력을 설정합니다.

		// CPU 디스패처를 생성하고 설정합니다.
		m_Dispatcher = physx::PxDefaultCpuDispatcherCreate(2); // CPU 디스패처를 생성합니다.

		
		m_MyEventCallback = new PhysicsSimulationEventCallback;
		physx::PxPairFlags pairFlags = physx::PxPairFlags();

		// Scene 설명자에 CPU 디스패처와 필터 셰이더를 설정합니다.
		sceneDesc.cpuDispatcher = m_Dispatcher; // Scene 설명자에 CPU 디스패처를 설정합니다.
		sceneDesc.filterShader = CustomSimulationFilterShader;
		sceneDesc.simulationEventCallback = m_MyEventCallback;		// 클래스 등록
		sceneDesc.flags |= physx::PxSceneFlag::eENABLE_CCD;
		sceneDesc.flags |= physx::PxSceneFlag::eENABLE_PCM;


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

		//std::cout << m_Scene->getTimestamp() << std::endl;

		//m_Scene->getVisualizationParameter(physx::PxVisualizationParameter::eCOLLISION_EDGES);
	}

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
		joint->setMotion(physx::PxArticulationAxis::eSWING1, physx::PxArticulationMotion::eFREE);
		joint->setMotion(physx::PxArticulationAxis::eSWING2, physx::PxArticulationMotion::eFREE);
		joint->setMotion(physx::PxArticulationAxis::eTWIST, physx::PxArticulationMotion::eFREE);


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

	void PhysX::move(DirectX::SimpleMath::Vector3& direction)
	{
		m_CharactorController->AddDirection(direction);
	}

	void PhysX::Jump(DirectX::SimpleMath::Vector3& direction)
	{
		m_CharactorController->AddDirection(direction);
	}
}