#include "stdafx.h"
#include "Mouse.h"
#include "ScrollMgr.h"

CMouse::CMouse()
{
}


CMouse::~CMouse()
{
	Release();
}

void CMouse::Initialize(void)
{
	
	m_tInfo.fCX = 30.f;
	m_tInfo.fCY = 30.f;
}

int CMouse::Update(void)
{
	POINT		Pt{};

	// 현재 마우스 커서의 위치 값을 얻어오는 함수
	GetCursorPos(&Pt);

	// 모니터 상태의 좌표를 우리가 생성한 창 좌표로 변환하는 함수
	ScreenToClient(g_hWnd, &Pt);

	m_tInfo.fX = (float)Pt.x;
	m_tInfo.fY = (float)Pt.y;

	Update_Rect();

	ShowCursor(true);	// 마우스 커서 출력을 제어하는 함수

	return OBJ_NOEVENT;
}

void CMouse::Late_Update(void)
{

}

void CMouse::Render(HDC hDC)
{
	int iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();
	Rectangle(hDC,
		m_tRect.left,
		m_tRect.top,
		m_tRect.right,
		m_tRect.bottom);
	
}

void CMouse::Release(void)
{

}
