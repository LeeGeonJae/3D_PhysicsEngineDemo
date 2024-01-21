#pragma once
#include "pch.h"
#include "GameProcess.h"

using namespace std;

// ���� : ���� ����
// ���� : ������ â ���� �� ���� ���μ��� ����
namespace Engine
{
	// ���� : Ŭ���� ����
	class RenderManager;

	// ���� : ������ ������ �� �ʿ��� ������ ����ü
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
	// ���� : ���� ���� Ŭ����
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