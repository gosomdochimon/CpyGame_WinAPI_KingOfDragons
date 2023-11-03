#include "stdafx.h"
#include "Weapon.h"
#include "ScrollMgr.h"
#include "BmpMgr.h"

CWeapon::CWeapon()
{
}


CWeapon::~CWeapon()
{
}

void CWeapon::Initialize(void)
{
	m_eDir = DIR_RIGHT;
	m_tInfo = { 300.f, 300.f, 80.f, 112.f };
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/Fighter_Sword_Left.bmp", L"Sword_LEFT");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/Fighter_Sword_Right.bmp", L"Sword_RIGHT");

	m_CurSword = SWORD_1;
	m_PreSword = SWORD_END;
	SwordLv = 0;
	m_iAtk = 10;

	m_pFrameKey = L"Sword_RIGHT";
	m_tFrame.dwFrameTime = GetTickCount();
	m_tFrame.dwFrameSpeed = 200;
	m_tFrame.iFrameEnd = 0;
	m_tFrame.iFrameStart = 0;
	m_tFrame.iMotion = SwordLv;

}

int CWeapon::Update(void)
{
	Sword_Change();
	Update_Rect();
	return 0;
}

void CWeapon::Late_Update(void)
{
	Motion_Change();
	Sword_Change();
	Move_Frame();
}

void CWeapon::Render(HDC hDC)
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

void CWeapon::Release(void)
{
}

void CWeapon::Motion_Change(void)
{
	if (m_eDir == DIR_RIGHT)
	{
		m_pFrameKey = L"Sword_RIGHT";
	}
	else if (m_eDir == DIR_LEFT)
	{
		m_pFrameKey = L"Sword_LEFT";
	}

	if (m_PlayerPreState != m_PlayerCurState )
	{
		switch (m_PlayerCurState)
		{
		case CPlayer::IDLE:
		case CPlayer::WALK:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 0;
			m_tFrame.iMotion = (int)(m_CurSword);

			m_tFrame.dwFrameSpeed = 200;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		case CPlayer::ATTACK:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 3;
			m_tFrame.iMotion = (int)(m_CurSword);

			m_tFrame.dwFrameSpeed = 100;
			m_tFrame.dwFrameTime = GetTickCount();
			break;

		case CPlayer::JUMP:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 0;
			m_tFrame.iMotion = SwordLv;

			m_tFrame.dwFrameSpeed = 200;
			m_tFrame.dwFrameTime = GetTickCount();
			break;

		case CPlayer::DAMAGED:
			m_tFrame.iFrameStart = 2;
			m_tFrame.iFrameEnd = 2;
			m_tFrame.iMotion = (int)(m_CurSword);

			m_tFrame.dwFrameSpeed = 200;
			m_tFrame.dwFrameTime = GetTickCount();
			break;

		case CPlayer::DEAD:
			m_tFrame.iFrameStart = 2;
			m_tFrame.iFrameEnd = 2;
			m_tFrame.iMotion = (int)(m_CurSword);

			m_tFrame.dwFrameSpeed = 200;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		}
		
		m_PlayerPreState = m_PlayerCurState;
	}
}

void CWeapon::Sword_Change(void)
{
	if (m_CurSword != m_PreSword)
	{	
		switch (m_CurSword)
		{
		case SWORD_1:
			m_CurSword = SWORD_1;
			m_iAtk = 10;
			break;
		case SWORD_2:
			m_CurSword = SWORD_2;
			m_iAtk = 20;
			break;
		case SWORD_3:
			m_CurSword = SWORD_3;
			m_iAtk = 30;
			break;
		}

		m_tFrame.iMotion = (int)(m_CurSword);
		m_PreSword = m_CurSword;
	}
}
