#include "pch.h"
#include "CharactorController.h"

#include "CharactorMovement.h"

namespace PhysicsEngine
{
    CharactorController::CharactorController()
    {
    }

    CharactorController::~CharactorController()
    {
    }

    void CharactorController::Initialzie(physx::PxMaterial* material, physx::PxControllerManager* controllerManager)
    {
		physx::PxCapsuleControllerDesc desc;
		desc.height = 10.f;
		desc.radius = 5.f;
		desc.contactOffset = 0.01f;
		desc.stepOffset = 0.f;
		//desc.climbingMode = physx::PxCapsuleClimbingMode::eCONSTRAINED;
		desc.nonWalkableMode = physx::PxControllerNonWalkableMode::ePREVENT_CLIMBING_AND_FORCE_SLIDING;
		desc.slopeLimit = 0.f;
		desc.maxJumpHeight = 20.f;
		desc.position = physx::PxExtendedVec3(0.f, 10.f, -100.f);
		desc.material = material;

		m_Controller = controllerManager->createController(desc);

		m_CharactorMovement = std::make_shared<CharactorMovement>();
		CharactorMovementInfo info;
		info.Acceleration = 1000.f;
		info.StaticFriction = 0.1f;
		info.DynamicFriction = 0.4f;
		info.Gravity = 9.81f;
		info.JumpDeceleration = 10.f;
		info.JumpSpeed = 500.f;
		info.MaxSpeed = 10.f;
		m_CharactorMovement->Initlaize(info);
    }

	void CharactorController::Update(float deltaTime)
	{
		m_CharactorMovement->Update(m_Direction, deltaTime);
		m_Direction = {};

		physx::PxVec3 direction;
		m_CharactorMovement->CopyDirectionToPxVec3(direction);

		m_Controller->move(direction, 0.01f, deltaTime, NULL);
	}


	void CharactorController::AddDirection(DirectX::SimpleMath::Vector3& inputVector)
	{
		m_Direction = m_Direction + inputVector;
	}
}