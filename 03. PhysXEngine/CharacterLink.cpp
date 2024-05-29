#include "CharacterLink.h"

#include "EngineDataConverter.h"
#include "CharacterJoint.h"

namespace physics
{
	CharacterLink::CharacterLink()
		: mName()
		, mDensity()
		, mLocalTransform()
		, mMyJoint(std::make_shared<CharacterJoint>())
		, mParentLink()
		, mMyChildrenLink()
		, mPxLink(nullptr)
	{
	}

	CharacterLink::~CharacterLink()
	{

	}

	bool CharacterLink::Initialize(const CharacterLinkInfo& info, std::shared_ptr<CharacterLink> parentLink, physx::PxArticulationReducedCoordinate* pxArticulation)
	{
		mName = info.boneName;
		mDensity = info.density;
		mLocalTransform = info.localTransform;
		mParentLink = parentLink;

		physx::PxTransform pxLocalTransform;
		CopyDirectXMatrixToPxTransform(mLocalTransform, pxLocalTransform);

		if (parentLink == nullptr)
			mPxLink = pxArticulation->createLink(nullptr, pxLocalTransform);
		else
		{
			mPxLink = pxArticulation->createLink(parentLink->GetPxLink(), pxLocalTransform);
			mMyJoint->Initialize(mParentLink.lock(), shared_from_this(), info.JointInfo);
		}

		return true;
	}

	physx::PxShape* CharacterLink::CreateShape(const physx::PxMaterial* material, std::shared_ptr<CollisionData> collisionData, const DirectX::SimpleMath::Vector3& extent)
	{
		physx::PxVec3 pxExtent;
		std::memcpy(&pxExtent, &extent, sizeof(pxExtent));

		physx::PxShape* shape = physx::PxRigidActorExt::createExclusiveShape(*mPxLink, physx::PxBoxGeometry(pxExtent), *material);
		physx::PxRigidBodyExt::updateMassAndInertia(*mPxLink, mDensity);

		shape->userData = collisionData.get();
		shape->setContactOffset(0.4f);
		return shape;
	}
	physx::PxShape* CharacterLink::CreateShape(const physx::PxMaterial* material, std::shared_ptr<CollisionData> collisionData, const float& radius, const float& halfHeight)
	{
		physx::PxShape* shape = physx::PxRigidActorExt::createExclusiveShape(*mPxLink, physx::PxCapsuleGeometry(radius, halfHeight), *material);
		physx::PxRigidBodyExt::updateMassAndInertia(*mPxLink, mDensity);

		shape->userData = collisionData.get();
		shape->setContactOffset(0.4f);
		return shape;
	}
	physx::PxShape* CharacterLink::CreateShape(const physx::PxMaterial* material, std::shared_ptr<CollisionData> collisionData, const float& radius)
	{
		physx::PxShape* shape = physx::PxRigidActorExt::createExclusiveShape(*mPxLink, physx::PxSphereGeometry(radius), *material);
		physx::PxRigidBodyExt::updateMassAndInertia(*mPxLink, mDensity);

		shape->userData = collisionData.get();
		shape->setContactOffset(0.4f);
		return shape;
	}
}
