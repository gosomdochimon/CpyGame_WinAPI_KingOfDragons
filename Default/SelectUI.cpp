#include "stdafx.h"
#include "SelectUI.h"
#include "BmpMgr.h"
#include "KeyMgr.h"
CSelectUI::CSelectUI()
{
}


CSelectUI::~CSelectUI()
{
}

void CSelectUI::Initialize(void)
{
	Init_Pos();
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/Select_UI.bmp", L"Select_UI");
	m_tInfo = { m_PosArr[2].fX, m_PosArr[2].fY, 64, 48 };
	m_iCurPos = 2;
}

int CSelectUI::Update(void)
{  // 메뉴에서 줘야한다.
	
	Update_Pos();
	Update_Rect();
	return 0;
}

void CSelectUI::Late_Update(void)
{
}

void CSelectUI::Render(HDC hDC)
{
	HDC	hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Select_UI");

	GdiTransparentBlt(hDC, 					// 복사 받을, 최종적으로 그림을 그릴 DC
		m_tRect.left,	// 2,3 인자 :  복사받을 위치 X, Y
		m_tRect.top,
		m_tInfo.fCX,				// 4,5 인자 : 복사받을 가로, 세로 길이
		m_tInfo.fCY,
		hMemDC,							// 비트맵을 가지고 있는 DC
		0,								// 비트맵 출력 시작 좌표, X,Y
		0,
		m_tInfo.fCX,				// 4,5 인자 : 복사받을 가로, 세로 길이
		m_tInfo.fCY,
		RGB(255, 0, 255));	// 
}

void CSelectUI::Release(void)
{
}

void CSelectUI::Update_Pos(void)
{
	m_tInfo.fX = m_PosArr[m_iCurPos].fX;
	m_tInfo.fY = m_PosArr[m_iCurPos].fY;
	
}

void CSelectUI::Init_Pos(void)
{
	m_PosArr[0] = { 110, 130, 0, 0 };
	m_PosArr[1] = { 260, 220, 0 ,0 };
	m_PosArr[2] = { 400, 223, 0 ,0 };
	m_PosArr[3] = { 550, 150, 0 ,0 };
	m_PosArr[4] = { 675, 150, 0 ,0 };
}
