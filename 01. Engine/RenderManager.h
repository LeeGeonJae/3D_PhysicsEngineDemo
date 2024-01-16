#pragma once

namespace GraphicsEngine
{
	class Graphics;
	class PipeLine;
}

namespace Engine
{
	class RenderManager
	{
		SINGLETON(RenderManager)

	public:
		void Initalize(HWND _hwnd);
		void Update();
		void Render();

	public:
		inline shared_ptr<GraphicsEngine::Graphics> GetGraphics();
		inline shared_ptr<GraphicsEngine::PipeLine> GetPipeLine();

	private:
		shared_ptr<GraphicsEngine::Graphics> m_Graphics;
		shared_ptr<GraphicsEngine::PipeLine> m_PipeLine;
	};

	shared_ptr<GraphicsEngine::Graphics> RenderManager::GetGraphics()
	{
		return m_Graphics;
	}
	shared_ptr<GraphicsEngine::PipeLine> RenderManager::GetPipeLine()
	{
		return m_PipeLine;
	}
}

#define RENDER Engine::RenderManager::GetInstance()
#define DEVICE Engine::RenderManager::GetInstance()->GetGraphics()->GetDevice()
#define DEVICE_CONTEXT Engine::RenderManager::GetInstance()->GetGraphics()->GetDeviceContext()