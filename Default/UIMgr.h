#pragma once
#include "Include.h"

class CUI;

class CUIMgr
{
private:
	CUIMgr();
	~CUIMgr();
public:
	int			Update(void);
	void		Late_Update(void);
	void		Render(HDC hDC);
	void		Release(void);
public:
	void		Add_UI(UIID eID, CUI* pObj);
	CUI*		Get_SkillUI() { return m_listUI[UI_SKILL].front(); }
	void		Delete_ID(UIID eID);
public:

private:
	list<CUI*> m_listUI[UI_END];


public:
	static		CUIMgr*		Get_Instance(void)
	{
		if (nullptr == m_pInstance)
			m_pInstance = new CUIMgr;

		return m_pInstance;
	}

	static		void			Destroy_Instance(void)
	{
		if (nullptr != m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}


private:
	static		CUIMgr*		m_pInstance;
};

