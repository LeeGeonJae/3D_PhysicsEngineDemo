#pragma once

#include <map>
#include <memory>
#include <unordered_map>
#include <physx\PxPhysicsAPI.h>

#include "Common.h"
#include "CharacterPhysics.h"

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

		template <typename ...Params>
		bool AddArticulationLink(unsigned int id, const CharacterLinkInfo& info, int* collisionMatrix, Params... params);

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

	template<typename ...Params>
	bool PhysicsCharacterPhysicsManager::AddArticulationLink(unsigned int id, const CharacterLinkInfo& info, int* collisionMatrix, Params ...params)
	{
		assert(mCharacterPhysicsContainer.find(id) != mCharacterPhysicsContainer.end());
		assert(mCollisionDataConttainer.find(id) != mCollisionDataConttainer.end());

		std::shared_ptr<CharacterPhysics> characterPhysics = mCharacterPhysicsContainer.find(id)->second;
		std::shared_ptr<CollisionData> collisionData = mCollisionDataConttainer.find(id)->second;

		characterPhysics->AddArticulationLink(info, collisionData, collisionMatrix, params...);

		return true;
	}
}