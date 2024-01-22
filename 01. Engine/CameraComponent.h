#pragma once
#include "SceneComponent.h"

namespace Engine
{
	// 건재 : 카메라 컴포넌트 데이터 구조체
	struct CameraComponentInfo
	{
		SceneComponentInfo m_SceneComponentInfo;
	};

	// -------------------------------------------------------------------------
	// 건재 : 오브젝트 카메라 컴포넌트 클래스
	// 건재 : 오브젝트에 카메라를 붙이고 렌더 매니저에게 해당 카메라 컴포넌트 포인터를 적용하면 
	// 건재 : 렌더 매니저가 자신이 가지고 있는 카메라를 적용해서 렌더 
	class CameraComponent : public SceneComponent
	{
	public:
		CameraComponent();
		virtual ~CameraComponent();

	public:
		void Setting(CameraComponentInfo info);

	public:
		virtual void Init();
		virtual void Update(float _deltaTime);
		virtual void LateUpdate(float _deltaTime);
		virtual void FixedUpdate(float _deltaTime);

	public:
		inline const SimpleMath::Matrix& GetViewMatrix();
		inline const SimpleMath::Matrix& GetProjectionMatrix();

	private:
		SimpleMath::Matrix m_View;
		SimpleMath::Matrix m_Projection;
	};

	// 건재 : CameraComponent의 Get & Set 함수
	const SimpleMath::Matrix& CameraComponent::GetViewMatrix()
	{
		return m_View;
	}
	const SimpleMath::Matrix& CameraComponent::GetProjectionMatrix()
	{
		return m_Projection;
	}
	// -------------------------------------------------------------------------
}

