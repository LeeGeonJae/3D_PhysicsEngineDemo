#pragma once
#include "SceneComponent.h"

namespace Engine
{
	// 건재 : 렌더 컴포넌트 정보 구조체
	struct RenderComponentInfo
	{
		SceneComponentInfo m_SceneComponentInfo;
		bool m_bIsVisible;
	};

	// 건재 : 렌더 컴포넌트 클래스
	class RenderComponent : public SceneComponent
	{
	public:
		RenderComponent();
		virtual ~RenderComponent();

	public:
		void Setting(RenderComponentInfo _info);

	public:
		virtual void Render() override;

	public:
		inline void SetVisible(bool _bisVisible);
		inline const bool& GetVisible();

	private:
		bool m_bIsVisible;
	};


	void RenderComponent::SetVisible(bool _bisVisible)
	{
		m_bIsVisible = _bisVisible;
	}
	const bool& RenderComponent::GetVisible()
	{
		return m_bIsVisible;
	}
}
