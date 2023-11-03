#include "stdafx.h"
#include "Stage02.h"
#include "AbstractFactory.h"
#include "ObjMgr.h"
#include "BmpMgr.h"
#include "Skeleton.h"
#include "Minotaur.h"
#include "GreatDragonian.h"
#include "WereWolf.h"
#include "Mouse.h"
#include "ScrollMgr.h"
#include "SoundMgr.h"
#include "Stat_UI.h"
#include "UIMgr.h"
#include "SceneMgr.h"
#include "Column.h"
CStage02::CStage02()
{
}


CStage02::~CStage02()
{
}

void CStage02::Initialize(void)
{
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/StageGround_Lv2.bmp", L"Ground_2");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/StageBackGround_2.bmp", L"BackGround_2");
	//CObjMgr::Get_Instance()->Add_Object(OBJ_PLAYER, CAbstractFactory<CPlayer>::Create());
	//load player
	dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player())->Load_Player();

	CObjMgr::Get_Instance()->Add_Object(OBJ_DECO, CAbstractFactory<CColumn>::Create());
	SKillInven = new CSkillInventory;
	SKillInven->Initialize();

	m_Stage = STAGE_2;
	m_Phase = (PHASE)m_iPhaseCount;
	m_bPhaseLock = false;
	m_iPhaseCount = 0;
	m_dwSpawnTime = GetTickCount();
	m_bStageEnd = false;


	m_fStartX[0] = 400.f;
	m_fStartX[1] = 1000.f;
	m_fStartX[2] = 1700.f;

	m_tMapLockX = { 0, 2000 };
	m_tMapLockY = { 250, 400 };
	m_fSound = 1.f;
}

int CStage02::Update(void)
{
	if (PHASE_BOSS != m_Phase)
		CSoundMgr::Get_Instance()->PlaySound(L"The Orc Village.wav", SOUND_BGM, m_fSound);
	else if (PHASE_BOSS == m_Phase)
	{
		if (m_bBgmPlay)
		{
			CSoundMgr::Get_Instance()->StopSound(SOUND_BGM);
			m_bBgmPlay = false;
		}
		CSoundMgr::Get_Instance()->PlaySound(L"Boss_1.wav", SOUND_BGM, m_fSound);
	}

	if (m_bStageEnd)
	{
		CSoundMgr::Get_Instance()->StopSound(SOUND_BGM);
		dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player())->Get_Weapon()->Set_eSword(CWeapon::SWORD_2);
		dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player())->Get_Shield()->Set_eShield(CShield::SHIELD_2);
		dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player())->Save_Player();
		//CSceneMgr::Get_Instance()->Set_CurStage(STAGE_2);
		//CObjMgr::Get_Instance()->Delete_ID(OBJ_PLAYER);
		Release();
		CScrollMgr::Get_Instance()->Set_ScrollX(-(CScrollMgr::Get_Instance()->Get_ScrollX()));
		CSceneMgr::Get_Instance()->Scene_Change(SC_ENDING);
		CScrollMgr::Get_Instance()->Set_ScrollX(-(CScrollMgr::Get_Instance()->Get_ScrollX()));
		return 1;
	}

	CObjMgr::Get_Instance()->Update();
	Phase_Lock();
	Spawn_Monster();
	CUIMgr::Get_Instance()->Update();
	//StatUI->Update();
	return 0;
}

void CStage02::Late_Update(void)
{
	CScrollMgr::Get_Instance()->Scroll_Lock(m_Stage);
	Lock_Player();
	CObjMgr::Get_Instance()->Late_Update();
}

void CStage02::Render(HDC hDC)
{
	HDC	hGroundDC = CBmpMgr::Get_Instance()->Find_Image(L"Ground_2");
	HDC	hBackGroundDC = CBmpMgr::Get_Instance()->Find_Image(L"BackGround_2");

	int iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	GdiTransparentBlt(hDC, 					// 복사 받을, 최종적으로 그림을 그릴 DC
		0,	// 2,3 인자 :  복사받을 위치 X, Y
		0,
		800,				// 4,5 인자 : 복사받을 가로, 세로 길이
		260,
		hBackGroundDC,							// 비트맵을 가지고 있는 DC
		0,								// 비트맵 출력 시작 좌표, X,Y
		0,
		800,				// 복사할 비트맵의 가로, 세로 길이
		260,
		RGB(255, 0, 255));
	//앞배경 넣음
	GdiTransparentBlt(hDC, 					// 복사 받을, 최종적으로 그림을 그릴 DC
		iScrollX,	// 2,3 인자 :  복사받을 위치 X, Y
		iScrollY,
		2000,				// 4,5 인자 : 복사받을 가로, 세로 길이
		450,
		hGroundDC,							// 비트맵을 가지고 있는 DC
		0,								// 비트맵 출력 시작 좌표, X,Y
		0,
		2000,				// 복사할 비트맵의 가로, 세로 길이
		450,
		RGB(255, 0, 255));			// 제거하고자 하는 색상

	CObjMgr::Get_Instance()->Render(hDC);
	CUIMgr::Get_Instance()->Render(hDC);
	/*TCHAR		szBuff[255] = L"";
	swprintf_s(szBuff, L" Remain:%3d, m_bPhase:%3d ", CObjMgr::Get_Instance()->Get_Monster().size(), (int)m_bPhaseLock);
	TextOut(hDC, 50, 10, szBuff, lstrlen(szBuff));*/

	//StatUI->Render(hDC);
}

void CStage02::Release(void)
{
	Safe_Delete<CSkillInventory*>(SKillInven);
	CObjMgr::Get_Instance()->Delete_ID(OBJ_PLAYER);
	CObjMgr::Get_Instance()->Delete_ID(OBJ_DECO);
	CUIMgr::Get_Instance()->Delete_ID(UI_CHARACTER);
	CUIMgr::Get_Instance()->Delete_ID(UI_SKILL);
}


void CStage02::Change_Phase(void)
{
	switch (m_Phase)
	{
	case PHASE_1:
		m_iSpawnMax = 6;
		m_iSpawnCount = 0;
		m_tPhaseX.fLeft = 50.f;
		m_tPhaseX.fRight = 800.f;
		break;
	case PHASE_2:
		m_iSpawnMax = 8;
		m_iSpawnCount = 0;
		m_tPhaseX.fLeft = 600.f;
		m_tPhaseX.fRight = 1400.f;
		break;
	case PHASE_BOSS:
		m_iSpawnMax = 1;
		m_iSpawnCount = 0;
		m_tPhaseX.fLeft = 1300.f;
		m_tPhaseX.fRight = 2000.f;
		break;
	}
}

void CStage02::Spawn_Monster(void)
{
	switch (m_Phase)
	{
	case PHASE_1:
		if (m_dwSpawnTime + 1000 < GetTickCount())
		{
			for (m_iSpawnCount; m_iSpawnCount < m_iSpawnMax-2;)
			{
				CObjMgr::Get_Instance()->Add_Object(OBJ_MONSTER, CAbstractFactory<CSkeleton>::Create(m_fStartX[0] + m_iSpawnCount * 50, 230 + 50 * m_iSpawnCount, 0));
				++m_iSpawnCount;
			}
			for (m_iSpawnCount; m_iSpawnCount < m_iSpawnMax;)
			{
				CObjMgr::Get_Instance()->Add_Object(OBJ_MONSTER, CAbstractFactory<CSkeleton>::Create(m_fStartX[1] + 200, 300 + 50 * m_iSpawnCount, 0));
				++m_iSpawnCount;
			}
		}
		break;
	case PHASE_2:
		if (m_dwSpawnTime + 1000 < GetTickCount())
		{
			for (m_iSpawnCount; m_iSpawnCount < m_iSpawnMax;)
			{
				CObjMgr::Get_Instance()->Add_Object(OBJ_MONSTER, CAbstractFactory<CWereWolf>::Create(m_fStartX[1] + m_iSpawnCount * 50, 230 + 50 * m_iSpawnCount, 0));
				CObjMgr::Get_Instance()->Add_Object(OBJ_MONSTER, CAbstractFactory<CSkeleton>::Create(m_fStartX[1] + m_iSpawnCount * 50, 230 + 50 * m_iSpawnCount, 0));
				++m_iSpawnCount;
				++m_iSpawnCount;
			}
		}
		break;
	case PHASE_BOSS:
		if (m_dwSpawnTime + 1000 < GetTickCount() && m_iSpawnMax != m_iSpawnCount)
		{
			for (m_iSpawnCount; m_iSpawnCount != m_iSpawnMax;)
			{
				CObjMgr::Get_Instance()->Add_Object(OBJ_MONSTER, CAbstractFactory<CGreatDragonian>::Create(2000, 330, 0));
				++m_iSpawnCount;
			}

			for (auto& Monster : CObjMgr::Get_Instance()->Get_Monster())
			{
				dynamic_cast<CMonster*>(Monster)->Set_PhaseX(m_tPhaseX);
			}
		}


	}
}
