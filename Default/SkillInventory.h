#pragma once
#include "Tornado.h"
#include "Storm.h"
#include "Explosion.h"
#include "ObjMgr.h"
#include "BmpMgr.h"
#include "Skill.h"
#include "Obj.h"
#include "DragonianSword.h"
//Èì,,,
//class CSkillInventory;
//typedef void (CSkillInventory::*pFunc) ();
class CSkillInventory
{
public:
	CSkillInventory();
	~CSkillInventory();

	
public:
	void		Initialize(void);
	//int			Update(void);
	//void		Late_Update(void);
	//void		Render(HDC hDC);
	//void		Release(void);
	enum SKILL {
		SKILL_STORM,
		SKILL_TORNADO,
		SKILL_EXPLOSION,
		SKILL_END
	};

public:
	

private:
	void		Create_Storm();
	void		Create_Tornado();
	void		Create_Explosion();

	void		Create_Sword_Throwing();
	void		Create_Sword_vertical();
	void		Create_Sword_Rain();

private:
	//UI·Î •û¾ßÇÏ³ª?
	
};

