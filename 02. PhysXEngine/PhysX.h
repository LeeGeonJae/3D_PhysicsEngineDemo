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
		inline physx::PxScene* GetPxScene();
		inline std::vector<physx::PxShape*>& GetPxShapes();
		inline std::vector<physx::PxRigidBody*>& GetPxBodies();

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

		std::vector<physx::PxShape*>	m_Shapes;
		std::vector<physx::PxRigidBody*> m_Bodies;
		// init physX

		std::vector<physx::PxVec3> m_ModelVertices;
		// create simulation


		// run simulation

	};

	physx::PxScene* PhysX::GetPxScene()
	{
		return m_Scene;
	}
	std::vector<physx::PxShape*>& PhysX::GetPxShapes()
	{
		return m_Shapes;
	}
	std::vector<physx::PxRigidBody*>& PhysX::GetPxBodies()
	{
		return m_Bodies;
	}
	void PhysX::AddVertexPosition(physx::PxVec3 _vertex)
	{
		m_ModelVertices.push_back(_vertex);
	}
} 