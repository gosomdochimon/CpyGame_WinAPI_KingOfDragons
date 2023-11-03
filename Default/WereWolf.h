#pragma once
#include "Monster.h"
class CWereWolf :
	public CMonster
{
public:
	CWereWolf();
	virtual ~CWereWolf();

public:
	enum STATE{ IDLE, WALK, ATTACK, DAMAGED, DEAD};

public:
	virtual void Initialize(void) override;
	virtual int Update(void) override;
	virtual void Late_Update(void) override;
	virtual void Render(HDC hDC) override;
	virtual void Release(void) override;

protected:
	virtual void		Attacking(void) override;
	virtual	void		Offset(void) override;
	virtual void		Insert_Bmp(void) override;
	virtual void		Motion_Change(void) override;
	virtual void		Damaged(void) override;

private:
	void	Capture_Pos();
	void	Move();
	void	Check_Dir();
	void	WalkForAtk();
	void	RunToEnemy();
private:
	bool				m_bIsAtk;
	bool				m_bMoveFrame;
	STATE				m_ePreState;
	STATE				m_eCurState;
	DWORD				m_WalkTime;
	DWORD				m_AtkDelay;
	DWORD				m_dwStateChange;
	DWORD				m_ChangeTime;
	float				m_fLastPosX, m_fLastPosY;
	list<CObj*>			m_NearList;
	CObj*				m_RunTarget;

};

