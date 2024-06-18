#include "PhysicsClothManager.h"

#include "CudaClothPhysics.h"

namespace physics
{
	PhysicsClothManager::PhysicsClothManager()
		: mCudaContextManager(nullptr)
		, mScene(nullptr)
		, mPhysics(nullptr)
		, mPhysicsClothContainer()
	{
	}

	PhysicsClothManager::~PhysicsClothManager()
	{
	}

	bool PhysicsClothManager::Initialize(physx::PxCudaContextManager* cudaContextManager, physx::PxPhysics* physics, physx::PxScene* scene)
	{
		PX_ASSERT(physics);
		PX_ASSERT(scene);
		PX_ASSERT(cudaContextManager);

		mPhysics = physics;
		mScene = scene;
		mCudaContextManager = cudaContextManager;

		return true;
	}
	bool PhysicsClothManager::Update()
	{
		for (auto cloth : mPhysicsClothContainer)
		{
			if (!cloth.second->UpdatePhysicsCloth(mCudaContextManager)) return false;
		}

		return true;
	}
	bool PhysicsClothManager::CreateCloth(const PhysicsClothInfo& info)
	{
		std::shared_ptr<CudaClothPhysics> cloth = std::make_shared<CudaClothPhysics>(info.id, info.layerNumber);

		if (!cloth->Initialize(info, mPhysics, mScene, mCudaContextManager))
			return false;

		mPhysicsClothContainer.insert(std::make_pair(info.id, cloth));

		return true;
	}

	bool PhysicsClothManager::RegisterD3D11Buffer(unsigned int id, ID3D11Buffer* clothBuffer)
	{
		auto cloth = mPhysicsClothContainer.find(id);

		if (cloth != mPhysicsClothContainer.end())
		{
			cloth->second->RegisterD3D11BufferWithCUDA(clothBuffer);
			return true;
		}

		return false;
	}
}