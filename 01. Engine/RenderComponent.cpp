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
		// 건재 : 정보 구조체에 따라 멤버 함수 세팅
		__super::Setting(_info.m_SceneComponentInfo);
		m_bIsVisible = _info.m_bIsVisible;
	}

	void RenderComponent::Render()
	{
		__super::Render();
	}
}