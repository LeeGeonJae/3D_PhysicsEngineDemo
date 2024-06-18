#include "pch.h"
//#include "CudaClothPhysics.h"
//
//#include <cudamanager/PxCudaContext.h>
//#include <iostream>
//
//#include "kernel.cu"
//
//namespace PhysicsEngine
//{
//	CudaClothPhysics::CudaClothPhysics(unsigned int id, unsigned int layerNumber)
//		: mID(id)
//		, mLayNumber(layerNumber)
//		, mTotalClothMass()
//		, mWorldTransform()
//		, mSprings()
//		, mPBDMaterial(nullptr)
//		, mParticleSystem(nullptr)
//		, mClothBuffer(nullptr)
//		, mClothBufferHelper(nullptr)
//		, mPhase(nullptr)
//		, mPositionInvMass(nullptr)
//		, mVelocity(nullptr)
//		, mVertices()
//		, mIndices()
//	{
//	}
//	CudaClothPhysics::~CudaClothPhysics()
//	{
//	}
//
//	bool CudaClothPhysics::Initialize(const PhysicsClothInfo& info, physx::PxPhysics* physics, physx::PxScene* scene, physx::PxCudaContextManager* cudaContextManager)
//	{
//		int deviceCount;
//		cudaError_t cudaStatus = cudaGetDeviceCount(&deviceCount)
//		if (cudaStatus != cudaSuccess || deviceCount == 0) {
//			std::cerr << "CUDA ����̽� �ʱ�ȭ ����" << std::endl;
//			return false;
//		}
//
//		cudaStatus = cudaSetDevice(0); // ù ��° CUDA ����̽� ���
//		if (cudaStatus != cudaSuccess) {
//			std::cerr << "CUDA ����̽� ���� ����" << std::endl;
//			return false;
//		}
//
//		if (cudaContextManager == nullptr)
//			return false;
//
//		settingInfoData(info);
//		extractSpringsData(info.indices, info.indexSize);
//
//		// ���(Material) ����
//		mPBDMaterial = physics->createPBDMaterial(
//			info.materialInfo.friction,
//			info.materialInfo.damping,
//			info.materialInfo.adhesion,
//			info.materialInfo.viscosity,
//			info.materialInfo.vorticityConfinement,
//			info.materialInfo.surfaceTension,
//			info.materialInfo.cohesion,
//			info.materialInfo.lift,
//			info.materialInfo.drag,
//			info.materialInfo.cflCoefficient,
//			info.materialInfo.gravityScale);
//
//		createClothParticle(physics, scene, cudaContextManager);
//
//		return true;
//	}
//
//	bool areVerticesEqual(const DirectX::SimpleMath::Vector3& vertex1, const DirectX::SimpleMath::Vector3& vertex2, float epsilon = 1e-6) {
//		return (std::abs(vertex1.x - vertex2.x) < epsilon) &&
//			(std::abs(vertex1.y - vertex2.y) < epsilon) &&
//			(std::abs(vertex1.z - vertex2.z) < epsilon);
//	}
//
//	void CudaClothPhysics::extractSpringsData(unsigned int* indices, unsigned int indexSize)
//	{
//		// �ﰢ�� ������ �ε����� ��ȸ
//		for (size_t i = 0; i < indexSize; i += 3)
//		{
//			unsigned int v1 = indices[i];
//			unsigned int v2 = indices[i + 1];
//			unsigned int v3 = indices[i + 2];
//
//			// ���ĵ� ������ vertex ���� �߰��Ͽ� �ߺ��� ���� ����
//			auto addEdge = [this](unsigned int a, unsigned int b)
//				{
//					if (a > b) std::swap(a, b);
//					mSprings.insert({ a, b });
//				};
//
//			addEdge(v1, v2);
//			addEdge(v2, v3);
//			addEdge(v3, v1);
//		}
//
//		mSameVertices.reserve(mVertices.size() / 3);
//		for (int i = 0; i < mVertices.size(); i++)
//		{
//			for (int j = i + 1; j < mVertices.size(); j++)
//			{
//				if (areVerticesEqual(mVertices[i], mVertices[j]))
//				{
//					mSprings.insert({ i, j });
//					mSameVertices.push_back({ i, j });
//				}
//			}
//		}
//	}
//
//	void CudaClothPhysics::settingInfoData(const PhysicsClothInfo& info)
//	{
//		mWorldTransform = info.worldTransform;
//		mTotalClothMass = info.totalClothMass;
//
//		mIndices.resize(info.indexSize);
//		memcpy(mIndices.data(), info.indices, info.indexSize * sizeof(unsigned int));
//
//		mVertices.resize(info.vertexSize);
//		mUV.resize(info.vertexSize);
//		for (int i = 0; i < info.vertexSize; i++)
//		{
//			mVertices[i].x = info.vertices[i].x;
//			mVertices[i].y = info.vertices[i].y;
//			mVertices[i].z = -info.vertices[i].z;
//			mUV[i] = info.uv[i];
//		}
//	}
//
//	void CudaClothPhysics::createClothParticle(physx::PxPhysics* physics, physx::PxScene* scene, physx::PxCudaContextManager* cudaContextManager)
//	{
//		// ���� �� ������, �ﰢ���� ���� ���
//		const physx::PxU32 numParticles = mVertices.size();	// ���� ����
//		const physx::PxU32 numSprings = mSprings.size();	// ���� �ϳ��� �̿��ϴ� ���ڵ鿡 ������ ���� �����µ�, �� ������ ����
//		const physx::PxU32 numTriangles = mIndices.size() / 3;	// �ﰢ�� ����
//
//		// ���� �ý����� ����
//		const physx::PxReal particleMass = mTotalClothMass / mVertices.size();
//		const physx::PxReal restOffset = 2.f;
//
//		// ���� �ý��� ����
//		mParticleSystem = physics->createPBDParticleSystem(*cudaContextManager);
//
//		mParticleSystem->setRestOffset(1.f);
//		mParticleSystem->setContactOffset(restOffset + 0.02f);
//		mParticleSystem->setParticleContactOffset(restOffset + 0.02f);
//		mParticleSystem->setSolidRestOffset(restOffset);
//
//		// ���� ���� �ý��� �߰�
//		scene->addActor(*mParticleSystem);
//
//		// ������ ���¸� �����ϴ� ���� ����
//		const physx::PxU32 particlePhase = mParticleSystem->createPhase(mPBDMaterial, physx::PxParticlePhaseFlags(
//			physx::PxParticlePhaseFlag::eParticlePhaseSelfCollideFilter | physx::PxParticlePhaseFlag::eParticlePhaseSelfCollide));
//
//		mClothBufferHelper = physx::ExtGpu::PxCreateParticleClothBufferHelper(1, numTriangles, numSprings, numParticles, cudaContextManager);
//		// ����, ������ �ﰢ���� ���¸� �����ϱ� ���� ���� �Ҵ�
//		mPhase = cudaContextManager->allocPinnedHostBuffer<physx::PxU32>(numParticles);
//		mPositionInvMass = cudaContextManager->allocPinnedHostBuffer<physx::PxVec4>(numParticles);
//		mVelocity = cudaContextManager->allocPinnedHostBuffer<physx::PxVec4>(numParticles);
//
//		settingParticleBuffer(numSprings, numTriangles, numParticles, particlePhase, particleMass);
//		createCloth(numParticles, cudaContextManager);
//	}
//
//	float calculateVectorMagnitude(const DirectX::SimpleMath::Vector3& point1, const DirectX::SimpleMath::Vector3& point2) {
//		float dx = point2.x - point1.x;
//		float dy = point2.y - point1.y;
//		float dz = point2.z - point1.z;
//
//		return std::sqrt(dx * dx + dy * dy + dz * dz);
//	}
//
//	void CudaClothPhysics::settingParticleBuffer(
//		const physx::PxU32& numSprings,
//		const physx::PxU32& numTriangles,
//		const physx::PxU32& numParticles,
//		const physx::PxU32& particlePhase,
//		const physx::PxReal& particleMass)
//	{
//		const physx::PxReal stretchStiffness = 100.f;
//		const physx::PxReal shearStiffness = 100.f;
//		const physx::PxReal springDamping = 0.1f;
//
//		// ��ƼŬ ������ �� Ʈ���̾ޱ� ����
//		physx::PxArray<physx::PxParticleSpring> springs;
//		springs.reserve(numSprings);
//		physx::PxArray<physx::PxU32> triangles;
//		triangles.reserve(numTriangles * 3);
//
//		// ���� ���� ����
//		for (int i = 0; i < numParticles; i++)
//		{
//			mPositionInvMass[i] = physx::PxVec4(mVertices[i].x, mVertices[i].y + 300.f, mVertices[i].z, 1.f / particleMass);
//			mPhase[i] = particlePhase;
//			mVelocity[i] = physx::PxVec4(0.f);
//		}
//
//		// ������ �߰�
//		for (auto line : mSprings)
//		{
//			physx::PxParticleSpring spring = { line.first, line.second, calculateVectorMagnitude(mVertices[line.first], mVertices[line.second]), stretchStiffness, springDamping, 0 };
//			springs.pushBack(spring);
//		}
//
//		// �ﰢ�� �߰�
//		for (int i = 0; i < mIndices.size(); i += 3)
//		{
//			triangles.pushBack(mIndices[i]);
//			triangles.pushBack(mIndices[i + 1]);
//			triangles.pushBack(mIndices[i + 2]);
//		}
//
//		// ������ ������ �� �ﰢ�� ���� ������ �����Ǿ����� Ȯ��
//		PX_ASSERT(numSprings == springs.size());
//		PX_ASSERT(numTriangles == triangles.size() / 3);
//
//		// õ���� ���ۿ� ������ �߰�
//		mClothBufferHelper->addCloth(0.f, 0.f, 0.f, triangles.begin(), numTriangles, springs.begin(), numSprings, mPositionInvMass, numParticles);
//	}
//
//	void CudaClothPhysics::createCloth(const physx::PxU32& numParticles, physx::PxCudaContextManager* cudaContextManager)
//	{
//		// ������ ���¸� ��Ÿ���� ���� ����
//		physx::ExtGpu::PxParticleBufferDesc bufferDesc;
//		bufferDesc.maxParticles = numParticles;
//		bufferDesc.numActiveParticles = numParticles;
//		bufferDesc.positions = mPositionInvMass;
//		bufferDesc.velocities = mVelocity;
//		bufferDesc.phases = mPhase;
//
//		// õ���� ���� ��������
//		const physx::PxParticleClothDesc& clothDesc = mClothBufferHelper->getParticleClothDesc();
//
//		// ���� õ���� ��ó���� ����
//		physx::PxParticleClothPreProcessor* clothPreProcessor = PxCreateParticleClothPreProcessor(cudaContextManager);
//
//		// ���� õ�� ���� �� ó��
//		physx::PxPartitionedParticleCloth output;
//		clothPreProcessor->partitionSprings(clothDesc, output);
//		clothPreProcessor->release();
//
//		// õ�� ���� ����
//		mClothBuffer = physx::ExtGpu::PxCreateAndPopulateParticleClothBuffer(bufferDesc, clothDesc, output, cudaContextManager);
//		mParticleSystem->addParticleBuffer(mClothBuffer);
//
//		// ���� ����
//		mClothBufferHelper->release();
//
//		// �Ҵ�� �޸� ����
//		cudaContextManager->freePinnedHostBuffer(mPositionInvMass);
//		cudaContextManager->freePinnedHostBuffer(mVelocity);
//		cudaContextManager->freePinnedHostBuffer(mPhase);
//	}
//
//	bool CudaClothPhysics::RegisterD3D11BufferWithCUDA(ID3D11Buffer* buffer)
//	{
//		cudaError_t cudaStatus = cudaGraphicsD3D11RegisterResource(&mCudaResource, buffer, cudaGraphicsRegisterFlagsNone);
//		if (cudaStatus != cudaSuccess) 
//		{
//			std::cerr << "Direct3D ���ҽ� ��� ����" << std::endl;
//			return false;
//		}
//		return true;
//	}
//
//	bool CudaClothPhysics::UpdatePhysicsCloth()
//	{
//		// CUDA ���ҽ��� ����
//		cudaGraphicsMapResources(1, &mCudaResource);
//
//		// CUDA ������ ��������
//		void* devPtr = nullptr;
//		size_t size = 0;
//		cudaGraphicsResourceGetMappedPointer(&devPtr, &size, mCudaResource);
//
//		unsigned int deviceVertexSize = mVertices.size();
//		unsigned int deviceIndexSize = mIndices.size();
//
//		float3* d_vertices;
//		float2* d_uvs;
//		unsigned int* d_indices;
//
//		cudaMalloc(&d_vertices, mVertices.size() * sizeof(DirectX::SimpleMath::Vector3));
//		cudaMalloc(&d_uvs, mVertices.size() * sizeof(DirectX::SimpleMath::Vector2));
//		cudaMalloc(&d_indices, mIndices.size() * sizeof(unsigned int));
//
//		cudaMemcpy(d_vertices, mVertices.data(), mVertices.size() * sizeof(DirectX::SimpleMath::Vector3), cudaMemcpyKind::cudaMemcpyHostToDevice);
//		cudaMemcpy(d_uvs, mUV.data(), mUV.size() * sizeof(DirectX::SimpleMath::Vector2), cudaMemcpyKind::cudaMemcpyHostToDevice);
//		cudaMemcpy(d_indices, mIndices.data(), mIndices.size() * sizeof(unsigned int), cudaMemcpyKind::cudaMemcpyHostToDevice);
//
//		int threadsPerBlock = 256;
//		int blocksPerGrid = (mIndices.size() / 3 + threadsPerBlock - 1) / threadsPerBlock;
//
//		// CUDA Ŀ�� ȣ��
//		launchCalculateNormals(
//			d_vertices,
//			d_uvs,
//			deviceVertexSize,
//			d_indices,
//			deviceIndexSize,
//			(PhysicsVertex*)devPtr);
//
//		// CUDA ���ҽ��� �����
//		cudaGraphicsUnmapResources(1, &mCudaResource);
//
//		// �޸� ����
//		cudaFree(d_vertices);
//		cudaFree(d_uvs);
//		cudaFree(d_indices);
//
//		return false;
//	}
//
//	void CudaClothPhysics::GetPhysicsCloth(physx::PxCudaContextManager* cudaContextManager, physx::PxCudaContext* cudaContext, PhysicsClothGetData& data)
//	{
//	}
//
//	void CudaClothPhysics::SetPhysicsCloth(physx::PxCudaContextManager* cudaContextManager, physx::PxCudaContext* cudaContext, const PhysicsClothSetData& data)
//	{
//
//	}
//}
