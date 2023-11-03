#include "stdafx.h"
#include "TutorialMSGHandler.h"

CTutorialMSGHandler* CTutorialMSGHandler::m_pInstance = nullptr;

CTutorialMSGHandler::CTutorialMSGHandler():m_pPlayer(nullptr), m_pUI(nullptr),
m_pf(nullptr), m_pcan(nullptr)
{
}


CTutorialMSGHandler::~CTutorialMSGHandler()
{
}

void CTutorialMSGHandler::Insert_CanInput(const TCHAR * _MSG, void(CObj::* Func)(void))
{
	m_pcan = Func;
}

void CTutorialMSGHandler::Insert_MOVE(const TCHAR * _MSG, void(CObj::* Func)(void))
{
	m_pf = Func;
}

void CTutorialMSGHandler::Insert_MSG(const TCHAR * _MSG, void(CTutorial_UI::* Func)(void))
{
	auto		iter = find_if(m_mapMSG.begin(), m_mapMSG.end(), CTag_Finder(_MSG));

	if (iter == m_mapMSG.end())
	{
		m_mapMSG.emplace(_MSG, Func);
	}
}

void CTutorialMSGHandler::Change_MSG(const TCHAR * _MSG)
{

	auto		iter = find_if(m_mapMSG.begin(), m_mapMSG.end(), CTag_Finder(_MSG));

	if (iter == m_mapMSG.end())
		return;

	pFunc pF = iter->second;
	((*m_pUI).*pF)();
}

void CTutorialMSGHandler::Set_PlayerMove()
{
	((*m_pPlayer).*m_pf)();
}

void CTutorialMSGHandler::Set_CanInput()
{
	((*m_pPlayer).*m_pcan)();
}

