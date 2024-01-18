#include "pch.h"
#include "SceneComponent.h"

namespace Engine
{
	SceneComponent::SceneComponent()
	{
	}

	SceneComponent::~SceneComponent()
	{
	}

	void SceneComponent::Setting(SceneComponentInfo _info)
	{
		SetName(_info.m_Name);
		m_Position = _info.m_Position;
		m_Rotation = _info.m_Rotation;
		m_Scale = _info.m_Scale;
	}

	void SceneComponent::Init()
	{
		for (auto component : m_pChildren)
		{
			component->Init();
		}
	}

	void SceneComponent::Update()
	{
		m_Local = SimpleMath::Matrix::CreateScale(m_Scale)
			* SimpleMath::Matrix::CreateFromYawPitchRoll(m_Rotation)
			* SimpleMath::Matrix::CreateTranslation(m_Position);

		if (m_pOwner.lock() != nullptr)
		{
			m_World = m_pOwner.lock()->GetWorldTransform() * m_Local;
		}
		else
		{
			m_World = m_Local;
		}

		for (auto component : m_pChildren)
		{
			component->Update();
		}
	}

	void SceneComponent::FixedUpdate()
	{
		for (auto component : m_pChildren)
		{
			component->FixedUpdate();
		}
	}

	void SceneComponent::LateUpdate()
	{
		for (auto component : m_pChildren)
		{
			component->LateUpdate();
		}
	}

	void SceneComponent::Render()
	{
		for (auto component : m_pChildren)
		{
			component->Render();
		}
	}
}

