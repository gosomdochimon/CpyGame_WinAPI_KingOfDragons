#pragma once
#include "Skill.h"
class CDragonianSword :
	public CSkill
{
public:
	CDragonianSword();
	virtual ~CDragonianSword();

public:
	virtual void	Initialize(void) override;
	virtual int		Update(void) override;
	virtual void	Late_Update(void);
	virtual void	Render(HDC hDC) override;
	virtual void	Release(void) override;

public:
	void		Set_IsRot(bool _bRot) { m_bRot = _bRot; }
protected:
	virtual void		Motion_Change(void) override;
	virtual void		Insert_Bmp(void) override;
	virtual void		Attacking(void) override;

private:
	void	Update_Shadow();
	void	Dead_Check();
	void	Move();
	void	Change_ColRect();
private:
	bool	m_bRot;
	DWORD	m_dwSleepTime;
	float	m_fAccel;
	float	m_fTime;

	DIRECTION	m_preDir;
	DWORD	m_dwLifeTime;
};

