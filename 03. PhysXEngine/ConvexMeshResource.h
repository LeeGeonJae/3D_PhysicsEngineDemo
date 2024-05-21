#pragma once
#include "ResourceBase.h"

#include "physx/PxPhysicsAPI.h"

namespace physics
{
    class ConvexMeshResource : public ResourceBase
    {
    public:
        ConvexMeshResource();
        virtual ~ConvexMeshResource();

    private:
        physx::PxConvexMesh* mConvexMesh;
    };
}

