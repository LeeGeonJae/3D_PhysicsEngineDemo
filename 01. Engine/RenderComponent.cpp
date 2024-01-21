#include "pch.h"
#include "RenderComponent.h"

namespace Engine
{
	RenderComponent::RenderComponent()
	{
	}

	RenderComponent::~RenderComponent()
	{
	}

	void RenderComponent::Setting(RenderComponentInfo _info)
	{
		// ���� : ���� ����ü�� ���� ��� �Լ� ����
		__super::Setting(_info.m_SceneComponentInfo);
		m_bIsVisible = _info.m_bIsVisible;
	}

	void RenderComponent::Render()
	{
		__super::Render();
	}
}