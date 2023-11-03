#include "stdafx.h"
#include "MyMenu.h"
#include "BmpMgr.h"
#include "AbstractFactory.h"
#include "MyButton.h"
#include "ObjMgr.h"
#include "UIMgr.h"
#include "SelectUI.h"
#include "KeyMgr.h"
#include "SceneMgr.h"
#include "SoundMgr.h"
CMyMenu::CMyMenu()
{
}

CMyMenu::~CMyMenu()
{
	Release();
}

void CMyMenu::Initialize(void)
{
	/*CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Button/Start.bmp", L"Start");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Button/Edit.bmp", L"Edit");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Button/Exit.bmp", L"Exit");*/

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Menu/Select.bmp", L"SELECT");

	CUIMgr::Get_Instance()->Add_UI(UI_SKILL, CAbstractFactory<CSelectUI>::Create_CUI());

 	/*CObj*	pObj = CAbstractFactory<CMyButton>::Create_UI(200.f, 400.f);
	pObj->Set_FrameKey(L"Start");
	CObjMgr::Get_Instance()->Add_Object(OBJ_BUTTON, pObj);

	pObj = CAbstractFactory<CMyButton>::Create_UI(400.f, 400.f);
	pObj->Set_FrameKey(L"Edit");
	CObjMgr::Get_Instance()->Add_Object(OBJ_BUTTON, pObj);

	pObj = CAbstractFactory<CMyButton>::Create_UI(600.f, 400.f);
	pObj->Set_FrameKey(L"Exit");
	CObjMgr::Get_Instance()->Add_Object(OBJ_BUTTON, pObj);*/

}

int CMyMenu::Update(void)
{
	CSoundMgr::Get_Instance()->PlaySound(L"Player Select.mp3", SOUND_BGM, m_fSound);

	if (CKeyMgr::Get_Instance()->Key_Down(VK_LEFT))
	{
		dynamic_cast<CSelectUI*>(CUIMgr::Get_Instance()->Get_SkillUI())->Dec_Pos();
	}
	if (CKeyMgr::Get_Instance()->Key_Down(VK_RIGHT))
	{
		dynamic_cast<CSelectUI*>(CUIMgr::Get_Instance()->Get_SkillUI())->Add_Pos();
	}
	if (CKeyMgr::Get_Instance()->Key_Down(VK_RETURN))
	{
		CSoundMgr::Get_Instance()->StopSound(SOUND_BGM);
		CSceneMgr::Get_Instance()->Scene_Change(SC_STAGE);
	}

	CObjMgr::Get_Instance()->Update();
	CUIMgr::Get_Instance()->Update();
	return 0;
}

void CMyMenu::Late_Update(void)
{
	CObjMgr::Get_Instance()->Late_Update();
}

void CMyMenu::Render(HDC hDC)
{
	HDC	hMenuDC = CBmpMgr::Get_Instance()->Find_Image(L"SELECT");


	BitBlt(hDC, 0, 0, WINCX, WINCY, hMenuDC, 0, 0, SRCCOPY);
	
	CObjMgr::Get_Instance()->Render(hDC);
	CUIMgr::Get_Instance()->Render(hDC);
}

void CMyMenu::Release(void)
{
	CUIMgr::Get_Instance()->Delete_ID(UI_SKILL);
	//CObjMgr::Get_Instance()->Delete_ID(OBJ_BUTTON);
}
