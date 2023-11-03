#pragma once

#include "Logo.h"
#include "MyMenu.h"
#include "MyEdit.h"
#include "Stage.h"

class CSceneMgr
{
private:
	CSceneMgr();
	~CSceneMgr();

public:
	void		Scene_Change(SCENEID eScene);
	void		Update(void);
	void		Late_Update(void);
	void		Render(HDC hDC);
	void		Release(void);

	void		Stage_Change(STAGE _Stage);
	STAGE		Get_CurStage(void) { return m_eCurStage; }
public:
	static		CSceneMgr*		Get_Instance(void)
	{
		if (nullptr == m_pInstance)
			m_pInstance = new CSceneMgr;

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

public:
	void	Set_CurStage(STAGE _Stage) { m_eCurStage = _Stage; }

private:
	static		CSceneMgr*		m_pInstance;
	CScene*						m_pScene;

	SCENEID						m_ePreScene;
	SCENEID						m_eCurScene;
	STAGE							m_eCurStage;
	STAGE							m_ePreStage;
	int							m_iStageCount;
};

