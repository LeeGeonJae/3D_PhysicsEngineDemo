#pragma once

namespace Engine
{
	// -------------------------------------------------------------------------
	// 건재 : 타임 매니저 클래스 ( 싱글톤 )
	class TimeManager
	{
	public:
		TimeManager();
		~TimeManager();

	public:
		void Initialize();
		void Update();

	public:
		double GetDT() { return m_dDT; }
		float GetfDT() { return (float)m_dDT; }

	private:
		LARGE_INTEGER	m_llcurCount;
		LARGE_INTEGER	m_llPrevCount;
		LARGE_INTEGER	m_llFrequency;

		double			m_dDT;	// 건재 : 프레임 사이의 시간값
		double			m_dAcc;	// 건재 : 1초 체크를 위한 누적 시간
		UINT			m_iCallCount;	// 건재 : 함수 호출 횟수 체크
		UINT			m_iFPS;		// 건재 : 초당 호출 횟수
	};
	// -------------------------------------------------------------------------
}