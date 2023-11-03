#pragma once

#include "Include.h"

class CObj;
class CObjMgr
{
private:
	CObjMgr();
	~CObjMgr();

public:
	CObj*		Get_Player() { return m_ObjList[OBJ_PLAYER].front(); }
	CObj*		Get_Target(OBJID eID, CObj* pObj);
	list<CObj*>	Get_Monster() { return m_ObjList[OBJ_MONSTER]; }
	list<CObj*>	Get_PlayerList() { return m_ObjList[OBJ_PLAYER]; }
	list<CObj*> Get_SkillList() { return m_ObjList[OBJ_SKILL]; }
public:
	void		Add_Object(OBJID eID, CObj* pObj);
	void		Delete_ID(OBJID eID);

	int			Update(void);
	void		Late_Update(void);
	void		Render(HDC hDC);
	void		Release(void);

	void		Set_Storm_Pos(void);
	
private:
	list<CObj*>	m_ObjList[OBJ_END];
	//ysorting용 리스트
	list<CObj*> m_RenderSort[RENDER_END];
public:
	static		CObjMgr*		Get_Instance(void)
	{
		if (nullptr == m_pInstance)
			m_pInstance = new CObjMgr;

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
	static		CObjMgr*		m_pInstance;
	//list<CObj*>  
};

