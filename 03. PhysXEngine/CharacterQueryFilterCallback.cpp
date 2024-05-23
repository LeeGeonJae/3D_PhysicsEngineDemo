#include "CharacterQueryFilterCallback.h"

namespace physics
{
	CharacterQueryFilterCallback::CharacterQueryFilterCallback(std::shared_ptr<physx::PxFilterData> filterData)
		: mFilterData(filterData)
	{
	}

	PxQueryHitType::Enum CharacterQueryFilterCallback::preFilter(const PxFilterData& filterData, const PxShape* shape, const PxRigidActor* actor, PxHitFlags& queryFlags)
	{
		if ((((1 << mFilterData->word0) & filterData.word1) > 0) && (((1 << filterData.word0) & mFilterData->word1) > 0))
		{
			return PxQueryHitType::eBLOCK;
		}

		return PxQueryHitType::eBLOCK;
	}

	PxQueryHitType::Enum CharacterQueryFilterCallback::postFilter(const PxFilterData& filterData, const PxQueryHit& hit, const PxShape* shape, const PxRigidActor* actor)
	{
		//if ((((1 << mFilterData->word0) & filterData.word1) > 0) && (((1 << filterData.word0) & mFilterData->word1) > 0))
		//{
		//	return PxQueryHitType::eBLOCK;
		//}

		return PxQueryHitType::eBLOCK;
	}
}
