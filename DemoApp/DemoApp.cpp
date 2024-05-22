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
	engineInfo.gravity = Vector3(0.f, - 30.f, 0.f);
	m_Physics->Initialize(engineInfo);
	m_Funtion = callbackFunction;
	m_Physics->SetCallBackFunction(m_Funtion);

	// 다이나믹 리지드 바디 & 캐릭터 컨트롤러
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

	// 캐릭터
	{
		shared_ptr<Pawn> pawn = std::make_shared<Pawn>(10000);
		PawnInfo pawnInfo;
		pawnInfo.m_ObjectDesc.m_Name = "Object";
		pawn->Setting(pawnInfo);

		m_ObjectVec.push_back(pawn);
	}

	// 컨벡스 메시
	{
		shared_ptr<Object> object = std::make_shared<Object>(2);
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

		shared_ptr<GraphicsEngine::StaticMeshSceneResource> staticMesh = RESOURCE->Find<GraphicsEngine::StaticMeshSceneResource>("../Resources/FBX/zeldaPosed001.fbx");
		std::vector<Vector3> meshVertex;
		meshVertex.reserve(100000);
		for (auto& mesh : staticMesh->GetStaticMeshVec())
		{
			for (const auto& vertices : mesh.GetVertices())
			{
				Vector3 vertex;
				vertex.x = -vertices.m_Position.x;
				vertex.y = -vertices.m_Position.y;
				vertex.z = -vertices.m_Position.z;
				meshVertex.push_back(vertex);
			}
		}
		physics::ConvexMeshColliderInfo convexInfo;
		convexInfo.colliderInfo.collisionTransform = Matrix::CreateTranslation(Vector3(200.f, 200.f, 0.f));
		convexInfo.colliderInfo.density = 1.f;
		convexInfo.colliderInfo.dynamicFriction = 1.f;
		convexInfo.colliderInfo.id = 2;
		convexInfo.colliderInfo.restitution = 1.f;
		convexInfo.colliderInfo.staticFriction = 1.f;
		convexInfo.convexPolygonLimit = 10.f;
		convexInfo.vertexSize = meshVertex.size();
		convexInfo.vertices = meshVertex.data();

		m_Physics->CreateDynamicBody(convexInfo, physics::EColliderType::COLLISION);
	}

	// 관절
	{
		physics::CharacterPhysicsInfo physicsInfo;
		physicsInfo.density = 1.f;
		physicsInfo.dynamicFriction = 1.f;
		physicsInfo.id = 3;
		physicsInfo.layerNumber = 0;
		physicsInfo.restitution = 1.f;
		physicsInfo.staticFriction = 1.f;
		physicsInfo.worldTransform = Matrix::CreateTranslation(Vector3(0.f, 200.f, 0.f));

		m_Physics->CreateCharacterphysics(physicsInfo);
		
		{
			physics::CharacterLinkInfo linkInfo;
			linkInfo.boneName = "pelvis";
			linkInfo.density = 100.f;
			linkInfo.localTransform = Matrix::CreateTranslation(Vector3(0.f, 0.f, 0.f));
			linkInfo.parentBoneName = "root";
			linkInfo.JointInfo.Swing1AxisInfo.motion = physics::EArticulationMotion::LIMITED;
			linkInfo.JointInfo.Swing1AxisInfo.limitsHigh = 10.f;
			linkInfo.JointInfo.Swing1AxisInfo.limitsLow = -10.f;
			linkInfo.JointInfo.Swing2AxisInfo.motion = physics::EArticulationMotion::LIMITED;
			linkInfo.JointInfo.Swing2AxisInfo.limitsHigh = 10.f;
			linkInfo.JointInfo.Swing2AxisInfo.limitsLow = -10.f;
			linkInfo.JointInfo.TwistAxisInfo.motion = physics::EArticulationMotion::LIMITED;
			linkInfo.JointInfo.TwistAxisInfo.limitsHigh = 50.f;
			linkInfo.JointInfo.TwistAxisInfo.limitsLow = -50.f;
			linkInfo.JointInfo.damping = 0.99f;
			linkInfo.JointInfo.localTransform = Matrix::CreateTranslation(Vector3(0.f, 0.f, 0.f));
			linkInfo.JointInfo.maxForce = 0.f;
			linkInfo.JointInfo.stiffness = 0.f;
			m_Physics->AddArticulationLink(3, linkInfo, 3.f, 5.f);

			// 하체
			{
				linkInfo.boneName = "thighI";
				linkInfo.parentBoneName = "pelvis";
				linkInfo.localTransform = Matrix::CreateTranslation(Vector3(5.f, 10.f, 0.f));
				linkInfo.JointInfo.localTransform = Matrix::CreateTranslation(Vector3(0.f, -10.f, 0.f));
				m_Physics->AddArticulationLink(3, linkInfo, DirectX::SimpleMath::Vector3(3.f, 10.f, 3.f));
				linkInfo.boneName = "thighR";
				linkInfo.parentBoneName = "pelvis";
				linkInfo.localTransform = Matrix::CreateTranslation(Vector3(-5.f, 10.f, 0.f));
				linkInfo.JointInfo.localTransform = Matrix::CreateTranslation(Vector3(0.f, -10.f, 0.f));
				m_Physics->AddArticulationLink(3, linkInfo, DirectX::SimpleMath::Vector3(3.f, 10.f, 3.f));
				{
					linkInfo.boneName = "calfI";
					linkInfo.parentBoneName = "thighI";
					linkInfo.localTransform = Matrix::CreateTranslation(Vector3(0.f, 16.f, 0.f));
					linkInfo.JointInfo.localTransform = Matrix::CreateTranslation(Vector3(0.f, -8.f, 0.f));
					m_Physics->AddArticulationLink(3, linkInfo, DirectX::SimpleMath::Vector3(3.f, 8.f, 3.f));
					linkInfo.boneName = "calfR";
					linkInfo.parentBoneName = "thighR";
					linkInfo.localTransform = Matrix::CreateTranslation(Vector3(0.f, 16.f, 0.f));
					linkInfo.JointInfo.localTransform = Matrix::CreateTranslation(Vector3(0.f, -8.f, 0.f));
					m_Physics->AddArticulationLink(3, linkInfo, DirectX::SimpleMath::Vector3(3.f, 8.f, 3.f));
					{
						linkInfo.boneName = "footI";
						linkInfo.parentBoneName = "calfI";
						linkInfo.localTransform = Matrix::CreateTranslation(Vector3(0.f, 9.f, -4.f));
						linkInfo.JointInfo.localTransform = Matrix::CreateTranslation(Vector3(0.f, -1.f, 4.f));
						m_Physics->AddArticulationLink(3, linkInfo, DirectX::SimpleMath::Vector3(3.f, 1.5f, 4.5f));
						linkInfo.boneName = "footR";
						linkInfo.parentBoneName = "calfR";
						linkInfo.localTransform = Matrix::CreateTranslation(Vector3(0.f, 9.f, -4.f));
						linkInfo.JointInfo.localTransform = Matrix::CreateTranslation(Vector3(0.f, -1.f, 4.f));
						m_Physics->AddArticulationLink(3, linkInfo, DirectX::SimpleMath::Vector3(3.f, 1.5f, 4.5f));
					}
				}
			}
			// 상체
			{
				linkInfo.boneName = "spine01";
				linkInfo.parentBoneName = "pelvis";
				linkInfo.localTransform = Matrix::CreateTranslation(Vector3(0.f, -6.f, 0.f));
				linkInfo.JointInfo.localTransform = Matrix::CreateTranslation(Vector3(0.f, 3.f, 0.f));
				m_Physics->AddArticulationLink(3, linkInfo, 3.f, 4.f);
				{
					linkInfo.boneName = "spine02";
					linkInfo.parentBoneName = "spine01";
					linkInfo.localTransform = Matrix::CreateTranslation(Vector3(0.f, -6.f, 0.f));
					linkInfo.JointInfo.localTransform = Matrix::CreateTranslation(Vector3(0.f, 3.f, 0.f));
					m_Physics->AddArticulationLink(3, linkInfo, 3.f, 4.f);
					{
						linkInfo.boneName = "spine03";
						linkInfo.parentBoneName = "spine02";
						linkInfo.localTransform = Matrix::CreateTranslation(Vector3(0.f, -6.f, 0.f));
						linkInfo.JointInfo.localTransform = Matrix::CreateTranslation(Vector3(0.f, 3.f, 0.f));
						m_Physics->AddArticulationLink(3, linkInfo, 2.f, 5.f);
						{
							linkInfo.boneName = "spine04";
							linkInfo.parentBoneName = "spine03";
							linkInfo.localTransform = Matrix::CreateTranslation(Vector3(0.f, -10.f, 0.f));
							linkInfo.JointInfo.localTransform = Matrix::CreateTranslation(Vector3(0.f, 5.f, 0.f));
							m_Physics->AddArticulationLink(3, linkInfo, 4.f, 8.f);
							{
								linkInfo.boneName = "clavicleI";
								linkInfo.parentBoneName = "spine04";
								linkInfo.localTransform = Matrix::CreateTranslation(Vector3(10.f, -8.f, 0.f));
								linkInfo.JointInfo.localTransform = Matrix::CreateTranslation(Vector3(-8.f, 4.f, 0.f));
								linkInfo.JointInfo.Swing1AxisInfo.limitsHigh = 20.f;
								linkInfo.JointInfo.Swing1AxisInfo.limitsLow = -20.f;
								linkInfo.JointInfo.Swing2AxisInfo.limitsHigh = 20.f;
								linkInfo.JointInfo.Swing2AxisInfo.limitsLow = -20.f;
								linkInfo.JointInfo.TwistAxisInfo.limitsHigh = 20.f;
								linkInfo.JointInfo.TwistAxisInfo.limitsLow = -20.f;
								m_Physics->AddArticulationLink(3, linkInfo, 2.f, 5.f);
								linkInfo.boneName = "clavicleR";
								linkInfo.parentBoneName = "spine04";
								linkInfo.localTransform = Matrix::CreateTranslation(Vector3(-10.f, -8.f, 0.f));
								linkInfo.JointInfo.localTransform = Matrix::CreateTranslation(Vector3(8.f, 4.f, 0.f));
								m_Physics->AddArticulationLink(3, linkInfo, 2.f, 5.f);
								{
									linkInfo.boneName = "upperarmI";
									linkInfo.parentBoneName = "clavicleI";
									linkInfo.localTransform = Matrix::CreateTranslation(Vector3(12.f, 0.f, 0.f));
									linkInfo.JointInfo.localTransform = Matrix::CreateTranslation(Vector3(-6.f, 0.f, 0.f));
									linkInfo.JointInfo.Swing1AxisInfo.limitsHigh = 30.f;
									linkInfo.JointInfo.Swing1AxisInfo.limitsLow = -30.f;
									linkInfo.JointInfo.Swing2AxisInfo.limitsHigh = 30.f;
									linkInfo.JointInfo.Swing2AxisInfo.limitsLow = -30.f;
									linkInfo.JointInfo.TwistAxisInfo.limitsHigh = 30.f;
									linkInfo.JointInfo.TwistAxisInfo.limitsLow = -30.f;
									m_Physics->AddArticulationLink(3, linkInfo, 6.f, 3.f);
									linkInfo.boneName = "upperarmR";
									linkInfo.parentBoneName = "clavicleR";
									linkInfo.localTransform = Matrix::CreateTranslation(Vector3(-12.f, 0.f, 0.f));
									linkInfo.JointInfo.localTransform = Matrix::CreateTranslation(Vector3(6.f, 0.f, 0.f));
									m_Physics->AddArticulationLink(3, linkInfo, 6.f, 3.f);
									{
										linkInfo.boneName = "lowerarmI";
										linkInfo.parentBoneName = "upperarmI";
										linkInfo.localTransform = Matrix::CreateTranslation(Vector3(12.f, 0.f, 0.f));
										linkInfo.JointInfo.localTransform = Matrix::CreateTranslation(Vector3(-6.f, 0.f, 0.f));
										linkInfo.JointInfo.Swing1AxisInfo.limitsHigh = 180.0f;
										linkInfo.JointInfo.Swing1AxisInfo.limitsLow = 0.0f;
										linkInfo.JointInfo.Swing2AxisInfo.limitsHigh = 5.f;
										linkInfo.JointInfo.Swing2AxisInfo.limitsLow = -5.f;
										linkInfo.JointInfo.TwistAxisInfo.limitsHigh = 5.f;
										linkInfo.JointInfo.TwistAxisInfo.limitsLow = -5.f;
										m_Physics->AddArticulationLink(3, linkInfo, 6.f, 2.f);
										linkInfo.boneName = "lowerarmR";
										linkInfo.parentBoneName = "upperarmR";
										linkInfo.localTransform = Matrix::CreateTranslation(Vector3(-12.f, 0.f, 0.f));
										linkInfo.JointInfo.localTransform = Matrix::CreateTranslation(Vector3(6.f, 0.f, 0.f));
										m_Physics->AddArticulationLink(3, linkInfo, 6.f, 2.f);
										{
											linkInfo.boneName = "handI";
											linkInfo.parentBoneName = "lowerarmI";
											linkInfo.localTransform = Matrix::CreateTranslation(Vector3(10.f, 0.f, 0.f));
											linkInfo.JointInfo.localTransform = Matrix::CreateTranslation(Vector3(-3.f, 0.f, 0.f));
											linkInfo.JointInfo.Swing1AxisInfo.limitsHigh = 60.0f;
											linkInfo.JointInfo.Swing1AxisInfo.limitsLow = 0.0f;
											linkInfo.JointInfo.Swing2AxisInfo.limitsHigh = 5.f;
											linkInfo.JointInfo.Swing2AxisInfo.limitsLow = -5.f;
											linkInfo.JointInfo.TwistAxisInfo.limitsHigh = 5.f;
											linkInfo.JointInfo.TwistAxisInfo.limitsLow = -5.f;
											m_Physics->AddArticulationLink(3, linkInfo, DirectX::SimpleMath::Vector3(3.f, 2.f, 1.f));
											linkInfo.boneName = "handR";
											linkInfo.parentBoneName = "lowerarmR";
											linkInfo.localTransform = Matrix::CreateTranslation(Vector3(-10.f, 0.f, 0.f));
											linkInfo.JointInfo.localTransform = Matrix::CreateTranslation(Vector3(3.f, 0.f, 0.f));
											m_Physics->AddArticulationLink(3, linkInfo, DirectX::SimpleMath::Vector3(3.f, 2.f, 1.f));
										}
									}
								}

								linkInfo.boneName = "neck01";
								linkInfo.parentBoneName = "spine04";
								linkInfo.localTransform = Matrix::CreateTranslation(Vector3(0.f, -8.f, 0.f));
								linkInfo.JointInfo.localTransform = Matrix::CreateTranslation(Vector3(0.f, 0.f, 0.f));
								linkInfo.JointInfo.Swing1AxisInfo.limitsHigh = 20.f;
								linkInfo.JointInfo.Swing1AxisInfo.limitsLow = -20.f;
								linkInfo.JointInfo.Swing2AxisInfo.limitsHigh = 20.f;
								linkInfo.JointInfo.Swing2AxisInfo.limitsLow = -20.f;
								linkInfo.JointInfo.TwistAxisInfo.limitsHigh = 40.f;
								linkInfo.JointInfo.TwistAxisInfo.limitsLow = -40.f;
								m_Physics->AddArticulationLink(3, linkInfo, 0.3f, 3.f);
								linkInfo.boneName = "neck02";
								linkInfo.parentBoneName = "neck01";
								linkInfo.localTransform = Matrix::CreateTranslation(Vector3(0.f, -3.f, -0.5f));
								linkInfo.JointInfo.localTransform = Matrix::CreateTranslation(Vector3(0.f, 0.f, 1.3f));
								m_Physics->AddArticulationLink(3, linkInfo, 0.3f, 3.f);
								linkInfo.boneName = "head";
								linkInfo.parentBoneName = "neck02";
								linkInfo.localTransform = Matrix::CreateTranslation(Vector3(0.f, -6.f, -1.3f));
								linkInfo.JointInfo.localTransform = Matrix::CreateTranslation(Vector3(0.f, 2.f, 1.7f));
								m_Physics->AddArticulationLink(3, linkInfo, 6.5f);
							}
						}
					}
				}
			}

			m_Physics->SimulationCharacter(3);
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

	if (INPUT->GetKeyboardState().IsKeyDown(DirectX::Keyboard::Keys::Left))
	{
		Vector3 direction = Vector3(-1.f, 0.f, 0.f);

		m_Physics->AddInputMove(1000, direction);
	}
	if (INPUT->GetKeyboardState().IsKeyDown(DirectX::Keyboard::Keys::Right))
	{
		Vector3 direction = Vector3(1.f, 0.f, 0.f);

		m_Physics->AddInputMove(1000, direction);
	}
	if (INPUT->GetKeyboardState().IsKeyDown(DirectX::Keyboard::Keys::Up))
	{
		Vector3 direction = Vector3(0.f, 0.f, 1.f);

		m_Physics->AddInputMove(1000, direction);
	}
	if (INPUT->GetKeyboardState().IsKeyDown(DirectX::Keyboard::Keys::Down))
	{
		Vector3 direction = Vector3(0.f, 0.f, -1.f);

		m_Physics->AddInputMove(1000, direction);
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