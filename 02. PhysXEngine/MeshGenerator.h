#pragma once

#include <physx/PxPhysicsAPI.h>

void createCube(physx::PxArray<physx::PxVec3>& triVerts, physx::PxArray<physx::PxU32>& triIndices, const physx::PxVec3& pos, physx::PxReal scaling);