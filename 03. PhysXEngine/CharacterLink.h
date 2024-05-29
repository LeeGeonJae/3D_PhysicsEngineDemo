#pragma once

#include <vector>
#include <memory>
#include <string>

#include <directxtk\SimpleMath.h>
#include <physx\PxPhysicsAPI.h>

#include "Common.h"

namespace physics
{
	class CharacterJoint;

	class CharacterLink : public std::enable_shared_from_this<CharacterLink>
	{
	public:
		CharacterLink();
		~CharacterLink();

		bool Initialize(const CharacterLinkInfo& info, std::shared_ptr<CharacterLink> parentLink, physx::PxArticulationReducedCoordinate* pxArticulation);

		physx::PxShape* CreateShape(const physx::PxMaterial* material, std::shared_ptr<CollisionData> collisionData, const DirectX::SimpleMath::Vector3& extent);
		physx::PxShape* CreateShape(const physx::PxMaterial* material, std::shared_ptr<CollisionData> collisionData, const float& radius, const float& halfHeight);
		physx::PxShape* CreateShape(const physx::PxMaterial* material, std::shared_ptr<CollisionData> collisionData, const float& radius);

		inline physx::PxArticulationLink* GetPxLink();
		inline const std::string& GetName();
		inline const DirectX::SimpleMath::Matrix& GetLocalTransform();
		inline const std::shared_ptr<CharacterJoint> GetCharacterJoint();
		inline const std::weak_ptr<CharacterLink> GetParentLink();
		inline const std::vector<std::weak_ptr<CharacterLink>>& GetChildrenCharacterLink();
		inline const std::weak_ptr<CharacterLink> GetChildCharacterLink(std::string linkName);

	private:
		std::string mName;
		float mDensity;
		DirectX::SimpleMath::Matrix mLocalTransform;

		std::shared_ptr<CharacterJoint> mMyJoint;
		std::weak_ptr<CharacterLink> mParentLink;
		std::vector<std::weak_ptr<CharacterLink>> mMyChildrenLink;

	private:
		physx::PxArticulationLink* mPxLink;
	};

#pragma region GetSet
	physx::PxArticulationLink* CharacterLink::GetPxLink()
	{
		return mPxLink;
	}
	const std::string& CharacterLink::GetName()
	{
		return mName;
	}
	const DirectX::SimpleMath::Matrix& CharacterLink::GetLocalTransform()
	{
		return mLocalTransform;
	}
	const std::shared_ptr<CharacterJoint> CharacterLink::GetCharacterJoint()
	{
		return mMyJoint;
	}
	const std::weak_ptr<CharacterLink> CharacterLink::GetParentLink()
	{
		return mParentLink;
	}
	const std::vector<std::weak_ptr<CharacterLink>>& CharacterLink::GetChildrenCharacterLink()
	{
		return mMyChildrenLink;
	}
	const std::weak_ptr<CharacterLink> CharacterLink::GetChildCharacterLink(std::string linkName)
	{
		for (const auto& childLink : mMyChildrenLink)
		{
			if (childLink.lock()->GetName() == linkName)
			{
				return childLink;
			}
		}
	}
#pragma endregion
}