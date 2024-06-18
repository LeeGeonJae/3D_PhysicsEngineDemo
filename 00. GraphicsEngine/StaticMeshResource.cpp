#include "pch.h"
#include "StaticMeshResource.h"

#include "ResourceManager.h"
#include "RenderManager.h"
#include "Graphics.h"

#include "Material.h"

namespace GraphicsEngine
{
	StaticMesh::StaticMesh()
	{
	}

	StaticMesh::~StaticMesh()
	{
	}

	void StaticMesh::Create(aiMesh* _aiMesh)
	{
		m_Name = _aiMesh->mName.C_Str();

		// 건재 : 메시 버텍스 가져오기
		m_VertexVec.reserve(_aiMesh->mNumVertices);
		for (UINT i = 0; i < _aiMesh->mNumVertices; i++)
		{
			Vertex vertex;

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

			m_VertexVec.push_back(vertex);
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

		createBuffer();
	}

	void StaticMesh::createBuffer()
	{
		// 건재 : 버텍스 및 인덱스 버퍼 생성
		HRESULT hr;

		D3D11_BUFFER_DESC vertexDesc;
		vertexDesc.Usage = D3D11_USAGE_IMMUTABLE;
		vertexDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexDesc.ByteWidth = static_cast<UINT>(sizeof(Vertex) * m_VertexVec.size());
		vertexDesc.CPUAccessFlags = 0;
		vertexDesc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA initData;
		initData.pSysMem = &m_VertexVec[0];

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



	StaticMeshSceneResource::StaticMeshSceneResource()
		:ResourceBase(ResourceType::StaticMesh)
	{
	}

	StaticMeshSceneResource::~StaticMeshSceneResource()
	{
	}

	void StaticMeshSceneResource::Create(const aiScene* _aiScene)
	{
		// 건재 : 스태틱 메시 씬 리소스 생성
		namespace Math = DirectX::SimpleMath;

		for (int i = 0; i < _aiScene->mNumMeshes; i++)
		{
			StaticMesh mesh;
			mesh.Create(_aiScene->mMeshes[i]);
			// 건재 : 메시 머터리얼 등록
			string materialName = _aiScene->mMaterials[_aiScene->mMeshes[i]->mMaterialIndex]->GetName().C_Str();
			shared_ptr<Material> material = RESOURCE->Find<Material>(materialName);
			mesh.SetMaterial(material);
			m_StaticMeshVec.push_back(mesh);

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


