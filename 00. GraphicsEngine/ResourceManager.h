#pragma once
#include "ResourceBase.h"

namespace GraphicsEngine
{
	// ���� : Ŭ���� ����
	class Material;
	class StaticMeshSceneResource;
	class SkeletalMeshSceneResource;
	class Shader;
	class AnimationResource;
	class Texture;
	class SkeletonResource;
	class NodeDataResource;

	// -------------------------------------------------------------------------
	// ���� : ���ҽ� �Ŵ��� Ŭ���� ( �̱��� )
	class ResourceManager
	{
		friend class GameManager;
	private:
		ResourceManager() {}
	public:
		static ResourceManager* GetInstance()
		{
			static ResourceManager instance;
			return &instance;
		}

	public:
		void Initalize();

		void Load(const string& _path);
		template <typename T>
		shared_ptr<T> Find(const string& _key);
		template <typename T>
		shared_ptr<T> Create(const string& _key);

	public:
		template <typename T>
		ResourceType GetResourceType();

	private:
		void createNodeData(const aiScene* _pScene, const string& _path);
		void createMaterial(const aiScene* _pScene, const string& _path);
		void createMesh(const aiScene* _pScene, const string& _path);
		void createAnimation(const aiScene* _pScene, const string& _path);

	private:
		using KeyObjMap = unordered_map<string, shared_ptr<ResourceBase>>;
		array<KeyObjMap, RESOURCE_TYPE_COUNT> m_ResourceMap;
	};

	// ���� : ���ҽ� ���̽��� ��ӹ޴� ���ҽ��� ã��
	template<typename T>
	inline shared_ptr<T> ResourceManager::Find(const string& _key)
	{
		bool bIsBase = std::is_base_of<ResourceBase, T>::value;
		assert(bIsBase == true);

		ResourceType resourceType = GetResourceType<T>();

		// ���� : ���ҽ��� ã���� ��ȯ
		auto resourceIter = m_ResourceMap[static_cast<int>(resourceType)].find(_key);
		
		if (resourceIter != m_ResourceMap[static_cast<int>(resourceType)].end())
		{
			return static_pointer_cast<T>(resourceIter->second);
		}
		//else // ���� : ��ã���� �ε� �� ��ȯ ( ���� �߻� ���� �� )
		//{
		//	Load(_key);
		//	resourceIter = m_ResourceMap[static_cast<int>(resourceType)].find(_key);
		//	return static_pointer_cast<T>(resourceIter->second);
		//}

		return nullptr;
	}

	// ���� : ���͸��� ���ҽ� ����
	template <typename T>
	shared_ptr<T> ResourceManager::Create(const string& _key)
	{
		ResourceType resourceType = GetResourceType<T>();

		shared_ptr<T> resource = make_shared<T>();
		resource->SetName(_key);
		m_ResourceMap[static_cast<int>(resourceType)].insert(make_pair(_key, resource));

		return resource;
	}

	// ���� : Ŭ������ �°� ���ҽ� Ÿ�� ã��
	template<typename T>
	inline ResourceType ResourceManager::GetResourceType()
	{
		if (is_same_v<T, StaticMeshSceneResource>)
			return ResourceType::StaticMesh;
		if (is_same_v<T, SkeletalMeshSceneResource>)
			return ResourceType::SkeletalMesh;
		if (is_same_v<T, Material>)
			return ResourceType::Material;
		if (is_same_v<T, Shader>)
			return ResourceType::Shader;
		if (is_same_v<T, AnimationResource>)
			return ResourceType::Animation;
		if (is_same_v<T, Texture>)
			return ResourceType::Texture;
		if (is_same_v<T, SkeletonResource>)
			return ResourceType::Bone;
		if (is_same_v<T, NodeDataResource>)
			return ResourceType::NodeData;

		assert(false);
		return ResourceType::None;
	}
	// -------------------------------------------------------------------------
}

#define RESOURCE GraphicsEngine::ResourceManager::GetInstance()