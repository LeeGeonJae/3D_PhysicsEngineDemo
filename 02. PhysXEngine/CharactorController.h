#pragma once

#include <memory>
#include <PxPhysicsAPI.h>
#include <directxtk/SimpleMath.h>

namespace PhysicsEngine
{
	class CharactorMovement;
	class PhsicsCharactorHitReportCallback;

	class CharactorController
	{
	public:
		CharactorController();
		~CharactorController();

	public:
		void Initialzie(physx::PxMaterial* material, physx::PxControllerManager* controllerManager);
		void Update(float deltaTime);

		void AddDirection(DirectX::SimpleMath::Vector3& inputVector);
		inline physx::PxController* GetPxController();

	private:
		DirectX::SimpleMath::Vector3 m_Direction;

		physx::PxController* m_Controller = nullptr;
		std::shared_ptr<CharactorMovement> m_CharactorMovement = nullptr;
		std::shared_ptr<PhsicsCharactorHitReportCallback> m_HitReportCallback = nullptr;

	};

	physx::PxController* CharactorController::GetPxController()
	{
		return m_Controller;
	}
}