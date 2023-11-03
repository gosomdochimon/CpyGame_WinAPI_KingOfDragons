#pragma once
#include "Bullet.h"
class CArrow :
	public CBullet
{
public:
	CArrow();
	virtual ~CArrow();

public:
	virtual void Initialize(void) override;
	virtual int Update(void) override;
	virtual void Late_Update(void);
	virtual void Render(HDC hDC) override;
	virtual void Release(void) override;

private:
	void	Move();
	void	Update_Shadow();
	void	Motion_Change();
	void	Attacking();

private:
	float m_fCrossroad;
};

