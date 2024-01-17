#pragma once

namespace Engine
{
	// 클래스 선언
	class Object;


	// 컴포넌트 클래스
	class Component
	{
	public:
		Component();
		virtual ~Component();

	public:
		virtual void Init();
		virtual void Update();
		virtual void LateUpdate();
		virtual void FixedUpdate();
		virtual void Render();

	public:
		inline void SetName(string _name);
		inline const string& GetName();
		inline void SetMyObject(weak_ptr<Object> _object);
		inline const weak_ptr<Object>& GetMyObject();

	private:
		string m_Name;
		weak_ptr<Object> m_pObject;
	};

	const string& Component::GetName()
	{
		return m_Name;
	}
	void Component::SetName(string _name)
	{
		m_Name = _name;
	}
	void Component::SetMyObject(weak_ptr<Object> _object)
	{
		m_pObject = _object;
	}
	const weak_ptr<Object>& Component::GetMyObject()
	{
		return m_pObject;
	}
}


