#include "pch.h"
#include "ResourceManager.h"

#include "StaticMeshResource.h"
#include "SkeletalMeshResource.h"
#include "Material.h"
#include "SkeletonResource.h"
#include "AnimationResource.h"
#include "Node.h"
#include "Texture.h"
#include "Shader.h"

namespace Engine
{
	void ResourceManager::Load(const string& _path)
	{
		if (_path.find(".fbx") != string::npos)
		{
			Assimp::Importer importer;
			importer.SetPropertyBool(AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS, 0); // $assimp_fbx$ 노드 생성안함	

			unsigned int importFalgs =
				aiProcess_Triangulate |			// 삼각형으로 변환
				aiProcess_GenNormals |			// 노말 생성
				aiProcess_GenUVCoords |			// UV 생성
				aiProcess_CalcTangentSpace |	// 탄젠트 생성
				aiProcess_LimitBoneWeights |	// 본의 영향을 받는 장점의 최대 갯수를 4개로 제한
				aiProcess_GenBoundingBoxes |	// AABB바운딩 박스 생성 
				aiProcess_ConvertToLeftHanded;	// 왼손 좌표계로 변환

			const aiScene* pScene = importer.ReadFile(_path, importFalgs);

			if (pScene == nullptr)
				return;


			// 노드 데이타 로드
			shared_ptr<NodeDataResource> nodeData = Find<NodeDataResource>(_path);
			if (nodeData == nullptr)
			{
				nodeData = Create<NodeDataResource>(_path);
				processNode(pScene->mRootNode, pScene, nodeData);
			}


			// 머터리얼 로드
			for (int i = 0; i < pScene->mNumMaterials; i++)
			{
				string materialName = pScene->mMaterials[i]->GetName().C_Str();
				shared_ptr<Material> material = Find<Material>(materialName);

				if (material == nullptr)
				{
					material = Create<Material>(materialName);
					material->Create(pScene->mMaterials[i], pScene, _path);
				}
			}


			// 스켈레탈 메시 로드
			if (pScene->mMeshes[0]->HasBones())
			{
				shared_ptr<SkeletalMeshSceneResource> skeletalMeshResource = Find<SkeletalMeshSceneResource>(_path);

				if (skeletalMeshResource == nullptr)
				{
					skeletalMeshResource = Create<SkeletalMeshSceneResource>(_path);
					shared_ptr<SkeletonResource> skeletonResource = Create<SkeletonResource>(_path);
					skeletalMeshResource->Create(pScene, skeletonResource);
				}
			}
			else  // 스태틱 메시 로드
			{
				shared_ptr<StaticMeshSceneResource> staticMesh = Find<StaticMeshSceneResource>(_path);

				if (staticMesh == nullptr)
				{
					staticMesh = Create<StaticMeshSceneResource>(_path);
					staticMesh->Create(pScene);
				}
			}


			// 애니메이션 로드
			if (pScene->HasAnimations())
			{
				// 애니메이션이 찾아서 없으면 생성
				shared_ptr<AnimationResource> animation = Find<AnimationResource>(_path);
				if (animation == nullptr)
				{
					animation = Create<AnimationResource>(_path);
					animation->Create(*pScene->mAnimations);
					m_ResourceMap[static_cast<int>(ResourceType::Animation)].insert(make_pair(_path, animation));
				}
			}

			int a = 0;
		}
		else
		{
			// FBX가 아닌 일반 텍스쳐 파일일 경우
			shared_ptr<Texture> texture = Find<Texture>(_path);

			if (texture == nullptr)
			{
				texture = Create<Texture>(_path);
				texture->LoadTexture(_path);
			}
		}
	}

	void ResourceManager::processNode(const aiNode* _aiNode, const aiScene* _aiScene, shared_ptr<NodeDataResource> _nodeDataResource)
	{
		NodeData nodeData;
		nodeData.m_Name = _aiNode->mName.C_Str();
		if (_aiNode->mParent != nullptr)
			nodeData.m_ParentName = _aiNode->mParent->mName.C_Str();
		_nodeDataResource->AddNodeData(nodeData);

		for (int i = 0; i < _aiNode->mNumChildren; i++)
		{
			processNode(_aiNode->mChildren[i], _aiScene, _nodeDataResource);
		}
	}
}
