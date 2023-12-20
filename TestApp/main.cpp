#include "EnginePch.h"
#include "Game.h"

#include "TestApp.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR IpCmdLine, int nShowCmd)
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