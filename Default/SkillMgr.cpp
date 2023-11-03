#include "stdafx.h"
#include "SkillMgr.h"

CSkillMgr*		CSkillMgr::m_pInstance = nullptr;

CSkillMgr::CSkillMgr()
{
	
}


CSkillMgr::~CSkillMgr()
{
}

pFunc CSkillMgr::Use_Skill(const TCHAR* _SkillID)
{
	auto		iter = find_if(m_mapSkill.begin(), m_mapSkill.end(), CTag_Finder(_SkillID));

	if (iter == m_mapSkill.end())
		return nullptr;

	void(CSkillInventory::*fp)();

	Func = iter->second;

	/*CSkillInventory* Inven = new CSkillInventory;
	((*Inven).*Func)();*/ //<- 가능하다.

	CSkillInventory In;
	(In.*Func)();

	return Func;
}

void CSkillMgr::Insert_Skill(const TCHAR* _SkillID, void(CSkillInventory::* Func)(void))
{
	auto		iter = find_if(m_mapSkill.begin(), m_mapSkill.end(), CTag_Finder(_SkillID));

	if (iter == m_mapSkill.end())
	{
		m_mapSkill.emplace(_SkillID, Func);
	}
}
