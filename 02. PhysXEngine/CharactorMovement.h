#pragma once

#include <directxtk\SimpleMath.h>
#include <physx\PxPhysicsAPI.h>

namespace PhysicsEngine
{
	enum class CharactorState
	{
		IDLE,
		WALK,
		RUN,
		JUMP,
		DASH
	};

	struct CharactorMovementInfo
	{
		float MaxSpeed;
		float Acceleration;
		float StaticFriction;
		float DynamicFriction;
		float JumpSpeed;
		float JumpDeceleration;
		float Gravity;
	};

	class CharactorMovement
	{
	public:
		CharactorMovement();
		~CharactorMovement();

		void Initlaize(CharactorMovementInfo info);

		void Update(DirectX::SimpleMath::Vector3& inputVector, float deltaTime);
		bool Compute(float deltaTime);

		void CopyDirectionToPxVec3(physx::PxVec3& direction);
		void LimitVelocity();

		inline const DirectX::SimpleMath::Vector3& GetDirection();
		inline const float& GetMaxSpeed();
		inline const float& GetSpeed();
		inline const float& GetAcceleration();
		inline const float& GetStaticFriction();
		inline const float& GetDynamicFriction();
		inline const float& GetJumpSpeed();
		inline const float& GetJumpDeceleration();
		inline const float& GetGravity();
		inline void SetMaxSpeed(const float& maxSpeed);
		inline void SetAcceleration(const float& acceleration);
		inline void SetStaticFriction(const float& staticFriction);
		inline void SetDynamicFriction(const float& dynamicFriction);
		inline void SetJumpSpeed(const float& jumpSpeed);
		inline void SetJumpDeceleration(const float& jumpDeceleration);
		inline void SetGravity(const float& gravity);

	private:
		CharactorState m_CharactorState;

		DirectX::SimpleMath::Vector3 m_Velocity;
		DirectX::SimpleMath::Vector3 m_Direction;

		float m_MaxSpeed;
		float m_Speed;
		float m_Acceleration;
		float m_StaticFriction;
		float m_DynamicFriction;
		float m_JumpSpeed;
		float m_JumpDeceleration;
		float m_Gravity;
		float m_MinDistance;
	};

#pragma region GetSet

	inline const DirectX::SimpleMath::Vector3& CharactorMovement::GetDirection()
	{
		return m_Direction;
	}

	const float& CharactorMovement::GetMaxSpeed()
	{
		return m_MaxSpeed;
	}
	const float& CharactorMovement::GetSpeed()
	{
		return m_Speed;
	}
	const float& CharactorMovement::GetAcceleration()
	{
		return m_Acceleration;
	}
	const float& CharactorMovement::GetStaticFriction()
	{
		return m_StaticFriction;
	}
	const float& CharactorMovement::GetDynamicFriction()
	{
		return m_DynamicFriction;
	}
	const float& CharactorMovement::GetJumpSpeed()
	{
		return m_JumpSpeed;
	}
	const float& CharactorMovement::GetJumpDeceleration()
	{
		return m_JumpDeceleration;
	}
	const float& CharactorMovement::GetGravity()
	{
		return m_Gravity;
	}
	void CharactorMovement::SetMaxSpeed(const float& maxSpeed)
	{
		m_MaxSpeed = maxSpeed;
	}
	void CharactorMovement::SetAcceleration(const float& acceleration)
	{
		m_Acceleration = acceleration;
	}
	void CharactorMovement::SetStaticFriction(const float& staticFriction)
	{
		m_StaticFriction = staticFriction;
	}
	void CharactorMovement::SetDynamicFriction(const float& dynamicFriction)
	{
		m_DynamicFriction = dynamicFriction;
	}
	void CharactorMovement::SetJumpSpeed(const float& jumpSpeed)
	{
		m_JumpSpeed = jumpSpeed;
	}
	void CharactorMovement::SetJumpDeceleration(const float& jumpDeceleration)
	{
		m_JumpDeceleration = jumpDeceleration;
	}
	void CharactorMovement::SetGravity(const float& gravity)
	{
		m_Gravity = gravity;
	}

#pragma endregion

}