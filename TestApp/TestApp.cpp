#include "pch.h"
#include "TestApp.h"

#include "InputManager.h"

#include "Object.h"
#include "Pawn.h"
#include "SkeletalMeshComponent.h"
#include "StaticMeshComponent.h"
#include "MovementComponent.h"
#include "PawnController.h"

#include "DebugShape.h"

#include "../02. GraphicsEngine/Graphics.h"
#include "../02. GraphicsEngine/RenderManager.h"
#include "../02. PhysXEngine/PhysX.h"


void TestApp::Init()
{
	using namespace Engine;

	__super::Init();

	m_PhysX = make_shared<PhysicsEngine::PhysX>();
	m_PhysX->Init();

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
		meshInfo.m_FilePath = "../Resources/FBX/Tree.fbx";
		meshInfo.m_RenderComponentInfo.m_bIsVisible = true;
		meshInfo.m_RenderComponentInfo.m_SceneComponentInfo.m_Name = "TestComponent";
		meshComponent->Setting(meshInfo);
		meshComponent->SetOwner(object->GetRootComponent());
		object->SetPosition(Vector3(200.f, 200.f, 0.f));
		object->SetScale(Vector3(10.f, 10.f, 10.f));

		m_ObjectVec.push_back(object);
	}

	int count = 1;
	for (auto body : m_PhysX->GetPxBodies())
	{
		shared_ptr<Object> object = std::make_shared<Object>(count);
		ObjectInfo objectInfo;
		objectInfo.m_Name = "PhysicsObject";
		object->Setting(objectInfo);

		std::shared_ptr<StaticMeshComponent> meshComponent = std::make_shared<StaticMeshComponent>();
		StaticMeshComponentInfo meshInfo;
		meshInfo.m_FilePath = "../Resources/FBX/box.fbx";
		meshInfo.m_RenderComponentInfo.m_bIsVisible = true;
		meshInfo.m_RenderComponentInfo.m_SceneComponentInfo.m_Name = "PhysicsMeshComponent";
		meshComponent->Setting(meshInfo);
		meshComponent->SetOwner(object->GetRootComponent());
		object->SetPosition(Vector3(body->getGlobalPose().p.x, body->getGlobalPose().p.y, body->getGlobalPose().p.z));
		object->SetScale(Vector3(0.01f, 0.01f, 0.01f));

		m_ObjectVec.push_back(object);

		count++;

		physx::PxShape* shape;
		body->getShapes(&shape, 1, 0);

		if (shape->getGeometry().getType() == physx::PxGeometryType::eBOX)
		{
			DirectX::BoundingBox boundingBox;

			const physx::PxBoxGeometry box = static_cast<const physx::PxBoxGeometry&>(shape->getGeometry());
			boundingBox.Extents = Vector3(box.halfExtents.x, box.halfExtents.y, box.halfExtents.z);
		}
	}
	
	for (auto object : m_ObjectVec)
	{
		object->Init();
	}
}

void TestApp::Update(float _deltaTime)
{
	__super::Update(_deltaTime);

	int count = 1;
	for (auto body : m_PhysX->GetPxBodies())
	{
		for (auto object : m_ObjectVec)
		{
			if (object->GetID() == count)
			{
				Matrix dxMatrix;
				CopyPxTransformToDirectXMatrix(body->getGlobalPose(), dxMatrix);

				Vector3 scale;
				Vector3 translation;
				Quaternion quaternion;
				dxMatrix.Decompose(scale, quaternion, translation);

 				object->SetPosition(translation);
				object->SetQuternion(quaternion);
				break;
			}
		}

		count++;
	}

	for (auto body : m_PhysX->GetPxBodies())
	{
		physx::PxShape* shape;
		body->getShapes(&shape, 1);

		if (physx::PxGeometryType::eBOX == shape->getGeometry().getType())
		{
			DebugBox(body, shape);
		}
		else if (physx::PxGeometryType::eSPHERE == shape->getGeometry().getType())
		{
			DebugSphere(body, shape);
		}
		else if (physx::PxGeometryType::eCAPSULE == shape->getGeometry().getType())
		{
			DebugCapsule(body, shape);
		}
		else if (physx::PxGeometryType::eCONVEXMESH == shape->getGeometry().getType())
		{
			DebugConvexMesh(body, shape);
		}
	}

	for (auto object : m_ObjectVec)
	{
		object->Update(_deltaTime);
	}

	m_PhysX->Update(_deltaTime);
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