#include "stdafx.h"
#include "Arrow.h"
#include "BmpMgr.h"
#include "ScrollMgr.h"
#include "ObjMgr.h"
#include "CollisionMgr.h"

CArrow::CArrow() 
{
}


CArrow::~CArrow()
{
}

void CArrow::Initialize(void)
{
	//m_tShadowInfo.fX = .f;
	//m_tShadowInfo.fY = 6.f;
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/WereWolf/WereWolf_Arrow.bmp", L"WereWolf_ARROW");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/WereWolf/Arrow_Shadow.bmp", L"Arrow_SHADOW");
	m_tInfo.fCX = 50.f;
	m_tInfo.fCY = 10.f;
	m_tShadowInfo.fCX = 30.f;
	m_tShadowInfo.fCY = 5.f;

	m_pFrameKey = L"WereWolf_ARROW";

	m_tFrame.dwFrameSpeed = 200.f;
	m_tFrame.dwFrameTime = GetTickCount();
	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 0;
	m_tFrame.iMotion = 0;
	m_bIsCol = false;
	m_fSpeed = 5.f;
	m_pTarget = CObjMgr::Get_Instance()->Get_Player();
	m_bJump = false;
	m_iAtk = 10;
	//render아이디 지정
	m_eGroup = RENDER_GAMEOBJECT;
	m_fCrossroad = 0.f;
}

int CArrow::Update(void)
{
	if (m_bDead)
		return OBJ_DEAD;

	Move();
	Update_Shadow();
	Update_Rect();
	Update_ShadowRect();
	return 0;
}

void CArrow::Late_Update(void)
{
	if (m_fCrossroad >= 1000.f)
		Set_Dead();
	Attacking();
	Motion_Change();
	Move_Frame();
}

void CArrow::Render(HDC hDC)
{
	int iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	HDC	hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);

	//그림자
	//Ellipse(hDC, m_tShadowRect.left + iScrollX, m_tShadowRect.top, m_tShadowRect.right + iScrollX, m_tShadowRect.bottom);
	HDC	hShadowDC = CBmpMgr::Get_Instance()->Find_Image(L"Arrow_SHADOW");

	GdiTransparentBlt(hDC, 					// 복사 받을, 최종적으로 그림을 그릴 DC
		int(m_tShadowRect.left + iScrollX),	// 2,3 인자 :  복사받을 위치 X, Y
		int(m_tShadowRect.top + iScrollY),
		int(m_tShadowInfo.fCX),				// 4,5 인자 : 복사받을 가로, 세로 길이
		int(m_tShadowInfo.fCY),
		hShadowDC,							// 비트맵을 가지고 있는 DC
		0 * (int)m_tShadowInfo.fCX,								// 비트맵 출력 시작 좌표, X,Y
		0 * (int)m_tShadowInfo.fCY,
		(int)m_tShadowInfo.fCX,				// 복사할 비트맵의 가로, 세로 길이
		(int)m_tShadowInfo.fCY,
		RGB(255, 0, 255));

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

void CArrow::Release(void)
{
}

void CArrow::Move()
{
	if (m_eDir == DIR_LEFT)
	{
		m_tShadowInfo.fX += m_fSpeed;
		
	}
	else
	{
		m_tShadowInfo.fX -= m_fSpeed;
	}
	m_fCrossroad += m_fSpeed;
}

void CArrow::Update_Shadow()
{
	m_tInfo.fX = m_tShadowInfo.fX;
	m_tInfo.fY = m_tShadowInfo.fY - m_tInfo.fCY* 4.f;
}

void CArrow::Motion_Change()
{
	if (m_eDir == DIR_LEFT)
	{
		m_tFrame.iMotion = 1;
	}
	else
	{
		m_tFrame.iMotion = 0;
	}
}

void CArrow::Attacking()
{
	RECT rt;
	if (CCollisionMgr::Collision_OneAtkRect(this, m_tShadowRect, CObjMgr::Get_Instance()->Get_PlayerList()))
	{
		Set_Dead();
	}
	
}
