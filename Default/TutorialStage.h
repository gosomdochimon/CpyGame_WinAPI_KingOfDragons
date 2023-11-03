#pragma once
#include "Stage.h"
#include "Tutorial_UI.h"
#include "UI.h"
class CTutorialStage :
	public CStage
{
public:
	CTutorialStage();
	virtual ~CTutorialStage();

public:
	virtual void Initialize(void) override;
	virtual int Update(void) override;
	virtual void Late_Update(void) override;
	virtual void Render(HDC hDC) override;
	virtual void Release(void) override;
public:
	virtual void		Change_Phase(void);
	virtual void		Spawn_Monster(void);
private:
	void TutorialFunc(void);

private:
	CTutorial_UI* UI;

	DWORD m_SpawnTime;
	
	
	bool m_bSpawncrow;
	bool m_bTimeStart;
	bool m_bArrow;
	bool m_bStartTutorial;
	float m_fAngle;
	bool m_bMsg7;
};

