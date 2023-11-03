#pragma once
#include "Obj.h"

class CShield;
class CWeapon;
class CPlayer :	public CObj
{
public:
	enum STATE	{ IDLE, WALK, ATTACK, BLOCKING, JUMP, DAMAGED, CRY, DEAD, END };

public:
	CPlayer();
	virtual ~CPlayer();
		
public:
	virtual void Initialize(void) override;
	virtual int Update(void) override;
	virtual void	Late_Update(void);
	virtual void Render(HDC hDC) override;
	virtual void Release(void) override;

public:
	CWeapon*		Get_Weapon(void) { return m_pWeapon; }
	CShield*		Get_Shield(void) { return m_pShield; }

	bool		Get_PlayerDead(void) { return m_bPlayerDead; }
	bool		Set_PlayerDead(void) { m_bPlayerDead = !m_bPlayerDead; }
	
	void		Reset_HP(void) { m_tStat.fCurHp = m_tStat.fMaxHp; }

	void		Save_Player(void);
	void		Load_Player(void);

	void		Set_SkillNames(TCHAR* _SkillName[]) { for (int i = 0; i < 3; ++i) { m_Skills[i] = _SkillName[i]; } }
private:
	void		Key_Input(void);
	void		Jumping(void);
	void		Offset(void);
	void		Motion_Change(void);

	void		Update_Shadow(void);
	void		Damaged(void); //공격받았을때
	void		Attacking(void); // 내가 공격했을 때	void		Damaged(void); //충돌 났을 때
	void		Weapon_Update(void);
	void		Shield_Update(void);
	void		Insert_Bmp(void);

	void		Update_MP(void);


private:
	bool				m_bMoveFrame;
	
	bool				m_bAttack;
	bool				m_bBlocking;

	DWORD				m_ManaUpdate;
	DWORD				m_dwAttackTime;
	DWORD				m_BlinkTIme; //무적동안 깜빡이게 하기
	float				m_fJumpPower;
	float				m_fTime;

	STATE				m_ePreState;
	STATE				m_eCurState;

	//추가
	float				m_oldY;
	CWeapon*			m_pWeapon; // Sword
	CShield*			m_pShield; // Shield

	TCHAR*				m_Skills[3];
	
	int					m_iTutoCount;
	//Effect
	int					m_iAtkCount;
	bool				m_bAtkEffect;
	RECT				m_Atk_Rect;//공격 범위
	int					m_iSkill;
	bool				m_bPlayerDead;
	bool				m_bMuJuck;
};

//프로그래밍에서 삼각비를 반환해주는 함수가 있다. 해당 함수들은 매개변수로 각도를 받는다. 하지만 이때 각도는 라디안 값으로 받는다.
//cos(), sin(), tan()

//360 -> 2ㅠr
//180->ㅠr
//90->ㅠr / 2
//1->ㅠr / 180

//45도의 끼인 각을 가진 삼각형의 밑변 / 빗변의 비율을 구하자

//float	m_fAngle = 45.f;	// degree
//cos(m_fAngle * PI / 180.f);