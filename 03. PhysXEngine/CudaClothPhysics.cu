#include "CudaClothPhysics.h"
#include "cudamanager\PxCudaContext.h"

#include <cuda_runtime.h>
#include <iostream>
#include <math.h>

#include <directxtk\SimpleMath.h>

#include "EngineDataConverter.h"

#pragma comment (lib, "cudart.lib")

__device__ physx::PxVec2 Sub(const physx::PxVec2& lhs, const physx::PxVec2& rhs) {
	return { lhs.x - rhs.x, lhs.y - rhs.y };
}

__device__ physx::PxVec4 Sub(const physx::PxVec4& lhs, const physx::PxVec4& rhs) {
	return { lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w };
}

__device__ physx::PxVec3 cross(const physx::PxVec4& a, const physx::PxVec4& b) {
	return { a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x };
}

__device__ float DotProduct(const physx::PxVec3& a, const physx::PxVec3& b) {
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

__device__ void NormalizeVector(physx::PxVec3& vec) {
	float length = sqrtf(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
	if (length > 0) {
		vec.x /= length;
		vec.y /= length;
		vec.z /= length;
	}
}

// CUDA Ŀ�� �Լ� ����
__global__ void CalculateNormals(
	physx::PxVec4* vertices,
	physx::PxVec2* uvs,
    unsigned int vertexSize,
    unsigned int* indices,
    unsigned int indexSize,
    physics::PhysicsVertex* buffer)
{
    int idx = blockIdx.x * blockDim.x + threadIdx.x;
    if (idx >= indexSize / 3) return;

    unsigned int i0 = indices[idx * 3];
    unsigned int i1 = indices[idx * 3 + 1];
    unsigned int i2 = indices[idx * 3 + 2];

    physx::PxVec4 v0 = vertices[i0];
    physx::PxVec4 v1 = vertices[i1];
    physx::PxVec4 v2 = vertices[i2];
	v0.z = -v0.z;
	v1.z = -v1.z;
	v2.z = -v2.z;

    physx::PxVec2 uv0 = uvs[i0];
    physx::PxVec2 uv1 = uvs[i1];
    physx::PxVec2 uv2 = uvs[i2];

    physx::PxVec4 edge1 = Sub(v1, v0);
    physx::PxVec4 edge2 = Sub(v2, v0);

	physx::PxVec2 deltaUV1 = Sub(uv1, uv0);
	physx::PxVec2 deltaUV2 = Sub(uv2, uv0);

    float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

	physx::PxVec3 tangent;
    tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
    tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
    tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
    NormalizeVector(tangent);

	physx::PxVec3 bitangent;
    bitangent.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
    bitangent.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
    bitangent.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
    NormalizeVector(bitangent);

	physx::PxVec3 normal = cross(edge1, edge2);
    NormalizeVector(normal);

	buffer[i0].position.x = v0.x;
	buffer[i0].position.y = v0.y;
	buffer[i0].position.z = v0.z;
	buffer[i1].position.x = v1.x;
	buffer[i1].position.y = v1.y;
	buffer[i1].position.z = v1.z;
	buffer[i2].position.x = v2.x;
	buffer[i2].position.y = v2.y;
	buffer[i2].position.z = v2.z;

	buffer[i0].normal.x = normal.x;
	buffer[i0].normal.y = normal.y;
	buffer[i0].normal.z = normal.z;
					    
	buffer[i1].normal.x = normal.x;
	buffer[i1].normal.y = normal.y;
	buffer[i1].normal.z = normal.z;
					    
	buffer[i2].normal.x = normal.x;
	buffer[i2].normal.y = normal.y;
	buffer[i2].normal.z = normal.z;

	buffer[i0].tangent.x = tangent.x;
	buffer[i0].tangent.y = tangent.y;
	buffer[i0].tangent.z = tangent.z;
						 
	buffer[i1].tangent.x = tangent.x;
	buffer[i1].tangent.y = tangent.y;
	buffer[i1].tangent.z = tangent.z;
						 
	buffer[i2].tangent.x = tangent.x;
	buffer[i2].tangent.y = tangent.y;
	buffer[i2].tangent.z = tangent.z;

	buffer[i0].biTangent.x = bitangent.x;
	buffer[i0].biTangent.y = bitangent.y;
	buffer[i0].biTangent.z = bitangent.z;
						   
	buffer[i1].biTangent.x = bitangent.x;
	buffer[i1].biTangent.y = bitangent.y;
	buffer[i1].biTangent.z = bitangent.z;
						   
	buffer[i2].biTangent.x = bitangent.x;
	buffer[i2].biTangent.y = bitangent.y;
	buffer[i2].biTangent.z = bitangent.z;
}

__global__ void processVerticesKernel(unsigned int* sameVerticesFirst, unsigned int* sameVerticesSecond,
	physics::PhysicsVertex* buffer, int size) {
	int idx = blockIdx.x * blockDim.x + threadIdx.x;

	if (idx < size) {
		int first = sameVerticesFirst[idx];
		int second = sameVerticesSecond[idx];

		// �߰����� ����Ͽ� ���� ������ �и�
		float3 normalTemp, tangentTemp, biTangentTemp;

		normalTemp.x = buffer[first].normal.x + buffer[second].normal.x;
		normalTemp.y = buffer[first].normal.y + buffer[second].normal.y;
		normalTemp.z = buffer[first].normal.z + buffer[second].normal.z;

		tangentTemp.x = buffer[first].tangent.x + buffer[second].tangent.x;
		tangentTemp.y = buffer[first].tangent.y + buffer[second].tangent.y;
		tangentTemp.z = buffer[first].tangent.z + buffer[second].tangent.z;

		biTangentTemp.x = buffer[first].biTangent.x + buffer[second].biTangent.x;
		biTangentTemp.y = buffer[first].biTangent.y + buffer[second].biTangent.y;
		biTangentTemp.z = buffer[first].biTangent.z + buffer[second].biTangent.z;

		buffer[first].normal.x = normalTemp.x;
		buffer[first].normal.y = normalTemp.y;
		buffer[first].normal.z = normalTemp.z;
		buffer[second].normal.x = normalTemp.x;
		buffer[second].normal.y = normalTemp.y;
		buffer[second].normal.z = normalTemp.z;

		buffer[first].tangent.x = tangentTemp.x;
		buffer[first].tangent.y = tangentTemp.y;
		buffer[first].tangent.z = tangentTemp.z;
		buffer[second].tangent.x = tangentTemp.x;
		buffer[second].tangent.y = tangentTemp.y;
		buffer[second].tangent.z = tangentTemp.z;

		buffer[first].biTangent.x = biTangentTemp.x;
		buffer[first].biTangent.y = biTangentTemp.y;
		buffer[first].biTangent.z = biTangentTemp.z;
		buffer[second].biTangent.x = biTangentTemp.x;
		buffer[second].biTangent.y = biTangentTemp.y;
		buffer[second].biTangent.z = biTangentTemp.z;
	}
}

struct SimpleVector3 {
	float x, y, z;
};

struct SimpleMatrix {
	float m[4][4];
};

__device__ SimpleVector3 multiply(SimpleMatrix& mat, const SimpleVector3& vec)
{
	SimpleVector3 result;
	result.x = mat.m[0][0] * vec.x + mat.m[1][0] * vec.y + mat.m[2][0] * vec.z + mat.m[3][0] * 1.0f;
	result.y = mat.m[0][1] * vec.x + mat.m[1][1] * vec.y + mat.m[2][1] * vec.z + mat.m[3][1] * 1.0f;
	result.z = mat.m[0][2] * vec.x + mat.m[1][2] * vec.y + mat.m[2][2] * vec.z + mat.m[3][2] * 1.0f;
	return result;
}

// Ŀ�� �Լ�
__global__ void TransformVertices(physx::PxVec4* particle, SimpleMatrix previousTransformInverse, SimpleMatrix newTransform, int vertexCount)
{
	int idx = blockIdx.x * blockDim.x + threadIdx.x;
	if (idx >= vertexCount) return;

	SimpleVector3 vertex;
	vertex.x = particle[idx].x;
	vertex.y = particle[idx].y;
	vertex.z = -particle[idx].z;

	// ���� worldTransform�� ����� ����
	vertex = multiply(previousTransformInverse, vertex);

	// ���ο� worldTransform ����
	vertex = multiply(newTransform, vertex);

	// ��ȯ�� vertex ����
	particle[idx].x = vertex.x;
	particle[idx].y = vertex.y;
	particle[idx].z = -vertex.z;
}

namespace physics
{
	CudaClothPhysics::CudaClothPhysics(unsigned int id, unsigned int layerNumber)
		: mID(id)
		, mLayNumber(layerNumber)
		, mTotalClothMass()
		, mWorldTransform()
		, mSprings()
		, mPBDMaterial(nullptr)
		, mParticleSystem(nullptr)
		, mClothBuffer(nullptr)
		, mClothBufferHelper(nullptr)
		, mPhase(nullptr)
		, mPositionInvMass(nullptr)
		, mVelocity(nullptr)
		, mVertices()
		, mIndices()
	{
	}
	CudaClothPhysics::~CudaClothPhysics()
	{
	}

	bool CudaClothPhysics::Initialize(const PhysicsClothInfo& info, physx::PxPhysics* physics, physx::PxScene* scene, physx::PxCudaContextManager* cudaContextManager)
	{
		int deviceCount;
		cudaError_t cudaStatus = cudaGetDeviceCount(&deviceCount);
		if (cudaStatus != cudaSuccess || deviceCount == 0) {
			std::cerr << "CUDA ����̽� �ʱ�ȭ ����" << std::endl;
			return false;
		}

		cudaStatus = cudaSetDevice(0); // ù ��° CUDA ����̽� ���
		if (cudaStatus != cudaSuccess) {
			std::cerr << "CUDA ����̽� ���� ����" << std::endl;
			return false;
		}

		if (cudaContextManager == nullptr)
			return false;

		settingInfoData(info);
		extractSpringsData(info.indices, info.indexSize);

		// ���(Material) ����
		mPBDMaterial = physics->createPBDMaterial(
			info.materialInfo.friction,
			info.materialInfo.damping,
			info.materialInfo.adhesion,
			info.materialInfo.viscosity,
			info.materialInfo.vorticityConfinement,
			info.materialInfo.surfaceTension,
			info.materialInfo.cohesion,
			info.materialInfo.lift,
			info.materialInfo.drag,
			info.materialInfo.cflCoefficient,
			info.materialInfo.gravityScale);

		createClothParticle(physics, scene, cudaContextManager);

		return true;
	}

	bool areVerticesEqual(const DirectX::SimpleMath::Vector3& vertex1, const DirectX::SimpleMath::Vector3& vertex2, float epsilon = 1e-6) {
		return (std::abs(vertex1.x - vertex2.x) < epsilon) &&
			(std::abs(vertex1.y - vertex2.y) < epsilon) &&
			(std::abs(vertex1.z - vertex2.z) < epsilon);
	}

	void CudaClothPhysics::extractSpringsData(unsigned int* indices, unsigned int indexSize)
	{
		// �ﰢ�� ������ �ε����� ��ȸ
		for (size_t i = 0; i < indexSize; i += 3)
		{
			unsigned int v1 = indices[i];
			unsigned int v2 = indices[i + 1];
			unsigned int v3 = indices[i + 2];

			// ���ĵ� ������ vertex ���� �߰��Ͽ� �ߺ��� ���� ����
			auto addEdge = [this](unsigned int a, unsigned int b)
				{
					if (a > b) std::swap(a, b);
					mSprings.insert({ a, b });
				};

			addEdge(v1, v2);
			addEdge(v2, v3);
			addEdge(v3, v1);
		}

		mSameVertices.reserve(mVertices.size() / 3);
		for (int i = 0; i < mVertices.size(); i++)
		{
			for (int j = i + 1; j < mVertices.size(); j++)
			{
				if (areVerticesEqual(mVertices[i], mVertices[j]))
				{
					mSprings.insert({ i, j });
					mSameVertices.push_back({ i, j });
				}
			}
		}
	}

	void CudaClothPhysics::settingInfoData(const PhysicsClothInfo& info)
	{
		RegisterD3D11VertexBufferWithCUDA((ID3D11Buffer*)info.vertexBuffer);
		RegisterD3D11IndexBufferWithCUDA((ID3D11Buffer*)info.indexBuffer);

		mWorldTransform = info.worldTransform;
		mTotalClothMass = info.totalClothMass;

		mIndices.resize(info.indexSize);
		memcpy(mIndices.data(), info.indices, info.indexSize * sizeof(unsigned int));

		mUV.resize(info.vertexSize);
		mVertices.resize(info.vertexSize);

		for (int i = 0; i < info.vertexSize; i++)
		{
			mVertices[i].x = info.vertices[i].x;
			mVertices[i].y = info.vertices[i].y;
			mVertices[i].z = info.vertices[i].z;
			mUV[i] = info.uv[i];
		}

		for (auto& vertex : mVertices)
		{
			MulMatrixVector3(mWorldTransform, vertex);
		}

		for (int i = 0; i < info.vertexSize; i++)
		{
			mVertices[i].z = -mVertices[i].z;
		}
	}

	void CudaClothPhysics::createClothParticle(physx::PxPhysics* physics, physx::PxScene* scene, physx::PxCudaContextManager* cudaContextManager)
	{
		// ���� �� ������, �ﰢ���� ���� ���
		const physx::PxU32 numParticles = mVertices.size();	// ���� ����
		const physx::PxU32 numSprings = mSprings.size();	// ���� �ϳ��� �̿��ϴ� ���ڵ鿡 ������ ���� �����µ�, �� ������ ����
		const physx::PxU32 numTriangles = mIndices.size() / 3;	// �ﰢ�� ����

		// ���� �ý����� ����
		const physx::PxReal particleMass = mTotalClothMass / mVertices.size();
		const physx::PxReal restOffset = 2.f;

		// ���� �ý��� ����
		mParticleSystem = physics->createPBDParticleSystem(*cudaContextManager);

		mParticleSystem->setRestOffset(1.f);
		mParticleSystem->setContactOffset(restOffset + 0.02f);
		mParticleSystem->setParticleContactOffset(restOffset + 0.02f);
		mParticleSystem->setSolidRestOffset(restOffset);

		// ���� ���� �ý��� �߰�
		scene->addActor(*mParticleSystem);

		// ������ ���¸� �����ϴ� ���� ����
		const physx::PxU32 particlePhase = mParticleSystem->createPhase(mPBDMaterial, physx::PxParticlePhaseFlags(
			physx::PxParticlePhaseFlag::eParticlePhaseSelfCollideFilter | physx::PxParticlePhaseFlag::eParticlePhaseSelfCollide));

		mClothBufferHelper = physx::ExtGpu::PxCreateParticleClothBufferHelper(1, numTriangles, numSprings, numParticles, cudaContextManager);
		// ����, ������ �ﰢ���� ���¸� �����ϱ� ���� ���� �Ҵ�
		mPhase = cudaContextManager->allocPinnedHostBuffer<physx::PxU32>(numParticles);
		mPositionInvMass = cudaContextManager->allocPinnedHostBuffer<physx::PxVec4>(numParticles);
		mVelocity = cudaContextManager->allocPinnedHostBuffer<physx::PxVec4>(numParticles);

		settingParticleBuffer(numSprings, numTriangles, numParticles, particlePhase, particleMass);
		createCloth(numParticles, cudaContextManager);
	}

	float calculateVectorMagnitude(const DirectX::SimpleMath::Vector3& point1, const DirectX::SimpleMath::Vector3& point2) {
		float dx = point2.x - point1.x;
		float dy = point2.y - point1.y;
		float dz = point2.z - point1.z;

		return std::sqrt(dx * dx + dy * dy + dz * dz);
	}

	void CudaClothPhysics::settingParticleBuffer(
		const physx::PxU32& numSprings,
		const physx::PxU32& numTriangles,
		const physx::PxU32& numParticles,
		const physx::PxU32& particlePhase,
		const physx::PxReal& particleMass)
	{
		const physx::PxReal stretchStiffness = 100.f;
		const physx::PxReal shearStiffness = 100.f;
		const physx::PxReal springDamping = 0.1f;

		// ��ƼŬ ������ �� Ʈ���̾ޱ� ����
		physx::PxArray<physx::PxParticleSpring> springs;
		springs.reserve(numSprings);
		physx::PxArray<physx::PxU32> triangles;
		triangles.reserve(numTriangles * 3);

		// ���� ���� ����
		for (int i = 0; i < numParticles; i++)
		{
			mPositionInvMass[i] = physx::PxVec4(mVertices[i].x, mVertices[i].y + 300.f, mVertices[i].z, 1.f / particleMass);
			mPhase[i] = particlePhase;
			mVelocity[i] = physx::PxVec4(0.f);
		}

		// ������ �߰�
		for (auto line : mSprings)
		{
			physx::PxParticleSpring spring = { line.first, line.second, calculateVectorMagnitude(mVertices[line.first], mVertices[line.second]), stretchStiffness, springDamping, 0 };
			springs.pushBack(spring);
		}

		// �ﰢ�� �߰�
		for (int i = 0; i < mIndices.size(); i += 3)
		{
			triangles.pushBack(mIndices[i]);
			triangles.pushBack(mIndices[i + 1]);
			triangles.pushBack(mIndices[i + 2]);
		}

		// ������ ������ �� �ﰢ�� ���� ������ �����Ǿ����� Ȯ��
		PX_ASSERT(numSprings == springs.size());
		PX_ASSERT(numTriangles == triangles.size() / 3);

		// õ���� ���ۿ� ������ �߰�
		mClothBufferHelper->addCloth(0.f, 0.f, 0.f, triangles.begin(), numTriangles, springs.begin(), numSprings, mPositionInvMass, numParticles);
	}

	void CudaClothPhysics::createCloth(const physx::PxU32& numParticles, physx::PxCudaContextManager* cudaContextManager)
	{
		// ������ ���¸� ��Ÿ���� ���� ����
		physx::ExtGpu::PxParticleBufferDesc bufferDesc;
		bufferDesc.maxParticles = numParticles;
		bufferDesc.numActiveParticles = numParticles;
		bufferDesc.positions = mPositionInvMass;
		bufferDesc.velocities = mVelocity;
		bufferDesc.phases = mPhase;

		// õ���� ���� ��������
		const physx::PxParticleClothDesc& clothDesc = mClothBufferHelper->getParticleClothDesc();

		// ���� õ���� ��ó���� ����
		physx::PxParticleClothPreProcessor* clothPreProcessor = PxCreateParticleClothPreProcessor(cudaContextManager);

		// ���� õ�� ���� �� ó��
		physx::PxPartitionedParticleCloth output;
		clothPreProcessor->partitionSprings(clothDesc, output);
		clothPreProcessor->release();

		// õ�� ���� ����
		mClothBuffer = physx::ExtGpu::PxCreateAndPopulateParticleClothBuffer(bufferDesc, clothDesc, output, cudaContextManager);
		mParticleSystem->addParticleBuffer(mClothBuffer);

		// ���� ����
		mClothBufferHelper->release();

		// �Ҵ�� �޸� ����
		//cudaContextManager->freePinnedHostBuffer(mPositionInvMass);
		cudaContextManager->freePinnedHostBuffer(mVelocity);
		cudaContextManager->freePinnedHostBuffer(mPhase);
	}

	bool CudaClothPhysics::RegisterD3D11VertexBufferWithCUDA(ID3D11Buffer* buffer)
	{
		cudaError_t cudaStatus = cudaGraphicsD3D11RegisterResource(&mCudaVertexResource, buffer, cudaGraphicsRegisterFlagsNone);
		if (cudaStatus != cudaSuccess)
		{
			std::cerr << "Direct3D ���ҽ� ��� ����" << std::endl;
			return false;
		}
		return true;
	}

	bool CudaClothPhysics::RegisterD3D11IndexBufferWithCUDA(ID3D11Buffer* buffer)
	{
		cudaError_t cudaStatus = cudaGraphicsD3D11RegisterResource(&mCudaIndexResource, buffer, cudaGraphicsRegisterFlagsNone);
		if (cudaStatus != cudaSuccess)
		{
			std::cerr << "Direct3D ���ҽ� ��� ����" << std::endl;
			return false;
		}
		return true;
	}

	bool CudaClothPhysics::UpdatePhysicsCloth(physx::PxCudaContextManager* cudaContextManager)
	{
		// CUDA ���ҽ��� ����
		cudaError_t cudaStatus = cudaGraphicsMapResources(1, &mCudaVertexResource);
		//if (cudaStatus != cudaSuccess) {
		//	std::cerr << "cudaGraphicsMapResources ����: " << cudaGetErrorString(cudaStatus) << std::endl;
		//	return false;
		//}

		// CUDA ������ ��������
		void* devPtr = nullptr;
		size_t size = 0;
		cudaGraphicsResourceGetMappedPointer(&devPtr, &size, mCudaVertexResource);
		//if (cudaStatus != cudaSuccess) {
		//	std::cerr << "cudaGraphicsResourceGetMappedPointer ����: " << cudaGetErrorString(cudaStatus) << std::endl;
		//	return false;
		//}

		unsigned int deviceVertexSize = mVertices.size();
		unsigned int deviceIndexSize = mIndices.size();

		physx::PxVec2* d_uvs;
		unsigned int* d_indices;

		cudaMalloc(&d_uvs, mUV.size() * sizeof(DirectX::SimpleMath::Vector2));
		cudaMalloc(&d_indices, mIndices.size() * sizeof(unsigned int));
		cudaMemcpy(d_uvs, mUV.data(), mUV.size() * sizeof(DirectX::SimpleMath::Vector2), cudaMemcpyKind::cudaMemcpyHostToDevice);
		cudaMemcpy(d_indices, mIndices.data(), mIndices.size() * sizeof(unsigned int), cudaMemcpyKind::cudaMemcpyHostToDevice);


		int particleSize = mClothBuffer->getNbActiveParticles();
		physx::PxVec4* particle = mClothBuffer->getPositionInvMasses();

		int threadsPerBlock = 256;
		int blocksPerGrid = (mIndices.size() / 3 + threadsPerBlock - 1) / threadsPerBlock;

		// CUDA �Լ� ����
		CalculateNormals <<<blocksPerGrid, threadsPerBlock>>> (
			particle, d_uvs, deviceVertexSize, d_indices, deviceIndexSize, (PhysicsVertex*)devPtr);

		blocksPerGrid = (size + threadsPerBlock - 1) / threadsPerBlock;

		std::vector<unsigned int> firstVertex;
		std::vector<unsigned int> secondVertex;
		firstVertex.resize(mSameVertices.size());
		secondVertex.resize(mSameVertices.size());
		for (int i = 0; i < mSameVertices.size(); i++)
		{
			firstVertex.push_back(mSameVertices[i].first);
			secondVertex.push_back(mSameVertices[i].second);
		}

		unsigned int* d_firstVertex;
		unsigned int* d_secondVertex;

		cudaMalloc(&d_firstVertex, firstVertex.size() * sizeof(unsigned int));
		cudaMalloc(&d_secondVertex, secondVertex.size() * sizeof(unsigned int));
		cudaMemcpy(d_firstVertex, firstVertex.data(), firstVertex.size() * sizeof(unsigned int), cudaMemcpyKind::cudaMemcpyHostToDevice);
		cudaMemcpy(d_secondVertex, secondVertex.data(), secondVertex.size() * sizeof(unsigned int), cudaMemcpyKind::cudaMemcpyHostToDevice);

		processVerticesKernel << <blocksPerGrid, threadsPerBlock >> > (d_firstVertex, d_secondVertex, (PhysicsVertex*)devPtr, mVertices.size());

		cudaDeviceSynchronize();

		// CUDA ���ҽ��� �����
		cudaGraphicsUnmapResources(1, &mCudaVertexResource);

		// �޸� ����
		cudaFree(d_uvs);
		cudaFree(d_indices);
		cudaFree(d_firstVertex);
		cudaFree(d_secondVertex);

		return false;
	}

	void CudaClothPhysics::GetPhysicsCloth(PhysicsClothGetData& data)
	{
		data.worldTransform = mWorldTransform;
	}

	void CudaClothPhysics::SetPhysicsCloth(const PhysicsClothSetData& data)
	{
		DirectX::SimpleMath::Matrix prevTransform = mWorldTransform.Invert();
		DirectX::SimpleMath::Matrix nextTransform = data.worldTransform;

		SimpleMatrix prevMatrix;
		SimpleMatrix nextMatrix;

		prevMatrix.m[0][0] = prevTransform._11;
		prevMatrix.m[0][1] = prevTransform._12;
		prevMatrix.m[0][2] = prevTransform._13;
		prevMatrix.m[0][3] = prevTransform._14;
		prevMatrix.m[1][0] = prevTransform._21;
		prevMatrix.m[1][1] = prevTransform._22;
		prevMatrix.m[1][2] = prevTransform._23;
		prevMatrix.m[1][3] = prevTransform._24;
		prevMatrix.m[2][0] = prevTransform._31;
		prevMatrix.m[2][1] = prevTransform._32;
		prevMatrix.m[2][2] = prevTransform._33;
		prevMatrix.m[2][3] = prevTransform._34;
		prevMatrix.m[3][0] = prevTransform._41;
		prevMatrix.m[3][1] = prevTransform._42;
		prevMatrix.m[3][2] = prevTransform._43;
		prevMatrix.m[3][3] = prevTransform._44;

		nextMatrix.m[0][0] = nextTransform._11;
		nextMatrix.m[0][1] = nextTransform._12;
		nextMatrix.m[0][2] = nextTransform._13;
		nextMatrix.m[0][3] = nextTransform._14;
		nextMatrix.m[1][0] = nextTransform._21;
		nextMatrix.m[1][1] = nextTransform._22;
		nextMatrix.m[1][2] = nextTransform._23;
		nextMatrix.m[1][3] = nextTransform._24;
		nextMatrix.m[2][0] = nextTransform._31;
		nextMatrix.m[2][1] = nextTransform._32;
		nextMatrix.m[2][2] = nextTransform._33;
		nextMatrix.m[2][3] = nextTransform._34;
		nextMatrix.m[3][0] = nextTransform._41;
		nextMatrix.m[3][1] = nextTransform._42;
		nextMatrix.m[3][2] = nextTransform._43;
		nextMatrix.m[3][3] = nextTransform._44;

		physx::PxVec4* particle = mClothBuffer->getPositionInvMasses();

		int threadsPerBlock = 256;
		int blocksPerGrid = (mVertices.size() + threadsPerBlock - 1) / threadsPerBlock;

		TransformVertices <<<blocksPerGrid, threadsPerBlock>>>(particle, prevMatrix, nextMatrix, mVertices.size());

		mClothBuffer->raiseFlags(physx::PxParticleBufferFlag::eUPDATE_POSITION);

		mWorldTransform = data.worldTransform;
	}
}
