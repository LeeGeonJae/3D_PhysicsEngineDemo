#pragma once

#include <physx\PxPhysicsAPI.h>

namespace PhysicsEngine
{
	class PhsicsCharactorEventCallback : physx::PxControllerFilterCallback
	{
	public:
		PhsicsCharactorEventCallback() {}
		virtual ~PhsicsCharactorEventCallback() {}

		/**
		\brief Filtering method for CCT-vs-CCT.

		\param[in] a	First CCT
		\param[in] b	Second CCT
		\return true to keep the pair, false to filter it out
		*/
		virtual bool filter(const physx::PxController& a, const physx::PxController& b) override;

	};

}