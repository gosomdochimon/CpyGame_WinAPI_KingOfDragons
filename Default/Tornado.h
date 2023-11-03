#pragma once
#include "Skill.h"
class CTornado :
	public CSkill
{
public:
	CTornado();
	virtual ~CTornado();

public:
	virtual void	Initialize(void) override;
	virtual int		Update(void) override;
	virtual void	Late_Update(void);
	virtual void	Render(HDC hDC) override;
	virtual void	Release(void) override;
protected:
	virtual void		Motion_Change(void) override;
	virtual void		Insert_Bmp(void) override;
	virtual void		Attacking(void) override;
private:
	void	Update_Shadow();
};

