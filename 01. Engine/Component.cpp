#include "pch.h"
#include "Component.h"

#include "Object.h"

namespace Engine
{
	Component::Component()
	{
	}

	Component::~Component()
	{
	}

	void Component::Init()
	{
	}

	void Component::Update(float _deltaTime)
	{
	}

	void Component::LateUpdate(float _deltaTime)
	{
	}

	void Component::FixedUpdate(float _deltaTime)
	{
	}

	void Component::Render()
	{
	}

	bool Component::GetIsDead()
	{
		return m_pObject.lock()->GetIsDead();
	}
}
