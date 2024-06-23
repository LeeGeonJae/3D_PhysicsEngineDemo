#pragma once

#include "Common.h"

#include <memory>
#include <unordered_map>

#include <PxPhysicsAPI.h>
#include <d3d11.h>

namespace physics
{
	class CudaClothPhysics;

	class PhysicsClothManager
	{
	public:
		PhysicsClothManager();
		~PhysicsClothManager();

		/// <summary>
		/// 천 시뮬레이션 매니저 초기화입니다.
		/// </summary>
		bool Initialize(physx::PxCudaContextManager* cudaContextManager, physx::PxPhysics* physics, physx::PxScene* scene);

		bool Update();

		bool CreateCloth(const PhysicsClothInfo& info);
		bool RegisterD3D11Buffer(unsigned int id, ID3D11Buffer* clothBuffer);

		bool GetClothData(const unsigned int& id, PhysicsClothGetData& data);
		bool SetClothData(const unsigned int& id, const  PhysicsClothSetData& data);

	private:
		std::unordered_map<unsigned int, std::shared_ptr<CudaClothPhysics>> mPhysicsClothContainer;

		physx::PxPhysics*				mPhysics;
		physx::PxScene*					mScene;
		physx::PxCudaContextManager*	mCudaContextManager;
	};
}