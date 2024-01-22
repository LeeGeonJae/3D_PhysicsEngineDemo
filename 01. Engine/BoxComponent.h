#pragma once
#include "CollisionComponent.h"

namespace Engine
{
	// 박스 컴포넌트 데이터 구조체
	struct BoxComponentInfo
	{
		CollisionComponentInfo m_CollisionComponentInfo;
		DirectX::BoundingBox m_BoundingBox;
	};

	// 박스 컴포넌트 클래스
	class BoxComponent : public CollisionComponent
	{
	public:
		BoxComponent(unsigned int _id);
		virtual ~BoxComponent();

	public:
		virtual void Update(float _deltaTime);

	public:
		void Setting(BoxComponentInfo _info);

	public:
		virtual bool IsCollision(unsigned int _otherCollisionID);

	public:
		inline const DirectX::BoundingBox& GetGeometry();

	private:
		DirectX::BoundingBox m_Geometry;
	};

	const DirectX::BoundingBox& BoxComponent::GetGeometry()
	{
		return m_Geometry;
	}
}
