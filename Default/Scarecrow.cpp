#include "stdafx.h"
#include "Scarecrow.h"
#include "TutorialMSGHandler.h"
#include "ObjMgr.h"
#include "ScrollMgr.h"
#include "BmpMgr.h"

CScarecrow::CScarecrow() :m_iCount(0)
{
}


CScarecrow::~CScarecrow()
{
}

void CScarecrow::Initialize(void)
{
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Scarecrow.bmp", L"SCARECROW");

	m_tInfo = { 300.f, 300.f, 90.f, 90.f };
	m_tShadowInfo = { m_tInfo.fX, m_tInfo.fY - m_tInfo.fCY*0.5f, 60.f,  m_tInfo.fCX *0.3f };
	
	m_pFrameKey = L"SCARECROW";
	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 5;
	m_tFrame.iMotion = 0;
	m_tFrame.dwFrameSpeed = 200;
	m_tFrame.dwFrameTime = GetTickCount();

	m_eGroup = RENDER_GAMEOBJECT;
}

int CScarecrow::Update(void)
{
	if (m_bDead)
		return OBJ_DEAD;

	m_tInfo.fX = m_tShadowInfo.fX;
	m_tInfo.fY = m_tShadowInfo.fY - 40.f;
	Update_Rect();
	Update_ShadowRect_();
	return 0;
}

void CScarecrow::Late_Update(void)
{
	if (m_iCount > 3)
		Set_Dead();

	if (m_bIsCol)
	{
		if (m_tFrame.iFrameStart == m_tFrame.iFrameEnd)
			++m_iCount;
		Move_Frame();
	}
}

void CScarecrow::Render(HDC hDC)
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
		RGB(255, 0, 255));	// 제거하고자 하는 색상
}

void CScarecrow::Release(void)
{
}

void CScarecrow::Update_ShadowRect_()
{
	
		m_tShadowRect.top = m_tShadowInfo.fY - m_tShadowInfo.fCY*0.5f;
		m_tShadowRect.bottom = m_tShadowInfo.fY + m_tShadowInfo.fCY*0.5f;
		m_tShadowRect.left = m_tShadowInfo.fX - m_tShadowInfo.fCX*0.5f;
		m_tShadowRect.right = m_tShadowInfo.fX + m_tShadowInfo.fCX*0.5f;
}
