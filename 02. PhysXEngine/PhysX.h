#pragma once

#include <PxPhysics.h>
#include <PxPhysicsAPI.h>

#include <vector>
#include <memory>
#include <directxtk/SimpleMath.h>
#include <d3d11.h>

//#pragma comment(lib, "../Libraries/Lib/cudart.lib")

#include "ClothPhysics.h"
#include "CudaClothPhysics.h"

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
		void CreateCloth();
		void CreateCudaCloth();
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
		inline physx::PxCudaContextManager* GetCudaContextManager();
		inline physx::PxParticleClothBuffer* GetParticleClothBuffer();
		inline const std::shared_ptr<ClothPhysics> GetClothPhysics();
		//const unsigned int& GetPhysicsVertexSize();
		//const unsigned int& GetPhysicsIndexSize();
		PhysicsClothGetData GetPhysicsClothGetData();
		//PhysicsClothGetData GetCudaPhysicsClothGetData();
		//cudaGraphicsResource* GetCudaGraphicsResource();
		//bool SetClothBuffer(ID3D11Buffer* buffer);

	public:
		inline void AddVertexPosition(physx::PxVec3 _vertex);
		inline void AddUV(physx::PxVec2 _uv);
		inline void AddIndex(unsigned int _index);

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
		std::vector<physx::PxVec2> m_ModelUV;
		std::vector<unsigned int> m_ModelIndices;
		// create simulation
		PhysicsSimulationEventCallback* m_MyEventCallback;

		// run simulation
		physx::PxRigidDynamic* m_DynamicBody;

		// Cloth
		std::shared_ptr<ClothPhysics> m_ClothPhysics;
		//std::shared_ptr<CudaClothPhysics> m_CudaClothPhysics;
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
	void PhysX::AddUV(physx::PxVec2 _uv)
	{
		m_ModelUV.push_back(_uv);
	}
	void PhysX::AddIndex(unsigned int _index)
	{
		m_ModelIndices.push_back(_index);
	}
	physx::PxCudaContextManager* PhysX::GetCudaContextManager()
	{
		return m_CudaContextManager;
	}
	physx::PxParticleClothBuffer* PhysX::GetParticleClothBuffer()
	{
		return m_ClothBuffer;
	}
	const std::shared_ptr<ClothPhysics> PhysX::GetClothPhysics()
	{
		return m_ClothPhysics;
	}
	//cudaGraphicsResource* PhysX::GetCudaGraphicsResource()
	//{
	//	return m_CudaClothPhysics->GetCudaGraphicsResource();
	//}
} 