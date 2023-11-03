#pragma once
#include "UI.h"
class CMove_UI :
	public CUI
{
public:
	CMove_UI();
	virtual ~CMove_UI();

	virtual		void	Initialize(void) override;
	virtual		int		Update(void)	override;
	virtual		void	Late_Update(void) override;
	virtual		void	Render(HDC hDC) override;
	virtual		void	Release(void) override;
};

