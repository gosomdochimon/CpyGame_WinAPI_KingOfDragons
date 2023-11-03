#pragma once

#include "Obj.h"
#include "UI.h"
template<typename T>
class CAbstractFactory
{
public:
	static CObj*		Create(void)
	{
		CObj*		pInstance = new T;
		pInstance->Initialize();

		return pInstance;
	}

	static CObj*		Create(float _fX, float _fY, float _fAngle)
	{
		CObj*		pInstance = new T;
		pInstance->Initialize();
		pInstance->Set_ShadowPos(_fX, _fY);
		//pInstance->Set_Angle(_fAngle);	

		return pInstance;
	}

	static CObj*		Create(float _fX, float _fY, DIRECTION eDir = DIR_END)
	{//애초에 여기에 들어와서 나가기전까진저쪽 함수에 들어가지도 못하는데 이미 초기화된상태일텐데
		CObj*		pInstance = new T;
		pInstance->Initialize();
		//pInstance->Set_Pos(_fX, _fY);
		pInstance->Set_ShadowPos(_fX, _fY);
		if(DIR_END != eDir)
			pInstance->Set_Dir(eDir);

		return pInstance;
	}

	static CObj*		Create_UI(float _fX, float _fY)
	{
		CObj*		pInstance = new T;
		pInstance->Initialize();
		pInstance->Set_Pos(_fX, _fY);

		return pInstance;
	}

	static CUI*		Create_CUI(void)
	{
		CUI*		pInstance = new T;
		pInstance->Initialize();
		//pInstance->Set_Pos(_fX, _fY);

		return pInstance;
	}

public:
	CAbstractFactory(){	}
	~CAbstractFactory(){ }
};

