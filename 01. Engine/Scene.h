#pragma once

namespace Engine
{
	// ���� : Ŭ���� ����
	class ObjectManager;

	// ���� : Scene Ŭ����
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

	// ���� : Scene Ŭ������ Get & Set
	wstring Scene::GetName()
	{
		return m_Name;
	}
	void Scene::SetName(wstring _name)
	{
		m_Name = _name;
	}
}