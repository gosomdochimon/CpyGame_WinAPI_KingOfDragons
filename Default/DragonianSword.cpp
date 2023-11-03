#include "stdafx.h"
#include "DragonianSword.h"
#include "BmpMgr.h"
#include "ObjMgr.h"
#include "ScrollMgr.h"
#include "CollisionMgr.h"

CDragonianSword::CDragonianSword() : m_bRot(false), m_preDir(DIR_END), m_dwLifeTime(GetTickCount())
{
}


CDragonianSword::~CDragonianSword()
{
	Release();
}

void CDragonianSword::Initialize(void)
{
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Skill/Dragonian_Sword.bmp", L"Dragonian_SWORD");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Skill/Dragonian_Sword_Shadow.bmp", L"Dragonian_Sword_SHADOW");
	m_tInfo = { 0.f, 100.f, 150.f, 150.f };
	m_tShadowInfo = { 0.f, 0.f, 25.f, 25.f };
	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 5;
	m_tFrame.dwFrameSpeed = 60.f;
	m_tFrame.dwFrameTime = GetTickCount();
	m_tFrame.iMotion = 0;

	m_pFrameKey = L"Dragonian_SWORD";
	m_eGroup = RENDER_GAMEOBJECT;

	m_dwSleepTime = GetTickCount();

	m_iAtk = 5;
	//m_fSpeed = 2.f;
	m_fAccel = 3.f;
	m_fTime = 0.f;
	m_fSpeed = 2.f;
	
}

int CDragonianSword::Update(void)
{//몬스터로부터 방향과 bool값 받아야함
	if (m_bDead)
		return OBJ_DEAD;
	Update_Shadow();
	Update_ShadowRect();
	
	if (!m_bRot && m_dwSleepTime + 1000 > GetTickCount())
		return 0;

	Change_ColRect();
	Move();
	Attacking();
	
	Update_Rect();
	
	Update_ColRect();
	return 0;
}

void CDragonianSword::Late_Update(void)
{
	if (!m_bRot&&m_dwSleepTime + 1000 > GetTickCount())
		return;
	if (m_bRot && m_dwLifeTime + 3000 < GetTickCount())
		Set_Dead();

	Dead_Check();
	Motion_Change();
	Move_Frame();
}

void CDragonianSword::Render(HDC hDC)
{
	int iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	HDC	hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);
	
	//Ellipse(hDC, m_tShadowRect.left + iScrollX, m_tShadowRect.top, m_tShadowRect.right + iScrollX, m_tShadowRect.bottom);
	
	if (!m_bRot && m_dwSleepTime + 1000 > GetTickCount())
		return;

	HDC	hShadowDC = CBmpMgr::Get_Instance()->Find_Image(L"Dragonian_Sword_SHADOW");

	GdiTransparentBlt(hDC, 					// 복사 받을, 최종적으로 그림을 그릴 DC
		int(m_tShadowRect.left + iScrollX),	// 2,3 인자 :  복사받을 위치 X, Y
		int(m_tShadowRect.top + iScrollY),
		int(m_tShadowInfo.fCX),				// 4,5 인자 : 복사받을 가로, 세로 길이
		int(m_tShadowInfo.fCY),
		hShadowDC,							// 비트맵을 가지고 있는 DC
		0 * (int)m_tShadowInfo.fCX,								// 비트맵 출력 시작 좌표, X,Y
		0 * (int)m_tShadowInfo.fCY,
		(int)m_tShadowInfo.fCX,				// 복사할 비트맵의 가로, 세로 길이
		(int)m_tShadowInfo.fCY,
		RGB(255, 0, 255));

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
		RGB(255, 0, 255));
	//Rectangle(hDC, m_tColRect.left + iScrollX, m_tColRect.top, m_tColRect.right + iScrollX, m_tColRect.bottom);
}

void CDragonianSword::Release(void)
{
}

void CDragonianSword::Motion_Change(void)
{
	if (m_preDir != m_eDir)
	{
		switch (m_eDir)
		{
		case DIR_LEFT:
			if (m_bRot)//회전 공격인가?
			{
				m_tFrame.iFrameStart = 0;
				m_tFrame.iFrameEnd = 7;
				m_tFrame.dwFrameSpeed = 60;
				m_tFrame.dwFrameTime = GetTickCount();
				m_tFrame.iMotion = 0;
			}
			else
			{
				m_tFrame.iFrameStart = 0;
				m_tFrame.iFrameEnd = 0;
				m_tFrame.dwFrameSpeed = 60;
				m_tFrame.dwFrameTime = GetTickCount();
				m_tFrame.iMotion = 2;
			}
			break;
		case DIR_RIGHT: //몬스터로부터 받아오기
			if (m_bRot)//회전 공격인가?
			{
				m_tFrame.iFrameStart = 0;
				m_tFrame.iFrameEnd = 7;
				m_tFrame.dwFrameSpeed = 60;
				m_tFrame.dwFrameTime = GetTickCount();
				m_tFrame.iMotion = 0;
			}
			else
			{
				m_tFrame.iFrameStart = 0;
				m_tFrame.iFrameEnd = 0;
				m_tFrame.dwFrameSpeed = 60;
				m_tFrame.dwFrameTime = GetTickCount();
				m_tFrame.iMotion = 1;
			}
			break;
		case DIR_DOWN://칼떨구기용
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 0;
			m_tFrame.dwFrameSpeed = 60.f;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.iMotion = 3;
			break;
		}
		m_preDir = m_eDir;
	}
	

}

void CDragonianSword::Insert_Bmp(void)
{
}

void CDragonianSword::Attacking(void)
{
}

void CDragonianSword::Update_Shadow()
{
	if (m_eDir == DIR_DOWN)
	{
		m_tInfo.fX = m_tShadowInfo.fX;
		//m_tInfo.fY = m_tShadowInfo.fY - m_tInfo.fCY*0.45f;
	}
	else
	{
		m_tInfo.fX = m_tShadowInfo.fX;
		m_tInfo.fY = m_tShadowInfo.fY - m_tInfo.fCY*0.4f;
	}
}

void CDragonianSword::Dead_Check()
{
	RECT rt;
	//LEFT, RIGHT는 x축길게 y축 짧게 그림자로 충돌체크,안맞고 맵밖에 나갈시 삭제
	//DOWN x축짧게 y축 길게,플레이어의 그림자와 칼의 그림자가 겹쳤을 때 플레이어의 충돌 렉트와 bottom부근과충돌체크,
	//아닐시 칼의 바텀과 그림자가 만났을 때 삭제
	

	if (m_eDir == DIR_DOWN)
	{
		m_iAtk = 10;
		if (m_tColRect.bottom -20 >= m_tShadowInfo.fY)
		{
			Set_Dead();
		}
		else if (IntersectRect(&rt, &(CObjMgr::Get_Instance()->Get_Player()->Get_ShadowRect()), &m_tShadowRect))
		{//ColRect
			if (IntersectRect(&rt, &(CObjMgr::Get_Instance()->Get_Player()->Get_ColRect()), &m_tColRect))
			{//여기서 충돌처리 하지말고 Colli에서 할까?귀찮다. 커플링 ㄱ
				CObjMgr::Get_Instance()->Get_Player()->Set_Damage(m_iAtk);
				CObjMgr::Get_Instance()->Get_Player()->Set_Col(true);

				Set_Dead();
			}
		}
	}
	else
	{//DIR_LEFT or DIR_RIGHT
		m_iAtk = 15;
		if (CObjMgr::Get_Instance()->Get_Player()->Get_ShadowRect(). bottom > m_tShadowInfo.fY
			&&CObjMgr::Get_Instance()->Get_Player()->Get_ShadowRect().top < m_tShadowInfo.fY)
		{
			if (IntersectRect(&rt, &(CObjMgr::Get_Instance()->Get_Player()->Get_ShadowRect()), &m_tShadowRect))
			{
				CObjMgr::Get_Instance()->Get_Player()->Set_Damage(m_iAtk);
				CObjMgr::Get_Instance()->Get_Player()->Set_Col(true);

				Set_Dead();
			}
		}
	}
}

void CDragonianSword::Move()
{
	m_fTime += 0.4f;
	if (m_fTime > 4.f)
		m_fTime = 4.f;
	if(!m_bRot)
	{
		switch (m_eDir)
		{
		case DIR_LEFT:
			m_tShadowInfo.fX += m_fSpeed + m_fAccel*m_fTime;;
			break;
		case DIR_RIGHT:
			m_tShadowInfo.fX -= m_fSpeed + m_fAccel*m_fTime;
			break;
		case DIR_DOWN:
			m_tInfo.fY += m_fSpeed + m_fAccel*m_fTime;;
			break;
		}
	}
	else {
		float fWidth = CObjMgr::Get_Instance()->Get_Player()->Get_ShadowInfo().fX - m_tShadowInfo.fX;
		float fHeight = CObjMgr::Get_Instance()->Get_Player()->Get_ShadowInfo().fY - m_tShadowInfo.fY;

		float fDiagnal = sqrtf(fWidth* fWidth + fHeight*fHeight);

		float fRadian = acosf(fWidth / fDiagnal);

		if (m_tShadowInfo.fY < CObjMgr::Get_Instance()->Get_Player()->Get_ShadowInfo().fY)
			fRadian = 2 * PI - fRadian;

		m_fAngle = fRadian;
		m_tShadowInfo.fX += m_fSpeed * cosf(m_fAngle);
		m_tShadowInfo.fY -= m_fSpeed * sinf(m_fAngle);
	}
}

void CDragonianSword::Change_ColRect()
{
	switch (m_eDir)
	{
	case DIR_LEFT:
	case DIR_RIGHT:
		m_tColInfo.fCX = 140.f;
		m_tColInfo.fCY = 20.f;
		break;
	case DIR_DOWN:
		m_tColInfo.fCX = 20.f;
		m_tColInfo.fCY = 140.f;
		
		break;
	}
}
