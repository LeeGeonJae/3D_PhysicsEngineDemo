#include "pch.h"
#include "PhysicsSimulationEventCallback.h"
#include "ActorUserData.h"

#include <iostream>
#include <vector>

namespace PhysicsEngine
{
	std::vector<physx::PxVec3> PhysicsSimulationEventCallback::CollisionPoints;

	PhysicsSimulationEventCallback::PhysicsSimulationEventCallback()
	{

	}

	PhysicsSimulationEventCallback::~PhysicsSimulationEventCallback()
	{

	}

	void PhysicsSimulationEventCallback::onConstraintBreak(physx::PxConstraintInfo* constraints, physx::PxU32 count)
	{
		std::cout << "onConstraintBreak " << std::endl;
	}

	void PhysicsSimulationEventCallback::onWake(physx::PxActor** actors, physx::PxU32 count)
	{
		std::cout << "OnWake " << std::endl;
	}

	void PhysicsSimulationEventCallback::onSleep(physx::PxActor** actors, physx::PxU32 count)
	{
		std::cout << "OnSleep " << std::endl;
	}

	void PhysicsSimulationEventCallback::onContact(const physx::PxContactPairHeader& pairHeader, const physx::PxContactPair* pairs, physx::PxU32 nbPairs)
	{
		std::vector<physx::PxContactPairPoint> contactPoints;

		physx::PxTransform spherePose(physx::PxIdentity);
		physx::PxU32 nextPairIndex = 0xfffffffff;

		physx::PxContactPairExtraDataIterator iter(pairHeader.extraDataStream, pairHeader.extraDataStreamSize);
		bool hasItemSet = iter.nextItemSet();
		if (hasItemSet)
			nextPairIndex = iter.contactPairIndex;

		for (physx::PxU32 i = 0; i < nbPairs; i++)
		{
			const physx::PxContactPair& cp = pairs[i];

			// 충돌 시 동적 물체의 자세를 취합니다.
			if (nextPairIndex == i)
			{
				if (pairHeader.actors[0]->is<physx::PxRigidDynamic>())
					spherePose = iter.eventPose->globalPose[0];
				else
					spherePose = iter.eventPose->globalPose[1];

				//ActorUserData* data = static_cast<ActorUserData*>(pairHeader.actors[i]->is<physx::PxShape>()->userData);
				//
				//if (data->GetActorType() == ActorType::PLAYER)
				//{
				//	std::cout << "Player" << std::endl;
				//}

				//std::cout << "오브젝트 위치 (" << spherePose.p.x << ", " << spherePose.p.y << ", " << spherePose.p.z << ")" << std::endl;

				hasItemSet = iter.nextItemSet();
				if (hasItemSet)
					nextPairIndex = iter.contactPairIndex;
			}

			if (cp.events & (physx::PxPairFlag::eNOTIFY_TOUCH_FOUND | physx::PxPairFlag::eNOTIFY_TOUCH_CCD))
			{
				physx::PxU32 contactCount = cp.contactCount;
				contactPoints.resize(contactCount);
				cp.extractContacts(&contactPoints[0], contactCount);

				for (physx::PxU32 j = 0; j < contactCount; j++)
				{
					CollisionPoints.push_back(contactPoints[j].position);

					std::cout << "==========================================================================" << std::endl;
					std::cout << "위치 : (" << contactPoints[j].position.x << ", " << contactPoints[j].position.y << ", " << contactPoints[j].position.z << ")" << std::endl;
					std::cout << "충돌 : (" << contactPoints[j].impulse.x << ", " << contactPoints[j].impulse.y << ", " << contactPoints[j].impulse.z << ")" << std::endl;
					std::cout << "==========================================================================" << std::endl;
				}
			}
		}
	}

	void PhysicsSimulationEventCallback::onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count)
	{
		std::cout << "OnTrigger" << std::endl;
	}

	void PhysicsSimulationEventCallback::onAdvance(const physx::PxRigidBody* const* bodyBuffer, const physx::PxTransform* poseBuffer, const physx::PxU32 count)
	{
		std::cout << "OnAdvance " << std::endl;
	}
}