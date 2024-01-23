#pragma once
#include "Scene.h"

namespace Engine
{
	// 건재 : 클래스 선언
	class ObjectManager;

	// 건재 : 씬 매니저 클래스
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

	// 건재 : Scene을 생성하는 템플릿 함수
	template<typename T>
	shared_ptr<T> SceneManager::CreateScene(wstring _sceneName)
	{
		// 건재 : Scene을 상속받는 클래스가 아니라면 오류
		bool bIsBase = std::is_base_of<Scene, T>::value;
		assert(bIsBase == true);

		// 건재 : 씬을 생성하고 SceneMap에 insert하고 해당 씬의 이름을 세팅
		shared_ptr<Scene> scene = make_shared<T>();
		m_pSceneMap.insert(_sceneName, scene);
		scene->SetName(_sceneName);

		return scene;
	}
	shared_ptr<Scene> SceneManager::FindSceneOrNull(wstring _sceneName) const
	{
		// 건재 : SceneMap에 Scene을 찾고 찾으면 반환 못 찾으면 nullptr
		auto sceneiter = m_pSceneMap.find(_sceneName);

		if (sceneiter->second != nullptr)
			return sceneiter->second;

		return nullptr;
	}

}


