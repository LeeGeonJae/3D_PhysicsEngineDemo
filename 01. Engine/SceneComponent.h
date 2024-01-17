#pragma once
#include "Component.h"

namespace Engine
{
	// 선언
	class Object;

	// 컴포넌트 정보 구조체
	struct SceneComponentInfo
	{
		string m_Name;
		Vector3 m_Position = Vector3::One;
		Vector3 m_Rotation = Vector3::One;
		Vector3 m_Scale = Vector3::One;
	};

	// 씬 컴포넌트 클래스
	class SceneComponent : public Component, public enable_shared_from_this<SceneComponent>
	{
	public:
		SceneComponent();
		virtual ~SceneComponent();

	public:
		void Setting(SceneComponentInfo _info);

	public:
		virtual void Init() override;
		virtual void Update() override;
		virtual void FixedUpdate() override;
		virtual void LateUpdate() override;
		virtual void Render() override;

	public:
		template <typename T>
		inline vector<shared_ptr<T>> FindComponent();
		inline const vector<shared_ptr<SceneComponent>>& GetChildrenComponent();
		inline void SetComponent(shared_ptr<SceneComponent> _component);

	public:
		inline void SetWorldTransform(Matrix _world);
		inline const Matrix& GetWorldTransform();
		inline void SetLocalTransform(Matrix _local);
		inline const Matrix& GetLocalTransform();
		inline void SetPosition(Vector3 _position);
		inline const Vector3& GetPosition();
		inline void SetRotation(Vector3 _rotation);
		inline const Vector3& GetRotation();
		inline void SetScale(Vector3 _scale);
		inline const Vector3& GetScale();
		inline void SetOwner(shared_ptr<SceneComponent> _owner);
		inline const weak_ptr<SceneComponent>& GetOwner();

	private:
		weak_ptr<SceneComponent> m_pOwner;
		vector<shared_ptr<SceneComponent>> m_pChildren;

		Matrix m_World;
		Matrix m_Local;
		Vector3 m_Position;
		Vector3 m_Rotation;
		Vector3 m_Scale;
	};

	// 원하는 타입의 자식 컴포넌트들을 가져오는 GetComponent
	template <typename T> 
	inline vector<shared_ptr<T>> SceneComponent::FindComponent()
	{
		bool bIsBase = std::is_base_of<Component, T>::value;
		assert(bIsBase == true);

		vector<shared_ptr<T>> Components;
		for (auto component : m_pChildren)
		{
			shared_ptr<T> tempComponent = dynamic_pointer_cast<T>(component);
			if (tempComponent != nullptr)
				Components.push_back(tempComponent);
		}

		return Components;
	}
	const vector<shared_ptr<SceneComponent>>& SceneComponent::GetChildrenComponent()
	{
		return m_pChildren;
	}
	// 자식 컴포넌트를 세팅하고 그 컴포넌트의 owner 세팅
	void SceneComponent::SetComponent(shared_ptr<SceneComponent> _component)
	{
		m_pChildren.push_back(_component);
	}

	void SceneComponent::SetWorldTransform(Matrix _world)
	{
		m_World = _world;
	}
	const Matrix& SceneComponent::GetWorldTransform()
	{
		return m_World;
	}
	void SceneComponent::SetLocalTransform(Matrix _local)
	{
		m_Local = _local;
	}
	const Matrix& SceneComponent::GetLocalTransform()
	{
		return m_Local;
	}
	void SceneComponent::SetPosition(Vector3 _position)
	{
		m_Position = _position;
	}
	const Vector3& SceneComponent::GetPosition()
	{
		return m_Position;
	}
	void SceneComponent::SetRotation(Vector3 _rotation)
	{
		m_Rotation = _rotation;
	}
	const Vector3& SceneComponent::GetRotation()
	{
		return m_Rotation;
	}
	void SceneComponent::SetScale(Vector3 _scale)
	{
		m_Scale = _scale;
	}
	const Vector3& SceneComponent::GetScale()
	{
		return m_Scale;
	}
	void SceneComponent::SetOwner(shared_ptr<SceneComponent> _owner)
	{
		_owner->SetComponent(shared_from_this());
		m_pOwner = _owner;
	}
	const weak_ptr<SceneComponent>& SceneComponent::GetOwner()
	{
		return m_pOwner;
	}
}


