#pragma once
#include "Monster.h"
class CMinotaur :
	public CMonster
{
public:
	enum STATE {SPAWN, IDLE, WALK, ATTACK, SMASHREADY,SMASH, JUMPING, JUMPINGEND, DAMAGED, DEAD};

public:
	CMinotaur();
	virtual ~CMinotaur();

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
	void		Action();
	void		Jumping(void);
	void		Capture_Pos();
	void		Check_Dir(void);
private:
	bool				m_bBlocking;
	bool				m_bSmashReady;
	bool				m_bJumpingEnd;
	DWORD				m_AtkDelay;
	DWORD				m_ChangeTime;
	DWORD				m_dwSpawnTime;
	STATE				m_ePreState;
	STATE				m_eCurState;

	float				m_fJumpPower;
	float				m_fTime;
	
	float				m_fLastPosX, m_fLastPosY;
	int					m_iCount; //점프 카운트

	int					m_iDamageCount; // 데미지 누적 카운트
	//
	RECT				m_Atk_Rect;
};

