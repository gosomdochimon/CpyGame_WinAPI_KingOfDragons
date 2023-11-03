#include "stdafx.h"
#include "UI.h"
#include "ObjMgr.h"
#include "BmpMgr.h"


CUI::CUI() :m_bShow(false)
{
}


CUI::~CUI()
{
}



//void CUI::Insert_Bmp(void)
//{
//	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/Stat_Ui.bmp", L"Stat_UI");
//	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/Go.bmp", L"GO");
//	//CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/Shield_Ui.bmp", L"Stat_UI");
//	//CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/Stat_Ui.bmp", L"Stat_UI");
//}

void CUI::Update_Rect(void)
{
	m_tRect.left = int(m_tInfo.fX - (m_tInfo.fCX * 0.5f));
	m_tRect.top = int(m_tInfo.fY - (m_tInfo.fCY * 0.5f));
	m_tRect.right = int(m_tInfo.fX + (m_tInfo.fCX * 0.5f));
	m_tRect.bottom = int(m_tInfo.fY + (m_tInfo.fCY * 0.5f));
}
