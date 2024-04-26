#pragma once
#include <physx/PxPhysicsAPI.h>
#include <vector>

namespace PhysicsEngine
{
	struct CollisionInfomation
	{

	};

	class PhysicsSimulationEventCallback : public physx::PxSimulationEventCallback
	{
	public:
		PhysicsSimulationEventCallback();
		virtual ~PhysicsSimulationEventCallback();

	public:
		virtual void onConstraintBreak(physx::PxConstraintInfo* constraints, physx::PxU32 count) override;
		virtual void onWake(physx::PxActor** actors, physx::PxU32 count) override;
		virtual void onSleep(physx::PxActor** actors, physx::PxU32 count) override;
		virtual void onContact(const physx::PxContactPairHeader& pairHeader, const physx::PxContactPair* pairs, physx::PxU32 nbPairs) override;
		virtual void onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count) override;
		virtual void onAdvance(const physx::PxRigidBody* const* bodyBuffer, const physx::PxTransform* poseBuffer, const physx::PxU32 count) override;

	public:
		static std::vector<physx::PxVec3> CollisionPoints;
	};
}