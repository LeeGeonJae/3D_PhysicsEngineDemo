#include "pch.h"
#include "CharactorMovement.h"

namespace PhysicsEngine
{
	CharactorMovement::CharactorMovement()
	{
	}

	CharactorMovement::~CharactorMovement()
	{
	}

	void CharactorMovement::AddMovement(DirectX::SimpleMath::Vector3 inputVector)
	{

	}

	bool CharactorMovement::CheckMaxSpeed()
	{
		float currentSpeed = sqrt(m_Velocity.x * m_Velocity.x + m_Velocity.z * m_Velocity.z);
		
		if (m_MaxSpeed <= currentSpeed)
			return false;

		return true;
	}
}
