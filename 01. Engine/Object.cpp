#include "pch.h"
#include "Object.h"

#include "SceneComponent.h"


namespace Engine
{
	Object::Object(unsigned int _Id)
		: m_ID(_Id)
	{
	}

	Object::~Object()
	{
	}

	void Object::Setting(ObjectInfo info)
	{
		m_Name = info.m_Name;

		// 건재 : 트랜스폼을 가지고 있는 컴포넌트를 최상위 계층으로 지정
		m_pRootComponent = make_shared<SceneComponent>();
		SceneComponentInfo sceneComponentInfo;
		sceneComponentInfo.m_Name = "SceneComponent";
		m_pRootComponent->Setting(sceneComponentInfo);
		m_pRootComponent->SetMyObject(shared_from_this());
	}

	void Object::Init()
	{
		m_pRootComponent->Init();

		for (auto defalutComponent : m_pMyDefalutComponents)
		{
			defalutComponent->Init();
		}
	}

	void Object::Update(float _deltaTime)
	{
		m_pRootComponent->Update(_deltaTime);

		for (auto defalutComponent : m_pMyDefalutComponents)
		{
			defalutComponent->Update(_deltaTime);
		}
	}

	void Object::LateUpdate(float _deltaTime)
	{
		m_pRootComponent->LateUpdate(_deltaTime);

		for (auto defalutComponent : m_pMyDefalutComponents)
		{
			defalutComponent->LateUpdate(_deltaTime);
		}
	}

	void Object::FixedUpdate(float _deltaTime)
	{
		m_pRootComponent->FixedUpdate(_deltaTime);

		for (auto defalutComponent : m_pMyDefalutComponents)
		{
			defalutComponent->FixedUpdate(_deltaTime);
		}
	}

	void Object::Render()
	{
		m_pRootComponent->Render();

		for (auto defalutComponent : m_pMyDefalutComponents)
		{
			defalutComponent->Render();
		}
	}


	// 건재 : Get & Set
	void Object::SetComponent(shared_ptr<Component> _component)
	{
		_component->SetMyObject(shared_from_this());
		m_pMyComponents.push_back(_component);

		auto sceneComponent = dynamic_pointer_cast<SceneComponent>(_component);
		if (!sceneComponent)
		{
			m_pMyDefalutComponents.push_back(_component);
		}
	}
	void Object::SetWorldTransform(Matrix _world)
	{
		m_pRootComponent->SetWorldTransform(_world);
	}
	const Matrix& Object::GetWorldTransform()
	{
		return m_pRootComponent->GetWorldTransform();
	}
	const Vector3& Object::GetPosition()
	{
		return m_pRootComponent->GetPosition();
	}
	void Object::SetPosition(Vector3 _position)
	{
		m_pRootComponent->SetPosition(_position);
	}
	const Vector3& Object::GetRotation()
	{
		return m_pRootComponent->GetRotation();
	}
	void Object::SetRotation(Vector3 _rotation)
	{
		m_pRootComponent->SetRotation(_rotation);
	}
	const Vector3& Object::GetScale()
	{
		return m_pRootComponent->GetScale();
	}
	void Object::SetScale(Vector3 _scale)
	{
		m_pRootComponent->SetScale(_scale);
	}
}

