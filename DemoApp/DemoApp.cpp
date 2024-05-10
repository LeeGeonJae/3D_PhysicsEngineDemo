#include "pch.h"
#include "DemoApp.h"

#include "InputManager.h"

#include "Object.h"
#include "Pawn.h"
#include "SkeletalMeshComponent.h"
#include "StaticMeshComponent.h"
#include "MovementComponent.h"
#include "PawnController.h"
#include "DebugShape.h"
#include "InputManager.h"

#include "../02. GraphicsEngine/Graphics.h"
#include "../02. GraphicsEngine/RenderManager.h"
#include "../02. GraphicsEngine/ResourceManager.h"
#include "../02. GraphicsEngine/StaticMeshResource.h"
#include "../02. GraphicsEngine/SkeletalMeshResource.h"

#include "../03. PhysXEngine/FQPhysics.h"
#include "../03. PhysXEngine/Common.h"

void callbackFunction(physics::CollisionData data, physics::ECollisionEventType type)
{

}

void DemoApp::Init()
{
	using namespace Engine;

	__super::Init();

	m_Physics = make_shared<physics::FQPhysics>();
	physics::PhysicsEngineInfo engineInfo;
	for (int i = 0; i < 16; i++)
	{
		engineInfo.collisionMatrix[i] = INT_MAX;
	}
	engineInfo.gravity = Vector3(0.f, - 10.f, 0.f);
	m_Physics->Initialize(engineInfo);
	m_Funtion = callbackFunction;
	m_Physics->SetCallBackFunction(m_Funtion);

	{
		physics::BoxColliderInfo boxInfo;
		boxInfo.boxExtent = Vector3(1000.f, 1.f, 1000.f);
		boxInfo.colliderInfo.collisionTransform = Matrix::CreateTranslation(Vector3(0.f, 0.f, 0.f));
		boxInfo.colliderInfo.density = 1.f;
		boxInfo.colliderInfo.dynamicFriction = 1.f;
		boxInfo.colliderInfo.id = 0;
		boxInfo.colliderInfo.layerNumber = 0;
		boxInfo.colliderInfo.restitution = 1.f;
		boxInfo.colliderInfo.staticFriction = 1.f;
		m_Physics->CreateStaticBody(boxInfo, physics::EColliderType::COLLISION);
		m_RigidBodyIDVec.push_back(0);

		boxInfo.boxExtent = Vector3(5.f, 5.f, 5.f);
		boxInfo.colliderInfo.collisionTransform = Matrix::CreateTranslation(Vector3(0.f, 20.f, 0.f));
		boxInfo.colliderInfo.id = 1;
		m_Physics->CreateDynamicBody(boxInfo, physics::EColliderType::COLLISION);

		physics::CharacterControllerInfo controllerInfo;
		controllerInfo.id = 1000;
		controllerInfo.position = Vector3(100.f, 20.f, 0.f);

		physics::CharacterMovementInfo movementInfo;
		m_Physics->CreateCCT(controllerInfo, movementInfo);
	}

	{
		shared_ptr<Pawn> pawn = std::make_shared<Pawn>(10000);
		PawnInfo pawnInfo;
		pawnInfo.m_ObjectDesc.m_Name = "Object";
		pawn->Setting(pawnInfo);

		m_ObjectVec.push_back(pawn);
	}
	{
		shared_ptr<Object> object = std::make_shared<Object>(0);
		ObjectInfo objectInfo;
		objectInfo.m_Name = "Object";
		object->Setting(objectInfo);

		std::shared_ptr<StaticMeshComponent> meshComponent = std::make_shared<StaticMeshComponent>();
		StaticMeshComponentInfo meshInfo;
		meshInfo.m_FilePath = "../Resources/FBX/cerberus_test.fbx";
		meshInfo.m_RenderComponentInfo.m_bIsVisible = true;
		meshInfo.m_RenderComponentInfo.m_SceneComponentInfo.m_Name = "TestComponent";
		meshComponent->Setting(meshInfo);
		meshComponent->SetOwner(object->GetRootComponent());
		object->SetPosition(Vector3(200.f, 200.f, 0.f));
		object->SetScale(Vector3(0.1f, 0.1f, 0.1f));

		m_ObjectVec.push_back(object);

		shared_ptr< GraphicsEngine::StaticMeshSceneResource> staticMesh = RESOURCE->Find<GraphicsEngine::StaticMeshSceneResource>("../Resources/FBX/zeldaPosed001.fbx");
		for (auto& mesh : staticMesh->GetStaticMeshVec())
		{
			for (const auto& vertices : mesh.GetVertices())
			{
				physx::PxVec3 vertex;
				vertex.x = -vertices.m_Position.x;
				vertex.y = -vertices.m_Position.y;
				vertex.z = -vertices.m_Position.z;
			}
		}
	}

	for (auto object : m_ObjectVec)
	{
		object->Init();
	}
}

void DemoApp::Update(float _deltaTime)
{
	__super::Update(_deltaTime);

	for (auto object : m_ObjectVec)
	{
		object->Update(_deltaTime);
	}

	bool IsMove = false;
	if (INPUT->GetKeyboardState().IsKeyDown(DirectX::Keyboard::Keys::Left))
	{
		Vector3 direction = Vector3(-1.f, 0.f, 0.f);

		m_Physics->AddInputMove(1000, direction);
		IsMove = true;
	}
	if (INPUT->GetKeyboardState().IsKeyDown(DirectX::Keyboard::Keys::Right))
	{
		Vector3 direction = Vector3(1.f, 0.f, 0.f);

		m_Physics->AddInputMove(1000, direction);
		IsMove = true;
	}
	if (INPUT->GetKeyboardState().IsKeyDown(DirectX::Keyboard::Keys::Up))
	{
		Vector3 direction = Vector3(0.f, 0.f, 1.f);

		m_Physics->AddInputMove(1000, direction);
		IsMove = true;
	}
	if (INPUT->GetKeyboardState().IsKeyDown(DirectX::Keyboard::Keys::Down))
	{
		Vector3 direction = Vector3(0.f, 0.f, -1.f);

		m_Physics->AddInputMove(1000, direction);
		IsMove = true;
	}
	if (INPUT->GetKeyboardState().IsKeyDown(DirectX::Keyboard::Keys::Space))
	{
		Vector3 direction = Vector3(0.f, 1.f, 0.f);

		m_Physics->AddInputMove(1000, direction);
	}

	m_Physics->Update(_deltaTime);
	m_Physics->FinalUpdate();
}

void DemoApp::LateUpdate(float _deltaTime)
{
	for (auto object : m_ObjectVec)
	{
		object->LateUpdate(_deltaTime);
	}
}

void DemoApp::FixedUpdate(float _deltaTime)
{
	for (auto object : m_ObjectVec)
	{
		object->FixedUpdate(_deltaTime);
	}
}

void DemoApp::Render()
{
	for (auto object : m_ObjectVec)
	{
		object->Render();
	}
}