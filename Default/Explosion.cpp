#include "stdafx.h"
#include "Explosion.h"
#include "BmpMgr.h"
#include "ObjMgr.h"
#include "ScrollMgr.h"
#include "CollisionMgr.h"

CExplosion::CExplosion()
{
}


CExplosion::~CExplosion()
{
}

void CExplosion::Initialize(void)
{
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Skill/Skill_Explosion.bmp", L"Skill_EXPLOSION");

	m_tInfo = { 0.f, 0.f, 55.f, 180.f };
	m_tShadowInfo = { 0.f, 0.f, 50.f, 50.f };
	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 5;
	m_tFrame.dwFrameSpeed = 60.f;
	m_tFrame.dwFrameTime = GetTickCount();
	m_tFrame.iMotion = 0;

	m_pFrameKey = L"Skill_EXPLOSION";
	m_eGroup = RENDER_EFFECT;

	m_iAtk = 50000;
}

int CExplosion::Update(void)
{
	if (m_bDead)
		return OBJ_DEAD;

	Attacking();
	Update_Shadow();
	Update_Rect();
	Update_ShadowRect();
	return 0;
}

void CExplosion::Late_Update(void)
{
	if (m_tFrame.iFrameStart == m_tFrame.iFrameEnd)
	{
		Set_Dead();
	}
	Move_Frame();
}

void CExplosion::Render(HDC hDC)
{
	int iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	HDC	hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);


	//Ellipse(hDC, m_tShadowRect.left + iScrollX, m_tShadowRect.top, m_tShadowRect.right + iScrollX, m_tShadowRect.bottom);

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
		RGB(255, 0, 255));
}

void CExplosion::Release(void)
{
}

void CExplosion::Motion_Change(void)
{
}

void CExplosion::Insert_Bmp(void)
{
}

void CExplosion::Attacking(void)
{
	if (m_tFrame.iFrameStart >= 2 && m_tFrame.iFrameStart <= 3)
	{
		CCollisionMgr::Collision_Skill(this, m_tShadowRect, CObjMgr::Get_Instance()->Get_Monster());
	}
}

void CExplosion::Update_Shadow()
{
	m_tInfo.fX = m_tShadowInfo.fX;
	m_tInfo.fY = m_tShadowInfo.fY - m_tInfo.fCY*0.45f;
}
