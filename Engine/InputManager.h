#pragma once

enum class KEY_TYPE
{
	UP = VK_UP,
	DOWN = VK_DOWN,
	LEFT = VK_LEFT,
	RIGHT = VK_RIGHT,

	W = 'W',
	A = 'A',
	S = 'S',
	D = 'D',

	Q = 'Q',
	E = 'E',
	Z = 'Z',
	C = 'C',

	KEY_1 = '1',
	KEY_2 = '2',
	KEY_3 = '3',
	KEY_4 = '4',

	LBUTTON = VK_LBUTTON,
	RBUTTON = VK_RBUTTON,
};

enum class KEY_STATE
{
	NONE,
	PRESS,
	DOWN,
	UP,
	END
};

enum
{
	KEY_TYPE_COUNT = 256,
	KEY_STATE_COUNT = static_cast<int>(KEY_STATE::END),
};

class InputManager
{
private:
	InputManager();
public:
	static InputManager* GetInstance()
	{
		static InputManager inputManager;
		return &inputManager;
	}

public:
	void Init(HWND hwnd);
	void Update();

	inline bool GetButton(KEY_TYPE key);
	inline bool GetButtonDown(KEY_TYPE key);
	inline bool GetButtonUp(KEY_TYPE key);

private:
	inline KEY_STATE GetKeyState(KEY_TYPE key);

private:
	HWND mHwnd;
	std::vector<KEY_STATE> mKeyStates;
	POINT mMousePos = {};
};

bool InputManager::GetButton(KEY_TYPE key)
{
	return GetKeyState(key) == KEY_STATE::PRESS;
}
bool InputManager::GetButtonDown(KEY_TYPE key)
{
	return GetKeyState(key) == KEY_STATE::DOWN;
}
bool InputManager::GetButtonUp(KEY_TYPE key)
{
	return GetKeyState(key) == KEY_STATE::UP;
}


KEY_STATE InputManager::GetKeyState(KEY_TYPE key)
{
	return mKeyStates[static_cast<unsigned char>(key)];
}