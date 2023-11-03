#pragma once
#include "Obj.h"
#include "SoundMgr.h"
#include "BmpMgr.h"
#include "ScrollMgr.h"
class CColumn :
	public CObj
{
public:
	CColumn();
	virtual ~CColumn();

public:
	virtual void Initialize(void) override;
	virtual int Update(void) override;
	virtual void	Late_Update(void);
	virtual void Render(HDC hDC) override;
	virtual void Release(void) override;
};

