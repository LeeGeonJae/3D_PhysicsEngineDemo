#include "pch.h"
#include "Game.h"

#include "InputManager.h"
#include "TimeManager.h"
#include "RenderManager.h"
#include "ResourceManager.h"
#include "SceneManager.h"
#include "ObjectManager.h"

#include <imgui.h>

constexpr float FIXED_TIME = 1.f / 60.f;

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace Engine
{
	WPARAM Game::Run(GameDesc& _desc)
	{
		m_Desc = _desc;
		assert(m_Desc.app != nullptr);

		// 건재 : 1) 윈도우 창 정보 등록
		MyRegisterClass();

		// 건재 : 2) 윈도우 창 생성
		if (!InitInstance(SW_SHOWNORMAL))
			return FALSE;

		// 건재 : 3) 매니저 세팅 및 게임 앱 세팅
		start();

		MSG msg = { 0 };

		// 건재 : 4) 게임 루프 진행
		while (msg.message != WM_QUIT)
		{
			if (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				::TranslateMessage(&msg);
				::DispatchMessage(&msg);
			}
			else
			{
				loop();
			}
		}

		return msg.wParam;
	}

	void Game::start()
	{
		m_TimeManager = make_shared<TimeManager>();
		m_TimeManager->Initialize();
		InputManager::GetInstance()->Initalize(m_Desc.hwnd);
		RenderManager::GetInstance()->Initalize(m_Desc.hwnd);
		ResourceManager::GetInstance()->Initalize();
		SceneManager::GetInstance()->Initalize();

		m_Desc.app->Init();
	}

	// 건재 : 게임 업데이트
	void Game::loop()
	{
		static float durationTime = 0;
		durationTime += m_TimeManager->GetfDT();
		if (durationTime > FIXED_TIME)	// 건재 : 60 프레임
		{
			m_Desc.app->FixedUpdate(FIXED_TIME);
			SceneManager::GetInstance()->FixedUpdate(FIXED_TIME);
			durationTime -= FIXED_TIME;
		}

		m_TimeManager->Update();
		InputManager::GetInstance()->Update();
		m_Desc.app->Update(m_TimeManager->GetfDT());
		RenderManager::GetInstance()->Update();
		ObjectManager::GetInstance()->Update();
		SceneManager::GetInstance()->Update(m_TimeManager->GetfDT());

		m_Desc.app->LateUpdate(m_TimeManager->GetfDT());
		SceneManager::GetInstance()->LateUpdate(m_TimeManager->GetfDT());
		m_Desc.app->Render();
		SceneManager::GetInstance()->Render();

		RenderManager::GetInstance()->Render();
	}


	// 건재 : 윈도우 창 세팅 ( WinApi )
	ATOM Game::MyRegisterClass()
	{
			WNDCLASSEXW wcex;

			wcex.cbSize = sizeof(WNDCLASSEX);

			wcex.style = CS_HREDRAW | CS_VREDRAW;
			wcex.lpfnWndProc = WndProc;
			wcex.cbClsExtra = 0;
			wcex.cbWndExtra = 0;
			wcex.hInstance = m_Desc.hInstance;
			wcex.hIcon = ::LoadIcon(NULL, IDI_WINLOGO);
			wcex.hCursor = ::LoadCursor(nullptr, IDC_ARROW);
			wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
			wcex.lpszMenuName = NULL;
			wcex.lpszClassName = m_Desc.appName.c_str();
			wcex.hIconSm = wcex.hIcon;

			return RegisterClassExW(&wcex);
	}

	BOOL Game::InitInstance(int _cmdShow)
	{
		RECT windowRect = { 0, 0, m_Desc.width, m_Desc.height };
		::AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, false);

		m_Desc.hwnd = CreateWindowW(m_Desc.appName.c_str(), m_Desc.appName.c_str(), WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, 0, windowRect.right - windowRect.left, windowRect.bottom - windowRect.top, nullptr, nullptr, m_Desc.hInstance, nullptr);

		if (!m_Desc.hwnd)
			return FALSE;

		::ShowWindow(m_Desc.hwnd, _cmdShow);
		::UpdateWindow(m_Desc.hwnd);

		return TRUE;
	}


	LRESULT CALLBACK Game::WndProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam)
	{
		if (ImGui_ImplWin32_WndProcHandler(handle, message, wParam, lParam))
			return true;

		switch (message)
		{
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		case WM_ACTIVATEAPP:
			DirectX::Keyboard::ProcessMessage(message, wParam, lParam);
			DirectX::Mouse::ProcessMessage(message, wParam, lParam);
			break;
		case WM_INPUT:
		case WM_MOUSEMOVE:
		case WM_LBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_RBUTTONDOWN:
		case WM_RBUTTONUP:
		case WM_MBUTTONDOWN:
		case WM_MBUTTONUP:
		case WM_MOUSEWHEEL:
		case WM_XBUTTONDOWN:
		case WM_XBUTTONUP:
		case WM_MOUSEHOVER:
			Mouse::ProcessMessage(message, wParam, lParam);
			break;
		case WM_KEYDOWN:
		case WM_KEYUP:
		case WM_SYSKEYUP:
			Keyboard::ProcessMessage(message, wParam, lParam);
			break;
		default:
			return DefWindowProc(handle, message, wParam, lParam);
		}
		return 0;
	}
}