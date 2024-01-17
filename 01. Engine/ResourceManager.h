#pragma once
#include "ResourceBase.h"

namespace Engine
{
	// 클래스 선언
	class Material;
	class StaticMeshSceneResource;
	class SkeletalMeshSceneResource;
	class Shader;
	class AnimationResource;
	class Texture;
	class SkeletonResource;
	class NodeDataResource;

	// 리소스 매니저 클래스
	class ResourceManager
	{
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
		void processNode(const aiNode* _aiNode, const aiScene* _aiScene, shared_ptr<NodeDataResource> _nodeDataResource);

	private:
		using KeyObjMap = unordered_map<string, shared_ptr<ResourceBase>>;
		array<KeyObjMap, RESOURCE_TYPE_COUNT> m_ResourceMap;
	};

	// 리소스 베이스를 상속받는 리소스들 찾기
	template<typename T>
	inline shared_ptr<T> ResourceManager::Find(const string& _key)
	{
		bool bIsBase = std::is_base_of<ResourceBase, T>::value;
		assert(bIsBase == true);

		ResourceType resourceType = GetResourceType<T>();

		// 리소스를 찾으면 반환
		auto resourceIter = m_ResourceMap[static_cast<int>(resourceType)].find(_key);
		
		if (resourceIter != m_ResourceMap[static_cast<int>(resourceType)].end())
		{
			return static_pointer_cast<T>(resourceIter->second);
		}

		// 못찾으면 nullptr
		return nullptr;
	}

	// 머터리얼 리소스 생성
	template <typename T>
	shared_ptr<T> ResourceManager::Create(const string& _key)
	{
		ResourceType resourceType = GetResourceType<T>();

		shared_ptr<T> resource = make_shared<T>();
		resource->SetName(_key);
		m_ResourceMap[static_cast<int>(resourceType)].insert(make_pair(_key, resource));

		return resource;
	}

	// 클래스에 맞게 리소스 타입 찾기
	template<typename T>
	inline ResourceType ResourceManager::GetResourceType()
	{
		// constexpr을 붙이면 컴파일 단계에서 완성시켜준다.
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
}

#define RESOURCE Engine::ResourceManager::GetInstance()