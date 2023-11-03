#pragma once
#include "UI.h"
class CSelectUI :
	public CUI
{
public:
	CSelectUI();
	virtual ~CSelectUI();

public:
	virtual		void	Initialize(void) override;
	virtual		int		Update(void)	override;
	virtual		void	Late_Update(void) override;
	virtual		void	Render(HDC hDC) override;
	virtual		void	Release(void) override;

public:
	void	Add_Pos(void) { if (m_iCurPos >= 4) { m_iCurPos = 4; } else { ++m_iCurPos; } }
	void	Dec_Pos(void) { if (m_iCurPos <= 0) { m_iCurPos = 0; } else { --m_iCurPos; } }
	int		Get_Pos(void) { return m_iCurPos; }
private:
	void Update_Pos(void);
	void Init_Pos(void);
private:
	INFO m_PosArr[5];
	int		m_iCurPos;
};

