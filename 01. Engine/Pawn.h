#pragma once
#include "Object.h"

#include "CollisionComponent.h"

namespace Engine
{
	// 건재 : 클래스 선언
	class PawnController;

	// 건재 : 폰 클래스에 필요한 데이터 구조체
	struct PawnInfo
	{
		ObjectInfo m_ObjectDesc;
	};

	// 건재 : 폰 클래스 ( 간단한 조작을 할 수 있는 기본적인 틀을 가진 클래스. 수정 중 )
	class Pawn : public Object, public CollisionNotify
	{
	public:
		Pawn(unsigned int _id);
		virtual ~Pawn();

	public:
		void Setting(PawnInfo info);

	public:
		virtual void Init();
		virtual void Update();
		virtual void LateUpdate();
		virtual void FixedUpdate();
		virtual void Render();

	public:
		virtual void OnBeginOverlap(unsigned int _otherObjectid) override;
		virtual void OnEndOverlap(unsigned int _otherObjectid) override;
		virtual void OnOverlap(unsigned int _otherObjectid) override;
		virtual void OnBlock(unsigned int _otherObjectid) override;

	public:
		inline shared_ptr<PawnController> GetController();

		virtual void OnController(shared_ptr<PawnController> controller);
		virtual void UnController();

	private:
		shared_ptr<PawnController> m_pController;
	};

	shared_ptr<PawnController> Pawn::GetController()
	{
		return m_pController;
	}
}
