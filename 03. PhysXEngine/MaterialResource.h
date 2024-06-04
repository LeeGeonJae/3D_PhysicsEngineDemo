#pragma once
#include "ResourceBase.h"

#include <PxPhysicsAPI.h>

namespace physics
{
	class MaterialResource : public ResourceBase
	{
	public:
		MaterialResource(physx::PxPhysics* physics, float staticFriction, float dynamicFriction, float restitution);
		virtual ~MaterialResource();

		inline physx::PxMaterial* GetPxMaterial();

	private:
		physx::PxMaterial* mMaterial;
	};

	physx::PxMaterial* MaterialResource::GetPxMaterial()
	{
		return mMaterial;
	}
}

