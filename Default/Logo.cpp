#include "stdafx.h"
#include "Logo.h"
#include "BmpMgr.h"
#include "KeyMgr.h"
#include "SceneMgr.h"
#include "SoundMgr.h"
CLogo::CLogo()
{
}


CLogo::~CLogo()
{
	Release();
}

void CLogo::Initialize(void)
{
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Logo/KOD_Logo.bmp", L"Logo");
}

int CLogo::Update(void)
{
	CSoundMgr::Get_Instance()->PlaySound(L"Title Fanfare.mp3", SOUND_BGM, m_fSound);
	return 0;
}

void CLogo::Late_Update(void)
{
	if (CKeyMgr::Get_Instance()->Key_Down(VK_RETURN))
	{
		CSoundMgr::Get_Instance()->StopSound(SOUND_BGM);
		CSceneMgr::Get_Instance()->Scene_Change(SC_MENU);
		return;
	}
	
}

void CLogo::Render(HDC hDC)
{
	HDC	hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Logo");

	BitBlt(hDC,
		0,
		0,
		WINCX, 
		WINCY,
		hMemDC,
		0,
		0,
		SRCCOPY);
}

void CLogo::Release(void)
{
	
}
