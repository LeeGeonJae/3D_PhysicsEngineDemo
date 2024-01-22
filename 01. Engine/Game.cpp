#include "pch.h"
#include "Game.h"

#include "InputManager.h"
#include "TimeManager.h"
#include "RenderManager.h"
#include "ResourceManager.h"

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
		InputManager::GetInstance()->Initalize(m_Desc.hwnd);
		m_TimeManager->Initialize();
		RenderManager::GetInstance()->Initalize(m_Desc.hwnd);
		ResourceManager::GetInstance()->Initalize();

		m_Desc.app->Init();
	}

	// 건재 : 게임 업데이트
	void Game::loop()
	{
		m_TimeManager->Update();
		InputManager::GetInstance()->Update();

		static float durationTime = 0;
		durationTime += m_TimeManager->GetfDT();
		if (durationTime > 0.0166667f)	// 건재 : 60 프레임
		{
			m_Desc.app->FixedUpdate(0.0166667f);
			durationTime -= 0.0166667f;
		}

		m_Desc.app->Update(m_TimeManager->GetfDT());
		m_Desc.app->LateUpdate(m_TimeManager->GetfDT());

		// ???
		m_Desc.app->Render();

		RenderManager::GetInstance()->Update();
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
		switch (message)
		{
		case WM_SIZE:
			break;
		case WM_CLOSE:
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return ::DefWindowProc(handle, message, wParam, lParam);
		}
	}
}