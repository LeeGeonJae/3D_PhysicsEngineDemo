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

		// 입자 및 스프링, 삼각형의 개수 계산
		const physx::PxU32 numParticles = mVertices.size();	// 입자 갯수
		const physx::PxU32 numSprings = mSprings.size();	// 입자 하나당 이웃하는 입자들에 스프링 값을 가지는데, 그 스프링 갯수
		const physx::PxU32 numTriangles = mIndices.size() / 3;	// 삼각형 갯수

		// 입자 시스템의 설정
		const physx::PxReal particleMass = mTotalClothMass / info.vertexSize;
		const physx::PxReal restOffset = 2.f;

		// 재료(Material) 설정
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

		// 입자 시스템 생성
		mParticleSystem = physics->createPBDParticleSystem(*cudaContextManager);

		mParticleSystem->setRestOffset(1.f);
		mParticleSystem->setContactOffset(restOffset + 0.02f);
		mParticleSystem->setParticleContactOffset(restOffset + 0.02f);
		mParticleSystem->setSolidRestOffset(restOffset);

		// 씬에 입자 시스템 추가
		scene->addActor(*mParticleSystem);

		// 입자의 상태를 저장하는 버퍼 생성
		const physx::PxU32 particlePhase = mParticleSystem->createPhase(mPBDMaterial, physx::PxParticlePhaseFlags(
			physx::PxParticlePhaseFlag::eParticlePhaseSelfCollideFilter | physx::PxParticlePhaseFlag::eParticlePhaseSelfCollide));


		mClothBufferHelper = physx::ExtGpu::PxCreateParticleClothBufferHelper(1, numTriangles, numSprings, numParticles, cudaContextManager);
		// 입자, 스프링 삼각형의 상태를 저장하기 위한 버퍼 할당
		mPhase = cudaContextManager->allocPinnedHostBuffer<physx::PxU32>(numParticles);
		mPositionInvMass = cudaContextManager->allocPinnedHostBuffer<physx::PxVec4>(numParticles);
		mVelocity = cudaContextManager->allocPinnedHostBuffer<physx::PxVec4>(numParticles);

		settingParticleBuffer(numSprings, numTriangles, numParticles, particlePhase, particleMass);

		// 입자의 상태를 나타내는 버퍼 설명
		physx::ExtGpu::PxParticleBufferDesc bufferDesc;
		bufferDesc.maxParticles = numParticles;
		bufferDesc.numActiveParticles = numParticles;
		bufferDesc.positions = mPositionInvMass;
		bufferDesc.velocities = mVelocity;
		bufferDesc.phases = mPhase;

		// 천막의 설명 가져오기
		const physx::PxParticleClothDesc& clothDesc = mClothBufferHelper->getParticleClothDesc();

		// 입자 천막의 전처리기 생성
		physx::PxParticleClothPreProcessor* clothPreProcessor = PxCreateParticleClothPreProcessor(cudaContextManager);

		// 입자 천막 분할 및 처리
		physx::PxPartitionedParticleCloth output;
		clothPreProcessor->partitionSprings(clothDesc, output);
		clothPreProcessor->release();

		// 천막 버퍼 생성
		mClothBuffer = physx::ExtGpu::PxCreateAndPopulateParticleClothBuffer(bufferDesc, clothDesc, output, cudaContextManager);
		mParticleSystem->addParticleBuffer(mClothBuffer);

		// 버파 해ㅐ제
		mClothBufferHelper->release();

		// 할당된 메모리 해제
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
		// 삼각형 단위로 인덱스를 순회
		for (size_t i = 0; i < indexSize; i += 3)
		{
			unsigned int v1 = indices[i];
			unsigned int v2 = indices[i + 1];
			unsigned int v3 = indices[i + 2];

			// 정렬된 순서로 vertex 쌍을 추가하여 중복된 선분 방지
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

		// 파티클 스프링 및 트라이앵글 생성
		physx::PxArray<physx::PxParticleSpring> springs;
		springs.reserve(numSprings);
		physx::PxArray<physx::PxU32> triangles;
		triangles.reserve(numTriangles * 3);

		// 입자 상태 저장
		for (int i = 0; i < numParticles; i++)
		{
			mPositionInvMass[i] = physx::PxVec4(mVertices[i].x, mVertices[i].y + 300.f, mVertices[i].z, 1.f / particleMass);
			mPhase[i] = particlePhase;
			mVelocity[i] = physx::PxVec4(0.f);
		}

		// 스프링 추가
		for (auto line : mSprings)
		{
			physx::PxParticleSpring spring = { line.first, line.second, calculateVectorMagnitude(mVertices[line.first], mVertices[line.second]), stretchStiffness, springDamping, 0 };
			springs.pushBack(spring);
		}

		// 삼각형 추가
		for (int i = 0; i < mIndices.size(); i += 3)
		{
			triangles.pushBack(mIndices[i]);
			triangles.pushBack(mIndices[i + 1]);
			triangles.pushBack(mIndices[i + 2]);
		}

		// 생성된 스프링 및 삼각형 수가 예상대로 생성되었는지 확인
		PX_ASSERT(numSprings == springs.size());
		PX_ASSERT(numTriangles == triangles.size() / 3);

		// 천막의 버퍼에 데이터 추가
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

		// 삼각형 단위로 face normal 계산
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

			// vertex normal에 face normal을 더함
			mNormals[idx0] += faceNormal;
			mNormals[idx1] += faceNormal;
			mNormals[idx2] += faceNormal;

			// tangent와 bitangent를 구하는 공식
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

		// vertex normal 정규화
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
