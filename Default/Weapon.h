#pragma once
#include "Item.h"
#include "Player.h"

class CWeapon :
	public CObj
{
public:
	enum SWORD{SWORD_1, SWORD_2, SWORD_3, SWORD_END};

public:
	CWeapon();
	virtual ~CWeapon();

public:
	virtual void	Initialize(void) override;
	virtual int		Update(void) override;
	virtual void	Late_Update(void);
	virtual void	Render(HDC hDC) override;
	virtual void	Release(void) override;

public:
	void		Set_Motion(CPlayer::STATE _PlayerState) {  m_PlayerCurState = _PlayerState; }

	void		Motion_Change(void);
	CWeapon::SWORD Get_eSword(void) { return m_CurSword; }
	void		Set_eSword(SWORD _swordID) { m_CurSword = _swordID; }

private:
	void Sword_Change(void);

private:
	CPlayer::STATE m_PlayerCurState;
	CPlayer::STATE m_PlayerPreState;
	CWeapon::SWORD	m_CurSword;
	CWeapon::SWORD	m_PreSword;

	int SwordLv;
};

