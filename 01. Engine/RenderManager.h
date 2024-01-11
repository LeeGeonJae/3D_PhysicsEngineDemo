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
	public:
		RenderManager();
		~RenderManager();

	public:
		void Initalize(HWND _hwnd);
		void Update();
		void Render();

	private:
		shared_ptr<GraphicsEngine::Graphics> m_Graphics;
		shared_ptr<GraphicsEngine::PipeLine> m_PipeLine;
	};
}