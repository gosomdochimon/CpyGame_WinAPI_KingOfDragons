#include "stdafx.h"
#include "Stat_UI.h"
#include "ObjMgr.h"
#include "BmpMgr.h"
#include "UIMgr.h"
#include "Skill_UI.h"
CStat_UI::CStat_UI()
{
}


CStat_UI::~CStat_UI()
{
}

void CStat_UI::Initialize(void)
{
	Insert_Bmp();
	m_tRect = { 0, 400,800, 600 };
	m_pFrameKey = L"Stat_UI";

	m_tHpRect = { 94, 521, 232, 540 };
	m_tMpRect = { 94, 550, 232, 569 };
	Player = CObjMgr::Get_Instance()->Get_Player();
	m_eId = UI_CHARACTER;

	m_tCoolTimeRect = { 355, 535, 373, 565 };
	m_tSwordRect = { 245, 525, 285, 565 };
	m_tShieldRect = { 300, 525, 345, 565 };
	m_tSkillRect = { 355, 510, 371, 526 };

	m_pSkillKey = dynamic_cast<CSkill_UI*>(CUIMgr::Get_Instance()->Get_SkillUI())->Get_Icon();

	m_iCooltime_Motion = 0;
	m_iWeapon_Motion = 0;
	m_iShield_Motion = 0;
}

int CStat_UI::Update(void)
{
	CoolTimeUpdate();
	ShieldMotionChange();
	CooltimedMotionChange();
	SwordMotionChange();
	SKillMotionChange();

	m_tHpRect.right = (Player->Get_Stat().fCurHp / Player->Get_Stat().fMaxHp *140.f);
	//마나로 수정할것
	m_tMpRect.right = (Player->Get_Stat().fCurMp / Player->Get_Stat().fMaxMp *140.f);
	return 0;
}

void CStat_UI::Late_Update(void)
{
}

void CStat_UI::Render(HDC hDC)
{
	HDC	hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);
	HDC	hHPBarDC = CBmpMgr::Get_Instance()->Find_Image(L"HP_BAR_UI");
	HDC	hMPBarDC = CBmpMgr::Get_Instance()->Find_Image(L"MP_BAR_UI");

	HDC	hSwordDC = CBmpMgr::Get_Instance()->Find_Image(L"SWORD_UI");
	HDC	hShieldDC = CBmpMgr::Get_Instance()->Find_Image(L"SHIELD_UI");
	HDC	hCooltimeDC = CBmpMgr::Get_Instance()->Find_Image(L"COOLTIME_UI");

	HDC hSkillDC = CBmpMgr::Get_Instance()->Find_Image(m_pSkillKey);

	GdiTransparentBlt(hDC, 					// 복사 받을, 최종적으로 그림을 그릴 DC
		0,	// 2,3 인자 :  복사받을 위치 X, Y
		450,
		800,				// 4,5 인자 : 복사받을 가로, 세로 길이
		150,
		hMemDC,							// 비트맵을 가지고 있는 DC
		0,								// 비트맵 출력 시작 좌표, X,Y
		0,
		800,				// 복사할 비트맵의 가로, 세로 길이
		150,
		RGB(255, 0, 255));	// 제거하고자 하는 색상
	 //hp
	GdiTransparentBlt(hDC, 					// 복사 받을, 최종적으로 그림을 그릴 DC
		94,	// 2,3 인자 :  복사받을 위치 X, Y
		521,
		int(m_tHpRect.right),				// 4,5 인자 : 복사받을 가로, 세로 길이
		19,
		hHPBarDC,							// 비트맵을 가지고 있는 DC
		0,								// 비트맵 출력 시작 좌표, X,Y
		0,
		139,				// 복사할 비트맵의 가로, 세로 길이
		19,
		RGB(255, 0, 255));	// 
	//mp
	GdiTransparentBlt(hDC, 					// 복사 받을, 최종적으로 그림을 그릴 DC
		94,	// 2,3 인자 :  복사받을 위치 X, Y
		550,
		int(m_tMpRect.right),				// 4,5 인자 : 복사받을 가로, 세로 길이
		19,
		hMPBarDC,							// 비트맵을 가지고 있는 DC
		0,								// 비트맵 출력 시작 좌표, X,Y
		0,
		139,				// 복사할 비트맵의 가로, 세로 길이
		19,
		RGB(255, 0, 255));	// 
	//sword
	GdiTransparentBlt(hDC, 					// 복사 받을, 최종적으로 그림을 그릴 DC
		246,	// 2,3 인자 :  복사받을 위치 X, Y
		525,
		39,				// 4,5 인자 : 복사받을 가로, 세로 길이
		39,
		hSwordDC,							// 비트맵을 가지고 있는 DC
		0,								// 비트맵 출력 시작 좌표, X,Y
		m_iWeapon_Motion * 39,
		39,				// 복사할 비트맵의 가로, 세로 길이
		39,
		RGB(255, 0, 255));	// 
	//shield
	GdiTransparentBlt(hDC, 					// 복사 받을, 최종적으로 그림을 그릴 DC
		300,	// 2,3 인자 :  복사받을 위치 X, Y
		525,
		39,				// 4,5 인자 : 복사받을 가로, 세로 길이
		39,
		hShieldDC,							// 비트맵을 가지고 있는 DC
		0,								// 비트맵 출력 시작 좌표, X,Y
		m_iShield_Motion *39,
		39,				// 복사할 비트맵의 가로, 세로 길이
		39,
		RGB(255, 0, 255));	// 
	//cooltime
	GdiTransparentBlt(hDC, 					// 복사 받을, 최종적으로 그림을 그릴 DC
		m_tCoolTimeRect.left,	// 2,3 인자 :  복사받을 위치 X, Y
		m_tCoolTimeRect.top,
		int(m_tCoolTimeRect.right - m_tCoolTimeRect.left),				// 4,5 인자 : 복사받을 가로, 세로 길이
		int(m_tCoolTimeRect.bottom - m_tCoolTimeRect.top),
		hCooltimeDC,							// 비트맵을 가지고 있는 DC
		0,								// 비트맵 출력 시작 좌표, X,Y
		m_iCooltime_Motion * 30.f,
		int(m_tCoolTimeRect.right - m_tCoolTimeRect.left),				// 복사할 비트맵의 가로, 세로 길이
		int(m_tCoolTimeRect.bottom - m_tCoolTimeRect.top),
		RGB(255, 0, 255));	// 
	//SkillIcon
	GdiTransparentBlt(hDC, 					// 복사 받을, 최종적으로 그림을 그릴 DC
		m_tSkillRect.left,	// 2,3 인자 :  복사받을 위치 X, Y
		m_tSkillRect.top,
		int(m_tSkillRect.right - m_tSkillRect.left),				// 4,5 인자 : 복사받을 가로, 세로 길이
		int(m_tSkillRect.bottom - m_tSkillRect.top),
		hSkillDC,							// 비트맵을 가지고 있는 DC
		0,								// 비트맵 출력 시작 좌표, X,Y
		0,
		int(m_tSkillRect.right - m_tSkillRect.left),				// 복사할 비트맵의 가로, 세로 길이
		int(m_tSkillRect.bottom - m_tSkillRect.top),
		RGB(255, 0, 255));
}

void CStat_UI::Release(void)
{
}

void CStat_UI::Insert_Bmp(void)
{
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/Stat_Ui.bmp", L"Stat_UI");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/Player_HP_Bar.bmp", L"HP_BAR_UI");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/Player_MP_Bar.bmp", L"MP_BAR_UI");


	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/Stat_Ui.bmp", L"Stat_UI");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/Shield_UI.bmp", L"SHIELD_UI");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/Sword_UI.bmp", L"SWORD_UI");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/Cooltime_UI.bmp", L"COOLTIME_UI");
}

void CStat_UI::CoolTimeUpdate(void)
{
	if (!dynamic_cast<CSkill_UI*>(CUIMgr::Get_Instance()->Get_SkillUI())->Get_CheckBool())
	{
		m_iCooltime_Motion = 0;
		m_tCoolTimeRect.top = 565 - (dynamic_cast<CSkill_UI*>(CUIMgr::Get_Instance()->Get_SkillUI())->Get_Cooltime() / dynamic_cast<CSkill_UI*>(CUIMgr::Get_Instance()->Get_SkillUI())->Get_MaxCooltime()) * 30.f;
	}
	else
	{
		m_iCooltime_Motion = 1;
		m_tCoolTimeRect.top = 535;
	}
}

void CStat_UI::ShieldMotionChange(void)
{

	m_iShield_Motion = int(dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player())->Get_Shield()->Get_eShield());
}

void CStat_UI::SwordMotionChange(void)
{
	m_iWeapon_Motion = int(dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player())->Get_Weapon()->Get_eSword());
}

void CStat_UI::CooltimedMotionChange(void)
{
}

void CStat_UI::SKillMotionChange(void)
{
	m_pSkillKey = dynamic_cast<CSkill_UI*>(CUIMgr::Get_Instance()->Get_SkillUI())->Get_Icon();
}

void CStat_UI::Move_Frame(void)
{

	if (m_tFrame.dwFrameTime + m_tFrame.dwFrameSpeed < GetTickCount())
	{
		++m_tFrame.iFrameStart;

		if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
			m_tFrame.iFrameStart = 0;

		m_tFrame.dwFrameTime = GetTickCount();
	}

}
