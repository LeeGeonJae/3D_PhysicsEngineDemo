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

		m_pRootComponent = make_shared<SceneComponent>();
		SceneComponentInfo sceneComponentInfo;
		sceneComponentInfo.m_Name = "SceneComponent";
		m_pRootComponent->Setting(sceneComponentInfo);
		m_pRootComponent->SetMyObject(shared_from_this());
	}

	void Object::Init()
	{
		m_pRootComponent->Init();
	}

	void Object::Update()
	{
		m_pRootComponent->Update();
	}

	void Object::LateUpdate()
	{
		m_pRootComponent->LateUpdate();
	}

	void Object::FixedUpdate()
	{
		m_pRootComponent->FixedUpdate();
	}

	void Object::Render()
	{
		m_pRootComponent->Render();
	}

	void Object::SetComponent(shared_ptr<SceneComponent> _component)
	{
		_component->SetMyObject(shared_from_this());
		m_pMyComponents.push_back(_component);
	}
}

