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

	void		Set_Stage(STAGE _Stage) { m_Stage = _Stage; }//필요없ㅇ므
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

	bool	m_bPhaseLock; //페이즈 걸릴때마다 락걸리게 
	bool	m_bStageEnd;	//보스 죽이면 true
	STAGE	m_Stage;
	PHASE	m_Phase;
	int		m_iPhaseCount;
	
	//
	bool m_bBossDead;

	PHASEX m_tPhaseX;

	DWORD	m_dwSpawnTime;
	int		m_iSpawnCount;
	int		m_iSpawnMax;

	//페이즈 시작 x 받기
	float m_fStartX[3];
	//스테이지 좌표
	PHASEX	m_tMapLockX;
	PHASEX  m_tMapLockY;

	//sound
	float m_fSound;
	bool  m_bBgmPlay;

	//
	CSkillInventory* SKillInven;
};
//일정 지점에 갔을 때 Phase Lock = true로 되고, 화면을 잠그는걸 어떻게해야할까 
//스크롤매니저에서 SetScroll로 페즈별 락위치 지정, bool값을 스크롤매니저에서
