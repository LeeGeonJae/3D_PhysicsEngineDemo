#pragma once

namespace Engine
{
	// 클래스 선언
	class Component;
	class SceneComponent;


	// 오브젝트 정보 구조체
	struct ObjectInfo
	{
		string m_Name;
	};


	// 오브젝트 클래스
	class Object : public enable_shared_from_this<Object>
	{
	public:
		Object(unsigned int _Id);
		virtual ~Object();

	public:
		virtual void Setting(ObjectInfo info);

	public:
		virtual void Init();
		virtual void Update();
		virtual void LateUpdate();
		virtual void FixedUpdate();
		virtual void Render();

	public:
		template <typename T>
		inline vector<shared_ptr<T>> FindComponent();
		inline const vector<shared_ptr<SceneComponent>>& GetComponent();
		void SetComponent(shared_ptr<SceneComponent> _component);
		inline shared_ptr<SceneComponent> GetRootComponent();

	public:
		inline void SetName(string _name);
		inline string GetName();
		inline const unsigned int& GetID();

	private:
		string m_Name;
		unsigned int m_ID;
		vector<shared_ptr<SceneComponent>> m_pMyComponents;
		shared_ptr<SceneComponent> m_pRootComponent;
	};

	// 컴포넌트를 상속받는 특정 컴포넌트들 반환
	template <typename T>
	vector<shared_ptr<T>> Object::FindComponent()
	{
		bool bIsBase = std::is_base_of<Component, T>::value;
		assert(bIsBase == true);

		vector<shared_ptr<T>> Components;

		for (auto component : m_pMyComponents)
		{
			shared_ptr<T> cameraComponent = dynamic_pointer_cast<T>(component);
			if (cameraComponent != nullptr)
				Components.push_back(cameraComponent);
		}

		return Components;
	}
	const vector<shared_ptr<SceneComponent>>& Object::GetComponent()
	{
		return m_pMyComponents;
	}
	shared_ptr<SceneComponent> Object::GetRootComponent()
	{
		return m_pRootComponent;
	}

	void Object::SetName(string _name)
	{
		m_Name = _name;
	}
	string Object::GetName()
	{
		return m_Name;
	}
	const unsigned int& Object::GetID()
	{
		return m_ID;
	}
}
