#include "CharacterPhysics.h"

#include "CharacterLink.h"
#include "EngineDataConverter.h"

namespace physics
{
	CharacterPhysics::CharacterPhysics()
		: mPxArticulation(nullptr)
		, mMaterial(nullptr)
		, mRootLink(std::make_shared<CharacterLink>())
		, mModelPath()
		, mID()
		, mLayerNumber()
		, mLinkContainer()
		, mWorldTransform()
	{
	}

	CharacterPhysics::~CharacterPhysics()
	{
	}

	bool CharacterPhysics::Initialize(const CharacterPhysicsInfo& info, physx::PxPhysics* physics)
	{
		mPxArticulation = physics->createArticulationReducedCoordinate();
		mPxArticulation->setArticulationFlag(physx::PxArticulationFlag::eFIX_BASE, false);
		mPxArticulation->setArticulationFlag(physx::PxArticulationFlag::eDISABLE_SELF_COLLISION, false);
		mPxArticulation->setSolverIterationCounts(2);
		mPxArticulation->setMaxCOMAngularVelocity(10000.f);

		mMaterial = physics->createMaterial(info.staticFriction, info.dynamicFriction, info.restitution);
		mModelPath = info.modelPath;
		mID = info.id;
		mLayerNumber = info.layerNumber;
		mWorldTransform = info.worldTransform;

		CharacterLinkInfo linkInfo;
		std::string str = "root";
		linkInfo.boneName = "root";
		linkInfo.parentBoneName = "";
		linkInfo.localTransform = mWorldTransform;
		mRootLink->Initialize(linkInfo, nullptr, mPxArticulation);

		return true;
	}
}

