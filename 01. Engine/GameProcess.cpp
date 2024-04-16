#include "pch.h"
#include "GameProcess.h"

#include "RenderManager.h"
#include "ImGuiTool.h"

void Engine::GameProcess::Init()
{
	m_FPSText = "FPS : 0";
	IMGUI->SetString(&m_FPSText);
}

void Engine::GameProcess::Update(float _deltaTime)
{
	// 게임 프레임 측정
	static float durationTime = 0;
	durationTime += _deltaTime;
	m_FPS++;

	if (durationTime >= 1.f)
	{
		m_FPSText = "FPS : ";
		m_FPSText += to_string(m_FPS);

		durationTime = 0.f;
		m_FPS = 0;
	}
}
