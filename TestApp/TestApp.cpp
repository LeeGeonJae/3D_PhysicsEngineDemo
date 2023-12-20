#include "TestApp.h"
#include "../Engine/InputManager.h"

void TestApp::Init()
{

}

void TestApp::Update()
{

	if (InputManager::GetInstance()->GetButtonDown(KEY_TYPE::A))
	{
		a = 0;
	}
}

void TestApp::LateUpdate()
{

}

void TestApp::FixedUpdate()
{

}

void TestApp::Render()
{

}
