#pragma once
#include "Monster.h"


class CGreatDragonian :
	public CMonster
{
public:
	CGreatDragonian();
	virtual ~CGreatDragonian();

public:
	enum STATE { 
		SPAWN, 
		IDLE,
		WALK,
		LONG_SWORD, 
		BASIC_ATTACK, 
		SWORD_THROW,// ���̽� ���ݰ� ���� ��������� Į 3�� �����ؼ� ����.
		SWORD_FALL, 
		SWORD_ROT_THROW,
		FLYING,
		FLY_FALL,
		ROTATION,
		DAMAGED, 
		DEAD,
		STATE_END
	};

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
	void	Action();
	void	Update_Range_Rect();
	void	Check_Dir();
	
	void Create_Sword_Rot();
	void Create_Sword_Vertical();
	void Capture_Pos();
private:

	bool				m_bBlocking;
	bool				m_bMoveFrame;
	DWORD				m_dwStateChange; //��ų ���� ����� ����
	DWORD				m_AtkDelay;	//���ݵ�����
	DWORD				m_ChangeTime; //�Ϲ� �����̳� 
	DWORD				m_dwSpawnTime;
	STATE				m_ePreState;
	STATE				m_eCurState;
	//���� ���� 
	DWORD				m_WalkTime;

	int					m_iDamageCount;
	//���ݹ��� Ȯ���ϱ�.
	RECT				m_Atk_Rect;
	RECT				m_tRange_Rect;

	int					iRand;
	int					m_iSwordLength;
	int					m_iPreStartFrame;
	int					m_iAtkCount;
	float				m_fJumpSpeed;
	float				m_fLastPosX, m_fLastPosY;

	STATE				m_eTempState;

	//accel
	float m_fAccel;
	float m_fTime;
};

