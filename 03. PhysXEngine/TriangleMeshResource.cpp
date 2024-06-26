#include "TriangleMeshResource.h"

#include "PhysicsCookingMeshTool.h"

namespace physics
{
	TriangleMeshResource::TriangleMeshResource(
		physx::PxPhysics* physics, 
		const physx::PxVec3* vertices,
		const unsigned int& vertexSize, 
		const unsigned int* indices, 
		const unsigned int& indexSize)
		: ResourceBase(EResourceType::TRIANGLE_MESH)
	{
		mTriangleMesh = PhysicsCookingMeshTool::CookingTriangleMesh(physics, vertices, vertexSize, indices, indexSize);
	}

	TriangleMeshResource::~TriangleMeshResource()
	{
		PX_RELEASE(mTriangleMesh);
	}
}
