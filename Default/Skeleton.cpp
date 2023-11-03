#include "stdafx.h"
#include "Skeleton.h"
#include "BmpMgr.h"
#include "ScrollMgr.h"
#include "ObjMgr.h"
#include "CollisionMgr.h"
CSkeleton::CSkeleton() :m_AtkDelay(GetTickCount())
{
}


CSkeleton::~CSkeleton()
{
}

void CSkeleton::Initialize(void)
{

	m_tShadowInfo.fX = 600.f;
	m_tShadowInfo.fY = 300.f;
	m_tInfo.fCX = 135.f;
	m_tInfo.fCY = 120.f;
	m_tShadowInfo.fCX = m_tInfo.fCX*0.6f;
	m_tShadowInfo.fCY = m_tShadowInfo.fCX* 0.5f;

	//스텟 공격력 조정
	m_tStat = { 30.f, 30.f };
	m_iAtk = 15;
	m_fSpeed = 2.0f;
	Insert_Bmp();

	m_eDir = DIR_RIGHT;
	m_eCurState = SPAWN;
	m_tFrame.dwFrameSpeed = 200.f;
	m_tFrame.dwFrameTime = GetTickCount();
	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 3;
	m_tFrame.iMotion = 0;
	m_pFrameKey = L"Skeleton_LEFT";

	m_dwSpawnTime = GetTickCount();
	m_ChangeTime = GetTickCount();
	m_pTarget = CObjMgr::Get_Instance()->Get_Player();
	m_bIsCol = false;

	//몬스터 아이디 지정
	m_eMonsterID = MONSTER_GENERAL;
	//render아이디 지정
	m_eGroup = RENDER_GAMEOBJECT;
}

int CSkeleton::Update(void)
{
	if (m_bDead)
		return OBJ_DEAD;

	Move();
	Update_Shadow();
	Update_Rect();
	Update_ShadowRect();
	return 0;
}

void CSkeleton::Late_Update(void)
{
	//if (m_bIsCol)
	//{
	//	m_eCurState = DAMAGED;
	//	m_ChangeTime = GetTickCount();
	//	//m_bIsCol = false;
	//	
	//}
	Damaged();
	Motion_Change();
	Move_Frame();
}

void CSkeleton::Render(HDC hDC)
{
	int iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	HDC	hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);

	//그림자
	//Ellipse(hDC, m_tShadowRect.left + iScrollX, m_tShadowRect.top, m_tShadowRect.right + iScrollX, m_tShadowRect.bottom);

	if (m_eUnbeatable == HITED)
	{
		if (m_BlinkTIme + 200 < GetTickCount())
		{
			GdiTransparentBlt(hDC, 					// 복사 받을, 최종적으로 그림을 그릴 DC
				int(m_tRect.left + iScrollX),	// 2,3 인자 :  복사받을 위치 X, Y
				int(m_tRect.top + iScrollY),
				int(m_tInfo.fCX),				// 4,5 인자 : 복사받을 가로, 세로 길이
				int(m_tInfo.fCY),
				hMemDC,							// 비트맵을 가지고 있는 DC
				m_tFrame.iFrameStart * (int)m_tInfo.fCX,								// 비트맵 출력 시작 좌표, X,Y
				m_tFrame.iMotion * (int)m_tInfo.fCY,
				(int)m_tInfo.fCX,				// 복사할 비트맵의 가로, 세로 길이
				(int)m_tInfo.fCY,
				RGB(255, 0, 255));			// 제거하고자 하는 색상
			m_BlinkTIme = GetTickCount();
		}
	}
	else {
		GdiTransparentBlt(hDC, 					// 복사 받을, 최종적으로 그림을 그릴 DC
			int(m_tRect.left + iScrollX),	// 2,3 인자 :  복사받을 위치 X, Y
			int(m_tRect.top + iScrollY),
			int(m_tInfo.fCX),				// 4,5 인자 : 복사받을 가로, 세로 길이
			int(m_tInfo.fCY),
			hMemDC,							// 비트맵을 가지고 있는 DC
			m_tFrame.iFrameStart * (int)m_tInfo.fCX,								// 비트맵 출력 시작 좌표, X,Y
			m_tFrame.iMotion * (int)m_tInfo.fCY,
			(int)m_tInfo.fCX,				// 복사할 비트맵의 가로, 세로 길이
			(int)m_tInfo.fCY,
			RGB(255, 0, 255));			// 제거하고자 하는 색상
	}



	

	
}

void CSkeleton::Release(void)
{
}

void CSkeleton::Attacking(void)
{

	RECT Atk_Rect;
	if (m_eDir == DIR_LEFT)
	{
		Atk_Rect.left = m_tInfo.fX + m_tInfo.fCX*0.5f;
		Atk_Rect.top = m_tInfo.fY - m_tInfo.fCY*0.5f;
		Atk_Rect.right = m_tInfo.fX + m_tInfo.fCX;
		Atk_Rect.bottom = m_tInfo.fY + m_tInfo.fCY*0.5f;
	}
	else
	{
		Atk_Rect.right = m_tInfo.fX - m_tInfo.fCX*0.5f;
		Atk_Rect.top = m_tInfo.fY - m_tInfo.fCY*0.5f;
		Atk_Rect.left = m_tInfo.fX - m_tInfo.fCX;
		Atk_Rect.bottom = m_tInfo.fY + m_tInfo.fCY*0.5f;
	}
	CCollisionMgr::Collision_AtkRect(this, Atk_Rect, CObjMgr::Get_Instance()->Get_PlayerList());

}

void CSkeleton::Offset(void)
{
}

void CSkeleton::Insert_Bmp(void)
{	//Spawn
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/Skeleton/Skeleton_Left.bmp", L"Skeleton_LEFT");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/Skeleton/Skeleton_Right.bmp", L"Skeleton_RIGHT");
}

void CSkeleton::Motion_Change(void)
{


	if (m_ePreState != m_eCurState)
	{
		switch (m_eCurState)
		{
		case SPAWN:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 2;
			m_tFrame.iMotion = 0;

			m_tFrame.dwFrameSpeed = 200;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		case WALK:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 5;
			m_tFrame.iMotion = 1;

			m_tFrame.dwFrameSpeed = 200;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		case ATTACKREADY:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 0;
			m_tFrame.iMotion = 2;

			m_tFrame.dwFrameSpeed = 200;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		case ATTACK:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 5;
			m_tFrame.iMotion = 2;

			m_tFrame.dwFrameSpeed = 200;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		case DAMAGED:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 0;
			m_tFrame.iMotion = 3;

			m_tFrame.dwFrameSpeed = 200;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		case DEAD:
			m_tFrame.iFrameStart = 1;
			m_tFrame.iFrameEnd = 1;
			m_tFrame.iMotion = 3;

			m_tFrame.dwFrameSpeed = 200;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		}
		m_ePreState = m_eCurState;
	}
}

void CSkeleton::Damaged(void)
{

	if (m_bIsCol)
	{
		switch (m_eUnbeatable)
		{
		case NOHITED:
		{
			Reduce_Hp(m_iDamage);
			if (m_tStat.fCurHp <= 0.f)
			{
				m_eCurState = DEAD;
				m_eUnbeatable = HITED;
				m_ChangeTime = GetTickCount();
				m_dwUnbeatTime = GetTickCount();
				return;
			}
			m_eCurState = DAMAGED;
			m_eUnbeatable = HITED;
			m_dwUnbeatTime = GetTickCount();
			m_ChangeTime = GetTickCount();
			m_BlinkTIme = GetTickCount();
			break;
		}
		case HITED:
		{
			if (m_dwUnbeatTime + 1500 < GetTickCount())
			{// 1.5초뒤 무적 해제
				m_eUnbeatable = NOHITED;
				m_bIsCol = false;
			}
		}
		//
		}
	}

	//if (m_ChangeTime + m_tFrame.dwFrameSpeed < GetTickCount())
}

void CSkeleton::Move()
{


	switch (m_eCurState)
	{
	case SPAWN:
		//2초 무적추가할 것
		if (m_dwSpawnTime + m_tFrame.dwFrameSpeed*m_tFrame.iFrameEnd < GetTickCount())
		{
			m_eCurState = WALK;
		}

		break;
	case WALK:
		if (m_tShadowInfo.fX < m_pTarget->Get_ShadowInfo().fX)
		{
			m_pFrameKey = L"Skeleton_RIGHT";
			m_eDir = DIR_LEFT;
		}
		else
		{
			m_pFrameKey = L"Skeleton_LEFT";
			m_eDir = DIR_RIGHT;
		}

		if (m_pTarget->Get_ShadowInfo().fY + 5 >= m_tShadowInfo.fY
			&&m_pTarget->Get_ShadowInfo().fY - 5 <= m_tShadowInfo.fY
			&& (m_pTarget->Get_ShadowInfo().fX + m_pTarget->Get_ShadowInfo().fCX >= m_tShadowInfo.fX
				&& m_pTarget->Get_ShadowInfo().fX - m_pTarget->Get_ShadowInfo().fCX <= m_tShadowInfo.fX)
			&& m_AtkDelay + 2000 < GetTickCount())
		{
			m_eCurState = ATTACKREADY;
			m_ReadyTime = GetTickCount();
		}
		else if (m_pTarget->Get_ShadowInfo().fY + 5 >= m_tShadowInfo.fY
			&&m_pTarget->Get_ShadowInfo().fY - 5 <= m_tShadowInfo.fY
			&&  m_AtkDelay + 2000 > GetTickCount())
		{
			if (m_eDir == DIR_RIGHT)
				m_tShadowInfo.fX += m_fSpeed;
			else {
				m_tShadowInfo.fX -= m_fSpeed;
			}
		}
		else {
			Guide_Move();
		}
		break;
	case ATTACKREADY:
		if (m_ReadyTime + 1000 < GetTickCount())
		{
			m_eCurState = ATTACK;
			m_dwAttackTime = GetTickCount();
		}
		break;
	case ATTACK:
		//m_tInfo.fX -= 2.f;
		if (m_dwAttackTime + m_tFrame.dwFrameSpeed*m_tFrame.iFrameEnd < GetTickCount())
		{
			m_eCurState = WALK;
			m_AtkDelay = GetTickCount();
		}
		if (m_tFrame.iFrameStart == 2)
			Attacking();
		break;
	case DAMAGED:
		if (m_eDir == DIR_LEFT)
		{
			m_tShadowInfo.fX -= 1.f;
		}
		else
		{
			m_tShadowInfo.fX += 1.f;
		}
		if (m_ChangeTime + m_tFrame.dwFrameSpeed * 3 < GetTickCount())
			m_eCurState = WALK;
		break;
	case DEAD:
		if (m_eDir == DIR_LEFT)
		{
			m_tShadowInfo.fX -= 1.f;
		}
		else
		{
			m_tShadowInfo.fX += 1.f;
		}
		if (m_ChangeTime +1000 <GetTickCount())
			Set_Dead();

		break;
	}
}
