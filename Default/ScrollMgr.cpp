#include "stdafx.h"
#include "ScrollMgr.h"

CScrollMgr*		CScrollMgr::m_pInstance = nullptr;

CScrollMgr::CScrollMgr()
	: m_fScrollX(0.f), m_fScrollY(0.f)
{
}


CScrollMgr::~CScrollMgr()
{
}

void CScrollMgr::Scroll_Lock(STAGE _CurStage) //매개변수를 STAGE별 최대 X Y값 받기
{

	//Stage마다 Stage별 스크롤 락 하기.
	switch (_CurStage)
	{
	case STAGE_TUTORIAL:
		if (0.f < m_fScrollX)
			m_fScrollX = 0.f;
		//맵최대값 만큼 빼기
		if (WINCX - 1533 > m_fScrollX)
			m_fScrollX = WINCX - 1533;


		if (0.f < m_fScrollY)
			m_fScrollY = 0.f;

		if (WINCY - 1280 > m_fScrollY)
			m_fScrollY = WINCY - 1280;
		break;
	case STAGE_1:
		if (0.f < m_fScrollX)
			m_fScrollX = 0.f;
		//맵최대값 만큼 빼기
		if (WINCX - 1533 > m_fScrollX)
			m_fScrollX = WINCX -1533;


		if (0.f < m_fScrollY)
			m_fScrollY = 0.f;

		if (WINCY - 1280 > m_fScrollY)
			m_fScrollY = WINCY - 1280;
		break;
	case STAGE_2:
		if (0.f < m_fScrollX)
			m_fScrollX = 0.f;
		//맵최대값 만큼 빼기
		if (WINCX - 2000 > m_fScrollX)
			m_fScrollX = WINCX - 2000;


		if (0.f < m_fScrollY)
			m_fScrollY = 0.f;

		if (WINCY - 1280 > m_fScrollY)
			m_fScrollY = WINCY - 1280;
		break;

	}
	

}
