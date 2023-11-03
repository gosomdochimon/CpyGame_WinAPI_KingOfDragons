#pragma once
#include "Include.h"
#include "SkillInventory.h"


typedef void (CSkillInventory::*pFunc) ();
class CSkillMgr
{
public:
	CSkillMgr();
	~CSkillMgr();
public:
	pFunc		Use_Skill(const TCHAR* _SkillID);
	void		Insert_Skill(const TCHAR* _SkillID, void (CSkillInventory::*Func)(void));
public:
	static		CSkillMgr*		Get_Instance(void)
	{
		if (nullptr == m_pInstance)
			m_pInstance = new CSkillMgr;

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
	//스킬저장용 맵
	map<const TCHAR*, pFunc>		m_mapSkill;
	pFunc Func;
private:
	static		CSkillMgr*		m_pInstance;
	//CSkillInventory Inven;
};

