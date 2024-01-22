#pragma once

namespace Engine
{
	// -------------------------------------------------------------------------
	// 건재 : 게임 프로세스 클래스
	// 건재 : 테스트 해볼 앱에 이걸 상속 받고 가상 함수를 실행하면 됩니다.
	class GameProcess
	{
	public:
		virtual void Init() abstract;
		virtual void Update(float _deltaTime) abstract;
		virtual void Render() abstract;
		virtual void LateUpdate(float _deltaTime) abstract;
		virtual void FixedUpdate(float _deltaTime) abstract;
	};
	// -------------------------------------------------------------------------
}
