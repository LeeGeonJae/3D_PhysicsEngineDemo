#pragma once
#include "GameProcess.h"

namespace Engine
{
	class Object;
}

class TestApp : public GameProcess
{
public:
	void Init() override;
	void Update() override;
	void LateUpdate() override;
	void FixedUpdate() override;
	void Render() override;

public:
	shared_ptr<Engine::Object> m_Object;
};