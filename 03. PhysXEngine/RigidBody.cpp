#include "pch.h"
#include "RigidBody.h"

namespace physics
{
	RigidBody::RigidBody(physics::EColliderType colliderType, unsigned int id, unsigned int layerNumber)
		: mColliderType(colliderType)
		, mID(id)
		, mLayerNumber(layerNumber)
	{
	}

	RigidBody::~RigidBody()
	{
	}
}
