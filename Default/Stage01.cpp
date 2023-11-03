#include "stdafx.h"
#include "Stage01.h"
#include "AbstractFactory.h"
#include "ObjMgr.h"
#include "Skeleton.h"
#include "Minotaur.h"
#include "Mouse.h"
#include "BmpMgr.h"
#include "ScrollMgr.h"
#include "SoundMgr.h"
#include "SceneMgr.h"
#include "GreatDragonian.h"
#include "WereWolf.h"
#include "UIMgr.h"
#include "Weapon.h"
#include "WereWolf.h"
CStage01::CStage01()
{
}


CStage01::~CStage01()
{
}

void CStage01::Initialize(void)
{
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/StageGround_1_1.bmp", L"Ground_1");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/StageBackGround_1.bmp", L"BackGround_1");
	//StageGround_Lv2
	//CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/StageGround_Lv2.bmp", L"Ground_1");
	
	dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player())->Load_Player();
	SKillInven = new CSkillInventory;
	SKillInven->Initialize();

	m_Stage = STAGE_1;

	m_Phase = (PHASE)m_iPhaseCount;
	m_bPhaseLock = false;
	m_iPhaseCount = 0;
	m_dwSpawnTime = GetTickCount();
	m_bStageEnd = false;

	m_fStartX[0] = 500.f;
	m_fStartX[1] = 1000.f;
	m_fStartX[2] = 1200.f;

	m_tMapLockX = {0, 1533 };
	m_tMapLockY = { 200, 390 };
	m_fSound = 0.5f;
	
	
	//CObjMgr::Get_Instance()->Add_Object(OBJ_MONSTER, CAbstractFactory<CWereWolf>::Create(120, 330, 0));
	//CObjMgr::Get_Instance()->Add_Object(OBJ_MONSTER, CAbstractFactory<CGreatDragonian>::Create(120, 330, 0));
}

int CStage01::Update(void)
{
	if (PHASE_BOSS != m_Phase)
	{
		CSoundMgr::Get_Instance()->PlaySound(L"The Orc Village.wav", SOUND_BGM, m_fSound);
	}
	else if (PHASE_BOSS == m_Phase)
	{
		if (m_bBgmPlay)
		{
			CSoundMgr::Get_Instance()->StopSound(SOUND_BGM);
			m_bBgmPlay = false;
		}
		CSoundMgr::Get_Instance()->PlaySound(L"Boss_1.wav", SOUND_BGM, m_fSound);
	}

	CObjMgr::Get_Instance()->Update();
	CUIMgr::Get_Instance()->Update();
	////야매넣
	Phase_Lock();
	//if(씬메니저의 현재 스테이지가 스테이지2라면) return;
	if (m_bStageEnd)
	{
		dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player())->Get_Weapon()->Set_eSword(CWeapon::SWORD_2);
		dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player())->Get_Shield()->Set_eShield(CShield::SHIELD_2);
		dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player())->Save_Player();
		CSceneMgr::Get_Instance()->Set_CurStage(STAGE_2);
		//CObjMgr::Get_Instance()->Delete_ID(OBJ_PLAYER);
		Release();
		CSceneMgr::Get_Instance()->Scene_Change(SC_STAGE);
		CScrollMgr::Get_Instance()->Set_ScrollX(-(CScrollMgr::Get_Instance()->Get_ScrollX()));
		return 1;
	}
		
	Spawn_Monster();
	//StatUI->Update();
	
	return 0;

}

void CStage01::Late_Update(void)
{
	CScrollMgr::Get_Instance()->Scroll_Lock(m_Stage);
	Lock_Player();
	Lock_Monster();
	CObjMgr::Get_Instance()->Late_Update();


	//CTileMgr::Get_Instance()->Late_Update();
}

void CStage01::Render(HDC hDC)
{
	HDC	hGroundDC = CBmpMgr::Get_Instance()->Find_Image(L"Ground_1");
	HDC	hBackGroundDC = CBmpMgr::Get_Instance()->Find_Image(L"BackGround_1");

	int iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	GdiTransparentBlt(hDC, 					// 복사 받을, 최종적으로 그림을 그릴 DC
		0,	// 2,3 인자 :  복사받을 위치 X, Y
		0,
		880,				// 4,5 인자 : 복사받을 가로, 세로 길이
		200,
		hBackGroundDC,							// 비트맵을 가지고 있는 DC
		0,								// 비트맵 출력 시작 좌표, X,Y
		0,
		880,				// 복사할 비트맵의 가로, 세로 길이
		200,
		RGB(255, 0, 255));
	//앞배경 넣음
	GdiTransparentBlt(hDC, 					// 복사 받을, 최종적으로 그림을 그릴 DC
		iScrollX,	// 2,3 인자 :  복사받을 위치 X, Y
		iScrollY,
		1533,				// 4,5 인자 : 복사받을 가로, 세로 길이
		450,
		hGroundDC,							// 비트맵을 가지고 있는 DC
		0,								// 비트맵 출력 시작 좌표, X,Y
		0,
		1533,				// 복사할 비트맵의 가로, 세로 길이
		450,
		RGB(255, 0, 255));			// 제거하고자 하는 색상

	CObjMgr::Get_Instance()->Render(hDC);
	CUIMgr::Get_Instance()->Render(hDC);

	/*TCHAR		szBuff[255] = L"";
	swprintf_s(szBuff, L" Remain:%3d, m_bPhase:%3d ", CObjMgr::Get_Instance()->Get_Monster().size(), (int)m_bPhaseLock);
	TextOut(hDC, 50, 10, szBuff, lstrlen(szBuff));*/

}

void CStage01::Release(void)
{
	//CTileMgr::Get_Instance()->Destroy_Instance();
	//Safe_Delete<CUI*>(StatUI);
	Safe_Delete<CSkillInventory*>(SKillInven);
	CObjMgr::Get_Instance()->Delete_ID(OBJ_PLAYER);
	CUIMgr::Get_Instance()->Delete_ID(UI_CHARACTER);
	CUIMgr::Get_Instance()->Delete_ID(UI_SKILL);
}



void CStage01::Change_Phase(void)
{
	switch (m_Phase)
	{
	case PHASE_1:
		m_iSpawnMax = 4;
		m_iSpawnCount = 0;
		m_tPhaseX.fLeft = 0.f;
		m_tPhaseX.fRight = 800.f;
		break;
	case PHASE_2:
		m_iSpawnMax = 4;
		m_iSpawnCount = 0;
		m_tPhaseX.fLeft = 500.f;
		m_tPhaseX.fRight = 1300.f;
		break;
	case PHASE_BOSS:
		m_iSpawnMax = 1;
		m_iSpawnCount = 0;
		m_tPhaseX.fLeft = 700.f;
		m_tPhaseX.fRight = 1500.f;
		break;
	}
}

void CStage01::Spawn_Monster(void)
{
	switch (m_Phase)
	{
	case PHASE_1:
		if (m_dwSpawnTime + 1000 < GetTickCount())
		{
			for (m_iSpawnCount; m_iSpawnCount < 4;)
			{
				CObjMgr::Get_Instance()->Add_Object(OBJ_MONSTER, CAbstractFactory<CSkeleton>::Create(200 + m_iSpawnCount * 50, 230 + 10 * m_iSpawnCount, 0));	
				++m_iSpawnCount;
			}

		}
	


		break;
	case PHASE_2:
		if (m_dwSpawnTime + 1000 < GetTickCount())
		{
			for (m_iSpawnCount; m_iSpawnCount != m_iSpawnMax;)
			{
				CObjMgr::Get_Instance()->Add_Object(OBJ_MONSTER, CAbstractFactory<CSkeleton>::Create(800 + m_iSpawnCount * 50, 230 + 10 * m_iSpawnCount, 0));
				++m_iSpawnCount;
			}
		}
		break;
	case PHASE_BOSS:
		if (m_dwSpawnTime + 1000 < GetTickCount() && m_iSpawnMax != m_iSpawnCount)
		{
			for (m_iSpawnCount; m_iSpawnCount != m_iSpawnMax;)
			{
				CObjMgr::Get_Instance()->Add_Object(OBJ_MONSTER, CAbstractFactory<CMinotaur>::Create(1530, 230, 0));
				++m_iSpawnCount;
			}

			for (auto& Monster : CObjMgr::Get_Instance()->Get_Monster())
			{
				dynamic_cast<CMonster*>(Monster)->Set_PhaseX(m_tPhaseX);
			}
		}

	}
}


