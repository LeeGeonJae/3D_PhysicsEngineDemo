#include "PhysicsCloth.h"

namespace physics
{
	PhysicsCloth::PhysicsCloth(unsigned int id, unsigned int layerNumber)
		: mID()
		, mLayNumber()
		, mParticleNumberX()
		, mParticleNumberZ()
		, mPosition()
		, mParticleSpacing()
		, mTotalClothMass()
		, mParticleSystem(nullptr)
		, mClothBuffer(nullptr)
	{
	}

	PhysicsCloth::~PhysicsCloth()
	{
	}

	bool PhysicsCloth::Initialize(const PhysicsClothInfo& info, const physx::PxCudaContextManager* cudaContextManager)
	{
		if (cudaContextManager == nullptr)
			return false;




		return true;
	}

	physx::PxU32 PhysicsCloth::id(const physx::PxU32& x, const physx::PxU32& y, const physx::PxU32& numY)
	{
		return x * numY + y;
	}
}