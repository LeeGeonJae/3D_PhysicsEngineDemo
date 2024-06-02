#pragma once
#include "ResourceBase.h"

#include <memory>

#include "physx/PxPhysicsAPI.h"
#include "directxtk\SimpleMath.h"

namespace physics
{
    class PhysicsCookingMeshTool;

    class ConvexMeshResource : public ResourceBase
    {
    public:
        ConvexMeshResource(physx::PxPhysics* physics, 
            DirectX::SimpleMath::Vector3* vertices, 
            int vertexSize, 
            int polygonLimit);
        virtual ~ConvexMeshResource();

        inline physx::PxConvexMesh* GetConvexMesh();

    private:
        physx::PxConvexMesh* mConvexMesh;
    };

    physx::PxConvexMesh* ConvexMeshResource::GetConvexMesh()
    {
        return mConvexMesh;
    }
}
