#pragma once
#include "UI.h"

class CWeapon;
class CShield;
class CStat_UI :
	public CUI
{
public:
	CStat_UI();
	virtual ~CStat_UI();

public:
	//virtual void	Initialize(void) override;
	//virtual int		Update(void) override;
	//virtual void	Late_Update(void) override;
	//virtual void	Render(HDC hDC) override;
	//virtual void	Release(void) override;

	virtual		void	Initialize(void) override;
	virtual		int		Update(void)	override;
	virtual		void	Late_Update(void) override;
	virtual		void	Render(HDC hDC) override;
	virtual		void	Release(void) override;

public:
	void		Insert_Bmp(void);


private:
	void CoolTimeUpdate(void);

	void ShieldMotionChange(void);
	void SwordMotionChange(void);
	void CooltimedMotionChange(void);
	void SKillMotionChange(void);
	void Move_Frame(void);
private:
	INFO	m_tInfo;
	RECT	m_tRect;
	TCHAR*		m_pFrameKey;

	CObj* Player;
	CObj* Boss;

	//피통 Rect
	RECT m_tHpRect;
	RECT m_tMpRect;

	//쿨타임Rect
	RECT m_tCoolTimeRect;
	//무기, 방패 Rect
	RECT m_tSwordRect;
	RECT m_tShieldRect;
	//스킬 Rect
	RECT m_tSkillRect;
	const TCHAR* m_pSkillKey;

	FRAME		m_tFrame;
	int			m_iCooltime_Motion;
	int			m_iWeapon_Motion;
	int			m_iShield_Motion;
	CWeapon::SWORD m_eWeapon;
	CShield::SHIELD m_eShield;
};

