#include "pch.h"
#include "ClothPhysics.h"

#include <cudamanager/PxCudaContext.h>

namespace PhysicsEngine
{
	ClothPhysics::ClothPhysics(unsigned int id, unsigned int layerNumber)
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
	ClothPhysics::~ClothPhysics()
	{
	}

	bool ClothPhysics::Initialize(const PhysicsClothInfo& info, physx::PxPhysics* physics, physx::PxScene* scene, physx::PxCudaContextManager* cudaContextManager)
	{
		if (cudaContextManager == nullptr)
			return false;

		settingInfoData(info);
		extractSpringsData(info.indices, info.indexSize);

		// ���� �� ������, �ﰢ���� ���� ���
		const physx::PxU32 numParticles = mVertices.size();	// ���� ����
		const physx::PxU32 numSprings = mSprings.size();	// ���� �ϳ��� �̿��ϴ� ���ڵ鿡 ������ ���� �����µ�, �� ������ ����
		const physx::PxU32 numTriangles = mIndices.size() / 3;	// �ﰢ�� ����

		// ���� �ý����� ����
		const physx::PxReal particleMass = mTotalClothMass / info.vertexSize;
		const physx::PxReal restOffset = 2.f;

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

		// ���� �ؤ���
		mClothBufferHelper->release();

		// �Ҵ�� �޸� ����
		cudaContextManager->freePinnedHostBuffer(mPositionInvMass);
		cudaContextManager->freePinnedHostBuffer(mVelocity);
		cudaContextManager->freePinnedHostBuffer(mPhase);

		return true;
	}

	physx::PxU32 ClothPhysics::id(const physx::PxU32& x, const physx::PxU32& y, const physx::PxU32& numY)
	{
		return x * numY + y;
	}

	bool areVerticesEqual(const DirectX::SimpleMath::Vector3& vertex1, const DirectX::SimpleMath::Vector3& vertex2, float epsilon = 1e-6) {
		return (std::abs(vertex1.x - vertex2.x) < epsilon) &&
			(std::abs(vertex1.y - vertex2.y) < epsilon) &&
			(std::abs(vertex1.z - vertex2.z) < epsilon);
	}

	void ClothPhysics::extractSpringsData(unsigned int* indices, unsigned int indexSize)
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
					mSprings.insert({i, j});
					mSameVertices.push_back({ i, j });
				}
			}
		}
	}

	void ClothPhysics::settingInfoData(const PhysicsClothInfo& info)
	{
		mWorldTransform = info.worldTransform;
		mTotalClothMass = info.totalClothMass;

		mIndices.resize(info.indexSize);
		memcpy(mIndices.data(), info.indices, info.indexSize * sizeof(unsigned int));

		mVertices.resize(info.vertexSize);
		mUV.resize(info.vertexSize);
		for (int i = 0; i < info.vertexSize; i++)
		{
			mVertices[i].x = info.vertices[i].x;
			mVertices[i].y = info.vertices[i].y;
			mVertices[i].z = -info.vertices[i].z;
			mUV[i] = info.uv[i];
		}
	}

	float calculateVectorMagnitude(const DirectX::SimpleMath::Vector3& point1, const DirectX::SimpleMath::Vector3& point2) {
		float dx = point2.x - point1.x;
		float dy = point2.y - point1.y;
		float dz = point2.z - point1.z;

		return std::sqrt(dx * dx + dy * dy + dz * dz);
	}

	void ClothPhysics::settingParticleBuffer(
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

	void ClothPhysics::calculateNormals()
	{
		mNormals.clear();
		mTangents.clear();
		mBiTangents.clear();
		mNormals.resize(mVertices.size(), { 0.f, 0.f, 0.f });
		mTangents.resize(mVertices.size(), { 0.f, 0.f, 0.f });
		mBiTangents.resize(mVertices.size(), { 0.f, 0.f, 0.f });

		// �ﰢ�� ������ face normal ���
		for (int i = 0; i < mIndices.size(); i += 3)
		{
			unsigned int idx0 = mIndices[i];
			unsigned int idx1 = mIndices[i + 1];
			unsigned int idx2 = mIndices[i + 2];

			DirectX::SimpleMath::Vector3 v0 = mVertices[idx0];
			DirectX::SimpleMath::Vector3 v1 = mVertices[idx1];
			DirectX::SimpleMath::Vector3 v2 = mVertices[idx2];
			v0.z = -v0.z;
			v1.z = -v1.z;
			v2.z = -v2.z;

			DirectX::SimpleMath::Vector2 uv0 = mUV[idx0];
			DirectX::SimpleMath::Vector2 uv1 = mUV[idx1];
			DirectX::SimpleMath::Vector2 uv2 = mUV[idx2];

			DirectX::SimpleMath::Vector3 edge1 = v1 - v0;
			DirectX::SimpleMath::Vector3 edge2 = v2 - v0;

			DirectX::SimpleMath::Vector2 edge1uv = uv1 - uv0;
			DirectX::SimpleMath::Vector2 edge2uv = uv2 - uv0;

			DirectX::SimpleMath::Vector3 faceNormal = edge1.Cross(edge2);
			faceNormal.Normalize();

			// vertex normal�� face normal�� ����
			mNormals[idx0] += faceNormal;
			mNormals[idx1] += faceNormal;
			mNormals[idx2] += faceNormal;

			// tangent�� bitangent�� ���ϴ� ����
			float r = 1.0f / (edge1uv.x * edge2uv.y - edge1uv.y * edge2uv.x);
			DirectX::SimpleMath::Vector3 tangent = (edge1 * edge2uv.y - edge2 * edge1uv.y) * r;
			DirectX::SimpleMath::Vector3 bitangent = (edge2 * edge1uv.x - edge1 * edge2uv.x) * r;

			tangent.Normalize();
			bitangent.Normalize();
			mTangents[idx0] += tangent;
			mTangents[idx1] += tangent;
			mTangents[idx2] += tangent;
			mBiTangents[idx0] += bitangent;
			mBiTangents[idx1] += bitangent;
			mBiTangents[idx2] += bitangent;
		}

		for (auto& sameVertex : mSameVertices)
		{
			mNormals[sameVertex.first] += mNormals[sameVertex.second];
			mNormals[sameVertex.second] += mNormals[sameVertex.first];
			mTangents[sameVertex.first] += mTangents[sameVertex.second];
			mTangents[sameVertex.second] += mTangents[sameVertex.first];
			mBiTangents[sameVertex.first] += mBiTangents[sameVertex.second];
			mBiTangents[sameVertex.second] += mBiTangents[sameVertex.first];
		}

		// vertex normal ����ȭ
		for (int i = 0; i < mVertices.size(); i++)
		{
			mNormals[i].Normalize();
			mTangents[i].Normalize();
			mBiTangents[i].Normalize();
		}
	}

	void ClothPhysics::GetPhysicsCloth(physx::PxCudaContextManager* cudaContextManager, physx::PxCudaContext* cudaContext, PhysicsClothGetData& data)
	{
		int particleSize = mClothBuffer->getNbActiveParticles();
		physx::PxVec4* particle = mClothBuffer->getPositionInvMasses();

		cudaContextManager->acquireContext();

		std::vector<physx::PxVec4> vertex;
		vertex.resize(particleSize);

		cudaContext->memcpyDtoH(vertex.data(), CUdeviceptr(particle), sizeof(physx::PxVec4) * particleSize);

		for (int i = 0; i < particleSize; i++)
		{
			mVertices[i].x = vertex[i].x;
			mVertices[i].y = vertex[i].y;
			mVertices[i].z = -vertex[i].z;
		}

		calculateNormals();

		data.vertices = mVertices.data();
		data.uv = mUV.data();
		data.vertexSize = mVertices.size();
		data.nomals = mNormals.data();
		data.indices = mIndices.data();
		data.tangents = mTangents.data();
		data.biTangents = mBiTangents.data();
		data.indexSize = mIndices.size();
		data.worldTransform = mWorldTransform;
	}

	void ClothPhysics::SetPhysicsCloth(physx::PxCudaContextManager* cudaContextManager, physx::PxCudaContext* cudaContext, const PhysicsClothSetData& data)
	{

	}
}
