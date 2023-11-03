#pragma once
#include "Scene.h"
#include "Include.h"
class CEnding :
	public CScene
{
public:
	CEnding();
	virtual ~CEnding();

public:
	virtual void Initialize(void) override;
	virtual int Update(void) override;
	virtual void Late_Update(void) override;
	virtual void Render(HDC hDC) override;
	virtual void Release(void) override;
public:

	void Update_Sun(void);
	void Update_Letter(void);
private:
	//�¾�
	INFO m_tInfo;
	RECT m_tRect;
	INFO m_tPoint;

	float m_fAngle;
	float m_fSpeed;

	//�ӱ۾�
	bool m_bLetter;
	INFO m_tByeInfo;
	RECT m_tByeRect;
};

