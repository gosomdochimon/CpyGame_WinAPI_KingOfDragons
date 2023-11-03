#include "stdafx.h"
#include "SkillInventory.h"
#include "AbstractFactory.h"
#include "SkillMgr.h"
CSkillInventory::CSkillInventory()
{
	
}


CSkillInventory::~CSkillInventory()
{
}

void CSkillInventory::Initialize(void)
{
	CSkillMgr::Get_Instance()->Insert_Skill(L"STORM", &CSkillInventory::Create_Storm);
	CSkillMgr::Get_Instance()->Insert_Skill(L"TORNADO", &CSkillInventory::Create_Tornado);
	CSkillMgr::Get_Instance()->Insert_Skill(L"EXPLOSION", &CSkillInventory::Create_Explosion);
	CSkillMgr::Get_Instance()->Insert_Skill(L"SWORD_RAIN", &CSkillInventory::Create_Sword_Rain);
}

void CSkillInventory::Create_Storm()
{
	
	for (unsigned int i = 0; i < CObjMgr::Get_Instance()->Get_Monster().size(); ++i)
	{
		CObjMgr::Get_Instance()->Add_Object(OBJ_SKILL, CAbstractFactory<CStorm>::Create(200, 300, DIR_END));
	}
	CObjMgr::Get_Instance()->Set_Storm_Pos();
}

void CSkillInventory::Create_Tornado()
{
	//CObjMgr::Get_Instance()->Add_Object(OBJ_SKILL, CAbstractFactory<CTornado>::Create(m_tShadowInfo.fX, m_tShadowInfo.fY, m_eDir));
	CObjMgr::Get_Instance()->Add_Object(OBJ_SKILL, CAbstractFactory<CTornado>::Create(CObjMgr::Get_Instance()->Get_Player()->Get_ShadowInfo().fX, CObjMgr::Get_Instance()->Get_Player()->Get_ShadowInfo().fY, CObjMgr::Get_Instance()->Get_Player()->Get_Dir()));
}

void CSkillInventory::Create_Explosion()
{
	for (int i = 0; i < 6; ++i)
		{
			float X = float(CObjMgr::Get_Instance()->Get_Player()->Get_ShadowInfo().fX + 50 * cosf(i*60 * PI / 180.f));
			float Y = float(CObjMgr::Get_Instance()->Get_Player()->Get_ShadowInfo().fY - 50 * sinf(i*60 * PI / 180.f));

			CObjMgr::Get_Instance()->Add_Object(OBJ_SKILL, CAbstractFactory<CExplosion>::Create(X, Y, DIR_END));
		}
}
void CSkillInventory::Create_Sword_Throwing()
{
	/*CObjMgr::Get_Instance()->Add_Object(OBJ_SKILL, CAbstractFactory<CDragonianSword>::Create(m_tInfo.fX, m_tInfo.fY + 20, m_eDir));
	dynamic_cast<CDragonianSword*>(CObjMgr::Get_Instance()->Get_SkillList().back())->Set_IsRot(true);*/
}
void CSkillInventory::Create_Sword_vertical()
{
	//CObjMgr::Get_Instance()->Add_Object(OBJ_SKILL, CAbstractFactory<CDragonianSword>::Create(m_tInfo.fX, m_tInfo.fY + 20, m_eDir));
}
void CSkillInventory::Create_Sword_Rain()
{
	for (int i = 0; i < 10; ++i)
	{
		float fRx = rand() % 700 + 1300;
		float fRy = rand() % 200 + 250;
		CObjMgr::Get_Instance()->Add_Object(OBJ_SKILL, CAbstractFactory<CDragonianSword>::Create(fRx, fRy, DIR_DOWN));
		CObjMgr::Get_Instance()->Get_SkillList().back()->Set_Pos(fRx, fRy - 400.f);
	}
}
//
//void CSkillInventory::Insert_Skill(const TCHAR * SkillName, void(CSkillInventory:: *Fun)(void))
//{
//	auto		iter = find_if(m_mapSkill_Create.begin(), m_mapSkill_Create.end(), CTag_Finder(SkillName));
//
//	if (iter == m_mapSkill_Create.end())
//	{
//		m_mapSkill_Create.emplace(SkillName, Fun);
//	}
//
//	CSkill want_Skill = SkillManager::GetInstance()->findSkill(skillId);
//}
//
//void CSkillInventory::Use_Skill(const TCHAR * SkillName)
//{
//	auto		iter = find_if(m_mapSkill_Create.begin(), m_mapSkill_Create.end(), CTag_Finder(SkillName));
//
//	if (iter == m_mapSkill_Create.end())
//		return;
//
//	void(CSkillInventory::*fp)();
//
//	pFunc Func = iter->second;
//
//	(this->*Func)();
//
//	return;
//}
