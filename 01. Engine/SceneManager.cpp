#include "pch.h"
#include "SceneManager.h"

#include "ObjectManager.h"

namespace Engine
{
	void SceneManager::Initalize()
	{
		if (m_pCurrentScene != nullptr)
			m_pCurrentScene->Init();
	}
	void SceneManager::Update(float _deltaTime)
	{
		if (m_pCurrentScene != nullptr)
			m_pCurrentScene->Update(_deltaTime);
	}
	void SceneManager::LateUpdate(float _deltaTime)
	{
		if (m_pCurrentScene != nullptr)
			m_pCurrentScene->LateUpdate(_deltaTime);
	}
	void SceneManager::FixedUpdate(float _deltaTime)
	{
		if (m_pCurrentScene != nullptr)
			m_pCurrentScene->FixedUpdate(_deltaTime);
	}
	void SceneManager::Render()
	{
		if (m_pCurrentScene != nullptr)
			m_pCurrentScene->Render();
	}

	void SceneManager::ChangeScene()
	{
	}
	bool SceneManager::StartScene(wstring _sceneName)
	{
		return false;
	}
}