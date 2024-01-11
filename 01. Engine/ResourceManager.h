#pragma once

#include "ResourceBase.h"

namespace Engine
{
	class StaticMesh;
	class SkeletalMesh;
	class Material;
	class Shader;
	class Animation;
	class Texture;
	class Bone;

	class ResourceManager
	{
		SINGLETON(ResourceManager)
	public:
		template <typename T>
		shared_ptr<T> Load(const wstring& _key, const wstring& _path);

		template <typename T>
		ResourceType GetResourceType(const wstring& key);

	private:
		using KeyObjMap = unordered_map<wstring, shared_ptr<ResourceBase>>;
		array<KeyObjMap, RESOURCE_TYPE_COUNT> m_ResourceMap;
	};

	template<typename T>
	inline shared_ptr<T> ResourceManager::Load(const wstring& _key, const wstring& _path)
	{
		return shared_ptr<T>();
	}
	template<typename T>
	inline ResourceType ResourceManager::GetResourceType(const wstring& key)
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
		if (is_same_v<T, Animation>)
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