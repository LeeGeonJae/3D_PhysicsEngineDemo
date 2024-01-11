#include "pch.h"
#include "Game.h"
#include "RenderManager.h"

#include "Graphics.h"
#include "PipeLine.h"

namespace Engine
{
	RenderManager::RenderManager()
	{
	}

	RenderManager::~RenderManager()
	{
	}

	void RenderManager::Initalize(HWND _hwnd)
	{
		m_Graphics = make_shared<GraphicsEngine::Graphics>();
		m_PipeLine = make_shared<GraphicsEngine::PipeLine>();

		GraphicsEngine::GraphicsInfo graphicsinfo;
		graphicsinfo.m_Height = GAME->GetGameDesc().height;
		graphicsinfo.m_Width = GAME->GetGameDesc().width;
		graphicsinfo.m_hwnd = GAME->GetGameDesc().hwnd;
		graphicsinfo.m_ClearColor = GAME->GetGameDesc().clearColor;
		m_Graphics->Initalize(graphicsinfo);
		m_PipeLine->Initalize(m_Graphics->GetDevice(), m_Graphics->GetDeviceContext());
	}

	void RenderManager::Update()
	{
	}

	void RenderManager::Render()
	{
		m_Graphics->RenderBegin();

		m_PipeLine->Update();

		m_Graphics->RenderEnd();
	}

}