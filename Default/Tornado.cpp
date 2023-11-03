#include "stdafx.h"
#include "Tornado.h"
#include "BmpMgr.h"
#include "ObjMgr.h"
#include "ScrollMgr.h"
#include "CollisionMgr.h"

CTornado::CTornado()
{
}


CTornado::~CTornado()
{
}

void CTornado::Initialize(void)
{
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Skill/Skill_Tornado.bmp", L"Skill_Tornado");

	m_tInfo = { 0.f, 0.f, 50.f, 100.f };
	m_tShadowInfo = { 0.f, 0.f, 30.f, 30.f };
	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 8;
	m_tFrame.dwFrameSpeed = 100.f;
	m_tFrame.dwFrameTime = GetTickCount();
	m_tFrame.iMotion = 0;

	m_pFrameKey = L"Skill_Tornado";
	m_eGroup = RENDER_EFFECT;

	m_iAtk = 20;
}

int CTornado::Update(void)
{
	if (m_bDead)
		return OBJ_DEAD;

	if (m_eDir == DIR_RIGHT)
	{
		m_tShadowInfo.fX += 5.f;
	}
	else
	{
		m_tShadowInfo.fX -= 5.f;
	}
	Attacking();
	Update_Shadow();
	Update_Rect();
	Update_ShadowRect();
	return 0;
}

void CTornado::Late_Update(void)
{
	if (m_tFrame.iFrameStart == m_tFrame.iFrameEnd)
	{
		Set_Dead();
	}
	Move_Frame();
}

void CTornado::Render(HDC hDC)
{
	int iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	HDC	hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);


	//Ellipse(hDC, m_tShadowRect.left + iScrollX, m_tShadowRect.top, m_tShadowRect.right + iScrollX, m_tShadowRect.bottom);

	GdiTransparentBlt(hDC, 					// ���� ����, ���������� �׸��� �׸� DC
		int(m_tRect.left + iScrollX),	// 2,3 ���� :  ������� ��ġ X, Y
		int(m_tRect.top + iScrollY),
		int(m_tInfo.fCX),				// 4,5 ���� : ������� ����, ���� ����
		int(m_tInfo.fCY),
		hMemDC,							// ��Ʈ���� ������ �ִ� DC
		m_tFrame.iFrameStart * (int)m_tInfo.fCX,								// ��Ʈ�� ��� ���� ��ǥ, X,Y
		m_tFrame.iMotion * (int)m_tInfo.fCY,
		(int)m_tInfo.fCX,				// ������ ��Ʈ���� ����, ���� ����
		(int)m_tInfo.fCY,
		RGB(255, 0, 255));			// �����ϰ��� �ϴ� ����

}

void CTornado::Release(void)
{
}

void CTornado::Motion_Change(void)
{
}

void CTornado::Insert_Bmp(void)
{
}

void CTornado::Attacking(void)
{
	
	CCollisionMgr::Collision_Skill(this, m_tShadowRect, CObjMgr::Get_Instance()->Get_Monster());
	//�˹��� ��Ű����? ���߿�


}

void CTornado::Update_Shadow()
{
	m_tInfo.fX = m_tShadowInfo.fX;
	m_tInfo.fY = m_tShadowInfo.fY - m_tInfo.fCY*0.45f;
}
