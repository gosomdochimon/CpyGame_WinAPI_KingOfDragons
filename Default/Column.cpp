#include "stdafx.h"
#include "Column.h"


CColumn::CColumn()
{
}


CColumn::~CColumn()
{
}

void CColumn::Initialize(void)
{
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Stage2_Column.bmp", L"COLUMN");

	m_tInfo = { 1280.f, 225.f, 65.f, 275.f };
	m_tShadowInfo = { m_tInfo.fX,440.f,  m_tInfo.fCX, m_tInfo.fCY };
	m_tShadowRect = { LONG(m_tShadowInfo.fX - m_tShadowInfo.fCX*0.5f), LONG(m_tShadowInfo.fY ), LONG(m_tShadowInfo.fX + m_tShadowInfo.fCX*0.5f),  LONG(m_tShadowInfo.fY + 30.f) };
	m_pFrameKey = L"COLUMN";
	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 0;
	m_tFrame.iMotion = 0;
	m_tFrame.dwFrameSpeed = 200;
	m_tFrame.dwFrameTime = GetTickCount();

	m_eGroup = RENDER_GAMEOBJECT;
}

int CColumn::Update(void)
{
	m_tInfo = { 1280.f, 225.f, 65.f, 275.f };
	Update_Rect();
	Update_ShadowRect();
	return 0;
}

void CColumn::Late_Update(void)
{
}

void CColumn::Render(HDC hDC)
{
	int iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	HDC	hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);
	//	Ellipse(hDC, m_tShadowRect.left + iScrollX, m_tShadowRect.top, m_tShadowRect.right + iScrollX, m_tShadowRect.bottom);
	GdiTransparentBlt(hDC, 					// ���� ����, ���������� �׸��� �׸� DC
		int(m_tRect.left + iScrollX),	// 2,3 ���� :  ������� ��ġ X, Y
		int(m_tRect.top + iScrollY),
		int(m_tInfo.fCX),				// 4,5 ���� : ������� ����, ���� ����
		int(m_tInfo.fCY),
		hMemDC,							// ��Ʈ���� ������ �ִ� DC
		m_tFrame.iFrameStart * (int)m_tInfo.fCX,								// ��Ʈ�� ��� ���� ��ǥ, X,Y
		m_tFrame.iMotion * (int)m_tInfo.fCY,
		(int)m_tInfo.fCX,				// ������ ��Ʈ���� ����, ���� ����
		(int)m_tInfo.fCY,
		RGB(255, 0, 255));	// �����ϰ��� �ϴ� ����
}

void CColumn::Release(void)
{
}
