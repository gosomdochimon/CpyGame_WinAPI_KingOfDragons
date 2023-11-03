#pragma once
#include "Stage.h"
class CStage02 :
	public CStage
{
public:
	CStage02();
	virtual ~CStage02();

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

};

