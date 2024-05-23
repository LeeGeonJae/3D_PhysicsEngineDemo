#pragma once
#include <physx\PxPhysicsAPI.h>

#include <memory>

namespace physics
{
	using namespace physx;

	class CharacterQueryFilterCallback : public physx::PxQueryFilterCallback
	{
	public:
		CharacterQueryFilterCallback(std::shared_ptr<physx::PxFilterData> filterData);
		virtual ~CharacterQueryFilterCallback() {}

		virtual PxQueryHitType::Enum preFilter(const PxFilterData& filterData, const PxShape* shape, const PxRigidActor* actor, PxHitFlags& queryFlags) override;
		virtual PxQueryHitType::Enum postFilter(const PxFilterData& filterData, const PxQueryHit& hit, const PxShape* shape, const PxRigidActor* actor) override;

	public:
		std::shared_ptr<physx::PxFilterData> mFilterData;
	};
}


