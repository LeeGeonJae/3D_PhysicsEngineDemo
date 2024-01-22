#include "Game.h"
#include "TestApp.h"

// main함수 실행
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	// 게임 엔진에서 윈도우를 생성하기 위한 데이터 세팅
	Engine::GameDesc desc;
	desc.appName = L"TestApp";
	desc.hInstance = hInstance;
	desc.hwnd = NULL;
	desc.width = 800;
	desc.height = 600;
	desc.clearColor = Color(0.f, 0.5f, 0.5f, 0.5);
	desc.app = make_shared<TestApp>();

	// 게임 엔진 실행
	GAME->Run(desc);

	return 0;
}
