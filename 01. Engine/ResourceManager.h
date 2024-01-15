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
		shared_ptr<T> Load( const string& _path, const string& key);

		template <typename T>
		shared_ptr<T> Find(const string& _path, const string& key);

		template <typename T>
		ResourceType GetResourceType();

	private:
		using KeyObjMap = unordered_map<string, vector<shared_ptr<ResourceBase>>>;
		array<KeyObjMap, RESOURCE_TYPE_COUNT> m_ResourceMap;
	};

	template<typename T>
	inline shared_ptr<T> ResourceManager::Load(const string& _path, const string& _key)
	{
		// 만약 해당 클래스가 리소스 베이스 클래스를 상속 받지 않는 클래스면 종료
		assert(!is_base_of_v<ResourceBase, T>);

		ResourceType resourceType = GetResourceType<T>();
		vector<shared_ptr<T>> resource = Find<T>(_path, _key);

		// 리소스를 찾으면 반환
		if (resource != nullptr)
			return resource;

		// 리소스를 찾지 못하면 생성하고 저장
		resource = make_shared<T>();
		resource->Create(_path);			// FBX Load 및 텍스쳐 Load

		auto resourceIter = m_ResourceMap[static_cast<int>(resourceType)].find(_path);

		// 해당 경로의 리소스맵이 있을 경우에 해당 벡터에 푸시 백 아니면 벡터 생성 후에 맵에 집어넣기
		if (resourceIter != m_ResourceMap[static_cast<int>(resourceType)].end())
		{
			resourceIter->second.push_back(resource);
		}
		else
		{
			vector<shared_ptr<ResourceBase>> resourceVec;
			resourceVec.push_back(resource);
			m_ResourceMap[static_cast<int>(GetResourceType<T>())].insert(make_pair(_path, resourceVec));
		}

		return resource;
	}

	template<typename T>
	inline shared_ptr<T> ResourceManager::Find(const string& _path, const string& _key)
	{
		// 만약 해당 클래스가 리소스 베이스 클래스를 상속 받지 않는 클래스면 종료
		assert(!is_base_of_v<ResourceBase, T>());

		ResourceType resourceType = GetResourceType<T>();

		// 리소스를 찾으면 반환
		auto resourceIter = m_ResourceMap[static_cast<int>(resourceType)].find(_path);
		
		if (resourceIter != m_ResourceMap[static_cast<int>(resourceType)].end())
		{
			// 
			for (auto resource : resourceIter->second)
			{
				if (resource->GetName() == key)
					return resource;
			}
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