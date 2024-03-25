#include "pch.h"
#include "CameraComponent.h"

#include "Game.h"
#include "RenderManager.h"

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
			Vector3 eye = GetWorldTransform().Translation();
			Vector3 target = GetWorldTransform().Translation() + -GetWorldTransform().Forward();
			Vector3 up = GetWorldTransform().Up();
			m_View = XMMatrixLookAtLH(eye, target, up);
			m_Projection = XMMatrixPerspectiveFovLH(50.f * 3.14f / 180.f, (float)GAME->GetGameDesc().width / (float)GAME->GetGameDesc().height, 1.f, 500000.f);

			GraphicsEngine::CB_Camera cameraData;
			cameraData.m_Projection = m_Projection.Transpose();
			cameraData.m_View = m_View.Transpose();
			cameraData.m_CameraPosition = GetPosition();
			RENDER->SetCamera(move(cameraData));
		}
	}

	void CameraComponent::LateUpdate(float _deltaTime)
	{
	}

	void CameraComponent::FixedUpdate(float _deltaTime)
	{
	}
}