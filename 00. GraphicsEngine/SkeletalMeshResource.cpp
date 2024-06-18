#include "pch.h"
#include "SkeletalMeshResource.h"

#include "ResourceManager.h"
#include "RenderManager.h"
#include "Graphics.h"

#include "SkeletonResource.h"
#include "Material.h"

namespace GraphicsEngine
{
	SkeletalMesh::SkeletalMesh()
	{
		
	}

	SkeletalMesh::~SkeletalMesh()
	{
	}

	void SkeletalMesh::Create(aiMesh* _aiMesh, shared_ptr<SkeletonResource> _skeleton)
	{
		m_Name = _aiMesh->mName.C_Str();

		// 건재 : 메시 버텍스 가져오기
		m_BoneWeightVertexVec.reserve(_aiMesh->mNumVertices);
		for (UINT i = 0; i < _aiMesh->mNumVertices; i++)
		{
			BoneWeightVertex vertex;

			if (_aiMesh->HasPositions())
			{
				vertex.m_Position.x = _aiMesh->mVertices[i].x;
				vertex.m_Position.y = _aiMesh->mVertices[i].y;
				vertex.m_Position.z = _aiMesh->mVertices[i].z;
			}
			if (_aiMesh->mTextureCoords[0])
			{
				vertex.m_TexCoord.x = static_cast<float>(_aiMesh->mTextureCoords[0][i].x);
				vertex.m_TexCoord.y = static_cast<float>(_aiMesh->mTextureCoords[0][i].y);
			}
			if (_aiMesh->HasNormals())
			{
				vertex.m_Normal.x = _aiMesh->mNormals[i].x;
				vertex.m_Normal.y = _aiMesh->mNormals[i].y;
				vertex.m_Normal.z = _aiMesh->mNormals[i].z;
			}
			if (_aiMesh->HasTangentsAndBitangents())
			{
				vertex.m_Tangent.x = _aiMesh->mTangents[i].x;
				vertex.m_Tangent.y = _aiMesh->mTangents[i].y;
				vertex.m_Tangent.z = _aiMesh->mTangents[i].z;

				vertex.m_BiTangents.x = _aiMesh->mBitangents[i].x;
				vertex.m_BiTangents.y = _aiMesh->mBitangents[i].y;
				vertex.m_BiTangents.z = _aiMesh->mBitangents[i].z;
			}

			m_BoneWeightVertexVec.push_back(vertex);
		}

		// 건재 : 메시 인덱스 가져오기
		m_IndexVec.reserve(_aiMesh->mNumFaces * 3);
		for (UINT i = 0; i < _aiMesh->mNumFaces; i++)
		{
			aiFace face = _aiMesh->mFaces[i];

			for (UINT j = 0; j < face.mNumIndices; j++)
			{
				m_IndexVec.push_back(face.mIndices[j]);
			}
		}

		// 건재 : 메시 본 가져오기
		if (_aiMesh->HasBones())
		{
			size_t nums = _aiMesh->mNumBones;
			_skeleton->Create(_aiMesh, &m_BoneWeightVertexVec);
		}

		createBuffer();
	}

	void SkeletalMesh::createBuffer()
	{
		// 건재 : 버텍스 및 인덱스 버퍼 생성
		HRESULT hr;

		D3D11_BUFFER_DESC vertexDesc;
		vertexDesc.Usage = D3D11_USAGE_IMMUTABLE;
		vertexDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexDesc.ByteWidth = static_cast<UINT>(sizeof(BoneWeightVertex) * m_BoneWeightVertexVec.size());
		vertexDesc.CPUAccessFlags = 0;
		vertexDesc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA initData;
		initData.pSysMem = &m_BoneWeightVertexVec[0];

		hr = DEVICE->CreateBuffer(&vertexDesc, &initData, m_pVertexBuffer.GetAddressOf());
		assert(SUCCEEDED(hr));

		D3D11_BUFFER_DESC indexDesc;
		indexDesc.Usage = D3D11_USAGE_IMMUTABLE;
		indexDesc.ByteWidth = static_cast<UINT>(sizeof(UINT) * m_IndexVec.size());
		indexDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		indexDesc.CPUAccessFlags = 0;
		indexDesc.MiscFlags = 0;

		initData.pSysMem = &m_IndexVec[0];

		hr = DEVICE->CreateBuffer(&indexDesc, &initData, m_pIndexBuffer.GetAddressOf());
		assert(SUCCEEDED(hr));
	}



	SkeletalMeshSceneResource::SkeletalMeshSceneResource()
		:ResourceBase(ResourceType::SkeletalMesh)
	{
	}

	SkeletalMeshSceneResource::~SkeletalMeshSceneResource()
	{
	}

	void SkeletalMeshSceneResource::Create(const aiScene* _aiScene, shared_ptr<SkeletonResource> _skeleton)
	{
		namespace Math = DirectX::SimpleMath;

		// 건재 : 메시 로드
		for (int i = 0; i < _aiScene->mNumMeshes; i++)
		{
			SkeletalMesh mesh;
			mesh.Create(_aiScene->mMeshes[i], _skeleton);
			string materialName = _aiScene->mMaterials[_aiScene->mMeshes[i]->mMaterialIndex]->GetName().C_Str();
			shared_ptr<Material> material = RESOURCE->Find<Material>(materialName);
			mesh.SetMaterial(material);
			m_SkeletalMeshVec.push_back(mesh);

			// 건재 : 바운딩 볼륨
			Vector3 min; memcpy(&min, &_aiScene->mMeshes[i]->mAABB.mMin, sizeof(min));
			Vector3 max; memcpy(&max, &_aiScene->mMeshes[i]->mAABB.mMax, sizeof(max));
			m_AABBmin = Math::Vector3::Min(m_AABBmin, min);
			m_AABBmax = Math::Vector3::Max(m_AABBmax, max);
		}

		// 건재 : 바운딩 박스 세팅
		{
			m_BoundingBox.Center = Math::Vector3(m_AABBmin + m_AABBmax) * 0.5;
			m_BoundingBox.Extents = Math::Vector3(m_AABBmax - m_AABBmin) * 0.5;

			// 건재 : 팔 벌릴 때를 대비해서 z축과 x축 중 큰값으로 AABB를 만든다.
			float maxValue = fmax(m_BoundingBox.Extents.z, fmax(m_BoundingBox.Extents.x, m_BoundingBox.Extents.y));
			m_BoundingBox.Extents.x = maxValue;
			m_BoundingBox.Extents.y = maxValue;
			m_BoundingBox.Extents.z = maxValue;
		}
	}
}
