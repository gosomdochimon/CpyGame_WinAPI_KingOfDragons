#pragma once

#include "Include.h"
#include "Tile.h"

class CTileMgr
{
private:
	CTileMgr();
	~CTileMgr();

public:
	void Initialize(void) ;
	void Update(void) ;
	void Late_Update(void) ;
	void Render(HDC hDC) ;
	void Release(void) ;

public:
	void Picking_Tile(POINT& _pt, int _iDrawID, int _iOption);
	void Save_Tile(void);
	void Load_Tile(void);
public:
	static		CTileMgr*		Get_Instance(void)
	{
		if (nullptr == m_pInstance)
			m_pInstance = new CTileMgr;

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
	static		CTileMgr*		m_pInstance;
	vector<CObj*>				m_vecTile;


};

