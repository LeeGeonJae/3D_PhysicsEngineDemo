#include "pch.h"
#include "DebugShape.h"

#include "../02. GraphicsEngine/RenderManager.h"

// 오른손 좌표계(PhysX)에서 왼손 좌표계(DirectX11)로 변환하기
void CopyPxTransformToDirectXMatrix(const physx::PxTransform& pxTransform, Matrix& dxMatrix)
{
	Vector3 translation = Vector3(pxTransform.p.x, pxTransform.p.y, -pxTransform.p.z);
	Quaternion rotation = Quaternion(pxTransform.q.x, pxTransform.q.y, pxTransform.q.z, pxTransform.q.w);

	// z축 방향 반전
	DirectX::XMVECTOR flippedZ = DirectX::XMVectorNegate(DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f));
	rotation = DirectX::XMQuaternionMultiply(rotation, flippedZ);

	dxMatrix = DirectX::XMMatrixAffineTransformation(DirectX::XMVectorSet(1.0f, 1.0f, 1.0f, 0.0f), DirectX::XMVectorZero(), rotation, translation);
}

// 왼손 좌표계(DirectX11)에서 오른손 좌표계(PhysX)로 변환하기
void CopyDirectXMatrixToPxTransform(const DirectX::XMMATRIX& dxMatrix, physx::PxTransform& pxTransform)
{
	DirectX::XMFLOAT4X4 dxMatrixData;
	DirectX::XMStoreFloat4x4(&dxMatrixData, dxMatrix);

	pxTransform.p.x = dxMatrixData._41;
	pxTransform.p.y = dxMatrixData._42;
	pxTransform.p.z = -dxMatrixData._43; // z축 방향 반전

	// 회전 정보에서 z축 방향 반전 적용
	DirectX::XMVECTOR quaternion = DirectX::XMQuaternionRotationMatrix(dxMatrix);
	DirectX::XMVECTOR flippedZ = DirectX::XMQuaternionRotationAxis(DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), DirectX::XM_PI);
	quaternion = DirectX::XMQuaternionMultiply(quaternion, flippedZ);

	pxTransform.q.x = DirectX::XMVectorGetX(quaternion);
	pxTransform.q.y = DirectX::XMVectorGetY(quaternion);
	pxTransform.q.z = DirectX::XMVectorGetZ(quaternion);
	pxTransform.q.w = DirectX::XMVectorGetW(quaternion);
}

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

	Matrix Sphere0Matrix = dxMatrix * Matrix::CreateTranslation( capsuleGeometry.radius * dxMatrix.Right());
	Matrix Sphere1Matrix = dxMatrix * Matrix::CreateTranslation( -capsuleGeometry.radius * dxMatrix.Right());

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

// 
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
	const physx::PxU32 indexCount = convexMeshGeometry.convexMesh->getNbPolygons() * 5;

	for (int i = 0; i < indexCount; i += 3)
	{
		vector<Vector3> vertices(3);
		physx::PxU32 index0 = convexMeshIndices[i];
		physx::PxU32 index1 = convexMeshIndices[i + 1];
		physx::PxU32 index2 = convexMeshIndices[i + 2];

		vertices[0].x = -convexMeshVertices[convexMeshIndices[index0]].x;
		vertices[0].y = -convexMeshVertices[convexMeshIndices[index0]].y;
		vertices[0].z = -convexMeshVertices[convexMeshIndices[index0]].z;

		vertices[1].x = -convexMeshVertices[convexMeshIndices[index1]].x;
		vertices[1].y = -convexMeshVertices[convexMeshIndices[index1]].y;
		vertices[1].z = -convexMeshVertices[convexMeshIndices[index1]].z;

		vertices[2].x = -convexMeshVertices[convexMeshIndices[index2]].x;
		vertices[2].y = -convexMeshVertices[convexMeshIndices[index2]].y;
		vertices[2].z = -convexMeshVertices[convexMeshIndices[index2]].z;

		vertices[0] = Vector3::Transform(vertices[0], dxMatrix);
		vertices[1] = Vector3::Transform(vertices[1], dxMatrix);
		vertices[2] = Vector3::Transform(vertices[2], dxMatrix);

		RENDER->AddDebugTriangle(vertices);
	}
}
