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
		/// ENTER_COLLISION 충돌 이벤트 실행
		if (pairs[0].events & (physx::PxPairFlag::eNOTIFY_TOUCH_FOUND | physx::PxPairFlag::eNOTIFY_TOUCH_CCD))
		{
			ActorUserData* myData = (ActorUserData*)pairHeader.actors[0]->userData;
			ActorUserData* otherData = (ActorUserData*)pairHeader.actors[1]->userData;

			std::cout << "=====================================================" << std::endl;
			if (myData->GetActorType() == ActorType::MONSTER)
			{
				std::cout << "I'm Monster!! Begin Collision" << std::endl;
			}
			if (myData->GetActorType() == ActorType::PLAYER)
			{
				std::cout << "I'm Player!! Begin Collision" << std::endl;
			}
			if (myData->GetActorType() == ActorType::TILE)
			{
				std::cout << "I'm Tile!! Begin Collision" << std::endl;
			}
			if (otherData->GetActorType() == ActorType::MONSTER)
			{
				std::cout << "You're Monster!! Begin Collision" << std::endl;
			}
			if (otherData->GetActorType() == ActorType::PLAYER)
			{
				std::cout << "You're Player!! Begin Collision" << std::endl;
			}
			if (otherData->GetActorType() == ActorType::TILE)
			{
				std::cout << "I'm Tile!! Begin Collision" << std::endl;
			}
			std::cout << "=====================================================" << std::endl;
		}

		/// END_COLLISION 충돌 이벤트 실행
		else if (pairs[0].events & (physx::PxPairFlag::eNOTIFY_TOUCH_LOST | physx::PxPairFlag::eNOTIFY_TOUCH_CCD))
		{
			ActorUserData* myData = (ActorUserData*)pairHeader.actors[0]->userData;
			ActorUserData* otherData = (ActorUserData*)pairHeader.actors[1]->userData;

			std::cout << "=====================================================" << std::endl;
			if (myData->GetActorType() == ActorType::MONSTER)
			{
				std::cout << "I'm Monster!! End Collision" << std::endl;
			}
			if (myData->GetActorType() == ActorType::PLAYER)
			{
				std::cout << "I'm Player!! End Collision" << std::endl;
			}
			if (myData->GetActorType() == ActorType::TILE)
			{
				std::cout << "I'm Tile!! End Collision" << std::endl;
			}
			if (otherData->GetActorType() == ActorType::MONSTER)
			{
				std::cout << "You're Monster!! End Collision" << std::endl;
			}
			if (otherData->GetActorType() == ActorType::PLAYER)
			{
				std::cout << "You're Player!! End Collision" << std::endl;
			}
			if (otherData->GetActorType() == ActorType::TILE)
			{
				std::cout << "I'm Tile!! End Collision" << std::endl;
			}
			std::cout << "=====================================================" << std::endl;
		}

		///// ON_COLLSION 충돌 이벤트 실행
		//else if (pairs[0].events & (physx::PxPairFlag::eNOTIFY_TOUCH_PERSISTS | physx::PxPairFlag::eNOTIFY_TOUCH_CCD))
		//{
		//	ActorUserData* myData = (ActorUserData*)pairHeader.actors[0]->userData;
		//	ActorUserData* otherData = (ActorUserData*)pairHeader.actors[1]->userData;

		//	if (myData->GetActorType() == ActorType::MONSTER)
		//	{
		//		std::cout << "Monster!! On Collision" << std::endl;
		//	}
		//	if (myData->GetActorType() == ActorType::PLAYER)
		//	{
		//		std::cout << "Player!! On Collision" << std::endl;
		//	}
		//}
	}

	void PhysicsSimulationEventCallback::onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count)
	{
		std::cout << "OnTrigger" << std::endl;

		//ActorUserData* data = (ActorUserData*)pairs->triggerActor->userData;

		//if (pairs->status == physx::PxPairFlag::eNOTIFY_TOUCH_FOUND)
		//	std::cout << "eNOTIFY_TOUCH_FOUND" << std::endl;

		//if (pairs->status == physx::PxPairFlag::eNOTIFY_TOUCH_LOST)
		//	std::cout << "eNOTIFY_TOUCH_LOST" << std::endl;

		//if (data->GetActorType() == ActorType::MONSTER)
		//{
		//	std::cout << "Monster!!!!" << std::endl;
		//}
	}

	void PhysicsSimulationEventCallback::onAdvance(const physx::PxRigidBody* const* bodyBuffer, const physx::PxTransform* poseBuffer, const physx::PxU32 count)
	{
		std::cout << "OnAdvance " << std::endl;
	}
}