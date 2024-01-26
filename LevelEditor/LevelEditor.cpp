#include "pch.h"
#include "LevelEditor.h"

#include "BoxComponent.h"
#include "Object.h"
#include "SkeletalMeshComponent.h"


void LevelEditor::Init()
{
	shared_ptr<Engine::Object> object = std::make_shared<Engine::Object>(1);
	Engine::ObjectInfo objectInfo;
	objectInfo.m_Name = "Object";
	object->Setting(objectInfo);

	std::shared_ptr<Engine::SkeletalMeshComponent> meshComponent = std::make_shared<Engine::SkeletalMeshComponent>();
	Engine::SkeletalMeshComponentInfo meshInfo;
	meshInfo.m_FilePath = "../Resources/FBX/SkinningTest.fbx";
	meshInfo.m_RenderComponentInfo.m_bIsVisible = true;
	meshInfo.m_RenderComponentInfo.m_SceneComponentInfo.m_Name = "TestComponent";
	meshComponent->Setting(meshInfo);
	meshComponent->SetOwner(object->GetRootComponent());

	m_ObjectVec.push_back(object);

	// ÄÝ¸®Àü..?

	//shared_ptr<Engine::BoxComponent> boxComponent = make_shared<Engine::BoxComponent>();

	//object->SetComponent()

	for (auto object : m_ObjectVec)
	{
		object->Init();
	}
}

void LevelEditor::Update(float _deltaTime)
{
	for (auto object : m_ObjectVec)
	{
		object->Update(_deltaTime);
	}
}

void LevelEditor::LateUpdate(float _deltaTime)
{
	for (auto object : m_ObjectVec)
	{
		object->LateUpdate(_deltaTime);
	}
}

void LevelEditor::FixedUpdate(float _deltaTime)
{
	for (auto object : m_ObjectVec)
	{
		object->FixedUpdate(_deltaTime);
	}
}

void LevelEditor::Render()
{
	for (auto object : m_ObjectVec)
	{
		object->Render();
	}
}