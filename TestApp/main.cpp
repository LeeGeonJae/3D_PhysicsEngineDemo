#include "pch.h"

#include "Game.h"
#include "TestApp.h"

LRESULT CALLBACK WndProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam)
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

// main�Լ� ����
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	// ���� �������� �����츦 �����ϱ� ���� ������ ����
	Engine::GameDesc desc;
	desc.appName = L"TestApp";
	desc.hInstance = hInstance;
	desc.hwnd = NULL;
	desc.width = 1920;
	desc.height = 1080;
	desc.clearColor = Color(0.f, 0.5f, 0.5f, 0.5);
	desc.app = make_shared<TestApp>();

	// ���� ���� ����
	GAME->Run(desc);

	return 0;
}
