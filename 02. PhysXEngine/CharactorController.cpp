#include "pch.h"
#include "CharactorController.h"

#include "CharactorMovement.h"
#include "PhsicsCharactorHitReportCallback.h"

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
		m_HitReportCallback = std::make_shared<PhsicsCharactorHitReportCallback>();

		physx::PxCapsuleControllerDesc desc;
		desc.height = 30.f;
		desc.radius = 20.f;
		desc.contactOffset = 0.1f;
		desc.stepOffset = 0.f;
		//desc.climbingMode = physx::PxCapsuleClimbingMode::eCONSTRAINED;
		desc.nonWalkableMode = physx::PxControllerNonWalkableMode::ePREVENT_CLIMBING_AND_FORCE_SLIDING;
		desc.slopeLimit = 0.1f;
		desc.maxJumpHeight = 1.f;
		desc.position = physx::PxExtendedVec3(100.f, 60.f, -0.f);
		desc.material = material;
		desc.reportCallback = m_HitReportCallback.get();

		m_Controller = controllerManager->createController(desc);

		physx::PxFilterData filterData;
		filterData.word0 = 2;
		filterData.word1 = 7;
		physx::PxShape* shape;
		m_Controller->getActor()->getShapes(&shape, 1);
		shape->setSimulationFilterData(filterData);
		
		m_CharactorMovement = std::make_shared<CharactorMovement>();
		CharactorMovementInfo info;
		info.Acceleration = 50.f;				// �̵�(x,y) ���ӵ�
		info.StaticFriction = 0.4f;				// ���� ������
		info.DynamicFriction = 0.1f;			// ���� ������
		info.Gravity = 9.81f;					// �߷�
		info.JumpDeceleration = 0.05f;			// ���� ���� ( ������ �� x, z�� ���� )
		info.JumpSpeed = 5.f;					// ����
		info.MaxSpeed = 5.f;					// �ִ� �̵��ӵ�
		info.JumpXZAcceleration = 10.f;			// ���� �� �̵�(x,z) ���ӵ�
		m_CharactorMovement->Initlaize(info);

		physx::PxControllerFilters filters;
		physx::PxFilterData data;

		m_HitReportCallback->SetIsFall(m_CharactorMovement.get());
    }

	void CharactorController::Update(float deltaTime)
	{
		m_CharactorMovement->Update(m_Direction, deltaTime);
		m_Direction = {};

		physx::PxVec3 direction;
		m_CharactorMovement->CopyDirectionToPxVec3(direction);

		m_Controller->move(direction, 0.01f, deltaTime, NULL);

		// �ٴڰ� �浹�ϸ� isfall
		physx::PxControllerState state;
		m_Controller->getState(state);
		if (state.collisionFlags & physx::PxControllerCollisionFlag::eCOLLISION_DOWN)
			m_CharactorMovement->SetIsFall(false);
		else
			m_CharactorMovement->SetIsFall(true);
	}


	void CharactorController::AddDirection(DirectX::SimpleMath::Vector3& inputVector)
	{
		m_Direction += inputVector;
	}
}