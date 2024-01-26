#include "pch.h"
#include "TestApp.h"

#include "ResourceBase.h"
#include "InputManager.h"
#include "ResourceManager.h"
#include "RenderManager.h"

#include "Object.h"
#include "SkeletalMeshComponent.h"
#include "StaticMeshComponent.h"

#include "../02. GraphicsEngine/Graphics.h"


void TestApp::Init()
{
	using namespace Engine;

	{
		shared_ptr<Object> object = std::make_shared<Object>(1);
		ObjectInfo objectInfo;
		objectInfo.m_Name = "Object";
		object->Setting(objectInfo);

		std::shared_ptr<StaticMeshComponent> meshComponent = std::make_shared<StaticMeshComponent>();
		StaticMeshComponentInfo meshInfo;
		meshInfo.m_FilePath = "../Resources/FBX/PBR_Test.fbx";
		meshInfo.m_RenderComponentInfo.m_bIsVisible = true;
		meshInfo.m_RenderComponentInfo.m_SceneComponentInfo.m_Name = "TestComponent";
		meshComponent->Setting(meshInfo);
		meshComponent->SetOwner(object->GetRootComponent());

		m_ObjectVec.push_back(object);
	}
	{
		shared_ptr<Object> object = std::make_shared<Object>(2);
		ObjectInfo objectInfo;
		objectInfo.m_Name = "Object";
		object->Setting(objectInfo);

		std::shared_ptr<StaticMeshComponent> meshComponent = std::make_shared<StaticMeshComponent>();
		StaticMeshComponentInfo meshInfo;
		meshInfo.m_FilePath = "../Resources/FBX/PBR_Test.fbx";
		meshInfo.m_RenderComponentInfo.m_bIsVisible = true;
		meshInfo.m_RenderComponentInfo.m_SceneComponentInfo.m_Name = "TestComponent";
		meshComponent->Setting(meshInfo);
		meshComponent->SetOwner(object->GetRootComponent());
		object->SetPosition(Vector3(200.f, 100.f, 0.f));

		m_ObjectVec.push_back(object);
	}
	{
		shared_ptr<Object> object = std::make_shared<Object>(3);
		ObjectInfo objectInfo;
		objectInfo.m_Name = "Object";
		object->Setting(objectInfo);

		std::shared_ptr<SkeletalMeshComponent> meshComponent = std::make_shared<SkeletalMeshComponent>();
		SkeletalMeshComponentInfo meshInfo;
		meshInfo.m_FilePath = "../Resources/FBX/SkinningTest.fbx";
		meshInfo.m_RenderComponentInfo.m_bIsVisible = true;
		meshInfo.m_RenderComponentInfo.m_SceneComponentInfo.m_Name = "TestComponent";
		meshComponent->Setting(meshInfo);
		meshComponent->SetOwner(object->GetRootComponent());
		object->SetPosition(Vector3(250.f, 0.f, 0.f));

		m_ObjectVec.push_back(object);
	}
	{
		shared_ptr<Object> object = std::make_shared<Object>(4);
		ObjectInfo objectInfo;
		objectInfo.m_Name = "Object";
		object->Setting(objectInfo);

		std::shared_ptr<SkeletalMeshComponent> meshComponent = std::make_shared<SkeletalMeshComponent>();
		SkeletalMeshComponentInfo meshInfo;
		meshInfo.m_FilePath = "../Resources/FBX/SkinningTest.fbx";
		meshInfo.m_RenderComponentInfo.m_bIsVisible = true;
		meshInfo.m_RenderComponentInfo.m_SceneComponentInfo.m_Name = "TestComponent";
		meshComponent->Setting(meshInfo);
		meshComponent->SetOwner(object->GetRootComponent());
		object->SetPosition(Vector3(300.f, 0.f, 0.f));

		m_ObjectVec.push_back(object);
	}

	for (auto object : m_ObjectVec)
	{
		object->Init();
	}
}

void TestApp::Update(float _deltaTime)
{
	for (auto object : m_ObjectVec)
	{
		object->Update(_deltaTime);
	}
}

void TestApp::LateUpdate(float _deltaTime)
{
	for (auto object : m_ObjectVec)
	{
		object->LateUpdate(_deltaTime);
	}
}

void TestApp::FixedUpdate(float _deltaTime)
{
	for (auto object : m_ObjectVec)
	{
		object->FixedUpdate(_deltaTime);
	}
}

void TestApp::Render()
{
	for (auto object : m_ObjectVec)
	{
		object->Render();
	}
}