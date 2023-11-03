#pragma once

#include "Include.h"

class CLine
{
public:
	CLine();
	CLine(const LINE& tLine);
	CLine(LINEPOINT& tLeft, LINEPOINT& tRight);
	~CLine();

public:
	const LINE&		Get_Line(void) const { return m_tInfo; }

public:
	void		Render(HDC hDC);
	void		Release(void);

private:
	LINE		m_tInfo;
};

