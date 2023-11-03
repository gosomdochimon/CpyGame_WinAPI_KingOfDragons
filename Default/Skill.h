#pragma once
#include "Obj.h"
class CSkill :
	public CObj
{
public:
	CSkill();
	virtual ~CSkill();

public:
	virtual void	Initialize(void) override;
	virtual int		Update(void) override;
	virtual void	Late_Update(void);
	virtual void	Render(HDC hDC) override;
	virtual void	Release(void) override;


protected:
	virtual void		Motion_Change(void) {}
	virtual void		Insert_Bmp(void) {}
	virtual void		Attacking(void) {}
	

};

