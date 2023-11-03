#include "stdafx.h"
#include "CheckPoint.h"
#include "TutorialMSGHandler.h"
#include "ObjMgr.h"
#include "ScrollMgr.h"
#include "BmpMgr.h"
CCheckPoint::CCheckPoint()
{
}


CCheckPoint::~CCheckPoint()
{
}

void CCheckPoint::Initialize(void)
{
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/CheckPoint.bmp", L"CHECKPOINT");

	m_tInfo = { 300.f, 300.f, 50.f, 70.f };
	m_tShadowInfo = { m_tInfo.fX, m_tInfo.fY - m_tInfo.fCY*0.5f, 60.f,  m_tInfo.fCX *0.3f };

	m_pFrameKey = L"CHECKPOINT";
	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 0;
	m_tFrame.iMotion = 0;
	m_tFrame.dwFrameSpeed = 200;
	m_tFrame.dwFrameTime = GetTickCount();

	m_eGroup = RENDER_GAMEOBJECT;
}

int CCheckPoint::Update(void)
{
	m_tInfo.fX = m_tShadowInfo.fX;
	m_tInfo.fY = m_tShadowInfo.fY - m_tShadowInfo.fCY *0.5f;
	Update_ShadowRect();
	Update_Rect();
	return 0;
}

void CCheckPoint::Late_Update(void)
{
	
}

void CCheckPoint::Render(HDC hDC)
{
	int iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	HDC	hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);
//	Ellipse(hDC, m_tShadowRect.left + iScrollX, m_tShadowRect.top, m_tShadowRect.right + iScrollX, m_tShadowRect.bottom);
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

void CCheckPoint::Release(void)
{
}
