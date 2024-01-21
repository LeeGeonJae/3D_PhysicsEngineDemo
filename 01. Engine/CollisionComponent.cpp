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
		// 건재 : 정보 구조체에 따라 멤버 함수 세팅
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

		// 건재 : 현재 충돌 중인 콜리전 아이디를 찾고 찾으면 OnOverlap함수 실행하고 종료
		for (auto id : m_otherCollisionID)
		{
			if (_otherCollisionID == id)
			{
				auto otherCollision = CollisionManager::GetInstance()->FindCollision(_otherCollisionID);
				m_CollisionNotify->OnOverlap(otherCollision->GetMyObject()->GetID());
				return;
			}
		}

		// 건재 : 충돌 중인 콜리전 아이디가 없으면 BeginOverlap 함수 실행
		m_otherCollisionID.push_back(_otherCollisionID);
		auto otherCollision = CollisionManager::GetInstance()->FindCollision(_otherCollisionID);
		m_CollisionNotify->OnBeginOverlap(otherCollision->GetMyObject()->GetID());
	}

	void CollisionComponent::ProcessBlock(unsigned int _otherCollisionID)
	{
		// 건재 : 현재 충돌 중이라면 현재 상태를 Block 처리하고 OnBlock함수 실행
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
			// 건재 : 충돌중인 콜리전이 제거 되었을 때 EndOverlap 함수 실행
			if (_otherCollisionID == m_otherCollisionID[i])
			{
				m_otherCollisionID.erase(m_otherCollisionID.begin() + i);
				m_CollisionNotify->OnEndOverlap(otherCollision->GetMyObject()->GetID());
			}
		}

		// 건재 : 충돌중인 콜리전이 없을 경우 현재 상태 None
		if (m_otherCollisionID.empty())
		{
			m_CollisionStateCurrent = CollisionState::None;
		}
	}
}
