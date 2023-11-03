#include "stdafx.h"
#include "TutorialStage.h"
#include "TutorialMSGHandler.h"
#include "AbstractFactory.h"
#include "ObjMgr.h"
#include "BmpMgr.h"
#include "ScrollMgr.h"
#include "SoundMgr.h"
#include "SceneMgr.h"
#include "CheckPoint.h"
#include "Scarecrow.h"
#include "Arrow.h"
#include "KeyMgr.h"
#include "Stat_UI.h"
#include "SoundMgr.h"
#include "UIMgr.h"
CTutorialStage::CTutorialStage() :m_SpawnTime(GetTickCount()), m_bArrow(false), m_bTimeStart(false),
 m_bSpawncrow(false), m_bMsg7(false)
{
}


CTutorialStage::~CTutorialStage()
{
}

void CTutorialStage::Initialize(void)
{
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/StageGround_1_1.bmp", L"Ground_1");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/StageBackGround_1.bmp", L"BackGround_1");

	//CObjMgr::Get_Instance()->Add_Object(OBJ_PLAYER, CAbstractFactory<CPlayer>::Create());
	
	UI = new CTutorial_UI;
	UI->Initialize();

	SKillInven = new CSkillInventory;
	SKillInven->Initialize();
	CTutorialMSGHandler::Get_Instance()->Set_Player(CObjMgr::Get_Instance()->Get_Player());
	CTutorialMSGHandler::Get_Instance()->Set_UI(UI);

	CObjMgr::Get_Instance()->Add_Object(OBJ_DECO, CAbstractFactory<CCheckPoint>::Create(500, 300, DIR_END));
	
	m_Phase = (PHASE)m_iPhaseCount;
	m_bPhaseLock = false;
	m_iPhaseCount = 0;
	m_dwSpawnTime = GetTickCount();
	m_bStageEnd = false;

	m_Stage = STAGE_TUTORIAL;

	m_tMapLockX = { 0, 1533 };
	m_tMapLockY = { 200, 390 };
	m_fSound = 0.5f;

	m_fStartX[0] = 500.f;
	m_fStartX[1] = 700.f;
	m_fStartX[2] = 1150.f;
}

int CTutorialStage::Update(void)
{
	CSoundMgr::Get_Instance()->PlaySound(L"Vagrant.mp3", SOUND_BGM, m_fSound);
	CObjMgr::Get_Instance()->Update();
	CUIMgr::Get_Instance()->Update();
	//StatUI->Update();
	if (m_bArrow == true && m_SpawnTime + 2000 < GetTickCount() && !m_bMsg7)
	{
		//m_bStageEnd = true;
		CTutorialMSGHandler::Get_Instance()->Change_MSG(L"TUTORIAL_MSG6");


		if (CKeyMgr::Get_Instance()->Key_Down('K'))
		{
			//CTutorialMSGHandler::Get_Instance()->Change_MSG(L"TUTORIAL_MSG7");
			m_bSpawncrow = true;
			//m_bStageEnd = true;
		}
		//m_bArrow = false;
	}
	if (m_bSpawncrow && !m_bMsg7)
	{
		CTutorialMSGHandler::Get_Instance()->Change_MSG(L"TUTORIAL_MSG7");
		for (int i = 0; i < 10; ++i)
		{
			float X = float(CObjMgr::Get_Instance()->Get_Player()->Get_ShadowInfo().fX + 100 * cosf(i * 36 * PI / 180.f));
			float Y = float(CObjMgr::Get_Instance()->Get_Player()->Get_ShadowInfo().fY - 100 * sinf(i * 36 * PI / 180.f));
			CObjMgr::Get_Instance()->Add_Object(OBJ_MONSTER, CAbstractFactory<CScarecrow>::Create(X, Y, DIR_END));
		}
		m_bSpawncrow = false;
		m_bMsg7 = true;
	}
	if (m_bMsg7 &&CObjMgr::Get_Instance()->Get_Monster().empty())
	{
		CTutorialMSGHandler::Get_Instance()->Change_MSG(L"TUTORIAL_MSG8");
		if (CKeyMgr::Get_Instance()->Key_Down('K'))
		{
			m_bStageEnd = true;
		}
	}
	if (m_bStartTutorial)
	{ 
		CTutorialMSGHandler::Get_Instance()->Set_CanInput(); //엔터됨
		CTutorialMSGHandler::Get_Instance()->Set_PlayerMove();// 시작 시 안움직임
		m_bStartTutorial = false;
	}
	TutorialFunc();

	if (m_bStageEnd)
	{
		dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player())->Save_Player();
		Release();
		CUIMgr::Get_Instance()->Delete_ID(UI_SKILL);
		CUIMgr::Get_Instance()->Delete_ID(UI_CHARACTER);
		CSoundMgr::Get_Instance()->StopSound(SOUND_BGM);
		CSceneMgr::Get_Instance()->Set_CurStage(STAGE_1);
		CObjMgr::Get_Instance()->Delete_ID(OBJ_PLAYER);
		CSceneMgr::Get_Instance()->Scene_Change(SC_STAGE);
		CScrollMgr::Get_Instance()->Set_ScrollX(-(CScrollMgr::Get_Instance()->Get_ScrollX()));
		return 1;
	}

	//StatUI->Update();
	return 0;
}

void CTutorialStage::Late_Update(void)
{
	CScrollMgr::Get_Instance()->Scroll_Lock(m_Stage);
	Lock_Player();
	Spawn_Monster();
	Lock_Monster();
	Phase_Lock();
	CObjMgr::Get_Instance()->Late_Update();
}

void CTutorialStage::Render(HDC hDC)
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
	swprintf_s(szBuff, L" Remain:%3d, m_bPhase:%3d ", CObjMgr::Get_Instance()->Get_Monster().size(), (int)m_Phase);
	TextOut(hDC, 50, 10, szBuff, lstrlen(szBuff));*/
	//StatUI->Render(hDC);
	UI->Render(hDC);
}

void CTutorialStage::Release(void)
{
	Safe_Delete<CTutorial_UI*>(UI);
	Safe_Delete<CSkillInventory*>(SKillInven);
	CObjMgr::Get_Instance()->Delete_ID(OBJ_PLAYER);
	CObjMgr::Get_Instance()->Delete_ID(OBJ_DECO);
}

void CTutorialStage::Change_Phase(void)
{
	switch (m_Phase)
	{
	case PHASE_1:
		m_iSpawnMax = 1;
		m_iSpawnCount = 1;
		m_tPhaseX.fLeft = 50.f;
		m_tPhaseX.fRight = 800.f;
		break;
	case PHASE_2:
		m_iSpawnMax = 1;
		m_iSpawnCount = 0;
		m_tPhaseX.fLeft = 600.f;
		m_tPhaseX.fRight = 1400.f;
		break;
	case PHASE_BOSS:
		m_iSpawnMax = 0;
		m_iSpawnCount = 0;
		m_tPhaseX.fLeft = 700.f;
		m_tPhaseX.fRight = 1500.f;
		break;
	}
	if (!m_bStageEnd&&m_bPhaseLock && m_iSpawnMax == m_iSpawnCount && CObjMgr::Get_Instance()->Get_Monster().empty())
	{
		//UI->Set_GOUi();
		m_bPhaseLock = false;
	}
}

void CTutorialStage::Spawn_Monster(void)
{
	switch (m_Phase)
	{
	case PHASE_2:
		for (m_iSpawnCount; m_iSpawnCount < m_iSpawnMax;)
		{
			CObjMgr::Get_Instance()->Add_Object(OBJ_MONSTER, CAbstractFactory<CScarecrow>::Create(900, 300, DIR_END));
			++m_iSpawnCount;
			break;
		}		
		break;
	case PHASE_BOSS:
		break;
	}
}

void CTutorialStage::TutorialFunc(void)
{//카운트값 줘서 다신 못들어가게 막기 + 
	if (m_iPhaseCount == 1 && CObjMgr::Get_Instance()->Get_Player()->Get_ShadowInfo().fX > m_fStartX[0])
	{
		CTutorialMSGHandler::Get_Instance()->Change_MSG(L"TUTORIAL_MSG4");
	}
	if (m_iPhaseCount >= 2 && CObjMgr::Get_Instance()->Get_Monster().empty() && CObjMgr::Get_Instance()->Get_Player()->Get_ShadowInfo().fX > 800.f)
	{
		if (m_iPhaseCount == 2)
		{
			CTutorialMSGHandler::Get_Instance()->Change_MSG(L"TUTORIAL_MSG5");
			if (!m_bTimeStart)
			{
				m_SpawnTime = GetTickCount();
				m_bTimeStart = true;
			}

		}
	
		if (!m_bArrow && m_SpawnTime + 2000 <GetTickCount())
		{
			for (int i = 0; i < 5; ++i)
			{
				CObjMgr::Get_Instance()->Add_Object(OBJ_BULLET, CAbstractFactory<CArrow>::Create(CObjMgr::Get_Instance()->Get_Player()->Get_ShadowInfo().fX + 500, 200 + i * 40, DIR_RIGHT));
			}
			m_bArrow = true;
			m_SpawnTime = GetTickCount();
			CTutorialMSGHandler::Get_Instance()->Set_CanInput();
		}

		

	}

	

}
