#include "pch.h"
#include "MovementComponent.h"

#include "TimeManager.h"

#include "CollisionComponent.h"
#include "Object.h"

namespace Engine
{
	MovementComponent::MovementComponent()
	{
		m_MaxSpeed = 400.f;
	}

	MovementComponent::~MovementComponent()
	{
	}

	void MovementComponent::Setting(MovementComponentInfo _info)
	{
		// 건재 : 정보 구조체에 따라 멤버 함수 세팅
		SetName(_info.m_Name);
		m_MaxSpeed = _info.m_MaxSpeed;
		m_Acceleration = _info.m_Acceleration;
	}

	void MovementComponent::AddInputVector(const Vector3& input)
	{
		m_InputVector += input;
		m_InputVector.Normalize();
	}

	void MovementComponent::Update()
	{
		// 건재 : 입력이 들어왔을 때
		if (m_InputVector.Length() > 0.f)
		{
			m_PositionBefore = GetMyObject()->GetPosition();
			m_PositionAfter = m_PositionBefore + m_InputVector * m_Velocity * TimeManager::GetInstance()->GetfDT();
			GetMyObject()->SetPosition(m_PositionAfter);
			m_InputVector = Vector3::Zero;
		}
	}
}
