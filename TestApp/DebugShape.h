#pragma once

void CopyPxTransformToDirectXMatrix(const physx::PxTransform& pxTransform, Matrix& dxMatrix);
void CopyDirectXMatrixToPxTransform(const DirectX::XMMATRIX& dxMatrix, physx::PxTransform& pxTransform);
void DebugCapsule(physx::PxRigidBody* _body, physx::PxShape* _shape);
void DebugBox(physx::PxRigidBody* _body, physx::PxShape* _shape);
void DebugSphere(physx::PxRigidBody* _body, physx::PxShape* _shape);
void DebugConvexMesh(physx::PxRigidBody* _body, physx::PxShape* _shape);
