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
#include "../02. GraphicsEngine/ResourceManager.h"
#include "../02. GraphicsEngine/StaticMeshResource.h"
#include "../02. GraphicsEngine/SkeletalMeshResource.h"
#include "../02. PhysXEngine/PhysX.h"
#include "../02. PhysXEngine/PhysicsSimulationEventCallback.h"
#include "../01. Engine/InputManager.h"
#include "../02. PhysXEngine/CharactorController.h"
#include "../02. PhysXEngine/SoftBody.h"
#include "../02. PhysXEngine/ClothPhysics.h"


void TestApp::Init()
{
	using namespace Engine;

	__super::Init();

	m_PhysX = make_shared<PhysicsEngine::PhysX>();

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

		shared_ptr<GraphicsEngine::StaticMeshSceneResource> staticMesh = RESOURCE->Find<GraphicsEngine::StaticMeshSceneResource>("../Resources/FBX/Vampire.fbx");
		auto& mesh = staticMesh->GetStaticMeshVec()[0];
		for (const auto& vertices : mesh.GetVertices())
		{
			physx::PxVec3 vertex;
			physx::PxVec2 uv;
			vertex.x = vertices.m_Position.x;
			vertex.y = vertices.m_Position.y;
			vertex.z = vertices.m_Position.z;
			uv.x = vertices.m_TexCoord.x;
			uv.y = vertices.m_TexCoord.y;
			m_PhysX->AddVertexPosition(vertex);
			m_PhysX->AddUV(uv);
		}

		for (const auto& indices : mesh.GetIndices())
		{
			m_PhysX->AddIndex(indices);
		}
	}

	m_PhysX->Init(RENDER->GetGraphics()->GetDevice().Get());

	int count = 1;
	for (auto body : m_PhysX->GetPxBodies())
	{
		shared_ptr<Object> object = std::make_shared<Object>(count);
		ObjectInfo objectInfo;
		objectInfo.m_Name = "PhysicsObject";
		object->Setting(objectInfo);

		std::shared_ptr<StaticMeshComponent> meshComponent = std::make_shared<StaticMeshComponent>();
		StaticMeshComponentInfo meshInfo;
		meshInfo.m_FilePath = "../Resources/FBX/zeldaPosed001.fbx";
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

	static int dtCount = 0;
	static float dt = 0;
	dtCount++;
	dt += _deltaTime;
	if (dt >= 1.f)
	{
		std::cout << "FPS : " << dtCount << std::endl;

		dt -= 1.f;
		dtCount = 0;
	}

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
	for (auto point : PhysicsEngine::PhysicsSimulationEventCallback::CollisionPoints)
	{
		shared_ptr<DirectX::BoundingSphere> box = make_shared<DirectX::BoundingSphere>();

		shared_ptr<Vector3> debugPoint = make_shared<Vector3>();
		debugPoint->x = point.x;
		debugPoint->y = point.y;
		debugPoint->z = -point.z;

		box->Center.x = point.x;
		box->Center.y = point.y;
		box->Center.z = -point.z;
		box->Radius = 0.5f;

		RENDER->AddDebugSphere(box);
	}
	for (auto softBody : m_PhysX->GetSoftBodies())
	{
		int vertexSize = softBody.mSoftBody->getCollisionMesh()->getNbVertices();
		
		for (int i = 0; i < vertexSize; i++)
		{
			physx::PxVec4 vertex = softBody.mPositionsInvMass[i];

			shared_ptr<DirectX::BoundingSphere> sphere = make_shared<DirectX::BoundingSphere>();
			sphere->Center.x = vertex.x;
			sphere->Center.y = vertex.y;
			sphere->Center.z = -vertex.z;
			sphere->Radius = 0.005f;

			RENDER->AddDebugSphere(sphere);
		}
	}
	{
		//physx::PxParticleClothBuffer* buffer = m_PhysX->GetParticleClothBuffer();
		//if (buffer != nullptr)
		//{
		//	int paticleSize = buffer->getNbActiveParticles();
		//	physx::PxVec4* particle = buffer->getPositionInvMasses();

		//	physx::PxCudaContextManager* cudaContextManager = m_PhysX->GetCudaContextManager();

		//	cudaContextManager->acquireContext();

		//	physx::PxCudaContext* cudaContext = cudaContextManager->getCudaContext();
		//	vector<physx::PxVec4> vertex;
		//	vertex.resize(paticleSize);

		//	cudaContext->memcpyDtoH(vertex.data(), CUdeviceptr(particle), sizeof(physx::PxVec4) * paticleSize);

		//	for (int i = 0; i < paticleSize; i++)
		//	{
		//		shared_ptr<DirectX::BoundingSphere> sphere = make_shared<DirectX::BoundingSphere>();
		//		sphere->Center.x = vertex[i].x;
		//		sphere->Center.y = vertex[i].y;
		//		sphere->Center.z = -vertex[i].z;
		//		sphere->Radius = 0.05f;

		//		RENDER->AddDebugSphere(sphere);
		//	}
		//}
	}
	{
		//PhysicsEngine::PhysicsClothGetData data = m_PhysX->GetPhysicsClothGetData();
		//auto cloth = m_PhysX->GetClothPhysics();

		//for (auto& line : cloth->GetSprings())
		//{
		//	shared_ptr<GraphicsEngine::DebugLineData> lineData = make_shared< GraphicsEngine::DebugLineData>();
		//	lineData->Pos0 = data.vertices[line.first];
		//	lineData->Pos1 = data.vertices[line.second];
		//	lineData->Color = Color(1.f, 0.f, 0.f, 1.f);

		//	RENDER->AddDebugLine(lineData);
		//}
	}

	physx::PxRigidActor* charactorBody = m_PhysX-> GetCharactorController()->GetPxController()->getActor();
	physx::PxShape* shape;
	charactorBody->getShapes(&shape, sizeof(shape));
	DebugCapsule(charactorBody, shape);

	for (auto object : m_ObjectVec)
	{
		object->Update(_deltaTime);
	}

	static float FixedTime = 0;
	FixedTime += _deltaTime;

	if (FixedTime >= 1 / 60.f)
	{
		FixedTime -= 1 / 60.f;
		bool IsMove = false;

		if (INPUT->GetKeyboardState().IsKeyDown(DirectX::Keyboard::Keys::Left))
		{
			Vector3 direction = Vector3(-1.f, 0.f, 0.f);

			m_PhysX->move(direction);
			IsMove = true;
		}
		if (INPUT->GetKeyboardState().IsKeyDown(DirectX::Keyboard::Keys::Right))
		{
			Vector3 direction = Vector3(1.f, 0.f, 0.f);

			m_PhysX->move(direction);
			IsMove = true;
		}
		if (INPUT->GetKeyboardState().IsKeyDown(DirectX::Keyboard::Keys::Up))
		{
			Vector3 direction = Vector3(0.f, 0.f, 1.f);

			m_PhysX->move(direction);
			IsMove = true;
		}
		if (INPUT->GetKeyboardState().IsKeyDown(DirectX::Keyboard::Keys::Down))
		{
			Vector3 direction = Vector3(0.f, 0.f, -1.f);

			m_PhysX->move(direction);
			IsMove = true;
		}
		if (INPUT->GetKeyboardState().IsKeyDown(DirectX::Keyboard::Keys::Space))
		{
			Vector3 direction = Vector3(0.f, 1.f, 0.f);

			m_PhysX->move(direction);
		}
		if (INPUT->GetKeyboardState().IsKeyDown(DirectX::Keyboard::Keys::Z))
		{
			m_PhysX->Update(1 / 60.f);
		}

		{
			PhysicsEngine::PhysicsClothGetData data = m_PhysX->GetPhysicsClothGetData();

			vector<GraphicsEngine::PhysicsVertex> vertices;
			vector<unsigned int> indices;

			vertices.resize(data.vertexSize);
			indices.resize(data.indexSize);

			for (int i = 0; i < data.vertexSize; i++)
			{
				vertices[i].position = data.vertices[i];
				vertices[i].normal = data.nomals[i];
				vertices[i].uv = data.uv[i];
			}

			for (int i = 0; i < data.indexSize; i++)
			{
				indices[i] = data.indices[i];
			}

			RENDER->SetPhysicsBuffer(vertices, indices);
		}
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