#include "PhysicsResourceManager.h"

namespace physics
{
	PhysicsResourceManager::PhysicsResourceManager()
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
