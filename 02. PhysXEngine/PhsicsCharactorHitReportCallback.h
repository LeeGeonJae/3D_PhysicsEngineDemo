#pragma once
#include <physx\PxPhysicsAPI.h>

namespace PhysicsEngine
{
	class PhsicsCharactorHitReportCallback : public physx::PxUserControllerHitReport
	{
	public:
		PhsicsCharactorHitReportCallback();
		virtual ~PhsicsCharactorHitReportCallback();

	public:
		virtual void onShapeHit(const physx::PxControllerShapeHit& hit) override;
		virtual void onControllerHit(const physx::PxControllersHit& hit) override;
		virtual void onObstacleHit(const physx::PxControllerObstacleHit& hit) override;

	private:


	};
}