#include "pch.h"
#include "CollisionManager.h"

#include "CollisionComponent.h"
#include "ObjectManager.h"
#include "Object.h"

namespace Engine
{
	void CollisionManager::Init()
	{

	}

	// 건재 : 콜리전 충돌 관리
	void CollisionManager::FixedUpdate()
	{
		// 건재 : 두 콜리전을 찾고 for문을 돌려 모든 콜리전 검사
		for (auto lhsIter = m_CollisionMap.begin(); lhsIter != m_CollisionMap.end(); lhsIter++)
		{
			shared_ptr<CollisionComponent> lhsCollsion = lhsIter->second.lock();

			auto rhsIter = lhsIter;
			++rhsIter;

			for (; rhsIter != m_CollisionMap.end(); rhsIter++)
			{
				shared_ptr<CollisionComponent> rhsCollsion = rhsIter->second.lock();

				if (lhsCollsion->GetCollisionType() == CollisionType::Block && rhsCollsion->GetCollisionType() == CollisionType::Block)
				{
					// 건재 : 콜리전이 충돌 되는지 검사
					if (lhsCollsion->IsCollision(rhsCollsion->GetID()))
					{
						lhsCollsion->ProcessBlock(rhsCollsion->GetID());
						rhsCollsion->ProcessBlock(lhsCollsion->GetID());
					}
					else
					{
						lhsCollsion->SetCollisionState(CollisionState::None);
						rhsCollsion->SetCollisionState(CollisionState::None);
					}
				}
				else
				{
					// 건재 : 콜리전이 충돌 되는지 검사
					if (lhsCollsion->IsCollision(rhsCollsion->GetID()))
					{
						lhsCollsion->ProcessOverlap(rhsCollsion->GetID());
						rhsCollsion->ProcessOverlap(lhsCollsion->GetID());
					}
					else
					{
						lhsCollsion->ProcessEscape(rhsCollsion->GetID());
						rhsCollsion->ProcessEscape(lhsCollsion->GetID());
					}
				}
			}
		}
	}
}
