#pragma once
#include "GameProcess.h"

// Engine 정적 라이브러리 프로젝트 내의 모든 클래스들은 namespace Engine을 썼으므로 클래스 선언 할 때에는 
// namespace Engine을 포함해야지 쓸 수 있습니다.
namespace Engine
{
	class Object;
}

// 게임 프로세스 클래스를 상속 받아야 사용할 수 있습니다.
class TestApp : public Engine::GameProcess
{
public:
	void Init() override;
	void Update(float _deltaTime) override;
	void LateUpdate(float _deltaTime) override;
	void FixedUpdate(float _deltaTime) override;
	void Render() override;

public:
	vector<shared_ptr<Engine::Object>> m_ObjectVec;
};