#pragma once
#include "pch.h"
#include "GameProcess.h"

using namespace std;

namespace Engine
{
	class RenderManager;

	struct GameDesc
	{
		shared_ptr<GameProcess> app = nullptr;
		string appName = L"4Q_Project";
		float width = 800.f;
		float height = 600.f;
		Color clearColor = Color(0.5f, 0.5f, 0.5f, 0.5f);
		HWND hwnd = nullptr;
		HINSTANCE hInstance = nullptr;
	};

	class Game
	{
		SINGLETON(Game)
	public:
		WPARAM Run(GameDesc& _desc);

		GameDesc& GetGameDesc() { return m_Desc; }

	private:
		ATOM MyRegisterClass();
		BOOL InitInstance(int _cmdshow);

		void Update();

		static LRESULT CALLBACK WndProc(HWND _handle, UINT _message, WPARAM _wParam, LPARAM _lParam);

	private:
		GameDesc m_Desc;

		shared_ptr<RenderManager> m_RenderManager;
	};
}

#define GAME Engine::Game::GetInstance()