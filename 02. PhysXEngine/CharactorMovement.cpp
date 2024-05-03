#include "pch.h"
#include "CharactorMovement.h"
#include <algorithm>
#include <iostream>

float square(float x)
{
	if (x < 0)
		return (x = -1 * (x * x));
	else
		return (x * x);
}

namespace PhysicsEngine
{
	CharactorMovement::CharactorMovement()
		: m_Direction{}
		, m_Velocity{}
		, m_MaxSpeed(0)
		, m_Speed(0)
		, m_Acceleration(0)
		, m_StaticFriction(0)
		, m_DynamicFriction(0)
		, m_JumpSpeed(0)
		, m_JumpDeceleration(0)
		, m_Gravity(0)
		, m_MinDistance(0.f)
	{
	}

	CharactorMovement::~CharactorMovement()
	{
	}

	void CharactorMovement::Initlaize(CharactorMovementInfo info)
	{
		m_Acceleration = info.Acceleration;
		m_StaticFriction = info.StaticFriction;
		m_DynamicFriction = info.DynamicFriction;
		m_Gravity = info.Gravity;
		m_JumpDeceleration = info.JumpDeceleration;
		m_JumpSpeed = info.JumpSpeed;
		m_MaxSpeed = info.MaxSpeed;

		m_MinDistance = 0.01f;
	}

	void CharactorMovement::Update(DirectX::SimpleMath::Vector3& inputVector, float deltaTime)
	{
		// 정지 상태인 경우 속도를 감소시킴
		if (inputVector.x == 0)
			m_Velocity.x = std::lerp(m_Velocity.x, 0.f, m_StaticFriction);
		else
			m_Velocity.x = std::lerp(m_Velocity.x, 0.f, m_DynamicFriction);
		if (inputVector.z == 0)
			m_Velocity.z = std::lerp(m_Velocity.z, 0.f, m_StaticFriction);
		else
			m_Velocity.z = std::lerp(m_Velocity.z, 0.f, m_DynamicFriction);

		m_Velocity.x += (inputVector.x * m_Acceleration * deltaTime);
		m_Velocity.z += (inputVector.z * m_Acceleration * deltaTime);

		// 속도 제한 적용
		LimitVelocity();

		// 속도 계산
		Compute(deltaTime);
	}

	void CharactorMovement::LimitVelocity()
	{
		const float maxSpeed = m_MaxSpeed;

		m_Velocity.x = std::clamp(m_Velocity.x, -maxSpeed, maxSpeed);
		m_Velocity.z = std::clamp(m_Velocity.z, -maxSpeed, maxSpeed);
	}

	bool CharactorMovement::Compute(float deltaTime)
	{
		// 중력 적용
		m_Velocity.y -= m_Gravity * deltaTime;

		// 현재 속도 계산
		m_Speed = abs(m_Velocity.x) + abs(m_Velocity.z);

		// 최대 속도 제한
		if (m_Speed > m_MaxSpeed)
			m_Speed = m_MaxSpeed;

		// 이동 방향 계산
		float triangleFunction = (m_Velocity.x * m_Velocity.x) + (m_Velocity.z * m_Velocity.z);
		m_Direction.x = square(m_Velocity.x) / triangleFunction * m_Speed;
		m_Direction.z = square(m_Velocity.z) / triangleFunction * m_Speed;

		std::cout << m_Direction.x << ", " << m_Direction.z << ", " << m_Speed << std::endl;

		return true;
	}

	void CharactorMovement::CopyDirectionToPxVec3(physx::PxVec3& direction)
	{
		direction.x = m_Direction.x;
		direction.y = m_Direction.y;
		direction.z = -m_Direction.z;
	}
}
