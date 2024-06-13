#include "pch.h"
#include "MovementComponent.h"

#include "SceneComponent.h"
#include "CollisionComponent.h"
#include "Object.h"

namespace Engine
{
	MovementComponent::MovementComponent()
	{
		m_MaxSpeed = 100.f;
		m_Velocity = 100.f;
	}

	MovementComponent::~MovementComponent()
	{
	}

	void MovementComponent::Setting(MovementComponentInfo _info)
	{
		// ���� : ���� ����ü�� ���� ��� �Լ� ����
		SetName(_info.m_Name);
		m_MaxSpeed = _info.m_MaxSpeed;
		m_Acceleration = _info.m_Acceleration;
	}

	void MovementComponent::AddInputVector(const Vector3& input)
	{
		m_InputVector += input;
		m_InputVector.Normalize();
	}

	void MovementComponent::Update(float _deltaTime)
	{
		// ���� : �Է��� ������ ��
		if (m_InputVector.Length() > 0.f)
		{
			m_PositionBefore = GetMyObject().lock()->GetPosition();
			m_PositionAfter = m_PositionBefore + m_InputVector * m_Velocity * _deltaTime;
			GetMyObject().lock()->SetPosition(m_PositionAfter);
			m_InputVector = Vector3::Zero;
		}
	}
}
