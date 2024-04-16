#include "pch.h"
#include "PhysX.h"

namespace PhysicsEngine
{
	PhysX::PhysX()
	{

	}

	PhysX::~PhysX()
	{

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
		m_ToleranceScale.speed = 981; // 속도 허용 오차 스케일을 설정합니다.

		// PhysX Physics를 생성하고 초기화합니다.
		m_Physics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_Foundation, m_ToleranceScale, true, m_Pvd); // Physics를 생성합니다.

		// PhysX 시뮬레이션을 위한 Scene을 설정합니다.
		physx::PxSceneDesc sceneDesc(m_Physics->getTolerancesScale()); // Scene을 생성할 때 물리적인 허용 오차 스케일을 설정합니다.

		// 중력을 설정합니다.
		sceneDesc.gravity = physx::PxVec3(0.f, -9.81f, 0.f); // 중력을 설정합니다.

		// CPU 디스패처를 생성하고 설정합니다.
		m_Dispatcher = physx::PxDefaultCpuDispatcherCreate(2); // CPU 디스패처를 생성합니다.

		// Scene 설명자에 CPU 디스패처와 필터 셰이더를 설정합니다.
		sceneDesc.cpuDispatcher = m_Dispatcher; // Scene 설명자에 CPU 디스패처를 설정합니다.
		sceneDesc.filterShader = physx::PxDefaultSimulationFilterShader; // Scene 설명자에 필터 셰이더를 설정합니다.

		// PhysX Physics에서 Scene을 생성합니다.
		m_Scene = m_Physics->createScene(sceneDesc); // Scene을 생성합니다.

		// 
		m_pvdClient = m_Scene->getScenePvdClient();
		if (m_pvdClient)
		{
			m_pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
			m_pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
			m_pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
		}

		// 시뮬레이션 생성
		m_Material = m_Physics->createMaterial(0.5f, 0.5f, 0.6f);
		m_groundPlane = physx::PxCreatePlane(*m_Physics, physx::PxPlane(0, 1, 0, 1), *m_Material);
		m_Scene->addActor(*m_groundPlane);

		float halfExtent = 5.f;
		physx::PxU32 size = 20;

		const physx::PxTransform t(physx::PxVec3(0));
		for (physx::PxU32 i = 0; i < size; i++)
		{
			for (physx::PxU32 j = 0; j < size - i; j++)
			{
				physx::PxShape* Shape = m_Physics->createShape(physx::PxBoxGeometry(halfExtent, halfExtent, halfExtent), *m_Material);
				physx::PxTransform localTm(physx::PxVec3(physx::PxReal(j * 2) - physx::PxReal(size - i), physx::PxReal(i * 2+1), 0) * halfExtent);
				physx::PxRigidDynamic* body = m_Physics->createRigidDynamic(t.transform(localTm));
				body->attachShape(*Shape);
				physx::PxRigidBodyExt::updateMassAndInertia(*body, 10.f);
				m_Scene->addActor(*body);
				m_Shapes.push_back(Shape);
				m_Bodies.push_back(body);
			}
		}

		physx::PxShape* Shape = m_Physics->createShape(physx::PxBoxGeometry(halfExtent * 10, halfExtent * 10, halfExtent * 10), *m_Material);
		physx::PxTransform localTm(physx::PxVec3(physx::PxReal(0), physx::PxReal(150), 0) * halfExtent);
		physx::PxRigidDynamic* body = m_Physics->createRigidDynamic(t.transform(localTm));
		body->attachShape(*Shape);
		physx::PxRigidBodyExt::updateMassAndInertia(*body, 10.f);
		m_Scene->addActor(*body);
		m_Shapes.push_back(Shape);
		m_Bodies.push_back(body);
	}

	void PhysX::Update(float elapsedTime)
	{
		m_Scene->simulate(elapsedTime);
		m_Scene->fetchResults(true);
	}
}