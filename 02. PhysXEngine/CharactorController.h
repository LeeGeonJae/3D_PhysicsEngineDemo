#pragma once

#include <memory>
#include <physx\PxPhysicsAPI.h>
#include <directxtk/SimpleMath.h>

namespace PhysicsEngine
{
	class CharactorMovement;

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
	};

	physx::PxController* CharactorController::GetPxController()
	{
		return m_Controller;
	}
}