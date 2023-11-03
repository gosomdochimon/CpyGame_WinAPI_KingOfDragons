#include "stdafx.h"
#include "SceneMgr.h"
#include "Stage01.h"
#include "Stage02.h"
#include "Ending.h"
#include "TutorialStage.h"
// STATE 패턴 (FSM : Finite State Machine 유한 상태 기계) :자신이 취할 수 있는 유한한 개수의 상태를 가지고, 그리고 그 중 반드시 하나의 상태만 가질 수 있다.

CSceneMgr* CSceneMgr::m_pInstance = nullptr;

CSceneMgr::CSceneMgr()
	: m_pScene(nullptr), m_ePreScene(SC_END), m_eCurScene(SC_LOGO), m_eCurStage(STAGE_TUTORIAL), m_ePreStage(STAGE_TUTORIAL)
{
	m_iStageCount = 0;
}


CSceneMgr::~CSceneMgr()
{
	Release();
}

void CSceneMgr::Scene_Change(SCENEID eScene)
{
	m_eCurScene = eScene;

	if (m_ePreScene != m_eCurScene || m_eCurStage != m_ePreStage)
	{
		Safe_Delete(m_pScene);		//

		switch (m_eCurScene)
		{
		case SC_LOGO:
			m_pScene = new CLogo;
			break;

		case SC_MENU:
			m_pScene = new CMyMenu;
			break;

		case SC_EDIT:
			m_pScene = new CMyEdit;
			break;

		case SC_STAGE:
			Stage_Change(m_eCurStage);
			break;
		case SC_ENDING:
			m_pScene = new CEnding;
			break;
		}

		m_pScene->Initialize();
		/*if(m_eCurScene == SC_STAGE)
			dynamic_cast<CStage*>(m_pScene)->Set_Stage(m_eCurStage);*/
		m_ePreScene = m_eCurScene;
		m_ePreStage = m_eCurStage;
	}
}

void CSceneMgr::Update(void)
{
	m_pScene->Update();
}

void CSceneMgr::Late_Update(void)
{
	m_pScene->Late_Update();
}

void CSceneMgr::Render(HDC hDC)
{
	m_pScene->Render(hDC);
}

void CSceneMgr::Release(void)
{
	Safe_Delete<CScene*>(m_pScene);
}

void CSceneMgr::Stage_Change(STAGE _Stage)
{
	switch (_Stage)
	{
	case STAGE_TUTORIAL:
		m_pScene = new CTutorialStage;
		break;
	case STAGE_1:
		m_pScene = new CStage01;
		break;
	case STAGE_2:
		m_pScene = new CStage02;
		break;
	case STAGE_END:
		//앤딩크래딧 넣기.
		break;
	}
}

