#pragma once
#include "pch.h"
#include "GameProcess.h"

using namespace std;

// 건재 : 게임 실행
// 건재 : 윈도우 창 생성 및 게임 프로세스 진행
namespace Engine
{
	// 건재 : 클래스 선언
	class RenderManager;

	// 건재 : 게임을 실행할 때 필요한 데이터 구조체
	struct GameDesc
	{
		shared_ptr<GameProcess> app = nullptr;
		wstring appName = L"4Q_Project";
		float width = 800.f;
		float height = 600.f;
		Color clearColor = Color(0.5f, 0.5f, 0.5f, 0.5f);
		HWND hwnd = nullptr;
		HINSTANCE hInstance = nullptr;
	};

	// -------------------------------------------------------------------------
	// 건재 : 게임 엔진 클래스
	class Game
	{
		SINGLETON(Game)
	public:
		WPARAM Run(GameDesc& _desc);

		GameDesc& GetGameDesc() { return m_Desc; }

	private:
		ATOM MyRegisterClass();
		BOOL InitInstance(int _cmdshow);

		void Loop();

		static LRESULT CALLBACK WndProc(HWND _handle, UINT _message, WPARAM _wParam, LPARAM _lParam);

	private:
		GameDesc m_Desc;
	};
	// -------------------------------------------------------------------------
}

#define GAME Engine::Game::GetInstance()