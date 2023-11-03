#pragma once

#include "Obj.h"

class CCollisionMgr
{
public:
	CCollisionMgr();
	~CCollisionMgr();

public:
	static void	Collision_Rect(list<CObj*> _Dest, list<CObj*> _Sour);
	static void	Collision_Rect_Ex(list<CObj*> _Dest, list<CObj*> _Sour);

	static void	Collision_Sphere(list<CObj*> _Dest, list<CObj*> _Sour);
	static bool Check_Sphere(CObj* pDest, CObj* pSour);
	static bool Check_Rect(CObj* pDest, CObj* pSour, float* _pX, float* _pY);

	static void	Collision_AtkRect(CObj* _This, RECT AtkRect, list<CObj*> _Target);
	static void Collision_Skill(CObj* _This, RECT AtkRect, list<CObj*> _Target);
	static bool	Collision_OneAtkRect(CObj* _This, RECT AtkRect, list<CObj*> _Target);
};

