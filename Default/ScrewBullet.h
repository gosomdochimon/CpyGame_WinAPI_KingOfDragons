#pragma once

#include "Obj.h"

class CScrewBullet : public CObj
{
public:
	CScrewBullet();
	virtual ~CScrewBullet();

public:
	virtual void Initialize(void) override;
	virtual int Update(void) override;
	virtual void Late_Update(void) override;
	virtual void Render(HDC hDC) override;
	virtual void Release(void) override;

private:
	float		m_fRotSpeed;
	float		m_fRotAngle;

	POINT		m_tCenter;

	bool		m_bStart;

};

