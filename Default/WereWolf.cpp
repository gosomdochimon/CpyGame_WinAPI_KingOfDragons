#include "stdafx.h"
#include "WereWolf.h"
#include "BmpMgr.h"
#include "ScrollMgr.h"
#include "ObjMgr.h"
#include "CollisionMgr.h"
#include "AbstractFactory.h"
#include "Arrow.h"
#include "SoundMgr.h"
CWereWolf::CWereWolf() : m_bMoveFrame(true)
{
}


CWereWolf::~CWereWolf()
{
}

void CWereWolf::Initialize(void)
{
	m_tShadowInfo.fX = 600.f;
	m_tShadowInfo.fY = 300.f;
	m_tInfo.fCX = 120.f;
	m_tInfo.fCY = 90.f;
	m_tShadowInfo.fCX = m_tInfo.fCX*0.6f;
	m_tShadowInfo.fCY = m_tShadowInfo.fCX* 0.5f;

	//스텟 공격력 조정
	m_tStat = { 20.f, 20.f };
	m_iAtk = 10;
	m_fSpeed = 2.0f;
	Insert_Bmp();

	m_eDir = DIR_RIGHT;
	m_eCurState = WALK;

	m_tFrame.dwFrameSpeed = 200.f;
	m_tFrame.dwFrameTime = GetTickCount();
	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 3;
	m_tFrame.iMotion = 0;
	m_pFrameKey = L"WereWolf_RIGHT";

	//m_dwSpawnTime = GetTickCount();
	m_AtkDelay = GetTickCount();
	m_ChangeTime = GetTickCount();
	m_pTarget = CObjMgr::Get_Instance()->Get_Player();
	m_bIsCol = false;
	m_bIsAtk = false;

	//몬스터 아이디 지정
	m_eMonsterID = MONSTER_GENERAL;
	//render아이디 지정
	m_eGroup = RENDER_GAMEOBJECT;
	m_RunTarget = nullptr;

}

int CWereWolf::Update(void)
{
	if (m_bDead)
		return OBJ_DEAD;

	Move();
 	Update_Shadow();
	Update_Rect();
	Update_ShadowRect();
	return 0;
}

void CWereWolf::Late_Update(void)
{

	Damaged();
	Motion_Change();
	if (m_bMoveFrame)
	{
		Move_Frame();
	}

}

void CWereWolf::Render(HDC hDC)
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

void CWereWolf::Release(void)
{
}

void CWereWolf::Attacking(void)
{
}

void CWereWolf::Offset(void)
{
}

void CWereWolf::Insert_Bmp(void)
{
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/WereWolf/WereWolf_Left.bmp", L"WereWolf_LEFT");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/WereWolf/WereWolf_Right.bmp", L"WereWolf_RIGHT");
}

void CWereWolf::Motion_Change(void)
{
	if (m_ePreState != m_eCurState)
	{
		switch (m_eCurState)
		{
		case IDLE:
			m_tFrame.dwFrameSpeed = 200.f;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 0;
			m_tFrame.iMotion = 0;
			break;
		case WALK:
			m_tFrame.dwFrameSpeed = 200.f;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 4;
			m_tFrame.iMotion = 1;
			break;
		case ATTACK:
			m_tFrame.dwFrameSpeed = 200.f;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 3;
			m_tFrame.iMotion = 0;
			break;
		case DAMAGED:
			m_tFrame.dwFrameSpeed = 200.f;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 3;
			m_tFrame.iMotion = 2;
			break;
		case DEAD:
			m_tFrame.dwFrameSpeed = 200.f;
			m_tFrame.dwFrameTime = GetTickCount();
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 3;
			m_tFrame.iMotion = 2;
			break;
		}
		m_ePreState = m_eCurState;
	}

}

void CWereWolf::Damaged(void)
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
}

void CWereWolf::Move()
{
	Check_Dir();
	switch (m_eCurState)
	{
	case IDLE:
		if (m_dwStateChange + 500 < GetTickCount())
		{
			m_dwStateChange = GetTickCount();
			m_eCurState = WALK;
		}

		break;
	case WALK:
		if (m_AtkDelay + 2000 < GetTickCount())
		{ // 공격준비가 되었을 시.
			WalkForAtk();
		}
		else if (m_AtkDelay + 2000 > GetTickCount())
		{// 공격준비가 안되었을 시
			if (!m_RunTarget)
			{
				if (!(CObjMgr::Get_Instance()->Get_Monster().size() == 1))
				{
					RunToEnemy();// 가장 가까운 타겟까지설정 완료.
				}
				else {
					if (m_eDir == DIR_LEFT)
					{
						m_tShadowInfo.fX -= m_fSpeed;
					}
					else if (m_eDir == DIR_RIGHT)
					{
						m_tShadowInfo.fX += m_fSpeed;
					}
				}
			}
			else if (m_RunTarget)
			{//타겟이 있다면
				////버그원인 예상. 스크롤값 넘어가서?
				if (m_RunTarget->Get_Dir() == DIR_LEFT)
				{
					float fWidth = m_RunTarget->Get_ShadowInfo().fX -200 - m_tShadowInfo.fX;
					float fHeight = m_RunTarget->Get_ShadowInfo().fY - m_tShadowInfo.fY;

					float fDiagnal = sqrtf(fWidth* fWidth + fHeight*fHeight);

					float fRadian = acosf(fWidth / fDiagnal);

					if (m_tShadowInfo.fY < m_RunTarget->Get_ShadowInfo().fY)
						fRadian = 2 * PI - fRadian;

					m_fAngle = fRadian;
					m_tShadowInfo.fX += m_fSpeed * cosf(m_fAngle);
					m_tShadowInfo.fY -= m_fSpeed * sinf(m_fAngle);
				}
				else if (m_RunTarget->Get_Dir() == DIR_RIGHT)
				{
					float fWidth = m_RunTarget->Get_ShadowInfo().fX + 200 - m_tShadowInfo.fX;
					float fHeight = m_RunTarget->Get_ShadowInfo().fY - m_tShadowInfo.fY;

					float fDiagnal = sqrtf(fWidth* fWidth + fHeight*fHeight);

					float fRadian = acosf(fWidth / fDiagnal);

					if (m_tShadowInfo.fY < m_RunTarget->Get_ShadowInfo().fY)
						fRadian = 2 * PI - fRadian;

					m_fAngle = fRadian;
					m_tShadowInfo.fX += m_fSpeed * cosf(m_fAngle);
					m_tShadowInfo.fY -= m_fSpeed * sinf(m_fAngle);
					
				}
			}
			if (m_dwStateChange + 1000 < GetTickCount())
			{
				m_dwStateChange = GetTickCount();
				m_dwStateChange = IDLE;
			}
		}
		break;
	case ATTACK:
		if (m_tFrame.iFrameStart == 1 && !m_bIsAtk)
		{
			CSoundMgr::Get_Instance()->StopSound(SOUND_ENEMY);
			CSoundMgr::Get_Instance()->PlaySound(L"ShootArrow.mp3", SOUND_ENEMY, m_fSound);
			CObjMgr::Get_Instance()->Add_Object(OBJ_BULLET, CAbstractFactory<CArrow>::Create(m_tShadowInfo.fX, m_tShadowInfo.fY + 2, m_eDir));
			m_bIsAtk = true;
		}
		if (m_tFrame.iFrameStart == m_tFrame.iFrameEnd)
		{
			m_AtkDelay = GetTickCount();
			m_dwStateChange = GetTickCount();
			m_eCurState = IDLE;
			m_bIsAtk = false;
		}

		break;
	case DAMAGED:
		CSoundMgr::Get_Instance()->StopSound(SOUND_ENEMY);
		CSoundMgr::Get_Instance()->PlaySound(L"WolfDamaged.mp3", SOUND_ENEMY, m_fSound);
		if (m_tFrame.iFrameStart == m_tFrame.iFrameEnd)
			m_bMoveFrame = false;

		if (m_eDir == DIR_LEFT)
		{
			m_tShadowInfo.fX -= 1.f;
		}
		else
		{
			m_tShadowInfo.fX += 1.f;
		}
		if (m_ChangeTime + m_tFrame.dwFrameSpeed * 5 < GetTickCount())
		{
			m_eCurState = WALK;
			m_bMoveFrame = true;
		}
		
		break;
	case DEAD:
		CSoundMgr::Get_Instance()->StopSound(SOUND_ENEMY);
		CSoundMgr::Get_Instance()->PlaySound(L"WolfDamaged.mp3", SOUND_ENEMY, m_fSound);
		if (m_tFrame.iFrameStart == m_tFrame.iFrameEnd)
			m_bMoveFrame = false;
		if (m_ChangeTime + m_tFrame.dwFrameSpeed * 5 < GetTickCount())
		{
			Set_Dead();
		}
		break;
	}
}

void CWereWolf::Check_Dir()
{
	if (m_pTarget->Get_ShadowInfo().fX > m_tShadowInfo.fX)
	{
		m_pFrameKey = L"WereWolf_LEFT";
		m_eDir = DIR_LEFT;
	}
	else {
		m_pFrameKey = L"WereWolf_RIGHT";
		m_eDir = DIR_RIGHT;
	}
}

void CWereWolf::WalkForAtk()
{
	switch (m_eDir)
	{
		//애초에 주소값을 받고 있으니까 들어와야하는거 아닌가??
		//복사 당한건가? 복사당한거면 일리있어요. 근데 이미 생성되면그바로밑줄에 위치 초기화해주는데???
	case DIR_LEFT:
		if (m_pTarget->Get_ShadowInfo().fX - 200 > m_tShadowInfo.fX
			&& m_pTarget->Get_ShadowInfo().fX - 250 < m_tShadowInfo.fX)
		{
			if (m_pTarget->Get_ShadowInfo().fY + 5 >= m_tShadowInfo.fY
				&&m_pTarget->Get_ShadowInfo().fY - 5 <= m_tShadowInfo.fY)
			{
				m_dwStateChange = GetTickCount();
				m_eCurState = ATTACK;
			}
			else {
				if (m_pTarget->Get_ShadowInfo().fY + 5 <= m_tShadowInfo.fY)
					m_tShadowInfo.fY -= m_fSpeed;
				else if (m_pTarget->Get_ShadowInfo().fY - 5 >= m_tShadowInfo.fY)
					m_tShadowInfo.fY += m_fSpeed;
			}
		}
		else if (m_pTarget->Get_ShadowInfo().fX - 200 < m_tShadowInfo.fX)
		{
			m_tShadowInfo.fX -= m_fSpeed;
		}
		else if (m_pTarget->Get_ShadowInfo().fX - 250 > m_tShadowInfo.fX)
		{
			m_tShadowInfo.fX += m_fSpeed;
		}
		break;
	case DIR_RIGHT:
		if (m_pTarget->Get_ShadowInfo().fX + 200 < m_tShadowInfo.fX
			&& m_pTarget->Get_ShadowInfo().fX + 250 > m_tShadowInfo.fX)
		{
			if (m_pTarget->Get_ShadowInfo().fY + 5 >= m_tShadowInfo.fY
				&&m_pTarget->Get_ShadowInfo().fY - 5 <= m_tShadowInfo.fY)
			{
				m_dwStateChange = GetTickCount();
				m_eCurState = ATTACK;
			}
			else {
				if (m_pTarget->Get_ShadowInfo().fY + 5 <= m_tShadowInfo.fY)
					m_tShadowInfo.fY -= m_fSpeed;
				else if (m_pTarget->Get_ShadowInfo().fY - 5 >= m_tShadowInfo.fY)
					m_tShadowInfo.fY += m_fSpeed;
			}
		}
		else if (m_pTarget->Get_ShadowInfo().fX + 200 > m_tShadowInfo.fX)
		{
			m_tShadowInfo.fX += m_fSpeed;
		}
		else if (m_pTarget->Get_ShadowInfo().fX + 250 < m_tShadowInfo.fX)
		{
			m_tShadowInfo.fX -= m_fSpeed;
		}
		break;
	}

}

void CWereWolf::RunToEnemy()
{
	m_NearList.clear();
	float fleastdig;

	for (auto& iter : CObjMgr::Get_Instance()->Get_Monster())
	{
		if (iter->Get_Dir() == m_eDir)
		{
			m_NearList.push_back(iter);
		}
	}
	for (auto& iter : m_NearList)
	{//가장 가까운 적 체크 버그 원인 자기자신을 제외안시킴
		float Width = fabs(iter->Get_ShadowInfo().fX - m_tShadowInfo.fX);
		float Height = fabs(iter->Get_ShadowInfo().fY - m_tShadowInfo.fY);

		float Diagnal = sqrtf(Width* Width + Height* Height);
		if (iter == this)
			continue;
		if (m_RunTarget == nullptr)
		{
			m_RunTarget = iter;
			fleastdig = Diagnal;
		}
		else if (fleastdig > Diagnal)
		{
			m_RunTarget = iter;
		}
	}

}
