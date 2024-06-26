#include "PhysicsResourceManager.h"

namespace physics
{
	PhysicsResourceManager::PhysicsResourceManager()
		: mPhysics(nullptr)
		, mResourceContainer()
	{
	}

	PhysicsResourceManager::~PhysicsResourceManager()
	{
	}

	bool PhysicsResourceManager::Initialize(physx::PxPhysics* physics)
	{
		mPhysics = physics;
		return true;
	}
}
