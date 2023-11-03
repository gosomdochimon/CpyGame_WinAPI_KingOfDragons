#pragma once

#include "Include.h"

class CObj
{
public:
	enum UNBEATABLE { NOHITED, HITED, CHEAT};

public:
	CObj();
	virtual ~CObj();

public:
	void		Set_Pos(float _fX, float _fY) 
	{
		m_tInfo.fX = _fX;
		m_tInfo.fY = _fY;
	}
	void		Set_ShadowPos(float _fX, float _fY)
	{
		m_tShadowInfo.fX = _fX;
		m_tShadowInfo.fY = _fY;
	}
	void		Set_PosX(float _fX) { m_tInfo.fX += _fX; }
	void		Set_PosY(float _fY) { m_tInfo.fY += _fY; }


	void		Set_Dir(DIRECTION eDir)		{ m_eDir = eDir; } //좌 우만 정해놓자.
	void		Set_Dead(void)				{ m_bDead = true; }
	void		Set_Angle(float _fAngle)	{ m_fAngle = _fAngle; }
	void		Set_Target(CObj* _pTarget)	{ m_pTarget = _pTarget; }
	void		Set_FrameKey(TCHAR* _pFrameKey) { m_pFrameKey = _pFrameKey; }

	float&		Get_Speed(void) { return m_fSpeed; }
	bool		Get_Dead(void) { return m_bDead;	}
	DIRECTION	Get_Dir(void) { return m_eDir; }
	const RECT&		Get_Rect(void) const { return m_tRect; }
	const INFO&		Get_Info(void) const { return m_tInfo; }
	void		Set_Col(bool _bCol) { m_bIsCol = _bCol; }
	
	void			Set_CanInput(void) { m_bCanInput = !m_bCanInput; }
	void			Set_TutoMove() { m_bTutoMove = !m_bTutoMove; }
	bool			Get_TutoMove(void) { return m_bTutoMove; }
	
	//그림자 전용 함수


public:
	virtual		void	Initialize(void) PURE;
	virtual		int		Update(void)	PURE;
	virtual		void	Late_Update(void) PURE;
	virtual		void	Render(HDC hDC) PURE;	
	virtual		void	Release(void) PURE;

protected:
	void		Update_Rect(void);
	void		Move_Frame(void);
	//void		Update_Shadow(void);
	void		Update_ShadowRect(void);
	void		Update_ColRect(void);
public:
	void		Set_Damage(int _Atk) { m_iDamage = _Atk; }
	void		Reduce_Hp(int _Atk) { m_tStat.fCurHp -= _Atk; }
	int			Get_Atk(void) { return m_iAtk; }
	STAT		Get_Stat(void) { return m_tStat; }
	void		Set_ColInfoC(float _fCX, float _fCY) { m_tColInfo.fCX = _fCX; m_tColInfo.fCY = _fCY; }
	void		Set_Unbeatable(UNBEATABLE _Unbeatable) { m_eUnbeatable = _Unbeatable; }
	void		Set_ShadowInfo(float _fX, float _fY) { m_tShadowInfo.fX = _fX; m_tShadowInfo.fY; }
	
	const INFO&		Get_ShadowInfo(void) const { return m_tShadowInfo; }
	const RECT&		Get_ShadowRect(void) const { return m_tShadowRect; }
			RECT&		Get_ColRect(void) { return m_tColRect; }
	const UNBEATABLE&		Get_Unbeatable(void) const { return m_eUnbeatable; }
	const RENDERID	Get_RenderID(void) const { return m_eGroup; }

	bool		Get_Jump(void) { return m_bJump; }

protected:
	INFO		m_tInfo;
	RECT		m_tRect;
	DIRECTION	m_eDir;
	FRAME		m_tFrame;

	float		m_fSpeed;
	bool		m_bDead;
	float		m_fAngle;
	float		m_fDistance;
	bool		m_bJump;


	TCHAR*		m_pFrameKey;
	CObj*		m_pTarget;

	bool		m_bIsCol;

	//그림자(실제 충돌처리할 대상)를 위한 변수
	float		m_fShadowRadius;
	INFO		m_tShadowInfo;
	RECT		m_tShadowRect;
	//캐릭터 크기만큼의 Rect형성할것
	RECT		m_tColRect;
	INFO		m_tColInfo;
	//체력
	STAT				m_tStat;
	int					m_iAtk;
	int					m_iDamage; //받는 데미지 넣기
	//무적 추가
	UNBEATABLE		m_eUnbeatable;
	DWORD			m_dwUnbeatTime;

	//랜더 
	RENDERID	m_eGroup;

	//sound
	float m_fSound;

	//튜토리얼 용
	bool				m_bTutoMove;
	bool				m_bCanInput;
};

// 추상 팩토리 패턴(abstract factory pattern)
// : 생성 패턴의 한 방법으로 인스턴스를 만드는 절차를 추상화시키는 패턴

// 이터레이터 패턴(반복자 패턴)
// : 내부 표현 방식은 공개하지 않고 순차적인 접근 방법을 제공하는 패턴
// 동일한 부모를 갖는 오브젝트들을 하나의 컨테이너로 관리하고자 하는 목적 