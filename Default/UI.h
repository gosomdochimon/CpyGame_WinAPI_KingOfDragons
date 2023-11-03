#pragma once
#include "Weapon.h"
#include "Shield.h"
#include "Player.h"
#include "Monster.h"


class CUI
{
public:
	CUI();
	virtual ~CUI();

public:
	virtual		void	Initialize(void) PURE;
	virtual		int		Update(void)	PURE;
	virtual		void	Late_Update(void) PURE;
	virtual		void	Render(HDC hDC) PURE;
	virtual		void	Release(void) PURE;

public:
	//void	Set_GOUi() { m_bShowGo = !m_bShowGo; }
	void	Set_Show(void) { m_bShow = !m_bShow; }
	bool	Get_Show(void) { return m_bShow; }
public:
	//void		Insert_Bmp(void);

protected:
	void Update_Rect(void);
	//칼 방패 캐릭터 유아이 그림 넣기
protected:
	INFO	m_tInfo;
	RECT	m_tRect;
	TCHAR*	m_pFrameKey;

	bool	m_bShow;

	//피통 Rect
	RECT m_tHpRect;

	//랜더 
	RENDERID	m_eGroup;
	UIID		m_eId;
};

