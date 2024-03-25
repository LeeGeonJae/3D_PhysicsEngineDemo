#pragma once

namespace Engine
{
	// 건재 : 클래스 선언
	class Object;

	// 건재 : 컴포넌트 클래스
	// 건재 : 모든 컴포넌트들은 이 클래스를 상속 받습니다. 
	// 건재 : 만약 트랜스폼이 존재하는 컴포넌트라면 Scene컴포넌트를 상속 받고 아니라면 그냥 Component를 상속 받습니다.
	class Component
	{
	public:
		Component();
		virtual ~Component();

	public:
		virtual void Init();
		virtual void Update(float _deltaTime);
		virtual void LateUpdate(float _deltaTime);
		virtual void FixedUpdate(float _deltaTime);
		virtual void Render();

	public:
		inline void SetName(string _name);
		inline const string& GetName();
		inline void SetMyObject(weak_ptr<Object> _object);
		inline const weak_ptr<Object> GetMyObject();
		inline bool GetIsDead();

	private:
		string m_Name;
		weak_ptr<Object> m_pObject;
	};

	// 건재 : Component의 Get & Set 함수
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
	const weak_ptr<Object> Component::GetMyObject()
	{
		return m_pObject;
	}
}


