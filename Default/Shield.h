#pragma once

#include "Player.h"
#include "Item.h"
class CShield : public CItem
{
public:
	enum SHIELD { SHIELD_1, SHIELD_2, SHIELD_3, SHIELD_END };

public:
	CShield();
	virtual ~CShield();

public:
	virtual void Initialize(void) override;
	virtual int Update(void) override;
	virtual void Late_Update(void) override;
	virtual void Render(HDC hDC) override;
	virtual void Release(void) override;
public:
	void		Set_Motion(CPlayer::STATE _PlayerState) { m_PlayerCurState = _PlayerState; }

	void		Motion_Change(void);

	CShield::SHIELD Get_eShield(void) { return m_CurShield; }
	void		Set_eShield(CShield::SHIELD _ShieldID) { m_CurShield = _ShieldID; }
private:
	void Shield_Change(void);
public:
	CPlayer::STATE m_PlayerCurState;
	CPlayer::STATE m_PlayerPreState;
	CShield::SHIELD m_CurShield;
	CShield::SHIELD m_PreShield;
	int ShieldLv;
};

