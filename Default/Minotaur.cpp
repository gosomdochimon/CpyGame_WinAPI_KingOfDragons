#include "stdafx.h"
#include "Minotaur.h"
#include "BmpMgr.h"
#include "ScrollMgr.h"
#include "ObjMgr.h"
#include "CollisionMgr.h"
#include "Stage.h"
#include "SoundMgr.h"
CMinotaur::CMinotaur():m_bJumpingEnd(false)
{
}


CMinotaur::~CMinotaur()
{
}

void CMinotaur::Initialize(void)
{

	m_tInfo.fCX = 190.f;
	m_tInfo.fCY = 190.f;
	m_tShadowInfo.fCX = 140.f;
	m_tShadowInfo.fCY = 60.f;
	m_tShadowInfo.fX = 500.f;
	m_tShadowInfo.fY = 300.f;
	Insert_Bmp();

	m_eDir = DIR_RIGHT;
	m_pFrameKey = L"Minotaur_RIGHT";

	m_AtkDelay = GetTickCount();
	m_dwSpawnTime = GetTickCount();
	m_ChangeTime = GetTickCount();
	m_pTarget = CObjMgr::Get_Instance()->Get_Player();
	m_bIsCol = false;
	m_bJump = false;
	m_bSmashReady = false;
	m_eCurState = WALK;

	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 0;
	m_tFrame.iMotion = 0;
	m_tFrame.dwFrameSpeed = 200;
	m_tFrame.dwFrameTime = GetTickCount();

	m_fJumpPower = 15.f;
	m_fTime = 0.0f;
	m_fSpeed = 3.0f;

	m_iCount = 0;
	m_iDamageCount = 0;
	//몬스터 아이디 지정
	m_eMonsterID = MONSTER_BOSS;
	//충돌용 렉트사이즈 지정
	m_tColInfo.fCX = 85.f;
	m_tColInfo.fCY = 145.f;
	m_tStat = { 200, 200 };
	//render아이디 지정
	m_eGroup = RENDER_GAMEOBJECT;

}

int CMinotaur::Update(void)
{
	if (m_bDead)
		return OBJ_DEAD;

	Action();
	Attacking();
	Update_Shadow();
	Update_ColRect();
	Update_Rect();
	Update_ShadowRect();
	return 0;
}

void CMinotaur::Late_Update(void)
{
	/*if (m_bIsCol && !m_bJump)
	{
		m_eCurState = DAMAGED;
		m_ChangeTime = GetTickCount();
		m_bIsCol = false;
	
	}*/
	if (m_eCurState == DEAD && m_tFrame.iFrameStart == m_tFrame.iFrameEnd)
		return;
	Damaged();
	Motion_Change();
	
	Move_Frame();
}

void CMinotaur::Render(HDC hDC)
{
	int iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	HDC	hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);
	//그림자
	//Ellipse(hDC, m_tShadowRect.left + iScrollX, m_tShadowRect.top, m_tShadowRect.right + iScrollX, m_tShadowRect.bottom);
	
	HDC	hShadowDC = CBmpMgr::Get_Instance()->Find_Image(L"Minotaur_SHADOW");

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

void CMinotaur::Release(void)
{
}

void CMinotaur::Attacking(void)
{

	RECT Atk_Rect;
	switch (m_eCurState)
	{
	case ATTACK:
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
	case JUMPINGEND:
		m_iAtk = 15;
		m_Atk_Rect.left = m_tShadowInfo.fX - m_tShadowInfo.fCX*0.6f;
		m_Atk_Rect.top = m_tShadowInfo.fY - m_tShadowInfo.fCY*0.5f;
		m_Atk_Rect.right = m_tShadowInfo.fX + m_tShadowInfo.fCX * 0.6f;
		m_Atk_Rect.bottom = m_tShadowInfo.fY + m_tShadowInfo.fCY*0.5f;
		if (m_bJumpingEnd)
		{
			m_bJumpingEnd = false;
			CCollisionMgr::Collision_AtkRect(this, m_Atk_Rect, CObjMgr::Get_Instance()->Get_PlayerList());
		}
		break;

	case SMASH:
		m_iAtk = 20;
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
	
}

void CMinotaur::Offset(void)
{
}

void CMinotaur::Insert_Bmp(void)
{
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/Minotaur/Minotaur_Left.bmp", L"Minotaur_LEFT");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/Minotaur/Minotaur_Right.bmp", L"Minotaur_RIGHT");

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/Minotaur/Minotaur_Shadow.bmp", L"Minotaur_SHADOW");
}

void CMinotaur::Motion_Change(void)
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
		{
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 4;
			m_tFrame.iMotion = 1;

			m_tFrame.dwFrameSpeed = 200;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		}
		case ATTACK:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 6;
			m_tFrame.iMotion = 2;

			m_tFrame.dwFrameSpeed = 200;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		case SMASHREADY:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 1;
			m_tFrame.iMotion = 3;

			m_tFrame.dwFrameSpeed = 200;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		case SMASH:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 5;
			m_tFrame.iMotion = 3;

			m_tFrame.dwFrameSpeed = 200;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		case JUMPING:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 0;
			m_tFrame.iMotion = 4;

			m_tFrame.dwFrameSpeed = 200;
			m_tFrame.dwFrameTime = GetTickCount();
			break;

		case JUMPINGEND:
			m_tFrame.iFrameStart = 1;
			m_tFrame.iFrameEnd = 1;
			m_tFrame.iMotion = 4;

			m_tFrame.dwFrameSpeed = 1000;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		case DAMAGED:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 5;
			m_tFrame.iMotion = 5;

			m_tFrame.dwFrameSpeed = 200;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		case DEAD:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 2;
			m_tFrame.iMotion = 6;

			m_tFrame.dwFrameSpeed = 200;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		}
		m_ePreState = m_eCurState;
	}
}

void CMinotaur::Damaged(void)
{
	/**/
	if (m_bIsCol&&!m_bJump)
	{
		switch (m_eUnbeatable)
		{
		case NOHITED:
		{
			Reduce_Hp(m_iDamage);
			m_iDamageCount += m_iDamage;
			m_iDamage = 0;
			if (m_tStat.fCurHp <= 0.f)
			{
				m_tStat.fCurHp = 0.f;
				m_eCurState = DEAD;
				m_eUnbeatable = HITED;
				m_ChangeTime = GetTickCount();
				m_dwUnbeatTime = GetTickCount();
				return;
			}
			if (m_iDamageCount >= 20)
			{
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
			if (m_dwUnbeatTime + 2000 < GetTickCount())
			{// 2초뒤 무적 해제
				m_eUnbeatable = NOHITED;
				m_bIsCol = false;
			}
		}
		//
		}
	}
}

void CMinotaur::Action()
{
	//if(m_eCurState == IDLE)
	int iRand = rand() % 5;
	int iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();
	switch (m_eCurState)
	{
	case SPAWN:
		//CSoundMgr::Get_Instance()->StopSound(SOUND_BOSS);
		CSoundMgr::Get_Instance()->PlaySound(L"CowSkill.mp3", SOUND_BOSS, m_fSound);
		break;
	case IDLE:

		if (m_ChangeTime + 2000 < GetTickCount())
		{
			switch (iRand)
			{
			
			case 1: //점프공격
				m_bJump = true;
				m_eCurState = JUMPING;
				Capture_Pos();
				break;
			case 0:
			case 4:
			case 2://일반공격
				m_eCurState = WALK;
				m_ChangeTime = GetTickCount();
				break;
			
			case 3:
				//스매쉬
				m_bSmashReady = true;
				m_eCurState = WALK;
				m_ChangeTime = GetTickCount();
				break;
			}		
		}
		//1초간 행동 결정 결정
		break;
	case WALK:
	{
		Check_Dir();
		if (m_bSmashReady)
		{
			if (m_eDir == DIR_RIGHT)
			{
				if (m_tShadowInfo.fX < m_tPhaseX.fRight-20.f)
				{
					m_tShadowInfo.fX += m_fSpeed * 1.5f;
				}
				else if (m_tShadowInfo.fX >= m_tPhaseX.fRight - 20.f)
				{
					Check_Dir();
					if (m_pTarget->Get_ShadowInfo().fY + 5 >= m_tShadowInfo.fY
						&&m_pTarget->Get_ShadowInfo().fY - 5 <= m_tShadowInfo.fY
						&& m_ChangeTime +3000 < GetTickCount())
					{
						m_eCurState = SMASHREADY;
						m_ReadyTime = GetTickCount();
					}
					else if (m_pTarget->Get_ShadowInfo().fY + 5 >= m_tShadowInfo.fY
						&&m_pTarget->Get_ShadowInfo().fY - 5 <= m_tShadowInfo.fY
						&& m_ChangeTime + 3000 > GetTickCount())
					{

					}
					else
					{
						if (m_tShadowInfo.fY < m_pTarget->Get_ShadowInfo().fY)
						{
							m_tShadowInfo.fY += m_fSpeed;
						}
						else{ m_tShadowInfo.fY -= m_fSpeed; }
					}
				}
			}
			else {
				if (m_tShadowInfo.fX > 20+ m_tPhaseX.fLeft)
				{
					m_tShadowInfo.fX -= m_fSpeed * 1.5f;
				}
				else if (m_tShadowInfo.fX <= 20 + m_tPhaseX.fLeft)
				{
					//m_tShadowInfo.fX = 0;
					Check_Dir();
					if (m_pTarget->Get_ShadowInfo().fY + 5 >= m_tShadowInfo.fY
						&&m_pTarget->Get_ShadowInfo().fY - 5 <= m_tShadowInfo.fY
						&& m_ChangeTime + 3000 < GetTickCount())
					{
						m_eCurState = SMASHREADY;
						m_ReadyTime = GetTickCount();
					}
					else if (m_pTarget->Get_ShadowInfo().fY + 5 >= m_tShadowInfo.fY
						&&m_pTarget->Get_ShadowInfo().fY - 5 <= m_tShadowInfo.fY
						&& m_ChangeTime + 3000 > GetTickCount())
					{

					}
					else
					{
						if (m_tShadowInfo.fY < m_pTarget->Get_ShadowInfo().fY)
						{
							m_tShadowInfo.fY += m_fSpeed;
						}
						else { m_tShadowInfo.fY -= m_fSpeed; }
					}
				}
			}
		}
		else {//smash가 아닌 일반 공격용
			if (m_pTarget->Get_ShadowInfo().fY + 5 >= m_tShadowInfo.fY
				&&m_pTarget->Get_ShadowInfo().fY - 5 <= m_tShadowInfo.fY
				&& (m_pTarget->Get_ShadowInfo().fX + m_pTarget->Get_ShadowInfo().fCX >= m_tShadowInfo.fX
					&& m_pTarget->Get_ShadowInfo().fX - m_pTarget->Get_ShadowInfo().fCX <= m_tShadowInfo.fX)
				&& m_AtkDelay + 1000 < GetTickCount())
			{
				m_eCurState = ATTACK;

			}
			else if ((m_pTarget->Get_ShadowInfo().fX + m_pTarget->Get_ShadowInfo().fCX >= m_tShadowInfo.fX
				&& m_pTarget->Get_ShadowInfo().fX - m_pTarget->Get_ShadowInfo().fCX <= m_tShadowInfo.fX)
				&& m_AtkDelay + 1000 > GetTickCount())
			{
				//가만히있는 모션 만들어야하는데
			}
			else {
				Check_Dir();
				Guide_Move();
			}
		}
		
		//스메시하고싶을 때 끝에 가서 y축만 따라다니다가 2초뒤에 스메시레디로 이동
		break;
	}
	case ATTACK:
		
		CSoundMgr::Get_Instance()->PlaySound(L"Weapon_Swing.wav", SOUND_BOSSSKILL, m_fSound);
		if (m_tFrame.iFrameStart == m_tFrame.iFrameEnd)
		{
			CSoundMgr::Get_Instance()->StopSound(SOUND_BOSSSKILL);
			m_eCurState = IDLE;
			m_AtkDelay = GetTickCount();
		}
		
		//공격 카운트 2번 
		break;
	case SMASHREADY:
		if (m_ReadyTime + 700 < GetTickCount())
		{
			m_eCurState = SMASH;
		}
		//한번 재생 후 스매시로 이동
		break;
	case SMASH:
		//화면 끝으로 이동 후 Idle state로 이동
		if (m_eDir == DIR_RIGHT)
		{
			m_tShadowInfo.fX -= m_fSpeed * 3.f;
			if (m_tShadowInfo.fX < m_tPhaseX.fLeft +30)
			{
				m_eCurState = IDLE;
				m_bSmashReady = false;
			}
		}
		else {
			m_tShadowInfo.fX += m_fSpeed * 3.f;
			if (m_tShadowInfo.fX >m_tPhaseX.fRight - 30)
			{
				m_eCurState = IDLE;
				m_bSmashReady = false;
			}			
		}
		break;
	case JUMPING:
		
		CSoundMgr::Get_Instance()->PlaySound(L"Jump.mp3", SOUND_BOSSSKILL, m_fSound);
		if (!m_bJumpingEnd)
			m_bJumpingEnd = !m_bJumpingEnd;
		Jumping();

		//1초동안 점프해서 x,y축 따라다니다가 1.4초즈음에 떨어짐Attacking에서 충돌처리
		break;
	case JUMPINGEND:
		CSoundMgr::Get_Instance()->StopSound(SOUND_BOSSSKILL);
		if (m_iCount == 1)
		{
			m_iCount = 0;
			m_eCurState = IDLE;
		}
		if(m_ChangeTime + 700 < GetTickCount())
			Capture_Pos();

		if (m_ChangeTime + 1000 < GetTickCount())
		{
			++m_iCount;
			m_eCurState = JUMPING;
			m_bJump = true;
		}
		//점프2번 더 하는 카운트 개설 카운트 끝나면 idle로 이동
		break;
	case DAMAGED:
		// 일정이상(ex.100이상 데미지 누적시 2초간 있다가 Idle로 이동
		//CSoundMgr::Get_Instance()->StopSound(SOUND_BOSS);
		CSoundMgr::Get_Instance()->PlaySound(L"Cow_Damaged.mp3", SOUND_BOSS, m_fSound);
		if (m_tFrame.iFrameStart == m_tFrame.iFrameEnd)
		{
			m_eCurState = IDLE;
		}	
		//Damaged();
		//무적처리 
		
		break;
	case DEAD:
		
		CSoundMgr::Get_Instance()->PlaySound(L"Cow_Dead.mp3", SOUND_BOSS, m_fSound);
		if (m_ChangeTime + 3000 < GetTickCount())
		{
			Set_Dead();
			CSoundMgr::Get_Instance()->StopSound(SOUND_BOSS);
		}
		
		//3초간 Dead 후 소멸 시키기
		break;
	}
}

void CMinotaur::Jumping(void)
{
	float	fY = 0.f;

	if (m_bJump)
	{
		m_tInfo.fY -= m_fJumpPower * m_fTime - (9.8f * m_fTime * m_fTime * 0.5f);
		m_fTime += 0.15f;
		if (m_fTime >= 3.5f)
			m_fTime = 3.5f;

		Guide_Move();

		if (m_tShadowInfo.fY - m_tShadowInfo.fCY*0.5f <= m_tInfo.fY)
		{
			m_bJump = false;
			m_fTime = 0.f;
			m_tInfo.fY = m_tShadowInfo.fY;
			m_eCurState = JUMPINGEND;
			m_ChangeTime = GetTickCount();
		}
	}

}

void CMinotaur::Capture_Pos()
{
	m_fLastPosX = CObjMgr::Get_Instance()->Get_Player()->Get_ShadowInfo().fX;
	m_fLastPosY = CObjMgr::Get_Instance()->Get_Player()->Get_ShadowInfo().fY;
}

void CMinotaur::Check_Dir(void)
{
	if (m_tShadowInfo.fX < CObjMgr::Get_Instance()->Get_Player()->Get_ShadowInfo().fX + 10)
	{
		m_eDir = DIR_LEFT;
		m_pFrameKey = L"Minotaur_LEFT";
	}
	else
	{
		m_eDir = DIR_RIGHT;

		m_pFrameKey = L"Minotaur_RIGHT";
	}
}
