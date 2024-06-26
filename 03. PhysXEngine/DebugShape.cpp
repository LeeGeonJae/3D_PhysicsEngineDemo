#include "DebugShape.h"

// 물리 데이터를 통해서 디버그 데이터 출력하기
void DebugCapsule(physx::PxRigidActor* _body, physx::PxShape* _shape)
{
	const physx::PxCapsuleGeometry& capsuleGeometry = static_cast<const physx::PxCapsuleGeometry&>(_shape->getGeometry());
	Matrix dxMatrix;
	CopyPxTransformToDirectXMatrix(_body->getGlobalPose(), dxMatrix);

	Vector3 scale;
	Vector3 translation;
	Quaternion quaternion;
	dxMatrix.Decompose(scale, quaternion, translation);

	Matrix Sphere0Matrix = dxMatrix * Matrix::CreateTranslation(capsuleGeometry.radius * dxMatrix.Right());
	Matrix Sphere1Matrix = dxMatrix * Matrix::CreateTranslation(-capsuleGeometry.radius * dxMatrix.Right());

	shared_ptr<DirectX::BoundingSphere> sphere0 = make_shared<DirectX::BoundingSphere>();
	shared_ptr<DirectX::BoundingSphere> sphere1 = make_shared<DirectX::BoundingSphere>();
	shared_ptr<GraphicsEngine::DebugLineData> line1 = make_shared<GraphicsEngine::DebugLineData>();
	shared_ptr<GraphicsEngine::DebugLineData> line2 = make_shared<GraphicsEngine::DebugLineData>();
	shared_ptr<GraphicsEngine::DebugLineData> line3 = make_shared<GraphicsEngine::DebugLineData>();
	shared_ptr<GraphicsEngine::DebugLineData> line4 = make_shared<GraphicsEngine::DebugLineData>();

	sphere0->Center = Sphere0Matrix.Translation();
	sphere1->Center = Sphere1Matrix.Translation();
	line1->Pos0 = Sphere0Matrix.Translation() + Vector3(0.f, capsuleGeometry.radius, 0.f);
	line1->Pos1 = Sphere1Matrix.Translation() + Vector3(0.f, capsuleGeometry.radius, 0.f);
	line1->Color = Color(1.f, 1.f, 0.f, 1.f);
	line2->Pos0 = Sphere0Matrix.Translation() + Vector3(0.f, -capsuleGeometry.radius, 0.f);
	line2->Pos1 = Sphere1Matrix.Translation() + Vector3(0.f, -capsuleGeometry.radius, 0.f);
	line2->Color = Color(1.f, 1.f, 0.f, 1.f);
	line3->Pos0 = Sphere0Matrix.Translation() + Vector3(0.f, 0.f, capsuleGeometry.radius);
	line3->Pos1 = Sphere1Matrix.Translation() + Vector3(0.f, 0.f, capsuleGeometry.radius);
	line3->Color = Color(1.f, 1.f, 0.f, 1.f);
	line4->Pos0 = Sphere0Matrix.Translation() + Vector3(0.f, 0.f, -capsuleGeometry.radius);
	line4->Pos1 = Sphere1Matrix.Translation() + Vector3(0.f, 0.f, -capsuleGeometry.radius);
	line4->Color = Color(1.f, 1.f, 0.f, 1.f);
	sphere0->Radius = capsuleGeometry.radius;
	sphere1->Radius = capsuleGeometry.radius;

	RENDER->AddDebugLine(line1);
	RENDER->AddDebugLine(line2);
	RENDER->AddDebugLine(line3);
	RENDER->AddDebugLine(line4);
	RENDER->AddDebugSphere(sphere0);
	RENDER->AddDebugSphere(sphere1);
}

void DebugBox(physx::PxRigidActor* _body, physx::PxShape* _shape)
{
	shared_ptr<DirectX::BoundingOrientedBox> orientBox = make_shared<DirectX::BoundingOrientedBox>();
	const physx::PxBoxGeometry& boxGeometry = static_cast<const physx::PxBoxGeometry&>(_shape->getGeometry());

	Matrix dxMatrix;
	CopyPxTransformToDirectXMatrix(_body->getGlobalPose(), dxMatrix);

	Vector3 scale;
	Vector3 translation;
	Quaternion quaternion;
	dxMatrix.Decompose(scale, quaternion, translation);

	orientBox->Center = translation;
	orientBox->Extents = (const Vector3&)boxGeometry.halfExtents;
	orientBox->Orientation = quaternion;

	RENDER->AddDebugBox(orientBox);
}

void DebugSphere(physx::PxRigidActor* _body, physx::PxShape* _shape)
{
	shared_ptr<DirectX::BoundingSphere> sphere = make_shared<DirectX::BoundingSphere>();
	const physx::PxSphereGeometry& sphereGeometry = static_cast<const physx::PxSphereGeometry&>(_shape->getGeometry());

	Matrix dxMatrix;
	CopyPxTransformToDirectXMatrix(_body->getGlobalPose(), dxMatrix);

	sphere->Center = dxMatrix.Translation();
	sphere->Radius = sphereGeometry.radius;

	RENDER->AddDebugSphere(sphere);
}

void DebugConvexMesh(physx::PxRigidActor* _body, physx::PxShape* _shape)
{
	const physx::PxConvexMeshGeometry& convexMeshGeometry = static_cast<const physx::PxConvexMeshGeometry&>(_shape->getGeometry());

	Matrix dxMatrix;
	Matrix matrix;
	CopyPxTransformToDirectXMatrix(_body->getGlobalPose(), dxMatrix);

	// PxConvexMesh에서 정점 및 인덱스 정보 얻기
	const physx::PxVec3* convexMeshVertices = convexMeshGeometry.convexMesh->getVertices();
	const physx::PxU32 vertexCount = convexMeshGeometry.convexMesh->getNbVertices();

	const physx::PxU8* convexMeshIndices = convexMeshGeometry.convexMesh->getIndexBuffer();
	const physx::PxU32 PolygonCount = convexMeshGeometry.convexMesh->getNbPolygons();

	for (int i = 0; i < PolygonCount - 1; i++)
	{
		physx::PxHullPolygon polygon;
		convexMeshGeometry.convexMesh->getPolygonData(i, polygon);
		int vertexTotalNumber = polygon.mNbVerts;
		int startIndexNumber = polygon.mIndexBase;

		vector<Vector3> vertices;
		vertices.reserve(vertexTotalNumber);

		for (int j = 0; j < vertexTotalNumber; j++)
		{
			Vector3 vertex;
			vertex.x = -convexMeshVertices[convexMeshIndices[startIndexNumber + j]].x;
			vertex.y = -convexMeshVertices[convexMeshIndices[startIndexNumber + j]].y;
			vertex.z = -convexMeshVertices[convexMeshIndices[startIndexNumber + j]].z;

			vertex = Vector3::Transform(vertex, dxMatrix);

			vertices.push_back(vertex);
		}

		RENDER->AddDebugPolygon(vertices);
	}
}

void DebugTriangleMesh(physx::PxRigidActor* _body, physx::PxShape* _shape)
{
	const physx::PxTriangleMeshGeometry& triangleGeometry = static_cast<const physx::PxTriangleMeshGeometry&>(_shape->getGeometry());

	Matrix dxMatrix;
	CopyPxTransformToDirectXMatrix(_body->getGlobalPose(), dxMatrix);

	const physx::PxVec3* vertices = triangleGeometry.triangleMesh->getVertices();
	const void* indices = triangleGeometry.triangleMesh->getTriangles();

	if (triangleGeometry.triangleMesh->getTriangleMeshFlags() && physx::PxTriangleMeshFlag::e16_BIT_INDICES)
	{
		const unsigned short* indices16Bits = static_cast<const unsigned short*>(indices);

		for (int i = 0; i < triangleGeometry.triangleMesh->getNbTriangles() * 3; i += 3)
		{
			std::shared_ptr<unsigned int> index1 = std::make_shared<unsigned int>();
			std::shared_ptr<unsigned int> index2 = std::make_shared<unsigned int>();
			std::shared_ptr<unsigned int> index3 = std::make_shared<unsigned int>();
			*index1 = indices16Bits[i];
			*index2 = indices16Bits[i+1];
			*index3 = indices16Bits[i+2];

			RENDER->AddDebugTriangleIndex(index1);
			RENDER->AddDebugTriangleIndex(index2);
			RENDER->AddDebugTriangleIndex(index3);
		}
	}
	else
	{
		const unsigned int* indices32Bits = static_cast<const unsigned int*>(indices);

		for (int i = 0; i < triangleGeometry.triangleMesh->getNbTriangles() * 3; i += 3)
		{
			std::shared_ptr<unsigned int> index1 = std::make_shared<unsigned int>();
			std::shared_ptr<unsigned int> index2 = std::make_shared<unsigned int>();
			std::shared_ptr<unsigned int> index3 = std::make_shared<unsigned int>();
			*index1 = indices32Bits[i];
			*index2 = indices32Bits[i+1];
			*index3 = indices32Bits[i+2];

			RENDER->AddDebugTriangleIndex(index1);
			RENDER->AddDebugTriangleIndex(index2);
			RENDER->AddDebugTriangleIndex(index3);
		}
	}

	for (int i = 0; i < triangleGeometry.triangleMesh->getNbVertices(); i++)
	{
		std::shared_ptr<DirectX::SimpleMath::Vector3> vertex = std::make_shared<DirectX::SimpleMath::Vector3>();
		vertex->x = -vertices[i].x;
		vertex->y = -vertices[i].y;
		vertex->z = -vertices[i].z;

		RENDER->AddDebugTriangleVertex(vertex);
	}
}
