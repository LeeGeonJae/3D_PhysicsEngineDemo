#pragma once
#include "Scene.h"

namespace Engine
{
	// ���� : Ŭ���� ����
	class ObjectManager;

	// ���� : �� �Ŵ��� Ŭ����
	class SceneManager
	{
		SINGLETON(SceneManager)
	public:
		virtual void Initalize();
		virtual void Update(float _deltaTime);
		virtual void LateUpdate(float _deltaTime);
		virtual void FixedUpdate(float _deltaTime);
		virtual void Render();

	public:
		template <typename T>
		shared_ptr<T> CreateScene(wstring _sceneName);
		inline shared_ptr<Scene> FindSceneOrNull(wstring _sceneName) const;

	public:
		void ChangeScene();
		bool StartScene(wstring _sceneName);


	private:
		unordered_map<wstring, shared_ptr<Scene>> m_pSceneMap;
		shared_ptr<Scene> m_pCurrentScene;
	};

	// ���� : Scene�� �����ϴ� ���ø� �Լ�
	template<typename T>
	shared_ptr<T> SceneManager::CreateScene(wstring _sceneName)
	{
		// ���� : Scene�� ��ӹ޴� Ŭ������ �ƴ϶�� ����
		bool bIsBase = std::is_base_of<Scene, T>::value;
		assert(bIsBase == true);

		// ���� : ���� �����ϰ� SceneMap�� insert�ϰ� �ش� ���� �̸��� ����
		shared_ptr<Scene> scene = make_shared<T>();
		m_pSceneMap.insert(_sceneName, scene);
		scene->SetName(_sceneName);

		return scene;
	}
	shared_ptr<Scene> SceneManager::FindSceneOrNull(wstring _sceneName) const
	{
		// ���� : SceneMap�� Scene�� ã�� ã���� ��ȯ �� ã���� nullptr
		auto sceneiter = m_pSceneMap.find(_sceneName);

		if (sceneiter->second != nullptr)
			return sceneiter->second;

		return nullptr;
	}

}


