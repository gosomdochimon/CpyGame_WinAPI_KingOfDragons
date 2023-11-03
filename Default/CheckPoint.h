#pragma once
#include "Obj.h"
class CCheckPoint :
	public CObj
{
public:
	CCheckPoint();
	virtual ~CCheckPoint();

public:
	virtual void Initialize(void) override;
	virtual int Update(void) override;
	virtual void	Late_Update(void);
	virtual void Render(HDC hDC) override;
	virtual void Release(void) override;

};

