#pragma once

#include "Common.h"

#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <set>
#include <directxtk\SimpleMath.h>
#include <PxPhysicsAPI.h>

#include <cuda_runtime.h>
#include <cuda_d3d11_interop.h>
#include <device_launch_parameters.h>

#include <extensions/PxParticleExt.h>
#include "d3d11.h"

namespace physics
{
	class CudaClothPhysics
	{
	public:
		CudaClothPhysics(unsigned int id, unsigned int layerNumber);
		~CudaClothPhysics();

		bool Initialize(const PhysicsClothInfo& info, physx::PxPhysics* physics, physx::PxScene* scene, physx::PxCudaContextManager* cudaContextManager);

		bool RegisterD3D11VertexBufferWithCUDA(ID3D11Buffer* buffer);
		bool RegisterD3D11IndexBufferWithCUDA(ID3D11Buffer* buffer);
		bool UpdatePhysicsCloth(physx::PxCudaContextManager* cudaContextManager);

		void GetPhysicsCloth(PhysicsClothGetData& data);
		void SetPhysicsCloth(const PhysicsClothSetData& data);

		inline void SetWorldTransform(const DirectX::SimpleMath::Matrix& position);
		inline void SetTotalClothMass(const float& toTalClothMass);
		inline const DirectX::SimpleMath::Matrix& GetWorldTransform() const;
		inline const float& GetTotalClothMass() const;
		inline const physx::PxPBDParticleSystem* GetParticleSystem() const;
		inline const physx::PxParticleClothBuffer* GetClothBuffer() const;
		inline const physx::ExtGpu::PxParticleClothBufferHelper* GetParticleClothBufferHelper() const;
		inline const std::set<std::pair<unsigned int, unsigned int>>& GetSprings() const;
		inline const unsigned int& GetVertexSize() const;
		inline const unsigned int& GetIndexSize() const;
		inline cudaGraphicsResource* GetCudaGraphicsResource() const;

	public:
		// cuda ÇÔ¼ö
		void CombineVertex();

	private:
		void extractSpringsData(unsigned int* indices, unsigned int indexSize);
		void settingInfoData(const PhysicsClothInfo& info);
		void createClothParticle(physx::PxPhysics* physics, physx::PxScene* scene, physx::PxCudaContextManager* cudaContextManager);
		void settingParticleBuffer(
			const physx::PxU32& numSprings,
			const physx::PxU32& numTriangles,
			const physx::PxU32& numParticles,
			const physx::PxU32& particlePhase,
			const physx::PxReal& particleMass);
		void createCloth(const physx::PxU32& numParticles, physx::PxCudaContextManager* cudaContextManager);

	private:
		unsigned int	mID;
		unsigned int	mLayNumber;

		float			mTotalClothMass;
		DirectX::SimpleMath::Matrix mWorldTransform;
		std::set<std::pair<unsigned int, unsigned int>> mSprings;
		std::vector<std::pair<unsigned int, unsigned int>> mSameVertices;

		physx::PxPBDMaterial* mPBDMaterial;
		physx::PxPBDParticleSystem* mParticleSystem;
		physx::PxParticleClothBuffer* mClothBuffer;
		physx::ExtGpu::PxParticleClothBufferHelper* mClothBufferHelper;

		physx::PxU32* mPhase;
		physx::PxVec4* mPositionInvMass;
		physx::PxVec4* mVelocity;
		std::vector<DirectX::SimpleMath::Vector3> mVertices;
		std::vector<DirectX::SimpleMath::Vector3> mWeldVertices;
		std::vector<DirectX::SimpleMath::Vector2> mUV;
		std::vector<DirectX::SimpleMath::Vector2> mWeldUV;
		std::vector<unsigned int> mIndices;
		std::vector<unsigned int> mWeldIndices;
		unsigned int mOverlapVertexCount;

		cudaGraphicsResource* mCudaVertexResource;
		cudaGraphicsResource* mCudaIndexResource;
	};

#pragma region GetSet
	void CudaClothPhysics::SetWorldTransform(const DirectX::SimpleMath::Matrix& position)
	{
		mWorldTransform = position;
	}
	void CudaClothPhysics::SetTotalClothMass(const float& toTalClothMass)
	{
		mTotalClothMass = toTalClothMass;
	}
	const DirectX::SimpleMath::Matrix& CudaClothPhysics::GetWorldTransform() const
	{
		return mWorldTransform;
	}
	const float& CudaClothPhysics::GetTotalClothMass() const
	{
		return mTotalClothMass;
	}
	const physx::PxPBDParticleSystem* CudaClothPhysics::GetParticleSystem() const
	{
		return mParticleSystem;
	}
	const physx::PxParticleClothBuffer* CudaClothPhysics::GetClothBuffer() const
	{
		return mClothBuffer;
	}
	const physx::ExtGpu::PxParticleClothBufferHelper* CudaClothPhysics::GetParticleClothBufferHelper() const
	{
		return mClothBufferHelper;
	}
	const std::set<std::pair<unsigned int, unsigned int>>& CudaClothPhysics::GetSprings() const
	{
		return mSprings;
	}
	const unsigned int& CudaClothPhysics::GetVertexSize() const
	{
		return mVertices.size();
	}
	const unsigned int& CudaClothPhysics::GetIndexSize() const
	{
		return mIndices.size();
	}
	cudaGraphicsResource* CudaClothPhysics::GetCudaGraphicsResource() const
	{
		return mCudaVertexResource;
	}
#pragma endregion
}

