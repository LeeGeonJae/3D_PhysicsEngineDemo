#include "MaterialResource.h"

namespace physics
{
	MaterialResource::MaterialResource(physx::PxPhysics* physics, float staticFriction, float dynamicFriction, float restitution)
		: ResourceBase(EResourceType::MATERIAL)
	{
		mMaterial = physics->createMaterial(staticFriction, dynamicFriction, restitution);
	}

	MaterialResource::~MaterialResource()
	{
		PX_RELEASE(mMaterial);
	}
}
