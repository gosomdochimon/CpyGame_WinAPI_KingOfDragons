#include "stdafx.h"
#include "NPC.h"
#include "ScrollMgr.h"
#include "ObjMgr.h"
#include "CollisionMgr.h"
#include "BmpMgr.h"
CNPC::CNPC()
{
}


CNPC::~CNPC()
{
}

void CNPC::Initialize(void)
{
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/WereWolf/WereWolf_Arrow.bmp", L"WereWolf_ARROW");
	m_tInfo.fCX = 50.f;
	m_tInfo.fCY = 10.f;
	m_tShadowInfo.fCX = m_tInfo.fCX*0.6f;
	m_tShadowInfo.fCY = m_tShadowInfo.fCX* 0.5f;

	m_pFrameKey = L"WereWolf_ARROW";

	m_tFrame.dwFrameSpeed = 200.f;
	m_tFrame.dwFrameTime = GetTickCount();
	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 0;
	m_tFrame.iMotion = 0;
}

int CNPC::Update(void)
{
	return 0;
}

void CNPC::Late_Update(void)
{
}

void CNPC::Render(HDC hDC)
{
}

void CNPC::Release(void)
{
}
