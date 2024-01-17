#pragma once
#include "SceneComponent.h"

namespace Engine
{
	// ·»´õ ÄÄÆ÷³ÍÆ® Á¤º¸ ±¸Á¶Ã¼
	struct RenderComponentInfo
	{
		SceneComponentInfo m_SceneComponentInfo;
		bool m_bIsVisible;
	};

	// ·»´õ ÄÄÆ÷³ÍÆ® Å¬·¡½º
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
