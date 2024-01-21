#pragma once
#include "GameProcess.h"

namespace Engine
{
	class Object;
}

class TestApp : public Engine::GameProcess
{
public:
	void Init() override;
	void Update() override;
	void LateUpdate() override;
	void FixedUpdate() override;
	void Render() override;

public:
	vector<shared_ptr<Engine::Object>> m_ObjectVec;
};