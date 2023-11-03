#pragma once
#include "Obj.h"
class CMonster : public CObj
{

public:
	CMonster();
	virtual ~CMonster();

public:
	virtual void Initialize(void) override;
	virtual int Update(void) override;
	virtual void Late_Update(void) override;
	virtual void Render(HDC hDC) override;
	virtual void Release(void) override;


public:
	MONSTERID	Get_MonsterID(void) { return m_eMonsterID; }
	void		Set_PhaseX(PHASEX _PhaseX) { m_tPhaseX = _PhaseX; }
protected:
	virtual void		Attacking(void) PURE;
	virtual	void		Offset(void) PURE;
	virtual void		Insert_Bmp(void) PURE;
	virtual void		Motion_Change(void) PURE;
	virtual void		Damaged(void) PURE;
protected:
	void		Guide_Move();
	void		Guide_Enemy();
	void		Update_Shadow(void);

protected:
	//bool				m_bJump;
	bool				m_bAttack;

	MONSTERID			m_eMonsterID;

	DWORD				m_dwAttackTime; //���� ������
	DWORD				m_ReadyTime; //���� �غ�ð�

	float				m_oldY;

	//�¾��� ��
	DWORD				m_BlinkTIme;
	PHASEX				m_tPhaseX;
};

