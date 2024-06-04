#include "pch.h"
#include "TimeManager.h"

namespace Engine
{
	TimeManager::TimeManager()
	{
	}
	TimeManager::~TimeManager()
	{
	}
	void TimeManager::Initialize()
	{
		// 건재 : 현재 카운트
		QueryPerformanceCounter(&m_llPrevCount);

		// 건재 : 초당 카운트 횟수 (천만)
		QueryPerformanceFrequency(&m_llFrequency);
	}

	void TimeManager::Update()
	{
		QueryPerformanceCounter(&m_llcurCount);

		// 건재 : 이전 프레임의 카운팅과 현재 프레임 카운팅 값의 차이를 구한다.
		m_dDT = (double)(m_llcurCount.QuadPart - m_llPrevCount.QuadPart) / (double)m_llFrequency.QuadPart;

		// 건재 : 이전카운트 값을 현재값으로 갱신 (다음번에 계산을 위해서)
		m_llPrevCount = m_llcurCount;

//		// 건재 : 만약에 중단점을 찍고 검사하는 디버깅 상태라면 시간이 흐르는 것을 방지하기 위해 만들어둔 전처리기
//#ifdef _DEBUG
//		if (m_dDT > (1. / 60.))
//			m_dDT = (1. / 60.);
//#endif
	}
}