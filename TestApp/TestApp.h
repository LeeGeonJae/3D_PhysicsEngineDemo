#pragma once
#include "GameProcess.h"

// Engine ���� ���̺귯�� ������Ʈ ���� ��� Ŭ�������� namespace Engine�� �����Ƿ� Ŭ���� ���� �� ������ 
// namespace Engine�� �����ؾ��� �� �� �ֽ��ϴ�.
namespace Engine
{
	class Object;
}

// ���� ���μ��� Ŭ������ ��� �޾ƾ� ����� �� �ֽ��ϴ�.
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