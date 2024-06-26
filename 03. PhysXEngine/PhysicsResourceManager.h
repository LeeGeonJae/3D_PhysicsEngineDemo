#pragma once

#include <string>
#include <memory>
#include <unordered_map>
#include <array>

#include <PxPhysicsAPI.h>

#include "ResourceBase.h"

namespace physics
{
	class MaterialResource;
	class ConvexMeshResource;
	class HeightFieldResource;
	class TriangleMeshResource;

	class PhysicsResourceManager
	{
	public:
		PhysicsResourceManager();
		~PhysicsResourceManager();

		bool Initialize(physx::PxPhysics* physics);

		template <typename T>
		std::shared_ptr<T> Find(const std::string& name);

		template <typename T, typename ...Params>
		std::shared_ptr<T> Create(const std::string& name, Params... params);

	private:
		template <typename T>
		const EResourceType& GetResourceType();

	private:
		physx::PxPhysics* mPhysics;

		using KeyObjMap = std::unordered_map<std::string, std::shared_ptr<ResourceBase>>;
		std::array<std::unordered_map<std::string, std::shared_ptr<ResourceBase>>, RESOURCE_TYPE_COUNT> mResourceContainer;
	};

	template<typename T>
	inline std::shared_ptr<T> PhysicsResourceManager::Find(const std::string& path)
	{
		bool bIsBase = std::is_base_of<ResourceBase, T>::value;
		assert(bIsBase == true);

		EResourceType resourceType = GetResourceType<MaterialResource>();
		auto resource = mResourceContainer[static_cast<int>(resourceType)].find(path);

		if (resource != mResourceContainer[static_cast<int>(resourceType)].end())
		{
			return std::dynamic_pointer_cast<T>(resource->second);
		}
		else
		{
			return nullptr;
		}
	}
	template<typename T, typename ...Params>
	inline std::shared_ptr<T> PhysicsResourceManager::Create(const std::string& path, Params... params)
	{
		bool bIsBase = std::is_base_of<ResourceBase, T>::value;
		assert(bIsBase == true);

		EResourceType resourceType = GetResourceType<T>();
		auto resoureceIter = mResourceContainer[static_cast<int>(resourceType)].find(path);

		if (resoureceIter != mResourceContainer[static_cast<int>(resourceType)].end())
		{
			return std::dynamic_pointer_cast<T>(resoureceIter->second);
		}
		else
		{
			std::shared_ptr<T> resource = std::make_shared<T>(mPhysics, params...);
			mResourceContainer[static_cast<int>(resourceType)].insert(std::make_pair(path, resource));
			return resource;
		}
	}
	template<typename T>
	inline const EResourceType& PhysicsResourceManager::GetResourceType()
	{
		if (std::is_same_v<T, MaterialResource>)
		{
			return EResourceType::MATERIAL;
		}
		else if (std::is_same_v<T, ConvexMeshResource>)
		{
			return EResourceType::CONVEX_MESH;
		}
		else if (std::is_same_v<T, TriangleMeshResource>)
		{
			return EResourceType::TRIANGLE_MESH;
		}
		else if (std::is_same_v<T, HeightFieldResource>)
		{
			return EResourceType::HEIGHT_FIELD_MESH;
		}

		return EResourceType::END;
	}
}