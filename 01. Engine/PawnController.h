#pragma once

namespace Engine
{
	// 건재 : 클래스 선언
	class Object;
	class Pawn;


	// -------------------------------------------------------------------------
	// 건재 : 폰 콘트롤러 클래스( 회전각 Pitch, Yaw 값 저장, 카메라 회전시 필요 )
	class PawnController : public enable_shared_from_this<PawnController>
	{
	public:
		PawnController();
		~PawnController();

	public:
		inline void SetPitch(float pitch);
		inline float GetPitch();
		inline void SetYaw(float yaw);
		inline float GetYaw();
		void AddPitch(float value);
		void AddYaw(float value);

		inline shared_ptr<Pawn> GetPawn();
		void SetPawn(shared_ptr<Object> object);

	private:
		weak_ptr<Pawn> m_pPawn;

		float m_Pitch = 0.f;
		float m_Yaw = 0.f;
	};

	// 건재 : PawnController의 Get & Set 함수
	void PawnController::SetPitch(float pitch)
	{
		m_Pitch = pitch;
	}
	float PawnController::GetPitch()
	{
		return m_Pitch;
	}
	void PawnController::SetYaw(float yaw)
	{
		m_Yaw = yaw;
	}
	float PawnController::GetYaw()
	{
		return m_Yaw;
	}

	shared_ptr<Pawn> PawnController::GetPawn()
	{
		return m_pPawn.lock();
	}
	// -------------------------------------------------------------------------
}