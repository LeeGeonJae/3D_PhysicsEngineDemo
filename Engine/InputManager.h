#pragma once
#include <vector>
#include <Windows.h>
#include "Define.h"

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
	SINGLETON(InputManager);

public:
	void Init(HWND _hwnd);
	void Update();

	inline bool GetButton(KEY_TYPE _key);
	inline bool GetButtonDown(KEY_TYPE _key);
	inline bool GetButtonUp(KEY_TYPE _key);

private:
	inline KEY_STATE GetKeyState(KEY_TYPE key);

private:
	HWND m_Hwnd;
	std::vector<KEY_STATE> m_KeyStates;
	POINT m_MousePos = {};
};

bool InputManager::GetButton(KEY_TYPE _key)
{
	return GetKeyState(_key) == KEY_STATE::PRESS;
}
bool InputManager::GetButtonDown(KEY_TYPE _key)
{
	return GetKeyState(_key) == KEY_STATE::DOWN;
}
bool InputManager::GetButtonUp(KEY_TYPE _key)
{
	return GetKeyState(_key) == KEY_STATE::UP;
}


KEY_STATE InputManager::GetKeyState(KEY_TYPE _key)
{
	return m_KeyStates[static_cast<unsigned char>(_key)];
}