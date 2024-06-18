#pragma once
#include "Component.h"

#include "../00. GraphicsEngine/Types.h"

namespace Engine
{
	// 건재 : 선언
	class Object;

	// 건재 : 컴포넌트 정보 구조체
	struct SceneComponentInfo
	{
		string m_Name;
		Vector3 m_Position = Vector3::Zero;
		Vector3 m_Rotation = Vector3::Zero;
		Vector3 m_Scale = Vector3::One;
	};

	// 건재 : 씬 컴포넌트 클래스
	// 건재 : 기본 컴포넌트에 트랜스폼이 더해진 컴포넌트로 위치 계층구조를 가지고 있는 컴포넌트
	class SceneComponent : public Component, public enable_shared_from_this<SceneComponent>
	{
	public:
		SceneComponent();
		virtual ~SceneComponent();

	public:
		void Setting(SceneComponentInfo _info);

	public:
		virtual void Init() override;
		virtual void Update(float _deltaTime) override;
		virtual void FixedUpdate(float _deltaTime) override;
		virtual void LateUpdate(float _deltaTime) override;
		virtual void Render() override;

	public:
		template <typename T>
		inline vector<weak_ptr<T>> FindComponent();
		inline const vector<shared_ptr<SceneComponent>>& GetChildrenComponent();
		inline void SetComponent(shared_ptr<SceneComponent> _component);

	public:
		inline void SetWorldTransform(Matrix _world);
		inline Matrix& GetWorldTransform();
		inline void SetLocalTransform(Matrix _local);
		inline const Matrix& GetLocalTransform();
		inline void SetPosition(Vector3 _position);
		inline const Vector3& GetPosition();
		inline void SetRotation(Vector3 _rotation);
		inline const Vector3& GetRotation();
		inline void SetQuaternion(Quaternion _quternion);
		inline const Quaternion& GetQuaternion();
		inline void SetScale(Vector3 _scale);
		inline const Vector3& GetScale();
		inline void SetOwner(shared_ptr<SceneComponent> _owner);
		inline const weak_ptr<SceneComponent> GetOwner();

	private:
		weak_ptr<SceneComponent> m_pOwner;
		vector<shared_ptr<SceneComponent>> m_pChildren;

		Matrix m_World;
		Matrix m_Local;
		Vector3 m_Position;
		Vector3 m_Rotation;
		Quaternion m_Quternion;
		Vector3 m_Scale;
	};

	// 건재 : 원하는 타입의 자식 컴포넌트들을 가져오는 GetComponent
	template <typename T> 
	inline vector<weak_ptr<T>> SceneComponent::FindComponent()
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

	// 건재 : SceneComponent의 Get & Set 함수
	const vector<shared_ptr<SceneComponent>>& SceneComponent::GetChildrenComponent()
	{
		return m_pChildren;
	}
	void SceneComponent::SetComponent(shared_ptr<SceneComponent> _component)
	{
		m_pChildren.push_back(_component);
	}

	void SceneComponent::SetWorldTransform(Matrix _world)
	{
		m_World = _world;
	}
	Matrix& SceneComponent::GetWorldTransform()
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
	void SceneComponent::SetQuaternion(Quaternion _quternion)
	{
		m_Quternion = _quternion;
	}
	const Quaternion& SceneComponent::GetQuaternion()
	{
		return m_Quternion;
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
	const weak_ptr<SceneComponent> SceneComponent::GetOwner()
	{
		return m_pOwner;
	}
}
