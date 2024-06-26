#pragma once

#include "ResourceBase.h"

#include <memory>

#include <PxPhysicsAPI.h>
#include "directxtk\SimpleMath.h"

namespace physics
{
	class TriangleMeshResource : public ResourceBase
	{
	public:
		TriangleMeshResource(
			physx::PxPhysics* physics, 
			const physx::PxVec3* vertices,
			const unsigned int& vertexSize, 
			const unsigned int* indices, 
			const unsigned int& indexSize);
		virtual ~TriangleMeshResource();

		inline physx::PxTriangleMesh* GetTriangleMesh();

	private:
		physx::PxTriangleMesh* mTriangleMesh;
	};

	physx::PxTriangleMesh* TriangleMeshResource::GetTriangleMesh()
	{
		return mTriangleMesh;
	}
}