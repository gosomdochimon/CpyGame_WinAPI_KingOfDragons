#include "stdafx.h"
#include "UIMgr.h"
#include "UI.h"
CUIMgr*		CUIMgr::m_pInstance = nullptr;

CUIMgr::CUIMgr()
{
}


CUIMgr::~CUIMgr()
{
}

int CUIMgr::Update(void)
{
	int	iEvent = 0;

	for (size_t i = 0; i < UI_END; ++i)
	{
		for (auto& iter = m_listUI[i].begin();
			iter != m_listUI[i].end();)
		{
			iEvent = (*iter)->Update();

			if (OBJ_DEAD == iEvent)
			{
				Safe_Delete<CUI*>(*iter);
				iter = m_listUI[i].erase(iter);
			}
			else
				++iter;
		}
	}

	return iEvent;
}

void CUIMgr::Late_Update(void)
{
}

void CUIMgr::Render(HDC hDC)
{
	for (size_t i = 0; i < UI_END; ++i)
	{
		for (auto& iter : m_listUI[i])
			iter->Render(hDC);
	}
}

void CUIMgr::Release(void)
{
	for (size_t i = 0; i < UI_END; ++i)
	{
		for (auto& iter : m_listUI[i])
			Safe_Delete<CUI*>(iter);

		m_listUI[i].clear();
	}
}

void CUIMgr::Add_UI(UIID eID, CUI * pObj)
{
	if (eID >= UI_END || nullptr == pObj)
		return;

	m_listUI[eID].push_back(pObj);
}

void CUIMgr::Delete_ID(UIID eID)
{
	for (auto& iter : m_listUI[eID])
		Safe_Delete(iter);

	m_listUI[eID].clear();
}
