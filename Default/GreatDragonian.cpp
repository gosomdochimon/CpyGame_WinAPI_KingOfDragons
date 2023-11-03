#include "stdafx.h"
#include "GreatDragonian.h"
#include "BmpMgr.h"
#include "ScrollMgr.h"
#include "ObjMgr.h"
#include "CollisionMgr.h"
#include "Stage.h"
#include "AbstractFactory.h"
#include "DragonianSword.h"
#include "SkillMgr.h"
#include "SoundMgr.h"
CGreatDragonian::CGreatDragonian() :m_iDamageCount(0), m_AtkDelay(GetTickCount()),
m_bMoveFrame(true), m_dwStateChange(GetTickCount()), m_iSwordLength(0),
m_iAtkCount(0), m_fAccel(3.f), m_fTime(0.f)
{

}


CGreatDragonian::~CGreatDragonian()
{
}

void CGreatDragonian::Initialize(void)
{
	//그림자
	m_tShadowInfo.fX = 0.f;
	m_tShadowInfo.fY = 0.f;
	//몸체
	m_tInfo.fCX = 900.f;
	m_tInfo.fCY = 240.f;
	//충돌몸체범위
	m_tColInfo.fCX = 200.f;
	m_tColInfo.fCY = 160.f;
	m_tShadowInfo.fCX = m_tColInfo.fCX;
	m_tShadowInfo.fCY = 80.f;

	//스텟 공격력 조정
	m_tStat = { 1000.f, 1000.f };
	m_iAtk = 10;
	m_fSpeed = 2.0f;
	Insert_Bmp();

	m_eDir = DIR_RIGHT;
	m_eCurState = WALK;
	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 5;
	m_tFrame.iMotion = 1;

	m_tFrame.dwFrameSpeed = 200;
	m_tFrame.dwFrameTime = GetTickCount();
	m_pFrameKey = L"Great_Dragonian_RIGHT";

	m_WalkTime = GetTickCount();
	m_dwSpawnTime = GetTickCount();
	m_ChangeTime = GetTickCount();
	m_pTarget = CObjMgr::Get_Instance()->Get_Player();
	m_bIsCol = false;

	//몬스터 아이디 지정
	m_eMonsterID = MONSTER_BOSS;
	//충돌용 렉트사이즈 지정

	m_tStat = { 100, 100 };
	//render아이디 지정
	m_eGroup = RENDER_GAMEOBJECT;

	m_fJumpSpeed = 7.f;
	m_eTempState = STATE_END;
}

int CGreatDragonian::Update(void)
{
	if (m_bDead)
		return OBJ_DEAD;

	Action();
	Attacking();
	Update_Shadow();
	Update_ColRect();
	Update_Rect();
	Update_ShadowRect();
	Update_Range_Rect();
	return 0;
}

void CGreatDragonian::Late_Update(void)
{
	if (m_eCurState == DEAD && m_tFrame.iFrameStart == m_tFrame.iFrameEnd)
		return;
	Damaged();
	Motion_Change();
	if (m_bMoveFrame)
		Move_Frame();
}

void CGreatDragonian::Render(HDC hDC)
{
	int iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	HDC	hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);
	//그림자
	//Ellipse(hDC, m_tShadowRect.left + iScrollX, m_tShadowRect.top, m_tShadowRect.right + iScrollX, m_tShadowRect.bottom);

	HDC	hShadowDC = CBmpMgr::Get_Instance()->Find_Image(L"Great_Dragonian_SHADOW");

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
		RGB(255, 0, 255));			// 제거하고자 하는 색상

	/*TCHAR		szBuff[255] = L"";
	swprintf_s(szBuff, L" hp: %3f count: %3d", m_tStat.fCurHp, m_iDamageCount);
	TextOut(hDC, 500, 30, szBuff, lstrlen(szBuff));*/

	//Rectangle(hDC, m_Atk_Rect.left + iScrollX, m_Atk_Rect.top, m_Atk_Rect.right + iScrollX, m_Atk_Rect.bottom);
	m_Atk_Rect = { 0, 0, 0, 0 };

}

void CGreatDragonian::Release(void)
{
}

void CGreatDragonian::Attacking(void)
{
	switch (m_eCurState)
	{
	case BASIC_ATTACK:
		if (m_tFrame.iFrameStart >= 1 && m_tFrame.iFrameEnd >= m_tFrame.iFrameStart)
		{
			m_iAtk = 10;
			if (m_eDir == DIR_RIGHT)
			{ //수정하자.
				m_Atk_Rect.right = m_tShadowInfo.fX - m_tShadowInfo.fCX*0.5f;
				m_Atk_Rect.top = m_tShadowInfo.fY - m_tShadowInfo.fCY*0.25f;
				m_Atk_Rect.left = m_tShadowInfo.fX - m_tShadowInfo.fCX;
				m_Atk_Rect.bottom = m_tShadowInfo.fY + m_tShadowInfo.fCY*0.5f;
			}
			else
			{
				m_Atk_Rect.left = m_tShadowInfo.fX + m_tShadowInfo.fCX*0.5f;
				m_Atk_Rect.top = m_tShadowInfo.fY - m_tShadowInfo.fCY*0.25f;
				m_Atk_Rect.right = m_tShadowInfo.fX + m_tShadowInfo.fCX;
				m_Atk_Rect.bottom = m_tShadowInfo.fY + m_tShadowInfo.fCY*0.5f;
			}
			CCollisionMgr::Collision_AtkRect(this, m_Atk_Rect, CObjMgr::Get_Instance()->Get_PlayerList());
			break;
		}
		break;

	case LONG_SWORD:
		if (m_eDir == DIR_RIGHT)
		{ //수정하자.
			m_Atk_Rect.right = m_tShadowInfo.fX - m_tShadowInfo.fCX*0.5f;
			m_Atk_Rect.top = m_tShadowInfo.fY - m_tShadowInfo.fCY*0.15f;
			m_Atk_Rect.left = m_tShadowInfo.fX - m_tShadowInfo.fCX*0.5f - 100 * m_iSwordLength;
			m_Atk_Rect.bottom = m_tShadowInfo.fY + m_tShadowInfo.fCY*0.15f;
		}
		else
		{
			m_Atk_Rect.left = m_tShadowInfo.fX + m_tShadowInfo.fCX*0.5f;
			m_Atk_Rect.top = m_tShadowInfo.fY - m_tShadowInfo.fCY*0.15f;
			m_Atk_Rect.right = m_tShadowInfo.fX + m_tShadowInfo.fCX*0.5f + 100 * m_iSwordLength;
			m_Atk_Rect.bottom = m_tShadowInfo.fY + m_tShadowInfo.fCY*0.15f;
		}
		CCollisionMgr::Collision_AtkRect(this, m_Atk_Rect, CObjMgr::Get_Instance()->Get_PlayerList());

		break;

	case ROTATION:
		if (m_tFrame.iFrameStart == 0 && m_iAtkCount == 0)
		{
			m_Atk_Rect.left = m_tShadowInfo.fX - m_tShadowInfo.fCX*0.7f;
			m_Atk_Rect.top = m_tShadowInfo.fY - m_tShadowInfo.fCY*0.4;
			m_Atk_Rect.right = m_tShadowInfo.fX + m_tShadowInfo.fCX*0.7f;
			m_Atk_Rect.bottom = m_tShadowInfo.fY + m_tShadowInfo.fCY*0.4;
		}
		else {
			m_Atk_Rect.left = m_tShadowInfo.fX - m_tShadowInfo.fCX*0.4f;
			m_Atk_Rect.top = m_tShadowInfo.fY - m_tShadowInfo.fCY*0.4;
			m_Atk_Rect.right = m_tShadowInfo.fX + m_tShadowInfo.fCX*0.4f;
			m_Atk_Rect.bottom = m_tShadowInfo.fY + m_tShadowInfo.fCY*0.4;
		}

		CCollisionMgr::Collision_AtkRect(this, m_Atk_Rect, CObjMgr::Get_Instance()->Get_PlayerList());
		break;

	}
}

void CGreatDragonian::Offset(void)
{
}

void CGreatDragonian::Insert_Bmp(void)
{
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/Great_Dragonian/Great_Dragonian_Right.bmp", L"Great_Dragonian_RIGHT");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/Great_Dragonian/Great_Dragonian_Left.bmp", L"Great_Dragonian_LEFT");


	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/Great_Dragonian/Great_Dragonian_Shadow.bmp", L"Great_Dragonian_SHADOW");
}

void CGreatDragonian::Motion_Change(void)
{
	if (m_ePreState != m_eCurState)
	{
		switch (m_eCurState)
		{
		case SPAWN:
			break;
		case IDLE:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 0;
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
		case LONG_SWORD:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 11;
			m_tFrame.iMotion = 2;

			m_tFrame.dwFrameSpeed = 100;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		case BASIC_ATTACK:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 3;
			m_tFrame.iMotion = 3;

			m_tFrame.dwFrameSpeed = 100;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		case SWORD_THROW:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 3;
			m_tFrame.iMotion = 3;

			m_tFrame.dwFrameSpeed = 100;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		case SWORD_FALL:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 2;
			m_tFrame.iMotion = 4;

			m_tFrame.dwFrameSpeed = 100;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		case SWORD_ROT_THROW:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 8;
			m_tFrame.iMotion = 5;

			m_tFrame.dwFrameSpeed = 100;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		case FLYING:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 7;
			m_tFrame.iMotion = 6;

			m_tFrame.dwFrameSpeed = 100;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		case FLY_FALL:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 2;
			m_tFrame.iMotion = 7;

			m_tFrame.dwFrameSpeed = 100;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		case ROTATION:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 3;
			m_tFrame.iMotion = 8;

			m_tFrame.dwFrameSpeed = 100;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		case DAMAGED:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 0;
			m_tFrame.iMotion = 9;

			m_tFrame.dwFrameSpeed = 100;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		case DEAD:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 7;
			m_tFrame.iMotion = 9;

			m_tFrame.dwFrameSpeed = 200;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		}
		m_ePreState = m_eCurState;
	}
}

void CGreatDragonian::Damaged(void)
{
	if (m_bIsCol && !m_bJump)
	{
		switch (m_eUnbeatable)
		{
		case NOHITED:
		{
			Reduce_Hp(m_iDamage);
			m_iDamageCount += m_iDamage;
			if (m_tStat.fCurHp <= 0.f)
			{
				m_tStat.fCurHp = 0.f;
				m_eCurState = DEAD;
				m_eUnbeatable = HITED;
				m_ChangeTime = GetTickCount();
				m_dwUnbeatTime = GetTickCount();
				return;
			}
			if (m_iDamageCount >= 30)
			{
				m_eTempState = m_eCurState;
				m_eCurState = DAMAGED;
				m_eUnbeatable = HITED;
				m_dwUnbeatTime = GetTickCount();
				m_ChangeTime = GetTickCount();
				m_BlinkTIme = GetTickCount();
				m_iDamageCount = 0;
				return;
			}
			m_bIsCol = false;
			break;
		}
		case HITED:
		{
			if (m_dwUnbeatTime + 1000 < GetTickCount())
			{// 2초뒤 무적 해제
				m_eUnbeatable = NOHITED;
				m_bIsCol = false;
			}
		}
		//
		}
	}
}

void CGreatDragonian::Action()
{
	// 대각선 위, 가운데, 대각선 아래
	int iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();
	///////////
	float fRandSpeed = rand() % 5;
	float fRight, fLeft, fUp, fDown;
	float fDir = -1;

	RECT rt;
	std::random_device rd;
	std::mt19937 rand(rd());
	std::uniform_int_distribution <std::mt19937::result_type> uniform_dist(1, 100);
	switch (m_eCurState)
	{
	case SPAWN:
		break;
	case IDLE:
		if (m_WalkTime + 500 < GetTickCount()) {
			m_WalkTime = GetTickCount(); //다시 초기화
			m_eCurState = WALK;
			iRand = rand() % 8;
		}

		break;
	case WALK:
		if (m_dwStateChange + 2000 < GetTickCount())
		{//랜덤으로 행동 변경
			if (1 <= uniform_dist(rand) && uniform_dist(rand) <= 20)
			{// longsword
				if (m_pTarget->Get_ShadowInfo().fY + 5 >= m_tShadowInfo.fY
					&&m_pTarget->Get_ShadowInfo().fY - 5 <= m_tShadowInfo.fY)
				{
					m_iAtkCount = 0;
					m_ChangeTime = GetTickCount();
					m_eCurState = LONG_SWORD;
				}
				else if (m_pTarget->Get_ShadowInfo().fY + 5 <= m_tShadowInfo.fY)
				{
					Check_Dir();
					m_tShadowInfo.fY -= m_fSpeed * 1.2f;

				}
				else if (m_pTarget->Get_ShadowInfo().fY - 5 >= m_tShadowInfo.fY)
				{
					Check_Dir();
					m_tShadowInfo.fY += m_fSpeed* 1.2f;
				}
			}
			else if (21 <= uniform_dist(rand) && uniform_dist(rand) <= 35)
			{//칼떨구기
				m_iAtkCount = 0;
				m_AtkDelay = GetTickCount();
				m_ChangeTime = GetTickCount();
				m_eCurState = SWORD_FALL;
				break;
			}
			else if (36 <= uniform_dist(rand) && uniform_dist(rand) <= 45)
			{//회전칼날
				m_iAtkCount = 0;
				m_ChangeTime = GetTickCount();
				m_eCurState = SWORD_ROT_THROW;
			}
			else if (46 <= uniform_dist(rand) && uniform_dist(rand) <= 75)
			{//날아오르기
				m_ChangeTime = GetTickCount();
				m_eCurState = FLYING;
			}
			else if (75 <= uniform_dist(rand) && uniform_dist(rand) <= 100)
			{
				if (m_pTarget->Get_ShadowInfo().fY + 5 >= m_tShadowInfo.fY
					&&m_pTarget->Get_ShadowInfo().fY - 5 <= m_tShadowInfo.fY)
				{
					m_iAtkCount = 0;
					m_AtkDelay = GetTickCount();
					m_ChangeTime = GetTickCount();
					m_eCurState = SWORD_THROW;
				}
				else if (m_pTarget->Get_ShadowInfo().fY + 5 <= m_tShadowInfo.fY)
				{
					Check_Dir();
					m_tShadowInfo.fY -= m_fSpeed * 1.2f;
					switch (m_eDir)
					{
					case DIR_LEFT:
						m_tShadowInfo.fX -= 1.f;
						break;
					case DIR_RIGHT:
						m_tShadowInfo.fX += 1.f;
					}

				}
				else if (m_pTarget->Get_ShadowInfo().fY - 5 >= m_tShadowInfo.fY)
				{
					Check_Dir();
					m_tShadowInfo.fY += m_fSpeed* 1.2f;
					switch (m_eDir)
					{
					case DIR_LEFT:
						m_tShadowInfo.fX -= 1.f;
						break;
					case DIR_RIGHT:
						m_tShadowInfo.fX += 1.f;
					}

				}
			}
		
		}
		else if (m_dwStateChange + 2000 > GetTickCount())
		{
			if (m_WalkTime + 1500 > GetTickCount())//0.5초마다 움직이도록
			{//나중에 새로운 방안 모색할 것 현재는 따라와서 공격하는 방식.
				Check_Dir();
				if (m_pTarget->Get_ShadowInfo().fY + 5 >= m_tShadowInfo.fY
					&&m_pTarget->Get_ShadowInfo().fY - 5 <= m_tShadowInfo.fY
					&& (m_pTarget->Get_ShadowInfo().fX + m_pTarget->Get_ShadowInfo().fCX >= m_tShadowInfo.fX
						&& m_pTarget->Get_ShadowInfo().fX - m_pTarget->Get_ShadowInfo().fCX <= m_tShadowInfo.fX)
					&& m_AtkDelay + 2000 < GetTickCount())
				{
					m_eCurState = BASIC_ATTACK;
					m_ChangeTime = GetTickCount();
					m_AtkDelay = GetTickCount();
				}
				else if ((m_pTarget->Get_ShadowInfo().fX + m_pTarget->Get_ShadowInfo().fCX >= m_tShadowInfo.fX
					&& m_pTarget->Get_ShadowInfo().fX - m_pTarget->Get_ShadowInfo().fCX <= m_tShadowInfo.fX)
					&& m_AtkDelay + 1000 > GetTickCount())
				{
					m_WalkTime = GetTickCount();
					m_eCurState = IDLE;
					//가만히있는 모션 만들어야하는데
				}
				else {
					Check_Dir();
					Guide_Move();
				}

			}
			else {
				m_WalkTime = GetTickCount();
				m_eCurState = IDLE;
			}
		}
		break;
	case LONG_SWORD:

		if (m_tFrame.iFrameStart >= 5 && m_tFrame.iFrameStart < 8)
		{
			if (m_tFrame.iFrameStart == 5)
			{
				m_iPreStartFrame = 5;
				m_iSwordLength = 0;
			}
			else if (m_iPreStartFrame != m_tFrame.iFrameStart)
			{
				++m_iSwordLength;
				m_iPreStartFrame = m_tFrame.iFrameStart;
			}

		}
		else if (m_tFrame.iFrameStart >= 8 && m_tFrame.iFrameStart <= 10)
		{
			if (m_tFrame.iFrameStart == 8)
			{
				m_iPreStartFrame = 8;
				m_iSwordLength = 3;
			}
			else if (m_iPreStartFrame != m_tFrame.iFrameStart)
			{
				--m_iSwordLength;
				m_iPreStartFrame = m_tFrame.iFrameStart;
			}

		}
		if (m_tFrame.iFrameStart == m_tFrame.iFrameEnd)
		{
			m_dwStateChange = GetTickCount();
			m_bMoveFrame = true;
			m_eCurState = IDLE;
			m_AtkDelay = GetTickCount();
			m_iSwordLength = 0;
			//m_bMoveFrame = false;
		}
		break;
	case BASIC_ATTACK:

		if (m_tFrame.iFrameStart == m_tFrame.iFrameEnd)
		{
			m_bMoveFrame = false;
		}
		if (m_ChangeTime + 700 < GetTickCount())
		{
			//m_dwStateChange = GetTickCount();
			m_bMoveFrame = true;
			m_eCurState = IDLE;
			m_AtkDelay = GetTickCount();
		}

		break;
	case SWORD_THROW:
		if (m_tFrame.iFrameStart == m_tFrame.iFrameEnd)
		{
			m_bMoveFrame = false;
		}
		if (m_iAtkCount < 3 && m_AtkDelay + 150 < GetTickCount())
		{
			CSoundMgr::Get_Instance()->StopSound(SOUND_BOSSSKILL);
			CSoundMgr::Get_Instance()->PlaySound(L"SwordCreate.mp3", SOUND_BOSSSKILL, m_fSound);
			Create_Sword_Vertical();
			m_iAtkCount++;
			m_AtkDelay = GetTickCount();
		}
		if (m_ChangeTime + 700 < GetTickCount())
		{
			m_dwStateChange = GetTickCount() + 1000;
			m_iAtkCount = 0;
			m_bMoveFrame = true;
			m_eCurState = IDLE;
			m_AtkDelay = GetTickCount();
		}
		break;
	case SWORD_FALL:

		if (m_tFrame.iFrameStart == m_tFrame.iFrameEnd)
		{
			m_bMoveFrame = false;
		}
		if (m_iAtkCount < 3 && m_AtkDelay + 400 < GetTickCount())
		{
			CSoundMgr::Get_Instance()->StopSound(SOUND_BOSSSKILL);
			CSoundMgr::Get_Instance()->PlaySound(L"SwordCreate.mp3", SOUND_BOSSSKILL, m_fSound);
			CSkillMgr::Get_Instance()->Use_Skill(L"SWORD_RAIN");
			m_iAtkCount++;
			m_AtkDelay = GetTickCount();
		}
		if (m_iAtkCount >= 3)
		{
			m_dwStateChange = GetTickCount() + 1000;
			m_iAtkCount = 0;
			m_bMoveFrame = true;
			m_eCurState = IDLE;
			m_AtkDelay = GetTickCount();
		}
		break;
	case SWORD_ROT_THROW:

		if (m_iAtkCount < 5 && m_ChangeTime + 300 < GetTickCount())
		{
			m_dwStateChange = GetTickCount();
			Create_Sword_Rot();
			m_iAtkCount++;
			m_ChangeTime = GetTickCount();
		}
		if (m_iAtkCount >= 5)
		{
			m_iAtkCount = 0;
			m_eCurState = IDLE;
			m_AtkDelay = GetTickCount();
			m_ChangeTime = GetTickCount();
		}
		break;
	case FLYING:
		//CSoundMgr::Get_Instance()->StopSound(SOUND_BOSSSKILL);
		CSoundMgr::Get_Instance()->PlaySound(L"Flying.mp3", SOUND_BOSSSKILL, m_fSound);
		m_bJump = true;
		if (m_ChangeTime + 1000 < GetTickCount() && m_tInfo.fY <= 10
			&& (m_tShadowInfo.fX < 0.f + CScrollMgr::Get_Instance()->Get_ScrollX() ||
				m_tShadowInfo.fX > 800.f + CScrollMgr::Get_Instance()->Get_ScrollX()))
		{
			m_fTime = 0.f;
			Check_Dir();
			Capture_Pos();
			m_eCurState = FLY_FALL;
			CSoundMgr::Get_Instance()->StopSound(SOUND_BOSSSKILL);
		}
		else if (m_tInfo.fY >= 10)
		{
			m_tInfo.fY -= 10;
		}
		if (m_eDir == DIR_RIGHT && m_tShadowInfo.fX < 800.f + CScrollMgr::Get_Instance()->Get_ScrollX())
		{
			m_tShadowInfo.fX += 10.f;
		}
		else if (m_eDir == DIR_LEFT && m_tShadowInfo.fX > 0.f + CScrollMgr::Get_Instance()->Get_ScrollX())
		{
			m_tShadowInfo.fX -= 10.f;
		}
		break;
	case FLY_FALL:
	{
		
		if (m_tFrame.iFrameStart == m_tFrame.iFrameEnd)
		{
			m_bMoveFrame = false;
		}
		////Shadow용
		float fWidth = m_fLastPosX - m_tShadowInfo.fX;
		float fHeight = m_fLastPosY - m_tShadowInfo.fY;

		float fDiagnal = sqrtf(fWidth* fWidth + fHeight*fHeight);

		float fRadian = acosf(fWidth / fDiagnal);

		if (m_tShadowInfo.fY < m_fLastPosY)
			fRadian = 2 * PI - fRadian;


		m_fAngle = fRadian;

		//m_tInfo용
		float ftWidth = m_fLastPosX - m_tInfo.fX;
		float ftHeight = m_fLastPosY - m_tInfo.fY;

		float ftDiagnal = sqrtf(ftWidth* ftWidth + ftHeight*ftHeight);

		float ftRadian = acosf(ftWidth / ftDiagnal);

		if (m_tInfo.fY < m_fLastPosY)
			ftRadian = 2 * PI - ftRadian;

		m_fTime += 0.3f;
		if (m_fTime >= 2.f)
			m_fTime = 2.f;

		m_tShadowInfo.fX += m_fJumpSpeed * cosf(m_fAngle)*m_fTime;
		m_tShadowInfo.fY -= m_fJumpSpeed * sinf(m_fAngle)*m_fTime;
		m_tInfo.fY -= m_fJumpSpeed * sinf(ftRadian)*m_fTime;


		//m_tInfo.fY -= m_fJumpSpeed* sinf(m_fAngle);
		if (m_tShadowInfo.fY + m_fJumpSpeed >= m_tInfo.fY + m_tInfo.fCY*0.3f
			&&m_tShadowInfo.fY - m_fJumpSpeed <= m_tInfo.fY + m_tInfo.fCY*0.3f)
		{
			m_ChangeTime = GetTickCount();
			m_bMoveFrame = true;
			m_bJump = false;
			m_eCurState = ROTATION;
		}
	}
	break;
	case ROTATION:
		if (m_tFrame.iFrameStart == m_tFrame.iFrameEnd
			&&m_iAtkCount < 4)
		{
			++m_iAtkCount;
		}
		if (m_ChangeTime + m_tFrame.dwFrameSpeed * 12 < GetTickCount())
		{
			m_dwStateChange = GetTickCount() + 500;
			m_eCurState = IDLE;
		}

		break;
	case DAMAGED:
		//CSoundMgr::Get_Instance()->StopSound(SOUND_BOSS);
		CSoundMgr::Get_Instance()->PlaySound(L"MonsterSpawn.mp3", SOUND_BOSS, m_fSound);
		if (m_tFrame.iFrameStart == m_tFrame.iFrameEnd)
			m_eCurState = m_eTempState;
		break;
	case DEAD:
		if (m_tFrame.iFrameStart == m_tFrame.iFrameEnd)
			m_tFrame.iFrameStart -= 1;

		if (m_ChangeTime + 3000 < GetTickCount())
			Set_Dead();
		break;
	}

}

void CGreatDragonian::Update_Range_Rect()
{
	m_tRange_Rect.left = m_tShadowInfo.fX - 200.f;
	m_tRange_Rect.right = m_tShadowInfo.fX + 200.f;
	m_tRange_Rect.top = m_tShadowInfo.fY - 100.f;
	m_tRange_Rect.bottom = m_tShadowInfo.fY + 100.f;
}

void CGreatDragonian::Check_Dir()
{
	if (m_tShadowInfo.fX < CObjMgr::Get_Instance()->Get_Player()->Get_ShadowInfo().fX + 10)
	{
		m_eDir = DIR_LEFT;
		m_pFrameKey = L"Great_Dragonian_LEFT";
	}
	else
	{
		m_eDir = DIR_RIGHT;

		m_pFrameKey = L"Great_Dragonian_RIGHT";
	}
}

void CGreatDragonian::Create_Sword_Rot()
{
	CObjMgr::Get_Instance()->Add_Object(OBJ_SKILL, CAbstractFactory<CDragonianSword>::Create(m_tInfo.fX, m_tInfo.fY + 20, m_eDir));
	dynamic_cast<CDragonianSword*>(CObjMgr::Get_Instance()->Get_SkillList().back())->Set_IsRot(true);
}

void CGreatDragonian::Create_Sword_Vertical()
{
	CObjMgr::Get_Instance()->Add_Object(OBJ_SKILL, CAbstractFactory<CDragonianSword>::Create(m_tShadowInfo.fX, m_tShadowInfo.fY + 50, m_eDir));
	CObjMgr::Get_Instance()->Add_Object(OBJ_SKILL, CAbstractFactory<CDragonianSword>::Create(m_tShadowInfo.fX, m_tShadowInfo.fY, m_eDir));
	CObjMgr::Get_Instance()->Add_Object(OBJ_SKILL, CAbstractFactory<CDragonianSword>::Create(m_tShadowInfo.fX, m_tShadowInfo.fY - 50, m_eDir));
}

void CGreatDragonian::Capture_Pos()
{
	m_fLastPosX = CObjMgr::Get_Instance()->Get_Player()->Get_ShadowInfo().fX;
	m_fLastPosY = CObjMgr::Get_Instance()->Get_Player()->Get_ShadowInfo().fY;
}