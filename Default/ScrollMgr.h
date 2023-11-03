#pragma once

#include "Include.h"

class CScrollMgr
{
private:
	CScrollMgr();
	~CScrollMgr();

public:
	void		Scroll_Lock(STAGE _CurStage);

public:
	static		CScrollMgr*		Get_Instance(void)
	{
		if (nullptr == m_pInstance)
			m_pInstance = new CScrollMgr;

		return m_pInstance;
	}

	static		void			Destroy_Instance(void)
	{
		if (nullptr != m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}

public:
	float		Get_ScrollX(void)		{ return m_fScrollX; }
	void		Set_ScrollX(float _x)	{ m_fScrollX += _x;  }

	float		Get_ScrollY(void) { return m_fScrollY; }
	void		Set_ScrollY(float _y) { m_fScrollY += _y; }

private:
	static		CScrollMgr*		m_pInstance;

	float		m_fScrollX;
	float		m_fScrollY;


};

