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


	void		Set_Dir(DIRECTION eDir)		{ m_eDir = eDir; } //�� �츸 ���س���.
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
	
	//�׸��� ���� �Լ�


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

	//�׸���(���� �浹ó���� ���)�� ���� ����
	float		m_fShadowRadius;
	INFO		m_tShadowInfo;
	RECT		m_tShadowRect;
	//ĳ���� ũ�⸸ŭ�� Rect�����Ұ�
	RECT		m_tColRect;
	INFO		m_tColInfo;
	//ü��
	STAT				m_tStat;
	int					m_iAtk;
	int					m_iDamage; //�޴� ������ �ֱ�
	//���� �߰�
	UNBEATABLE		m_eUnbeatable;
	DWORD			m_dwUnbeatTime;

	//���� 
	RENDERID	m_eGroup;

	//sound
	float m_fSound;

	//Ʃ�丮�� ��
	bool				m_bTutoMove;
	bool				m_bCanInput;
};

// �߻� ���丮 ����(abstract factory pattern)
// : ���� ������ �� ������� �ν��Ͻ��� ����� ������ �߻�ȭ��Ű�� ����

// ���ͷ����� ����(�ݺ��� ����)
// : ���� ǥ�� ����� �������� �ʰ� �������� ���� ����� �����ϴ� ����
// ������ �θ� ���� ������Ʈ���� �ϳ��� �����̳ʷ� �����ϰ��� �ϴ� ���� 