#pragma once

#include <PxPhysics.h>
#include <PxPhysicsAPI.h>

#include <vector>

namespace PhysicsEngine
{
	class PhysX
	{
	public:
		PhysX();
		~PhysX();

	public:
		void Init();
		void Update(float elapsedTime);

	public:
		inline const physx::PxScene* GetPxScene();
		inline const std::vector<physx::PxShape*>& GetPxShapes();
		inline const std::vector<physx::PxRigidBody*>& GetPxBodies();

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

		std::vector<physx::PxShape*>	m_Shapes;
		std::vector<physx::PxRigidBody*> m_Bodies;
		// init physX


		// create simulation


		// run simulation

	};

	const physx::PxScene* PhysX::GetPxScene()
	{
		return m_Scene;
	}

	const std::vector<physx::PxShape*>& PhysX::GetPxShapes()
	{
		return m_Shapes;
	}

	const std::vector<physx::PxRigidBody*>& PhysX::GetPxBodies()
	{
		return m_Bodies;
	}
} 