#include "pch.h"
#include "CollisionComponent.h"

#include "CollisionManager.h"
#include "Object.h"

namespace Engine
{
	CollisionComponent::CollisionComponent(unsigned int _id)
		: m_ID(_id)
	{
		m_ColliderType = ColliderType::End;
		m_CollisionType = CollisionType::NoCollision;
	}

	CollisionComponent::~CollisionComponent()
	{
	}

	void CollisionComponent::Setting(CollisionComponentInfo _info)
	{
		// ���� : ���� ����ü�� ���� ��� �Լ� ����
		__super::Setting(_info.m_SceneComponentInfo);
		m_CollisionType = _info.m_CollisionType;
		m_CollisionNotify = _info.m_CollisionNotify;
	}

	void CollisionComponent::Update()
	{
		__super::Update();
	}

	void CollisionComponent::ProcessOverlap(unsigned int _otherCollisionID)
	{
		assert(m_CollisionNotify != nullptr);
		m_CollisionStateCurrent = CollisionState::Overlap;

		// ���� : ���� �浹 ���� �ݸ��� ���̵� ã�� ã���� OnOverlap�Լ� �����ϰ� ����
		for (auto id : m_otherCollisionID)
		{
			if (_otherCollisionID == id)
			{
				auto otherCollision = CollisionManager::GetInstance()->FindCollision(_otherCollisionID);
				m_CollisionNotify->OnOverlap(otherCollision->GetMyObject()->GetID());
				return;
			}
		}

		// ���� : �浹 ���� �ݸ��� ���̵� ������ BeginOverlap �Լ� ����
		m_otherCollisionID.push_back(_otherCollisionID);
		auto otherCollision = CollisionManager::GetInstance()->FindCollision(_otherCollisionID);
		m_CollisionNotify->OnBeginOverlap(otherCollision->GetMyObject()->GetID());
	}

	void CollisionComponent::ProcessBlock(unsigned int _otherCollisionID)
	{
		// ���� : ���� �浹 ���̶�� ���� ���¸� Block ó���ϰ� OnBlock�Լ� ����
		assert(m_CollisionNotify != nullptr);
		m_CollisionStateCurrent = CollisionState::Block;

		auto otherCollision = CollisionManager::GetInstance()->FindCollision(_otherCollisionID);
		m_CollisionNotify->OnBlock(otherCollision->GetMyObject()->GetID());
	}

	void CollisionComponent::ProcessEscape(unsigned int _otherCollisionID)
	{
		assert(m_CollisionNotify != nullptr);

		auto otherCollision = CollisionManager::GetInstance()->FindCollision(_otherCollisionID);

		for (int i = 0; i < m_otherCollisionID.size(); i++)
		{
			// ���� : �浹���� �ݸ����� ���� �Ǿ��� �� EndOverlap �Լ� ����
			if (_otherCollisionID == m_otherCollisionID[i])
			{
				m_otherCollisionID.erase(m_otherCollisionID.begin() + i);
				m_CollisionNotify->OnEndOverlap(otherCollision->GetMyObject()->GetID());
			}
		}

		// ���� : �浹���� �ݸ����� ���� ��� ���� ���� None
		if (m_otherCollisionID.empty())
		{
			m_CollisionStateCurrent = CollisionState::None;
		}
	}
}
