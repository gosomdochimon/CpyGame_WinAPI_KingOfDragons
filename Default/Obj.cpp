#include "stdafx.h"
#include "Obj.h"


CObj::CObj() : m_fSpeed(0.f), m_eDir(DIR_END), m_bDead(false), m_fAngle(0.f), m_fDistance(0.f), m_pTarget(nullptr), m_pFrameKey(L""),
m_fShadowRadius(0.f), m_bIsCol(false), m_eUnbeatable(NOHITED), m_iAtk(0),
m_bJump(false), m_iDamage(0), m_fSound(1.f), m_bTutoMove(false), m_bCanInput(false)
{
	ZeroMemory(&m_tInfo, sizeof(INFO));
	ZeroMemory(&m_tRect, sizeof(RECT));
	ZeroMemory(&m_tFrame, sizeof(FRAME));

	ZeroMemory(&m_tShadowInfo, sizeof(INFO));
	ZeroMemory(&m_tShadowRect, sizeof(FRAME));

	ZeroMemory(&m_tColInfo, sizeof(INFO));
	ZeroMemory(&m_tColRect, sizeof(FRAME));

}

CObj::~CObj()
{
}

void CObj::Update_Rect(void)
{
	m_tRect.left	= int(m_tInfo.fX - (m_tInfo.fCX * 0.5f));
	m_tRect.top		= int(m_tInfo.fY - (m_tInfo.fCY * 0.5f));
	m_tRect.right	= int(m_tInfo.fX + (m_tInfo.fCX * 0.5f));
	m_tRect.bottom	= int(m_tInfo.fY + (m_tInfo.fCY * 0.5f));
}

void CObj::Move_Frame(void) // 점프용 프레임 함수 추가나 수정 필요
{
	if (m_tFrame.dwFrameTime + m_tFrame.dwFrameSpeed < GetTickCount())
	{
		++m_tFrame.iFrameStart;

		if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
			m_tFrame.iFrameStart = 0;

		m_tFrame.dwFrameTime = GetTickCount();
	}

}

void CObj::Update_ShadowRect(void)
{
	m_tShadowRect.top = m_tShadowInfo.fY - m_tShadowInfo.fCY*0.5f;
	m_tShadowRect.bottom = m_tShadowInfo.fY + m_tShadowInfo.fCY*0.5f;
	m_tShadowRect.left = m_tShadowInfo.fX - m_tShadowInfo.fCX*0.5f;
	m_tShadowRect.right = m_tShadowInfo.fX + m_tShadowInfo.fCX*0.5f;
}

void CObj::Update_ColRect(void)
{
	m_tColRect.left = m_tInfo.fX - m_tColInfo.fCX*0.5f;
	m_tColRect.top = m_tInfo.fY - m_tColInfo.fCY *0.5f;
	m_tColRect.right = m_tInfo.fX + m_tColInfo.fCX * 0.5f;
	m_tColRect.bottom = m_tInfo.fY + m_tColInfo.fCY * 0.5f;

}


