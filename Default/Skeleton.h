#pragma once
#include "Monster.h"
class CSkeleton :
	public CMonster
{
public:
	enum STATE { SPAWN, WALK, ATTACKREADY, ATTACK, DAMAGED, DEAD };
public:
	CSkeleton();
	virtual ~CSkeleton();

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
	void Move();
private:
	bool				m_bBlocking;

	DWORD				m_AtkDelay;
	DWORD				m_ChangeTime;
	DWORD				m_dwSpawnTime;
	STATE				m_ePreState;
	STATE				m_eCurState;


	float				m_fLastPosX, m_fLastPosY;
	RECT				m_Atk_Rect;
};

