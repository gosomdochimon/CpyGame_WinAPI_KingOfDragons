#include "stdafx.h"
#include "ObjMgr.h"
#include "CollisionMgr.h"
#include "Obj.h"

CObjMgr*		CObjMgr::m_pInstance = nullptr;

CObjMgr::CObjMgr()
{
}


CObjMgr::~CObjMgr()
{
	Release();
}

CObj* CObjMgr::Get_Target(OBJID eID, CObj* pObj)
{
	if (m_ObjList[eID].empty())
		return nullptr;

	CObj*		pTarget = nullptr;
	float		fDistance = 0.f;

	for(auto& iter : m_ObjList[eID])
	{
		if(iter->Get_Dead())
			continue;

		float	fWidth = abs(iter->Get_Info().fX - pObj->Get_Info().fX);
		float	fHeight = abs(iter->Get_Info().fY - pObj->Get_Info().fY);

		float	fDiagonal = sqrtf(fWidth * fWidth + fHeight * fHeight);

		if (!pTarget || (fDistance > fDiagonal))
		{
			pTarget = iter;
			fDistance = fDiagonal;		
		}
	}
	
	return pTarget;
}



void CObjMgr::Add_Object(OBJID eID, CObj* pObj)
{
	if (eID >= OBJ_END || nullptr == pObj)
		return;

	m_ObjList[eID].push_back(pObj);
}

void CObjMgr::Delete_ID(OBJID eID)
{
	for (auto& iter : m_ObjList[eID])
		Safe_Delete(iter);

	m_ObjList[eID].clear();
}

int CObjMgr::Update(void)
{
	int	iEvent = 0;

	for (size_t i = 0; i < OBJ_END; ++i)
	{
		for (auto& iter = m_ObjList[i].begin();
			iter != m_ObjList[i].end();)
		{
			iEvent = (*iter)->Update();

			if (OBJ_DEAD == iEvent)
			{
				Safe_Delete<CObj*>(*iter);
				iter = m_ObjList[i].erase(iter);
			}
			else
				++iter;
		}
	}

	return iEvent;
}

void CObjMgr::Late_Update(void)
{
	for (size_t i = 0; i < OBJ_END; ++i)
	{
		for (auto& iter : m_ObjList[i])
		{
			iter->Late_Update();

			if (m_ObjList[i].empty())
				break;

			RENDERID eID = iter->Get_RenderID();
			m_RenderSort[eID].push_back(iter);

		}
	}

	//CCollisionMgr::Collision_Rect_Ex(m_ObjList[OBJ_MONSTER], m_ObjList[OBJ_PLAYER]);
	//CCollisionMgr::Collision_Sphere(m_ObjList[OBJ_BULLET], m_ObjList[OBJ_MONSTER]);
}

void CObjMgr::Render(HDC hDC)
{
	/*for (size_t i = 0; i < OBJ_END; ++i)
	{
		for (auto& iter : m_ObjList[i])
			iter->Render(hDC);
	}*/
 //YSorting¿ë
	for (size_t i = 0; i < RENDER_END; ++i)
	{
		m_RenderSort[i].sort(CompareY<CObj*>);

		for (auto& iter : m_RenderSort[i])
			iter->Render(hDC);

		m_RenderSort[i].clear();
	}

}

void CObjMgr::Release(void)
{
	for (size_t i = 0; i < OBJ_END; ++i)
	{
		for (auto& iter : m_ObjList[i])
			Safe_Delete<CObj*>(iter);

		m_ObjList[i].clear();
	}
}

void CObjMgr::Set_Storm_Pos(void)
{

	list<CObj*>::iterator  Skill = m_ObjList[OBJ_SKILL].begin();
	for (auto& monster : m_ObjList[OBJ_MONSTER])
	{
		(*Skill)->Set_ShadowPos(monster->Get_ShadowInfo().fX, monster->Get_ShadowInfo().fY);
		if(Skill != m_ObjList[OBJ_SKILL].end())
			Skill++;
	}
	Skill = m_ObjList[OBJ_SKILL].begin();
}
