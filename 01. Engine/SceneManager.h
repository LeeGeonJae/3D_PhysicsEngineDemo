#pragma once

namespace Engine
{
	class Scene;

	class SceneManager
	{
	private:
		SceneManager() {}
	public:
		~SceneManager();

	public:
		virtual void Init();
		virtual void Update(float _deltaTime);
		virtual void LateUpdate(float _deltaTime);
		virtual void FixedUpdate(float _deltaTime);
		virtual void Render();

	public:
		template <typename T>
		T* CreateScene(wstring _sceneName);
		inline Scene* FindSceneOrNull(wstring _sceneName) const;

	private:
		map<wstring, shared_ptr<Scene>> m_pSceneMap;
		shared_ptr<Scene> m_pScene;
	};


	template<typename T>
	T* SceneManager::CreateScene(wstring _sceneName)
	{
		//assert(is_base_of_v<Scene, T>);

		//shared_ptr<T> scene = make_shared<T>(_sceneName);


		return nullptr;
	}
	Scene* SceneManager::FindSceneOrNull(wstring _sceneName) const
	{
		return nullptr;
	}
}


