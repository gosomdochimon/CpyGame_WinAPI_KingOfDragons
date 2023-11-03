#pragma once
#include "UI.h"
class CTutorial_UI :
	public CUI
{
public:
	CTutorial_UI();
	virtual ~CTutorial_UI();

	//enum STATE{ MSG_1, MSG_2, MSG_3, MSG_4, MSG_5, MSG_6, MSG_7, MSG_8 };

public:
	/*virtual void	Initialize(void) override;
	virtual int		Update(void) override;
	virtual void	Late_Update(void);
	virtual void	Render(HDC hDC) override;
	virtual void	Release(void) override;*/
	virtual		void	Initialize(void) override;
	virtual		int		Update(void)	override;
	virtual		void	Late_Update(void) override;
	virtual		void	Render(HDC hDC) override;
	virtual		void	Release(void) override;

public:
	void		Insert_Bmp(void);
private:
	void		UI_MSG_Change(void);
	void		UI_MSG1(void);
	void		UI_MSG2(void);
	void		UI_MSG3(void);
	void		UI_MSG4(void);
	void		UI_MSG5(void);
	void		UI_MSG6(void);
	void		UI_MSG7(void);
	void		UI_MSG8(void);

private:
	int			m_MSGNum;
};

