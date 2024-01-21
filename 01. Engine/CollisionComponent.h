#pragma once
#include "SceneComponent.h"

namespace Engine
{
	// 건재 : 클래스 선언
	class CollisionComponent;

	// -------------------------------------------------------------------------
	// 건재 : 콜리전 노티파이 클래스( 인터페이스 )
	// 건재 : 이 클래스를 상속 받는 특정 오브젝트만 이 가상함수를 쓸 수 있으며, 콜리전 컴포넌트를 쓸 수 있습니다.
	// 건재 : 이 클래스를 상속 받은 오브젝트들은 콜리전 매니저에서 충돌 검사 후 컴포넌트에서 해당 가상 함수를 실행해줍니다.
	class CollisionNotify
	{
	public:
		virtual void OnBeginOverlap(unsigned int _otherObjectid) {}
		virtual void OnEndOverlap(unsigned int _otherObjectid) {}
		virtual void OnOverlap(unsigned int _otherObjectid) {}
		virtual void OnBlock(unsigned int _otherObjectid) {}
	};
	// -------------------------------------------------------------------------

	// 건재 : 콜라이더 타입 enum class ( 충돌 형체 )
	enum class ColliderType
	{
		Box = 0,
		Sphere,
		OrientedBox,
		End
	};

	// 건재 : 콜리전 타입 enum class ( 충돌 상태 )
	enum class CollisionType
	{
		NoCollision = 0,
		Overlap,
		Block,
		End
	};

	// 건재 : 콜리전 상태 enum class
	enum class CollisionState
	{
		None = 0,
		Overlap,
		Block,
		End
	};

	// 건재 : 콜리전 컴포넌트 정보 구조체
	struct CollisionComponentInfo
	{
		SceneComponentInfo m_SceneComponentInfo;
		CollisionNotify* m_CollisionNotify;
		CollisionType m_CollisionType;
	};

	// -------------------------------------------------------------------------
	// 건재 : 콜리전 컴포넌트 클래스
	class CollisionComponent : public SceneComponent
	{
	public:
		CollisionComponent(unsigned int _id);
		virtual ~CollisionComponent();

	public:
		virtual void Setting(CollisionComponentInfo _info);

	public:
		virtual void Update();

	public:
		void ProcessOverlap(unsigned int _otherCollisionID);
		void ProcessBlock(unsigned int _otherCollisionID);
		void ProcessEscape(unsigned int _otherCollisionID);
		virtual bool IsCollision(unsigned int _otherCollisionID) abstract;

	public:
		inline unsigned int GetID();
		inline const ColliderType& GetColliderType();
		inline void SetColliderType(ColliderType _colliderType);
		inline const CollisionType& GetCollisionType();
		inline void SetCollisionType(CollisionType _collisionType);
		inline const CollisionState& GetCollisionState();
		inline void SetCollisionState(CollisionState _collisionState);
		inline void SetCollisionNotify(CollisionNotify* _collisionNotify);

	private:
		unsigned int m_ID;
		vector<unsigned int> m_otherCollisionID;
		CollisionNotify* m_CollisionNotify;

		ColliderType m_ColliderType = ColliderType::End;
		CollisionType m_CollisionType = CollisionType::End;
		CollisionState m_CollisionStateCurrent = CollisionState::None;

		friend class CollisionManager;
	};

	// 건재 : CollisionComponent의 Get & Set 함수
	unsigned int CollisionComponent::GetID()
	{
		return m_ID;
	}
	const ColliderType& CollisionComponent::GetColliderType()
	{
		return m_ColliderType;
	}
	void CollisionComponent::SetColliderType(ColliderType _colliderType)
	{
		m_ColliderType = _colliderType;
	}
	const CollisionType& CollisionComponent::GetCollisionType()
	{
		return m_CollisionType;
	}
	void CollisionComponent::SetCollisionType(CollisionType _collisionType)
	{
		m_CollisionType = _collisionType;
	}
	const CollisionState& CollisionComponent::GetCollisionState()
	{
		return m_CollisionStateCurrent;
	}
	void CollisionComponent::SetCollisionState(CollisionState _collisionState)
	{
		m_CollisionStateCurrent = _collisionState;
	}
	void CollisionComponent::SetCollisionNotify(CollisionNotify* _collisionNotify)
	{
		m_CollisionNotify = _collisionNotify;
	}
	// -------------------------------------------------------------------------
}
