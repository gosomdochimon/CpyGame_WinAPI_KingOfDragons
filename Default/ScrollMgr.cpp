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

void CScrollMgr::Scroll_Lock(STAGE _CurStage) //�Ű������� STAGE�� �ִ� X Y�� �ޱ�
{

	//Stage���� Stage�� ��ũ�� �� �ϱ�.
	switch (_CurStage)
	{
	case STAGE_TUTORIAL:
		if (0.f < m_fScrollX)
			m_fScrollX = 0.f;
		//���ִ밪 ��ŭ ����
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
		//���ִ밪 ��ŭ ����
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
		//���ִ밪 ��ŭ ����
		if (WINCX - 2000 > m_fScrollX)
			m_fScrollX = WINCX - 2000;


		if (0.f < m_fScrollY)
			m_fScrollY = 0.f;

		if (WINCY - 1280 > m_fScrollY)
			m_fScrollY = WINCY - 1280;
		break;

	}
	

}
