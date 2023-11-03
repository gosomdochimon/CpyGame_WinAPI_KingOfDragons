#pragma once
#include "Scene.h"
#include "UI.h"
#include "SkillInventory.h"

class CStage :
	public CScene
{
public:
	enum PHASE { PHASE_START, PHASE_1, PHASE_2,PHASE_BOSS, PHASE_END};

	

public:
	CStage();
	virtual ~CStage();

public:
	virtual void Initialize(void) override;
	virtual int Update(void) override;
	virtual void Late_Update(void) override;
	virtual void Render(HDC hDC) override;
	virtual void Release(void) override;

public:

	void		Lock_Monster(void);

	void		Set_Stage(STAGE _Stage) { m_Stage = _Stage; }//�ʿ������
	STAGE		Get_Stage(void) { return m_Stage; }
	
	bool		Get_PhaseLock(void) { return m_bPhaseLock; }
	PHASEX		Get_PhaseX(void) { return m_tPhaseX; }

	bool		Get_BossDead(void) { return m_bBossDead; }
	bool		Set_BossDead(bool _bDead) { m_bBossDead = _bDead; }

protected:
	virtual void		Spawn_Monster(void);
	virtual void		Change_Phase(void);
	void		Lock_Player(void);
	void		Phase_Lock(void);
protected:
	
	CUI* StatUI;

	bool	m_bPhaseLock; //������ �ɸ������� ���ɸ��� 
	bool	m_bStageEnd;	//���� ���̸� true
	STAGE	m_Stage;
	PHASE	m_Phase;
	int		m_iPhaseCount;
	
	//
	bool m_bBossDead;

	PHASEX m_tPhaseX;

	DWORD	m_dwSpawnTime;
	int		m_iSpawnCount;
	int		m_iSpawnMax;

	//������ ���� x �ޱ�
	float m_fStartX[3];
	//�������� ��ǥ
	PHASEX	m_tMapLockX;
	PHASEX  m_tMapLockY;

	//sound
	float m_fSound;
	bool  m_bBgmPlay;

	//
	CSkillInventory* SKillInven;
};
//���� ������ ���� �� Phase Lock = true�� �ǰ�, ȭ���� ��״°� ����ؾ��ұ� 
//��ũ�ѸŴ������� SetScroll�� ��� ����ġ ����, bool���� ��ũ�ѸŴ�������
