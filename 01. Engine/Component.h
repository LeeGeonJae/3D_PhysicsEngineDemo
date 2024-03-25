#pragma once

namespace Engine
{
	// ���� : Ŭ���� ����
	class Object;

	// ���� : ������Ʈ Ŭ����
	// ���� : ��� ������Ʈ���� �� Ŭ������ ��� �޽��ϴ�. 
	// ���� : ���� Ʈ�������� �����ϴ� ������Ʈ��� Scene������Ʈ�� ��� �ް� �ƴ϶�� �׳� Component�� ��� �޽��ϴ�.
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

	// ���� : Component�� Get & Set �Լ�
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


