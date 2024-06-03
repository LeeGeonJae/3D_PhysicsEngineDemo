#include "pch.h"
#include "MeshGenerator.h"

void createCube(physx::PxArray<physx::PxVec3>& triVerts, physx::PxArray<physx::PxU32>& triIndices, const physx::PxVec3& pos, physx::PxReal scaling)
{
	triVerts.clear();
	triIndices.clear();
	triVerts.pushBack(scaling * physx::PxVec3(0.5f, -0.5f, -0.5f) + pos);
	triVerts.pushBack(scaling * physx::PxVec3(0.5f, -0.5f, 0.5f) + pos);
	triVerts.pushBack(scaling * physx::PxVec3(-0.5f, -0.5f, 0.5f) + pos);
	triVerts.pushBack(scaling * physx::PxVec3(-0.5f, -0.5f, -0.5f) + pos);
	triVerts.pushBack(scaling * physx::PxVec3(0.5f, 0.5f, -0.5f) + pos);
	triVerts.pushBack(scaling * physx::PxVec3(0.5f, 0.5f, 0.5f) + pos);
	triVerts.pushBack(scaling * physx::PxVec3(-0.5f, 0.5f, 0.5f) + pos);
	triVerts.pushBack(scaling * physx::PxVec3(-0.5f, 0.5f, -0.5f) + pos);

	triIndices.pushBack(1); triIndices.pushBack(2); triIndices.pushBack(3);
	triIndices.pushBack(7); triIndices.pushBack(6); triIndices.pushBack(5);
	triIndices.pushBack(4); triIndices.pushBack(5); triIndices.pushBack(1);
	triIndices.pushBack(5); triIndices.pushBack(6); triIndices.pushBack(2);

	triIndices.pushBack(2); triIndices.pushBack(6); triIndices.pushBack(7);
	triIndices.pushBack(0); triIndices.pushBack(3); triIndices.pushBack(7);
	triIndices.pushBack(0); triIndices.pushBack(1); triIndices.pushBack(3);
	triIndices.pushBack(4); triIndices.pushBack(7); triIndices.pushBack(5);

	triIndices.pushBack(0); triIndices.pushBack(4); triIndices.pushBack(1);
	triIndices.pushBack(1); triIndices.pushBack(5); triIndices.pushBack(2);
	triIndices.pushBack(3); triIndices.pushBack(2); triIndices.pushBack(7);
	triIndices.pushBack(4); triIndices.pushBack(0); triIndices.pushBack(7);
}