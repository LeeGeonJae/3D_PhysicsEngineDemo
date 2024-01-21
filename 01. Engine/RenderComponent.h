#pragma once
#include "SceneComponent.h"

namespace Engine
{
	// ���� : ���� ������Ʈ ���� ����ü
	struct RenderComponentInfo
	{
		SceneComponentInfo m_SceneComponentInfo;
		bool m_bIsVisible;
	};

	// ���� : ���� ������Ʈ Ŭ����
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
