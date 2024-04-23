#pragma once

void CopyPxTransformToDirectXMatrix(const physx::PxTransform& pxTransform, Matrix& dxMatrix);
void CopyDirectXMatrixToPxTransform(const DirectX::XMMATRIX& dxMatrix, physx::PxTransform& pxTransform);
void DebugCapsule(physx::PxRigidActor* _body, physx::PxShape* _shape);
void DebugBox(physx::PxRigidActor* _body, physx::PxShape* _shape);
void DebugSphere(physx::PxRigidActor* _body, physx::PxShape* _shape);
void DebugConvexMesh(physx::PxRigidActor* _body, physx::PxShape* _shape);
