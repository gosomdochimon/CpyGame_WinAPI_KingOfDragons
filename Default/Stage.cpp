#include "stdafx.h"
#include "Stage.h"
#include "ObjMgr.h"
#include "LineMgr.h"
#include "AbstractFactory.h"
#include "Player.h"
#include "BmpMgr.h"
#include "ScrollMgr.h"
#include "TileMgr.h"
#include "Skeleton.h"
#include "Minotaur.h"
#include "Mouse.h"
#include "SceneMgr.h"
#include "StageHandler.h"
#include "Stat_UI.h"
#include "UIMgr.h"
#include "AbstractFactory.h"
#include "Skill_UI.h"
CStage::CStage() : m_fSound(1.f), m_bBgmPlay(true)
{
	ZeroMemory(&m_tPhaseX, sizeof(PHASEX));

	CStageHandler::Get_Instance()->Set_Stage(this);
	CObjMgr::Get_Instance()->Add_Object(OBJ_PLAYER, CAbstractFactory<CPlayer>::Create());
	CUIMgr::Get_Instance()->Add_UI(UI_SKILL, CAbstractFactory<CSkill_UI>::Create_CUI());
	CUIMgr::Get_Instance()->Add_UI(UI_CHARACTER, CAbstractFactory<CStat_UI>::Create_CUI());

	//StatUI = CAbstractFactory<CUI>::Create_CUI();
	/*StatUI = new CStat_UI;
	StatUI->Initialize();*/

	
}


CStage::~CStage()
{
	Release();
}

void CStage::Initialize(void)
{
	
	//CLineMgr::Get_Instance()->Initialize();
	//CObjMgr::Get_Instance()->Add_Object(OBJ_MONSTER, CAbstractFactory<CSkeleton>::Create());
	//CObjMgr::Get_Instance()->Add_Object(OBJ_MONSTER, CAbstractFactory<CMinotaur>::Create());
	//CTileMgr::Get_Instance()->Load_Tile();
	//CObjMgr::Get_Instance()->Add_Object(OBJ_MOUSE, CAbstractFactory<CMouse>::Create());
	


}

int CStage::Update(void)
{

	return 0;
}

void CStage::Late_Update(void)
{


//	CTileMgr::Get_Instance()->Late_Update();
}

void CStage::Render(HDC hDC)
{

}

void CStage::Release(void)
{
	//CTileMgr::Get_Instance()->Destroy_Instance();
	//Safe_Delete<CUI*>(UI);
	Safe_Delete<CSkillInventory*>(SKillInven);
	//Safe_Delete<CUI*>(StatUI);
}

void CStage::Lock_Monster(void)
{
	for (auto& iter : CObjMgr::Get_Instance()->Get_Monster())
	{
		if (iter->Get_ShadowInfo().fX > m_tMapLockX.fRight)
		{
			//iter->Set_PosX(-(CObjMgr::Get_Instance()->Get_Player()->Get_Speed()));
			iter->Set_ShadowPos(iter->Get_ShadowInfo().fX-(iter->Get_Speed()), iter->Get_ShadowInfo().fY);
		}
		if (iter->Get_ShadowInfo().fX < 0)
		{
			//CObjMgr::Get_Instance()->Get_Player()->Set_PosX(CObjMgr::Get_Instance()->Get_Player()->Get_Speed());
			iter->Set_ShadowPos(iter->Get_ShadowInfo().fX+ (iter->Get_Speed()), iter->Get_ShadowInfo().fY);
		}
		if (iter->Get_ShadowInfo().fY > m_tMapLockY.fRight)
		{
			//CObjMgr::Get_Instance()->Get_Player()->Set_PosY(-(CObjMgr::Get_Instance()->Get_Player()->Get_Speed()));
			iter->Set_ShadowPos(iter->Get_ShadowInfo().fX, iter->Get_ShadowInfo().fY -(iter->Get_Speed()));
		}
		if (iter->Get_ShadowInfo().fY < m_tMapLockY.fLeft)
		{
			//CObjMgr::Get_Instance()->Get_Player()->Set_PosY(CObjMgr::Get_Instance()->Get_Player()->Get_Speed());
			iter->Set_ShadowPos(iter->Get_ShadowInfo().fX, iter->Get_ShadowInfo().fY + (iter->Get_Speed()));
		}

	}
	
}

void CStage::Lock_Player(void)
{//스테이지 1만 만든경우임 다른 스테이지도 적용할 수 있게 이넘값으로해야할 듯 함
	//m_bPhase가 true고 페이즈 별로 PhaseX값만큼만 움직이게?
	//
	if (m_bPhaseLock)
	{
		if (CObjMgr::Get_Instance()->Get_Player()->Get_Info().fX > m_tPhaseX.fRight)
		{
			CObjMgr::Get_Instance()->Get_Player()->Set_PosX(-(CObjMgr::Get_Instance()->Get_Player()->Get_Speed()));
		}
		if (CObjMgr::Get_Instance()->Get_Player()->Get_Info().fX < m_tPhaseX.fLeft)
		{
			CObjMgr::Get_Instance()->Get_Player()->Set_PosX(CObjMgr::Get_Instance()->Get_Player()->Get_Speed());
		}
		if (CObjMgr::Get_Instance()->Get_Player()->Get_Info().fY > m_tMapLockY.fRight)
		{
			CObjMgr::Get_Instance()->Get_Player()->Set_PosY(-(CObjMgr::Get_Instance()->Get_Player()->Get_Speed()));
		}
		if (CObjMgr::Get_Instance()->Get_Player()->Get_Info().fY < m_tMapLockY.fLeft)
		{
			CObjMgr::Get_Instance()->Get_Player()->Set_PosY(CObjMgr::Get_Instance()->Get_Player()->Get_Speed());
		}
	}
	else {
		if (CObjMgr::Get_Instance()->Get_Player()->Get_Info().fX > m_tMapLockX.fRight)
		{
			CObjMgr::Get_Instance()->Get_Player()->Set_PosX(-(CObjMgr::Get_Instance()->Get_Player()->Get_Speed()));
		}
		if (CObjMgr::Get_Instance()->Get_Player()->Get_Info().fX < 0)
		{
			CObjMgr::Get_Instance()->Get_Player()->Set_PosX(CObjMgr::Get_Instance()->Get_Player()->Get_Speed());
		}
		if (CObjMgr::Get_Instance()->Get_Player()->Get_Info().fY > m_tMapLockY.fRight)
		{
			CObjMgr::Get_Instance()->Get_Player()->Set_PosY(-(CObjMgr::Get_Instance()->Get_Player()->Get_Speed()));
		}
		if (CObjMgr::Get_Instance()->Get_Player()->Get_Info().fY < m_tMapLockY.fLeft)
		{
			CObjMgr::Get_Instance()->Get_Player()->Set_PosY(CObjMgr::Get_Instance()->Get_Player()->Get_Speed());
		}
	}
	
}

void CStage::Phase_Lock(void)
{//플레이어가 해당 좌표에 도착했을 때 락 걸기
	if (!m_bPhaseLock)
	{
		switch (m_iPhaseCount)
		{
		case 0:
			if (CObjMgr::Get_Instance()->Get_Player()->Get_ShadowInfo().fX > m_fStartX[0])
			{
				m_bPhaseLock = true;
				m_iPhaseCount++;
				m_Phase = (PHASE)m_iPhaseCount;
				Change_Phase();
				m_dwSpawnTime = GetTickCount();
				//UI->Set_GOUi();
			}
			break;
		case 1:
			if (CObjMgr::Get_Instance()->Get_Player()->Get_ShadowInfo().fX > m_fStartX[1])
			{
				m_bPhaseLock = true;
				m_iPhaseCount++;
				m_Phase = (PHASE)m_iPhaseCount;
				Change_Phase();
				m_dwSpawnTime = GetTickCount();
				//UI->Set_GOUi();
			}
			break;
		case 2://보스
			if (CObjMgr::Get_Instance()->Get_Player()->Get_ShadowInfo().fX >m_fStartX[2])
			{
				m_bPhaseLock = true;
				m_iPhaseCount++;
				m_Phase = (PHASE)m_iPhaseCount;
				Change_Phase();
				m_dwSpawnTime = GetTickCount();
				//UI->Set_GOUi();
			}
			break;
		}
		
	}
	
	if (!m_bStageEnd&&m_bPhaseLock && m_iSpawnMax == m_iSpawnCount &&CObjMgr::Get_Instance()->Get_Monster().empty())
	{
		//UI->Set_GOUi();
		m_bPhaseLock = false;
	}
	
	if (m_Stage != STAGE_TUTORIAL &&m_Phase == PHASE_BOSS && m_dwSpawnTime + 3000 < GetTickCount() && CObjMgr::Get_Instance()->Get_Monster().empty())
	{
		m_bStageEnd = true;

		dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player())->Save_Player();
		
	}
}

void CStage::Change_Phase(void)
{
	
}

void CStage::Spawn_Monster(void)
{

}
