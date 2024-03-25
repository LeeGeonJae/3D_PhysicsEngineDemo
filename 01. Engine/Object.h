#pragma once

namespace Engine
{
	// 건재 : 클래스 선언
	class Component;
	class SceneComponent;


	// 건재 : 오브젝트 정보 구조체
	struct ObjectInfo
	{
		string m_Name;
	};

	// 건재 : 오브젝트 클래스 ( 오브젝트 매니저 생성 대기 중 )
	// 건재 : 씬 매니저 -> 현재 씬 -> 오브젝트 매니저 -> 오브젝트
	// 건재 : 현재 씬들은 오브젝트 매니저 클래스를 가지고 있고 해당 오브젝트 클래스(오브젝트 ID관리)에서 오브젝트를 생성할 구조를 작성할 예정입니다.
	class Object : public enable_shared_from_this<Object>
	{
	public:
		Object(unsigned int _Id);
		virtual ~Object();

	public:
		virtual void Setting(ObjectInfo info);

	public:
		virtual void Init();
		virtual void Update(float _deltaTime);
		virtual void LateUpdate(float _deltaTime);
		virtual void FixedUpdate(float _deltaTime);
		virtual void Render();

	public:
		template <typename T>
		inline vector<weak_ptr<T>> FindComponent();
		inline const vector<shared_ptr<Component>>& GetComponent();
		void SetComponent(shared_ptr<Component> _component);
		inline shared_ptr<SceneComponent> GetRootComponent();

	public:
		inline void SetName(string _name);
		inline string GetName();
		inline const unsigned int& GetID();
		inline bool GetIsDead();
		inline void SetIsDead(bool _isDead);

		void SetWorldTransform(Matrix _world);
		const Matrix& GetWorldTransform();
		const Vector3& GetPosition();
		void SetPosition(Vector3 _position);
		const Vector3& GetRotation();
		void SetRotation(Vector3 _rotation);
		const Vector3& GetScale();
		void SetScale(Vector3 _scale);

	private:
		string m_Name;
		unsigned int m_ID;
		bool m_bIsDead;

		vector<shared_ptr<Component>> m_pMyComponents;
		vector<shared_ptr<Component>> m_pMyDefalutComponents;
		shared_ptr<SceneComponent> m_pRootComponent;
	};

	// 건재 : 컴포넌트를 상속받는 모든 특정 컴포넌트들을 선별해서 반환해주는 템플릿 함수
	template <typename T>
	vector<weak_ptr<T>> Object::FindComponent()
	{
		bool bIsBase = std::is_base_of<Component, T>::value;
		assert(bIsBase == true);

		vector<weak_ptr<T>> Components;

		for (auto component : m_pMyComponents)
		{
			shared_ptr<T> findComponent = dynamic_pointer_cast<T>(component);
			if (findComponent != nullptr)
				Components.push_back(findComponent);
		}

		return Components;
	}

	// 건재 : Object의 Get & Set 함수
	const vector<shared_ptr<Component>>& Object::GetComponent()
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
	bool Object::GetIsDead()
	{
		return m_bIsDead;
	}
	void Object::SetIsDead(bool _isDead)
	{
		m_bIsDead = _isDead;
	}
}
