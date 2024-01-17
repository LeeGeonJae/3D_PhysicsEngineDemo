#include "pch.h"
#include "TestApp.h"

#include "ResourceBase.h"
#include "InputManager.h"
#include "ResourceManager.h"

#include "Object.h"
#include "SkeletalMeshComponent.h"


void TestApp::Init()
{
	using namespace Engine;



	m_Object = std::make_shared<Object>(1);
	ObjectInfo objectInfo;
	objectInfo.m_Name = "Object";
	m_Object->Setting(objectInfo);

	std::shared_ptr<SkeletalMeshComponent> meshComponent = std::make_shared<SkeletalMeshComponent>();
	SkeletalMeshComponentInfo meshInfo;
	meshInfo.m_FilePath = "../Resources/FBX/SkinningTest.fbx";
	meshInfo.m_RenderComponentInfo.m_bIsVisible = true;
	meshInfo.m_RenderComponentInfo.m_SceneComponentInfo.m_Name = "TestComponent";
	meshComponent->Setting(meshInfo);
	meshComponent->SetOwner(m_Object->GetRootComponent());
	
	m_Object->Init();
}

void TestApp::Update()
{
	m_Object->Update();
}

void TestApp::LateUpdate()
{
	m_Object->LateUpdate();
}

void TestApp::FixedUpdate()
{
	m_Object->FixedUpdate();
}

void TestApp::Render()
{
	m_Object->Render();
}