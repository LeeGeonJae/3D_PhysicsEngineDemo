#pragma once
#include "pch.h"
#include "GameProcess.h"

	using namespace std;

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

	class Game
	{
	private:
		Game() {}
	public:
		static Game* GetInstance()
		{
			static Game game;
			return &game;
		}
	public:
		WPARAM Run(GameDesc& desc);

		GameDesc& GetGameDesc() { return mdesc; }

	private:
		ATOM MyRegisterClass();
		BOOL InitInstance(int cmdshow);

		void Update();

		static LRESULT CALLBACK WndProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);

	private:
		GameDesc mdesc;
	};