#pragma once

namespace Engine
{
	// ���� : ���� ���μ��� Ŭ����
	// ���� : �׽�Ʈ �غ� �ۿ� �̰� ��� �ް� ���� �Լ��� �����ϸ� �˴ϴ�.
	class GameProcess
	{
	public:
		virtual void Init() abstract;
		virtual void Update() abstract;
		virtual void Render() abstract;
		virtual void LateUpdate() abstract;
		virtual void FixedUpdate() abstract;
	};
}
