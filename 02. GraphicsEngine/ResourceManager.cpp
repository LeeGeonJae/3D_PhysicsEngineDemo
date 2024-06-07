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
#include "Helper.h"

namespace GraphicsEngine
{
	void ResourceManager::Initalize()
	{
		RESOURCE->Load("../Resources/FBX/SkinningTest.fbx");
		RESOURCE->Load("../Resources/FBX/Dying.fbx");
		RESOURCE->Load("../Resources/FBX/cerberus_test.fbx");
		RESOURCE->Load("../Resources/FBX/zeldaPosed001.fbx");
		RESOURCE->Load("../Resources/FBX/PBR_Test.fbx");
		RESOURCE->Load("../Resources/FBX/box.fbx");
		RESOURCE->Load("../Resources/FBX/Tree.fbx");
		RESOURCE->Load("../Resources/FBX/Monkey.fbx");
		RESOURCE->Load("../Resources/FBX/Vampire.fbx");
		RESOURCE->Load("../Resources/FBX/plane.fbx");

		shared_ptr<Shader> skeletalMeshShader = Create<Shader>("SkeletalMeshShader");
		shared_ptr<Shader> staticMeshShader = Create<Shader>("StaticMeshShader");
		skeletalMeshShader->Init(L"../Resources/Shader/SkeletalMeshShader.hlsl", ShaderType::SkeletalMesh);
		staticMeshShader->Init(L"../Resources/Shader/StaticMeshShader.hlsl", ShaderType::StaticMesh);
	}

	void ResourceManager::Load(const string& _path)
	{
		if (_path.find(".fbx") != string::npos)
		{
			Assimp::Importer importer;
			importer.SetPropertyBool(AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS, 0); // ���� : $assimp_fbx$ ��� ��������	

			unsigned int importFalgs =
				aiProcess_Triangulate |			// ���� : �ﰢ������ ��ȯ
				aiProcess_GenNormals |			// ���� : �븻 ����
				aiProcess_GenUVCoords |			// ���� : UV ����
				aiProcess_CalcTangentSpace |	// ���� : ź��Ʈ ����
				aiProcess_LimitBoneWeights |	// ���� : ���� ������ �޴� ������ �ִ� ������ 4���� ����
				aiProcess_GenBoundingBoxes |	// ���� : AABB�ٿ�� �ڽ� ���� 
				aiProcess_ConvertToLeftHanded;	// ���� : �޼� ��ǥ��� ��ȯ

			const aiScene* pScene = importer.ReadFile(_path, importFalgs);

			if (pScene == nullptr)
				return;

			createNodeData(pScene, _path);
			createMaterial(pScene, _path);
			createMesh(pScene, _path);
			createAnimation(pScene, _path);
		}
		else
		{
			// ���� : FBX�� �ƴ� �Ϲ� �ؽ��� ������ ���
			shared_ptr<Texture> texture = Find<Texture>(_path);

			if (texture == nullptr)
			{
				texture = Create<Texture>(_path);
				texture->LoadTexture(_path);
			}
		}
	}


	void ResourceManager::createNodeData(const aiScene* _pScene, const string& _path)
	{
		// ���� : ��� ����Ÿ �ε�
		shared_ptr<NodeDataResource> nodeData = Find<NodeDataResource>(_path);
		if (nodeData == nullptr)
		{
			nodeData = Create<NodeDataResource>(_path);
			nodeData->processNode(_pScene->mRootNode, _pScene);
		}
	}

	void ResourceManager::createMaterial(const aiScene* _pScene, const string& _path)
	{
		// ���� : ���͸��� �ε�
		for (int i = 0; i < _pScene->mNumMaterials; i++)
		{
			string materialName = _pScene->mMaterials[i]->GetName().C_Str();
			shared_ptr<Material> material = Find<Material>(materialName);

			if (material == nullptr)
			{
				material = Create<Material>(materialName);
				material->Create(_pScene->mMaterials[i], _pScene, _path);
			}
		}
	}

	void ResourceManager::createMesh(const aiScene* _pScene, const string& _path)
	{
		// ���� : ���̷�Ż �޽� �ε�
		if (_pScene->mMeshes[0]->HasBones())
		{
			shared_ptr<SkeletalMeshSceneResource> skeletalMeshResource = Find<SkeletalMeshSceneResource>(_path);

			if (skeletalMeshResource == nullptr)
			{
				skeletalMeshResource = Create<SkeletalMeshSceneResource>(_path);
				shared_ptr<SkeletonResource> skeletonResource = Create<SkeletonResource>(_path);
				skeletalMeshResource->Create(_pScene, skeletonResource);
			}
		}
		else  // ���� : ����ƽ �޽� �ε�
		{
			shared_ptr<StaticMeshSceneResource> staticMesh = Find<StaticMeshSceneResource>(_path);

			if (staticMesh == nullptr)
			{
				staticMesh = Create<StaticMeshSceneResource>(_path);
				staticMesh->Create(_pScene);
			}
		}
	}

	void ResourceManager::createAnimation(const aiScene* _pScene, const string& _path)
	{
		// ���� : �ִϸ��̼� �ε�
		if (_pScene->HasAnimations())
		{
			// ���� : �ִϸ��̼��� ã�Ƽ� ������ ����
			shared_ptr<AnimationResource> animation = Find<AnimationResource>(_path);
			if (animation == nullptr)
			{
				shared_ptr<AnimationResource> animation = Create<AnimationResource>(_path);
				animation->Create(*_pScene->mAnimations);
				m_ResourceMap[static_cast<int>(ResourceType::Animation)].insert(make_pair(_path, animation));
			}
		}
	}
}
