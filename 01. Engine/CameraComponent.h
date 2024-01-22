#pragma once
#include "SceneComponent.h"

namespace Engine
{
	// ���� : ī�޶� ������Ʈ ������ ����ü
	struct CameraComponentInfo
	{
		SceneComponentInfo m_SceneComponentInfo;
	};

	// -------------------------------------------------------------------------
	// ���� : ������Ʈ ī�޶� ������Ʈ Ŭ����
	// ���� : ������Ʈ�� ī�޶� ���̰� ���� �Ŵ������� �ش� ī�޶� ������Ʈ �����͸� �����ϸ� 
	// ���� : ���� �Ŵ����� �ڽ��� ������ �ִ� ī�޶� �����ؼ� ���� 
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

	// ���� : CameraComponent�� Get & Set �Լ�
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

