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
		m_ToleranceScale.speed = 981; // �ӵ� ��� ���� �������� �����մϴ�.

		// PhysX Physics�� �����ϰ� �ʱ�ȭ�մϴ�.
		m_Physics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_Foundation, m_ToleranceScale, true, m_Pvd); // Physics�� �����մϴ�.

		// PhysX �ùķ��̼��� ���� Scene�� �����մϴ�.
		physx::PxSceneDesc sceneDesc(m_Physics->getTolerancesScale()); // Scene�� ������ �� �������� ��� ���� �������� �����մϴ�.

		// �߷��� �����մϴ�.
		sceneDesc.gravity = physx::PxVec3(0.f, -9.81f, 0.f); // �߷��� �����մϴ�.

		// CPU ����ó�� �����ϰ� �����մϴ�.
		m_Dispatcher = physx::PxDefaultCpuDispatcherCreate(2); // CPU ����ó�� �����մϴ�.

		// Scene �����ڿ� CPU ����ó�� ���� ���̴��� �����մϴ�.
		sceneDesc.cpuDispatcher = m_Dispatcher; // Scene �����ڿ� CPU ����ó�� �����մϴ�.
		sceneDesc.filterShader = physx::PxDefaultSimulationFilterShader; // Scene �����ڿ� ���� ���̴��� �����մϴ�.

		// PhysX Physics���� Scene�� �����մϴ�.
		m_Scene = m_Physics->createScene(sceneDesc); // Scene�� �����մϴ�.

		// 
		m_pvdClient = m_Scene->getScenePvdClient();
		if (m_pvdClient)
		{
			m_pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
			m_pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
			m_pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
		}

		// �ùķ��̼� ����
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