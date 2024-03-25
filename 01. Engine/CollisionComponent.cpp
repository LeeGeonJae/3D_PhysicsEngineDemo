#include "pch.h"
#include "CollisionComponent.h"

#include "CollisionManager.h"
#include "ObjectManager.h"

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

	void CollisionComponent::Update(float _deltaTime)
	{
		__super::Update(_deltaTime);

		// 건재 : 해당 콜리전을 가지고 있는 오브젝트가 삭제 예정이라면 제거
		for (const auto& otherCollisionID : m_otherCollisionID)
		{
			// 건재 : 충돌중인 콜리전이 제거 되었다면 콜리전 매니저에서 관리해주던 콜리전 삭제
			auto collision = CollisionManager::GetInstance()->FindCollision(otherCollisionID);

			if (collision->GetIsDead())
			{
				m_otherCollisionID.erase(otherCollisionID);
				CollisionManager::GetInstance()->DeleteCollision(otherCollisionID);
			}
		}
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
				m_CollisionNotify->OnOverlap(otherCollision->GetMyObject().lock()->GetID());
				return;
			}
		}

		// 건재 : 충돌 중인 콜리전 아이디가 없으면 BeginOverlap 함수 실행
		m_otherCollisionID.insert(_otherCollisionID);
		auto otherCollision = CollisionManager::GetInstance()->FindCollision(_otherCollisionID);
		m_CollisionNotify->OnBeginOverlap(otherCollision->GetMyObject().lock()->GetID());
	}

	void CollisionComponent::ProcessBlock(unsigned int _otherCollisionID)
	{
		// 건재 : 현재 충돌 중이라면 현재 상태를 Block 처리하고 OnBlock함수 실행
		assert(m_CollisionNotify != nullptr);
		m_CollisionStateCurrent = CollisionState::Block;

		auto otherCollision = CollisionManager::GetInstance()->FindCollision(_otherCollisionID);
		m_CollisionNotify->OnBlock(otherCollision->GetMyObject().lock()->GetID());
	}

	void CollisionComponent::ProcessEscape(unsigned int _otherCollisionID)
	{
		assert(m_CollisionNotify != nullptr);

		auto otherCollision = CollisionManager::GetInstance()->FindCollision(_otherCollisionID);

		for (const auto& otherCollisionID : m_otherCollisionID)
		{
			// 건재 : 충돌중인 콜리전이 제거 되었을 때 EndOverlap 함수 실행
			if (otherCollisionID == _otherCollisionID)
			{
				m_otherCollisionID.erase(otherCollisionID);
				m_CollisionNotify->OnEndOverlap(otherCollision->GetMyObject().lock()->GetID());
			}
		}

		// 건재 : 충돌중인 콜리전이 없을 경우 현재 상태 None
		if (m_otherCollisionID.empty())
		{
			m_CollisionStateCurrent = CollisionState::None;
		}
	}
}
