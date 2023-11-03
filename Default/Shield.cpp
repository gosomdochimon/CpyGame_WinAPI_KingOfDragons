#include "stdafx.h"
#include "Shield.h"
#include "BmpMgr.h"
#include "ScrollMgr.h"
CShield::CShield()
{
}


CShield::~CShield()
{
	Release();
}

void CShield::Initialize(void)
{	//Shield
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/Fighter_Shield_Left.bmp", L"Shield_LEFT");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/Fighter_Shield_Right.bmp", L"Shield_RIGHT");
	m_tInfo.fCX = 30.f;
	m_tInfo.fCY = 35.f;


	ShieldLv = 0;
	m_eDir = DIR_RIGHT;
	m_pFrameKey = L"Shield_RIGHT";
	m_tFrame.dwFrameTime = GetTickCount();
	m_tFrame.dwFrameSpeed = 200;
	m_tFrame.iFrameEnd = 0;
	m_tFrame.iFrameStart = 0;
	m_tFrame.iMotion = ShieldLv;
	m_CurShield = SHIELD_1;
	m_PreShield = SHIELD_END;
}

int CShield::Update(void)
{


	Update_Rect();

	return OBJ_NOEVENT;
}

void CShield::Late_Update(void)
{
	Motion_Change();
	Move_Frame();
}

void CShield::Render(HDC hDC)
{
	int iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	HDC	hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);

	GdiTransparentBlt(hDC, 					// 복사 받을, 최종적으로 그림을 그릴 DC
		int(m_tRect.left + iScrollX),	// 2,3 인자 :  복사받을 위치 X, Y
		int(m_tRect.top + iScrollY),
		int(m_tInfo.fCX),				// 4,5 인자 : 복사받을 가로, 세로 길이
		int(m_tInfo.fCY),
		hMemDC,							// 비트맵을 가지고 있는 DC
		m_tFrame.iFrameStart * (int)m_tInfo.fCX,								// 비트맵 출력 시작 좌표, X,Y
		m_tFrame.iMotion * (int)m_tInfo.fCY,
		(int)m_tInfo.fCX,				// 복사할 비트맵의 가로, 세로 길이
		(int)m_tInfo.fCY,
		RGB(255, 0, 255));			// 제거하고자 하는 색상
}

void CShield::Release(void)
{

}

void CShield::Motion_Change(void)
{
	if (m_eDir == DIR_RIGHT)
	{
		m_pFrameKey = L"Shield_RIGHT";
	}
	else if (m_eDir == DIR_LEFT)
	{
		m_pFrameKey = L"Shield_LEFT";
	}
	if (m_PlayerPreState != m_PlayerCurState || m_CurShield != m_PreShield)
	{
		switch (m_PlayerCurState)
		{
		case CPlayer::IDLE:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 0;
			m_tFrame.iMotion = int(m_CurShield);

			m_tFrame.dwFrameSpeed = 200;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		case CPlayer::WALK:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 0;
			m_tFrame.iMotion = int(m_CurShield);

			m_tFrame.dwFrameSpeed = 200;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		case CPlayer::BLOCKING:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 0;
			m_tFrame.iMotion = int(m_CurShield);

			m_tFrame.dwFrameSpeed = 200;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		case CPlayer::DAMAGED:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 2;
			m_tFrame.iMotion = int(m_CurShield);

			m_tFrame.dwFrameSpeed = 200;
			m_tFrame.dwFrameTime = GetTickCount();
		case CPlayer::ATTACK:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 2;
			m_tFrame.iMotion = int(m_CurShield);

			m_tFrame.dwFrameSpeed = 100;
			m_tFrame.dwFrameTime = GetTickCount();

		//default:
		//	m_tFrame.iFrameStart = 0;
		//	m_tFrame.iFrameEnd = 0;
		//	m_tFrame.iMotion = ShieldLv;

		//	//m_tFrame.dwFrameSpeed = 200;
		//	m_tFrame.dwFrameTime = GetTickCount();
		//	break;

		}
		Shield_Change();
		m_PlayerPreState = m_PlayerCurState;
	}
	

}

void CShield::Shield_Change(void)
{
	if (m_CurShield != m_PreShield)
	{
		m_PreShield = m_CurShield;
		/*switch (m_CurShield)
		{
		case SHIELD_1:
			m_CurShield = SHIELD_1;
			break;
		case SHIELD_2:
			m_CurShield = SHIELD_2;
			break;
		case SHIELD_3:
			m_CurShield = SHIELD_3;
			break;
		}*/
	}
	
}

