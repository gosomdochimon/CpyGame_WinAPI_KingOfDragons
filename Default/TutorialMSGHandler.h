#pragma once
#include "Tutorial_UI.h"
#include "Player.h"
#include "UI.h"
#include "Obj.h"
class CTutorialMSGHandler
{
private:
	CTutorialMSGHandler();
	~CTutorialMSGHandler();

public:
	typedef void (CTutorial_UI::* pFunc) (void);
	typedef void (CObj::* PlayerFunc)(void);

	void	Insert_CanInput(const TCHAR* _MSG, void (CObj::* Func)(void));
	void	Insert_MOVE(const TCHAR* _MSG, void (CObj::* Func)(void));
	void	Insert_MSG(const TCHAR* _MSG, void (CTutorial_UI::* Func)(void));
	void	Change_MSG(const TCHAR* _MSG);
	void	Set_PlayerMove();
	void	Set_CanInput();
	void	Set_Player(CObj* _Player) { m_pPlayer = _Player; }
	void	Set_UI(CTutorial_UI* _UI) { m_pUI = _UI; }
public:
	

	static		CTutorialMSGHandler* Get_Instance(void)
	{
		if (nullptr == m_pInstance)
			m_pInstance = new CTutorialMSGHandler;

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

private:
	static CTutorialMSGHandler* m_pInstance;
	map<const TCHAR*, pFunc>	m_mapMSG;

	CObj* m_pPlayer;
	CTutorial_UI* m_pUI;
	PlayerFunc m_pf;
	PlayerFunc m_pcan;
};

