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

	// ���� ���콺 Ŀ���� ��ġ ���� ������ �Լ�
	GetCursorPos(&Pt);

	// ����� ������ ��ǥ�� �츮�� ������ â ��ǥ�� ��ȯ�ϴ� �Լ�
	ScreenToClient(g_hWnd, &Pt);

	m_tInfo.fX = (float)Pt.x;
	m_tInfo.fY = (float)Pt.y;

	Update_Rect();

	ShowCursor(true);	// ���콺 Ŀ�� ����� �����ϴ� �Լ�

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
