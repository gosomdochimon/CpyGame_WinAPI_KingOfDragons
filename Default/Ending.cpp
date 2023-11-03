#include "stdafx.h"
#include "Ending.h"
#include "BmpMgr.h"
#include "ScrollMgr.h"
#include "SoundMgr.h"
#include "SceneMgr.h"

CEnding::CEnding()
{
}


CEnding::~CEnding()
{
}

void CEnding::Initialize(void)
{
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Ending/Ending_Ground.bmp", L"Ground_ENDING");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Ending/Ending_BackGround.bmp", L"BackGround_ENDING");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Ending/Sun.bmp", L"Sun");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Ending/BYE.bmp", L"BYE");
	m_tInfo = { 400.f, 0.f, 250.f, 230.f };
	m_tPoint = { 800.f, 300.f, 0 ,0 };

	m_fAngle = 300.f;
	m_fSpeed = 0.4f;

	m_tByeInfo = { 320.f, 500.f, 450.f, 170.f };
	m_bLetter = false;
}

int CEnding::Update(void)
{
	CSoundMgr::Get_Instance()->PlaySound(L"Credit Roll.mp3", SOUND_BGM, m_fSound);
	Update_Sun();
	Update_Letter();

	return 0;
}

void CEnding::Late_Update(void)
{

}

void CEnding::Render(HDC hDC)
{
	HDC	hGroundDC = CBmpMgr::Get_Instance()->Find_Image(L"Ground_ENDING");
	HDC	hBackGroundDC = CBmpMgr::Get_Instance()->Find_Image(L"BackGround_ENDING");
	HDC	hSunDC = CBmpMgr::Get_Instance()->Find_Image(L"Sun");
	HDC	hBYEDC = CBmpMgr::Get_Instance()->Find_Image(L"BYE");
	GdiTransparentBlt(hDC, 					// 복사 받을, 최종적으로 그림을 그릴 DC
		0,	// 2,3 인자 :  복사받을 위치 X, Y
		0,
		610,				// 4,5 인자 : 복사받을 가로, 세로 길이
		270,
		hBackGroundDC,							// 비트맵을 가지고 있는 DC
		0,								// 비트맵 출력 시작 좌표, X,Y
		0,
		610,				// 복사할 비트맵의 가로, 세로 길이
		270,
		RGB(255, 0, 255));

	GdiTransparentBlt(hDC, 					// 복사 받을, 최종적으로 그림을 그릴 DC
		m_tRect.left,	// 2,3 인자 :  복사받을 위치 X, Y
		m_tRect.top,
		m_tInfo.fCX,				// 4,5 인자 : 복사받을 가로, 세로 길이
		m_tInfo.fCY,
		hSunDC,							// 비트맵을 가지고 있는 DC
		0,								// 비트맵 출력 시작 좌표, X,Y
		0,
		m_tInfo.fCX,				// 4,5 인자 : 복사받을 가로, 세로 길이
		m_tInfo.fCY,
		RGB(255, 0, 255));

	GdiTransparentBlt(hDC, 					// 복사 받을, 최종적으로 그림을 그릴 DC
		m_tByeRect.left,	// 2,3 인자 :  복사받을 위치 X, Y
		m_tByeRect.top,
		m_tByeInfo.fCX,				// 4,5 인자 : 복사받을 가로, 세로 길이
		m_tByeInfo.fCY,
		hBYEDC,							// 비트맵을 가지고 있는 DC
		0,								// 비트맵 출력 시작 좌표, X,Y
		0,
		m_tByeInfo.fCX,				// 4,5 인자 : 복사받을 가로, 세로 길이
		m_tByeInfo.fCY,
		RGB(255, 0, 255));

	GdiTransparentBlt(hDC, 					// 복사 받을, 최종적으로 그림을 그릴 DC
		0,	// 2,3 인자 :  복사받을 위치 X, Y
		0,
		800,				// 4,5 인자 : 복사받을 가로, 세로 길이
		600,
		hGroundDC,							// 비트맵을 가지고 있는 DC
		0,								// 비트맵 출력 시작 좌표, X,Y
		0,
		800,				// 4,5 인자 : 복사받을 가로, 세로 길이
		600,
		RGB(255, 0, 255));

	
}

void CEnding::Release(void)
{
}

void CEnding::Update_Sun(void)
{
	if (m_fAngle <= 160.f)
	{
		m_bLetter = true;
	}
	else {
		m_fAngle -= m_fSpeed;
	}


	m_tInfo.fX = m_tPoint.fX + (400 * cosf(m_fAngle * PI / 180.f));
	m_tInfo.fY = m_tPoint.fY - (400 * sinf(m_fAngle * PI / 180.f));

	m_tRect.left = int(m_tInfo.fX - (m_tInfo.fCX * 0.5f));
	m_tRect.top = int(m_tInfo.fY - (m_tInfo.fCY * 0.5f));
	m_tRect.right = int(m_tInfo.fX + (m_tInfo.fCX * 0.5f));
	m_tRect.bottom = int(m_tInfo.fY + (m_tInfo.fCY * 0.5f));
}

void CEnding::Update_Letter(void)
{
	if (m_bLetter)
	{
		if (m_tByeInfo.fY <= 120.f)
		{
			m_tByeInfo.fY = 120.f;
		}
		else {
			m_tByeInfo.fY -= 3.f;
		}

	}

	m_tByeRect.left = int(m_tByeInfo.fX - (m_tByeInfo.fCX * 0.5f));
	m_tByeRect.top = int(m_tByeInfo.fY - (m_tByeInfo.fCY * 0.5f));
	m_tByeRect.right = int(m_tByeInfo.fX + (m_tByeInfo.fCX * 0.5f));
	m_tByeRect.bottom = int(m_tByeInfo.fY + (m_tByeInfo.fCY * 0.5f));
}
