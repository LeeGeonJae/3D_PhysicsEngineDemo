#include "pch.h"
#include "CameraComponent.h"

#include "Game.h"

namespace Engine
{
	CameraComponent::CameraComponent()
	{
	}

	CameraComponent::~CameraComponent()
	{
	}

	void CameraComponent::Setting(CameraComponentInfo info)
	{
		// ���� : ���� ����ü�� ���� ��� �Լ� ����
		__super::Setting(info.m_SceneComponentInfo);
	}

	void CameraComponent::Init()
	{
	}

	void CameraComponent::Update(float _deltaTime)
	{
		__super::Update(_deltaTime);

		// ���� : Camera
		{
			Vector3 eye = GetPosition();
			Vector3 target = GetPosition() + -GetWorldTransform().Forward();
			Vector3 up = GetWorldTransform().Up();
			m_View = XMMatrixLookAtLH(eye, target, up);
			m_Projection = XMMatrixPerspectiveFovLH(50.f * 3.14f / 180.f, (float)GAME->GetGameDesc().width / (float)GAME->GetGameDesc().height, 1.f, 500000.f);
		}
	}

	void CameraComponent::LateUpdate(float _deltaTime)
	{
	}

	void CameraComponent::FixedUpdate(float _deltaTime)
	{
	}
}