#pragma once
#include "Stage.h"
#include "Tutorial_UI.h"

class CStage01 :
	public CStage
{
public:
	CStage01();
	virtual ~CStage01();

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


private:


};

