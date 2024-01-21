#pragma once
#include "Component.h"
#include "Types.h"

namespace Engine
{
	struct MovementComponentInfo
	{
		string m_Name;
		float m_Acceleration;		// 건재 : 가속도
		float m_MaxSpeed;			// 건재 : 속도
	};

	// 건재 : 오브젝트가 움직일 때 필요한 컴포넌트
	class MovementComponent : public Component
	{
	public:
		MovementComponent();
		virtual ~MovementComponent();

	public:
		void Setting(MovementComponentInfo _info);

	public:
		virtual void Update() override;
		void AddInputVector(const Vector3& input);

	public:
		inline float GetMaxSpeed();
		inline void SetMaxSpeed(float _speed);
		inline float GetAcceleration();
		inline void SetAcceleration(float _acceleration);
		inline Vector3 GetBeforePosition();
		inline void SetBeforePosition(Vector3 _position);
		inline Vector3 GetAfterPosition();
		inline void SetAfterPosition(Vector3 _position);
		inline Vector3 GetInputVector();

	private:
		Vector3 m_InputVector;
		float m_Acceleration;
		float m_Velocity;
		float m_MaxSpeed;

		Vector3 m_PositionBefore;
		Vector3 m_PositionAfter;
	};

	float MovementComponent::GetMaxSpeed()
	{
		return m_MaxSpeed;
	}
	void MovementComponent::SetMaxSpeed(float _speed)
	{
		m_MaxSpeed = _speed;
	}
	float MovementComponent::GetAcceleration()
	{
		return m_Acceleration;
	}
	void MovementComponent::SetAcceleration(float _acceleration)
	{
		m_Acceleration = _acceleration;
	}
	Vector3 MovementComponent::GetBeforePosition()
	{
		return m_PositionBefore;
	}
	void MovementComponent::SetBeforePosition(Vector3 _position)
	{
		m_PositionBefore = _position;
	}
	Vector3 MovementComponent::GetAfterPosition()
	{
		return m_PositionAfter;
	}
	void MovementComponent::SetAfterPosition(Vector3 _position)
	{
		m_PositionAfter = _position;
	}
	Vector3 MovementComponent::GetInputVector()
	{
		return m_InputVector;
	}
}


