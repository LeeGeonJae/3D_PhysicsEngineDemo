#pragma once

#include <physx\PxPhysicsAPI.h>

namespace PhysicsEngine
{
	class CharactorController
	{
	public:
		CharactorController();
		~CharactorController();

	public:


	private:
		physx::PxController* m_Controller;


	};
}