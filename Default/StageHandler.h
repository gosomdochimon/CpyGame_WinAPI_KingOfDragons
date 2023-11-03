#pragma once
#include "Include.h"
#include "Stage.h"

class CStageHandler
{
public:
	CStageHandler();
	~CStageHandler();

public:
	bool	Get_HasStage(void) { if (m_pStage == nullptr) return false; else return true; }
	PHASEX	Get_PhaseX(void) { return m_pStage->Get_PhaseX(); }
	bool	Get_BossDead(void) { return m_pStage->Get_BossDead(); }
	bool	Get_PhaseLock(void) { return m_pStage->Get_PhaseLock(); }

	void	Set_BossDead(bool _bDead) { m_pStage->Set_BossDead(_bDead); }
	void	Set_Stage(CStage* _Stage) { m_pStage = _Stage; }
public:
	static		CStageHandler*		Get_Instance(void)
	{
		if (nullptr == m_pInstance)
			m_pInstance = new CStageHandler;

		return m_pInstance;
	}

	static		void			Destroy_Instance(void)
	{
		if (nullptr != m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}

public:
	static		CStageHandler*		m_pInstance;

private:
	CStage* m_pStage;


};

