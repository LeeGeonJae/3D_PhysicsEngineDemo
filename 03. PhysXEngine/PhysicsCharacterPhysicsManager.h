#pragma once

#include <map>
#include <memory>
#include <unordered_map>
#include <physx\PxPhysicsAPI.h>

#include "Common.h"

namespace physics
{
	class CharacterPhysics;

	class PhysicsCharacterPhysicsManager
	{
	public:
		PhysicsCharacterPhysicsManager();
		~PhysicsCharacterPhysicsManager();
		
		bool initialize(physx::PxPhysics* physics, physx::PxScene* scene);

		bool CreateCharacterphysics(const CharacterPhysicsInfo& info);

		bool AddArticulationLink(unsigned int id, const CharacterLinkInfo& info, int* collisionMatrix, const DirectX::SimpleMath::Vector3& extent);
		bool AddArticulationLink(unsigned int id, const CharacterLinkInfo& info, int* collisionMatrix, const float& radius);
		bool AddArticulationLink(unsigned int id, const CharacterLinkInfo& info, int* collisionMatrix, const float& halfHeight, const float& radius);

		bool SimulationCharacter(unsigned int id);

		inline std::shared_ptr<CharacterPhysics> GetCharacterPhysics(unsigned int id);

	private:
		physx::PxPhysics* mPhysics;
		physx::PxScene* mScene;

		std::unordered_map<unsigned int, std::shared_ptr<CharacterPhysics>> mCharacterPhysicsContainer;
		std::unordered_map<unsigned int, std::shared_ptr<CollisionData>>	mCollisionDataConttainer;
	};

	std::shared_ptr<CharacterPhysics> PhysicsCharacterPhysicsManager::GetCharacterPhysics(unsigned int id)
	{
		return mCharacterPhysicsContainer.find(id)->second;
	}
}