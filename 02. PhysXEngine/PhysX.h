#pragma once

#pragma comment(lib, "cudart.lib")

#include <physx/PxPhysics.h>
#include <physx/PxPhysicsAPI.h>

#include <vector>
#include <memory>
#include <directxtk/SimpleMath.h>
#include <d3d11.h>

namespace PhysicsEngine
{
	class PhysicsSimulationEventCallback;
	class CharactorController;
	class SoftBody;

	class PhysX
	{
	public:
		PhysX();
		~PhysX();

	public:
		void Init(ID3D11Device* device);
		void Update(float elapsedTime);

		void CreateActor();
		void CreateCharactorController();
		void CreateArticulation();
		void CreateCloth(const physx::PxU32 numX, const physx::PxU32 numZ, const physx::PxVec3& position = physx::PxVec3(0, 0, 0), const physx::PxReal particleSpacing = 0.2f, const physx::PxReal totalClothMass = 10.f);
		physx::PxSoftBody* CreateSoftBody(const physx::PxCookingParams& params, const physx::PxArray<physx::PxVec3>& triVerts, const physx::PxArray<physx::PxU32>& triIndices, bool useCollisionMeshForSimulation = false);
		void addSoftBody(physx::PxSoftBody* softBody, const physx::PxFEMParameters& femParams, const physx::PxTransform& transform, const physx::PxReal density, const physx::PxReal scale, const physx::PxU32 iterCount);

		void CreateSoftBodies();

		void move(DirectX::SimpleMath::Vector3& direction);
		void Jump(DirectX::SimpleMath::Vector3& direction);

	public:
		inline physx::PxScene* GetPxScene();
		inline std::vector<physx::PxShape*>& GetPxShapes();
		inline std::vector<physx::PxRigidActor*>& GetPxBodies();
		inline std::vector<SoftBody>& GetSoftBodies();
		inline std::shared_ptr<CharactorController> GetCharactorController();

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

		//
		physx::PxCudaContextManager* m_CudaContextManager = nullptr;
		physx::PxPBDParticleSystem* m_ParticleSystem = nullptr;

		physx::PxParticleClothBuffer* m_ClothBuffer = nullptr;
		//

		std::shared_ptr<CharactorController> m_CharactorController = nullptr;

		// init physX
		std::vector<physx::PxShape*>	m_Shapes;
		std::vector<physx::PxRigidActor*> m_Bodies;
		std::vector<SoftBody> m_SoftBodies;

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
	std::vector<SoftBody>& PhysX::GetSoftBodies()
	{
		return m_SoftBodies;
	}
	std::shared_ptr<CharactorController> PhysX::GetCharactorController()
	{
		return m_CharactorController;
	}
	void PhysX::AddVertexPosition(physx::PxVec3 _vertex)
	{
		m_ModelVertices.push_back(_vertex);
	}
} 