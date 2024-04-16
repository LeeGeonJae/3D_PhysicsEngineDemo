#pragma once

namespace Engine
{
	// -------------------------------------------------------------------------
	// ���� : ���� ���μ��� Ŭ����
	// ���� : �׽�Ʈ �غ� �ۿ� �̰� ��� �ް� ���� �Լ��� �����ϸ� �˴ϴ�.
	class GameProcess
	{
	public:
		virtual void Init();
		virtual void Update(float _deltaTime);
		virtual void Render() abstract;
		virtual void LateUpdate(float _deltaTime) abstract;
		virtual void FixedUpdate(float _deltaTime) abstract;

	private:
		int m_FPS;
		string m_FPSText;
	};
	// -------------------------------------------------------------------------
}
