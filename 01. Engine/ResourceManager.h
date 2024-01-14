#pragma once

#include "ResourceBase.h"

namespace Engine
{
	class StaticMesh;
	class SkeletalMesh;
	class Material;
	class Shader;
	class AnimationResource;
	class Texture;
	class Bone;

	class ResourceManager
	{
		SINGLETON(ResourceManager)
	public:
		template <typename T>
		shared_ptr<T> Load( const string& _path);

		template <typename T>
		shared_ptr<T> Find(const string& _path);

		template <typename T>
		ResourceType GetResourceType();

	private:
		using KeyObjMap = unordered_map<string, shared_ptr<ResourceBase>>;
		array<KeyObjMap, RESOURCE_TYPE_COUNT> m_ResourceMap;
	};

	template<typename T>
	inline shared_ptr<T> ResourceManager::Load(const string& _path)
	{
		// 만약 해당 클래스가 리소스 베이스 클래스를 상속 받지 않는 클래스면 종료
		assert(!is_base_of_v<ResourceBase, T>);

		shared_ptr<ResourceBase> resource = Find<T>(_path);

		// 리소스를 찾으면 반환
		if (resource != nullptr)
			return resource;

		// 리소스를 찾지 못하면 생성하고 저장
		resource = make_shared<T>();
		resource->Create(_path);
		m_ResourceMap[static_cast<int>(GetResourceType<T>())].insert(make_pair(_path, resource));
		return resource;
	}

	template<typename T>
	inline shared_ptr<T> ResourceManager::Find(const string& _path)
	{
		// 만약 해당 클래스가 리소스 베이스 클래스를 상속 받지 않는 클래스면 종료
		assert(!is_base_of_v<ResourceBase, T>());

		ResourceType resourceType = GetResourceType<T>();

		// 리소스를 찾으면 반환
		auto resourceIter = m_ResourceMap[static_cast<int>(resourceType)].find(_path);
		
		if (resourceIter != m_ResourceMap[static_cast<int>(resourceType)].end())
		{
			return resourceIter->second;
		}

		// 못찾으면 nullptr
		return nullptr;
	}

	template<typename T>
	inline ResourceType ResourceManager::GetResourceType()
	{
		// constexpr을 붙이면 컴파일 단계에서 완성시켜준다.
		if (is_same_v<T, StaticMesh>)
			return ResourceType::StaticMesh;
		if (is_same_v<T, SkeletalMesh>)
			return ResourceType::SkeletalMesh;
		if (is_same_v<T, Material>)
			return ResourceType::Material;
		if (is_same_v<T, Shader>)
			return ResourceType::Shader;
		if (is_same_v<T, AnimationResource>)
			return ResourceType::Animation;
		if (is_same_v<T, Texture>)
			return ResourceType::Texture;
		if (is_same_v<T, Bone>)
			return ResourceType::Bone;

		assert(false);
		return ResourceType::None;
	}
}

#define RESOURCE Engine::ResourceManager::GetInstance()