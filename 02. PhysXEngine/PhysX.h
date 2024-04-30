#pragma once

#include <physx\PxPhysics.h>
#include <physx\PxPhysicsAPI.h>

#include <vector>
#include <directxtk/SimpleMath.h>

namespace PhysicsEngine
{
	class PhysicsSimulationEventCallback;

	class PhysX
	{
	public:
		PhysX();
		~PhysX();

	public:
		void Init();
		void Update(float elapsedTime);

		void CreateActor();
		void CreateCharactorController();

		void move(DirectX::SimpleMath::Vector3& direction, float deltaTime);

	public:
		inline physx::PxScene* GetPxScene();
		inline std::vector<physx::PxShape*>& GetPxShapes();
		inline std::vector<physx::PxRigidActor*>& GetPxBodies();
		inline physx::PxController* GetCharactorController();

	public:
		inline void AddVertexPosition(physx::PxVec3 _vertex);

	private:
		physx::PxDefaultAllocator		m_DefaultAllocatorCallback;
		physx::PxDefaultErrorCallback	m_DefaultErrorCallback;
		physx::PxDefaultCpuDispatcher*	m_Dispatcher = nullptr;
		physx::PxTolerancesScale		m_ToleranceScale;

		physx::PxFoundation*			m_Foundation = nullptr;
		physx::PxPhysics*				m_Physics = nullptr;

		physx::PxScene*					m_Scene = nullptr;
		physx::PxMaterial*				m_Material = nullptr;

		physx::PxPvd*					m_Pvd = nullptr;

		physx::PxPvdSceneClient*		m_pvdClient = nullptr;
		physx::PxRigidStatic*			m_groundPlane = nullptr;

		physx::PxControllerManager*		m_ControllerManager = nullptr;
		physx::PxController*			m_CharactorController = nullptr;
		float m_Speed = 500.f;
		float m_Deceleration = 100.f;

		// init physX
		std::vector<physx::PxShape*>	m_Shapes;
		std::vector<physx::PxRigidActor*> m_Bodies;

		std::vector<physx::PxVec3> m_ModelVertices;
		// create simulation
		PhysicsSimulationEventCallback* m_MyEventCallback;

		// run simulation
		physx::PxRigidDynamic* m_DynamicBody;

	};

	physx::PxScene* PhysX::GetPxScene()
	{
		return m_Scene;
	}
	std::vector<physx::PxShape*>& PhysX::GetPxShapes()
	{
		return m_Shapes;
	}
	std::vector<physx::PxRigidActor*>& PhysX::GetPxBodies()
	{
		return m_Bodies;
	}
	void PhysX::AddVertexPosition(physx::PxVec3 _vertex)
	{
		m_ModelVertices.push_back(_vertex);
	}
	physx::PxController* PhysX::GetCharactorController()
	{
		return m_CharactorController;
	}
} 