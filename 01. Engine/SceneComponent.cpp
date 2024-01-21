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
		// 건재 : 정보 구조체에 따라 멤버 함수 세팅
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
		// 건재 : 계층구조로 월드 트랜스폼 배치
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

