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
	//Į ���� ĳ���� ������ �׸� �ֱ�
protected:
	INFO	m_tInfo;
	RECT	m_tRect;
	TCHAR*	m_pFrameKey;

	bool	m_bShow;

	//���� Rect
	RECT m_tHpRect;

	//���� 
	RENDERID	m_eGroup;
	UIID		m_eId;
};

