#pragma once

#include <directxtk\SimpleMath.h>

namespace PhysicsEngine
{
	enum class CharactorState
	{
		IDLE,
		WALK,
		RUN,
		JUMP,
		DUMBLING
	};

	class CharactorMovement
	{
	public:
		CharactorMovement();
		~CharactorMovement();

		void AddMovement(DirectX::SimpleMath::Vector3 inputVector);
		bool CheckMaxSpeed();

	private:
		DirectX::SimpleMath::Vector3 m_Velocity;

		float m_MaxSpeed;
		float m_Acceleration;
		float m_Deceleration;
		float m_JumpSpeed;
		float m_JumpDeceleration;
	};

}