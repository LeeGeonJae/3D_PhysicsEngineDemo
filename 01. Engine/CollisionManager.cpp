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

	// ���� : �ݸ��� �浹 ����
	void CollisionManager::FixedUpdate()
	{
		// ���� : �� �ݸ����� ã�� for���� ���� ��� �ݸ��� �˻�
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
					// ���� : �ݸ����� �浹 �Ǵ��� �˻�
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
					// ���� : �ݸ����� �浹 �Ǵ��� �˻�
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
