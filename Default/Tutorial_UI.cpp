#include "stdafx.h"
#include "Tutorial_UI.h"
#include "ObjMgr.h"
#include "BmpMgr.h"
#include "TutorialMSGHandler.h"
CTutorial_UI::CTutorial_UI() :m_MSGNum(0)
{
}


CTutorial_UI::~CTutorial_UI()
{
}

void CTutorial_UI::Initialize(void)
{
	CTutorialMSGHandler::Get_Instance()->Insert_MSG(L"TUTORIAL_MSG1", &CTutorial_UI::UI_MSG1);
	CTutorialMSGHandler::Get_Instance()->Insert_MSG(L"TUTORIAL_MSG2", &CTutorial_UI::UI_MSG2);
	CTutorialMSGHandler::Get_Instance()->Insert_MSG(L"TUTORIAL_MSG3", &CTutorial_UI::UI_MSG3);
	CTutorialMSGHandler::Get_Instance()->Insert_MSG(L"TUTORIAL_MSG4", &CTutorial_UI::UI_MSG4);
	CTutorialMSGHandler::Get_Instance()->Insert_MSG(L"TUTORIAL_MSG5", &CTutorial_UI::UI_MSG5);
	CTutorialMSGHandler::Get_Instance()->Insert_MSG(L"TUTORIAL_MSG6", &CTutorial_UI::UI_MSG6);
	CTutorialMSGHandler::Get_Instance()->Insert_MSG(L"TUTORIAL_MSG7", &CTutorial_UI::UI_MSG7);
	CTutorialMSGHandler::Get_Instance()->Insert_MSG(L"TUTORIAL_MSG8", &CTutorial_UI::UI_MSG8);

	m_eGroup = RENDER_BACKGROUND;
	Insert_Bmp();
	m_tInfo = { 400, 150, 600, 100 };
	m_pFrameKey = L"TUTORIAL_MSG1";
	m_eId = UI_TUTORIAL;
}

int CTutorial_UI::Update(void)
{
	Update_Rect();
	return 0;
}

void CTutorial_UI::Late_Update(void)
{
}

void CTutorial_UI::Render(HDC hDC)
{	HDC	hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);

	//HDC hGoDC = CBmpMgr::Get_Instance()->Find_Image(L"GO");
	
		GdiTransparentBlt(hDC, 					// 복사 받을, 최종적으로 그림을 그릴 DC
			100,	// 2,3 인자 :  복사받을 위치 X, Y
			50,
			600,				// 4,5 인자 : 복사받을 가로, 세로 길이
			100,
			hMemDC,							// 비트맵을 가지고 있는 DC
			0,								// 비트맵 출력 시작 좌표, X,Y
			0,
			600,				// 복사할 비트맵의 가로, 세로 길이
			100,
			RGB(255, 0, 255));	// 제거하고자 하는 색상
		
}

void CTutorial_UI::Release(void)
{
}

void CTutorial_UI::Insert_Bmp(void)
{
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/Tutorial_Msg1.bmp", L"TUTORIAL_MSG1");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/Tutorial_Msg2.bmp", L"TUTORIAL_MSG2");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/Tutorial_Msg3.bmp", L"TUTORIAL_MSG3");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/Tutorial_Msg4.bmp", L"TUTORIAL_MSG4");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/Tutorial_Msg5.bmp", L"TUTORIAL_MSG5");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/Tutorial_Msg6.bmp", L"TUTORIAL_MSG6");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/Tutorial_Msg7.bmp", L"TUTORIAL_MSG7");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/Tutorial_Msg8.bmp", L"TUTORIAL_MSG8");



}

void CTutorial_UI::UI_MSG_Change(void)
{
	
}

void CTutorial_UI::UI_MSG1(void)
{
	m_pFrameKey = L"TUTORIAL_MSG1";
	
}

void CTutorial_UI::UI_MSG2(void)
{
	m_pFrameKey = L"TUTORIAL_MSG2";
}

void CTutorial_UI::UI_MSG3(void)
{
	m_pFrameKey = L"TUTORIAL_MSG3"; 
	CTutorialMSGHandler::Get_Instance()->Set_PlayerMove();//움직임
	CTutorialMSGHandler::Get_Instance()->Set_CanInput(); //엔터안됨
}

void CTutorial_UI::UI_MSG4(void)
{
	m_pFrameKey = L"TUTORIAL_MSG4";
}

void CTutorial_UI::UI_MSG5(void)
{
	m_pFrameKey = L"TUTORIAL_MSG5";
}

void CTutorial_UI::UI_MSG6(void)
{
	//CTutorialMSGHandler::Get_Instance()->Set_PlayerMove();//못움직임
	m_pFrameKey = L"TUTORIAL_MSG6";
}

void CTutorial_UI::UI_MSG7(void)
{
	//CTutorialMSGHandler::Get_Instance()->Set_PlayerMove();//움직임
	CTutorialMSGHandler::Get_Instance()->Set_CanInput(); //엔터안됨
	m_pFrameKey = L"TUTORIAL_MSG7";
}

void CTutorial_UI::UI_MSG8(void)
{
	m_pFrameKey = L"TUTORIAL_MSG8";
}
