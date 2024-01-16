#include "TestApp.h"
#include "Header.h"

#include "ResourceBase.h"
#include "InputManager.h"
#include "ResourceManager.h"
#include "Material.h"

void TestApp::Init()
{
	RESOURCE->Load("../Resources/FBX/SkinningTest.fbx");
	RESOURCE->Load("../Resources/FBX/Dying.fbx");
	RESOURCE->Load("../Resources/FBX/Dying.fbx");
	RESOURCE->Load("../Resources/FBX/cerberus_test.fbx");
	RESOURCE->Load("../Resources/FBX/cerberus_test.fbx");
}

void TestApp::Update()
{
}

void TestApp::LateUpdate()
{
}

void TestApp::FixedUpdate()
{
}

void TestApp::Render()
{
}