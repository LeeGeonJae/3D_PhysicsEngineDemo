#include "../Engine/Game.h"
#include "TestApp.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	GameDesc desc;
	desc.appName = L"TestApp";
	desc.hInstance = hInstance;
	desc.hwnd = NULL;
	desc.width = 800;
	desc.height = 600;
	desc.clearColor = Color(0.5f, 0.5f, 0.5f, 0.5);
	desc.app = make_shared<TestApp>();

	Game::GetInstance()->Run(desc);

	return 0;
}