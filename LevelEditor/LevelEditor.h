#pragma once
#include "GameProcess.h"

namespace Engine
{
	class Object;
}

class LevelEditor : public Engine::GameProcess
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