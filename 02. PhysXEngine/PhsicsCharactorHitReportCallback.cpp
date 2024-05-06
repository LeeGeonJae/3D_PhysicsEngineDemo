#include "pch.h"
#include "PhsicsCharactorHitReportCallback.h"

#include "CharactorMovement.h"


namespace PhysicsEngine
{
	PhsicsCharactorHitReportCallback::PhsicsCharactorHitReportCallback()
	{

	}

	PhsicsCharactorHitReportCallback::~PhsicsCharactorHitReportCallback()
	{

	}

	void PhsicsCharactorHitReportCallback::onShapeHit(const physx::PxControllerShapeHit& hit)
	{
		int a = 0;
	}

	void PhsicsCharactorHitReportCallback::onControllerHit(const physx::PxControllersHit& hit)
	{
		int a = 0;
	}

	void PhsicsCharactorHitReportCallback::onObstacleHit(const physx::PxControllerObstacleHit& hit)
	{
		int a = 0;
	}
}