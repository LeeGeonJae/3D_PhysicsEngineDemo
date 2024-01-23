#pragma once

namespace Engine
{
	// 건재 : 클래스 선언
	class ObjectManager;

	// 건재 : Scene 클래스
	class Scene
	{
	public:
		Scene();
		virtual ~Scene();

	public:
		virtual void Init();
		virtual void Update(float _deltaTime);
		virtual void FixedUpdate(float _deltaTime);
		virtual void LateUpdate(float _deltaTime);
		virtual void Render();

	public:
		virtual void SceneEnter();
		virtual void SceneExit();

	public:
		inline wstring GetName();
		inline void SetName(wstring _name);

	private:
		wstring m_Name;
	};

	// 건재 : Scene 클래스의 Get & Set
	wstring Scene::GetName()
	{
		return m_Name;
	}
	void Scene::SetName(wstring _name)
	{
		m_Name = _name;
	}
}