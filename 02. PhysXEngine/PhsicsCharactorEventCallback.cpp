#include "pch.h"
#include "PhsicsCharactorEventCallback.h"

namespace PhysicsEngine
{
	bool PhsicsCharactorEventCallback::filter(const physx::PxController& a, const physx::PxController& b)
	{

		return false;
	}
}
