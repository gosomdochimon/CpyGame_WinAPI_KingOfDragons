#include "stdafx.h"
#include "Player.h"
#include "AbstractFactory.h"
#include "ObjMgr.h"
#include "KeyMgr.h"
#include "ScrollMgr.h"
#include "BmpMgr.h"
#include "Weapon.h"
#include "Shield.h"
#include "CollisionMgr.h"
#include "Storm.h"
#include "Tornado.h"
#include "Explosion.h"
#include "SoundMgr.h"
#include "SkillMgr.h"
#include "DragonianSword.h"
#include "TutorialMSGHandler.h"
#include "StageHandler.h"
#include "UIMgr.h"
#include "Skill_UI.h"
CPlayer::CPlayer()
	: m_fTime(0.f), m_fJumpPower(0.f), m_ePreState(END),
	m_eCurState(IDLE), m_oldY(0.f), m_bAttack(false), m_dwAttackTime(GetTickCount()), m_bBlocking(false),
	m_BlinkTIme(GetTickCount()), m_iSkill(0), m_bMoveFrame(true), m_iTutoCount(0), m_ManaUpdate(GetTickCount()),
	m_bPlayerDead(false), m_bMuJuck(false)
{
	for(int i= 0; i<3; ++i)
		ZeroMemory(&m_Skills[i], sizeof(TCHAR));
	CTutorialMSGHandler::Get_Instance()->Insert_CanInput(L"Input", &CObj::Set_CanInput);
	CTutorialMSGHandler::Get_Instance()->Insert_MOVE(L"MOVE", &CObj::Set_TutoMove);
	//ZeroMemory(&m_tHand_Socket, sizeof(POINT));
}


CPlayer::~CPlayer()
{
	Release();
}

void CPlayer::Initialize(void)
{
	

	//m_tInfo = { 400.f, 300.f, 200.f, 200.f };
	m_tInfo = { 300.f, 300.f, 100.f, 125.f };
	m_fSpeed = 5.f;
	m_fDistance = 100.f;
	m_fJumpPower = 17.f;

	m_tShadowInfo = { m_tInfo.fX, m_tInfo.fY - m_tInfo.fCY*0.5f, 60.f, 37.f };
	//BMP삽입
	Insert_Bmp();

	//충돌용 렉트 설정
	m_tColInfo = { m_tInfo.fX, m_tInfo.fY+m_tInfo.fCY*0.25f, 66.f, 43.f };
	//체력
	m_tStat= { 100.f, 100.f, 100.f, 100.f };

	m_pFrameKey = L"Player_IDLE_RIGHT";
	m_eDir = DIR_RIGHT;
	m_tFrame.iFrameStart = 0;
	//m_tFrame.iFrameEnd = 3;
	m_tFrame.iFrameEnd = 1;

	m_tFrame.iMotion = 0;

	m_tFrame.dwFrameSpeed = 200;
	m_tFrame.dwFrameTime = GetTickCount();
	//공격카운트
	m_iAtkCount = 0;
	//무기처기화
	m_pWeapon = new CWeapon;
	m_pWeapon->Initialize();
	m_iAtk = m_pWeapon->Get_Atk();
	
	m_pShield = new CShield;
	m_pShield->Initialize();

	//랜더 아이디
	m_eGroup = RENDER_GAMEOBJECT;
}

int CPlayer::Update(void)
{
	if (m_bDead)
		return OBJ_DEAD;


	m_iAtk = m_pWeapon->Get_Atk();
	if (CKeyMgr::Get_Instance()->Key_Down(VK_RETURN)&&m_bCanInput)
	{
		switch (m_iTutoCount)
		{
		case 0:	
			CTutorialMSGHandler::Get_Instance()->Change_MSG(L"TUTORIAL_MSG2");
			break;
		case 1:
			CTutorialMSGHandler::Get_Instance()->Change_MSG(L"TUTORIAL_MSG3");
			break;
		//case 2: //스테이지에서 해야함
		//	CTutorialMSGHandler::Get_Instance()->Change_MSG(L"TUTORIAL_MSG6");
		//	//CTutorialMSGHandler::Get_Instance()->Change_MSG(L"TUTORIAL_MSG4");
		//	break;
		//case 3://스테이지에서 해야함
		//	CTutorialMSGHandler::Get_Instance()->Change_MSG(L"TUTORIAL_MSG7");
			//CTutorialMSGHandler::Get_Instance()->Change_MSG(L"TUTORIAL_MSG5");
			break;
		/*case 4:
			CTutorialMSGHandler::Get_Instance()->Change_MSG(L"TUTORIAL_MSG8");
			break;*/

		}
		++m_iTutoCount;
	}

	if (!m_bTutoMove)
	{
		Update_MP();
		Jumping();
		Attacking();
		Key_Input();
		Offset();
		//실험용////////////////////////////
		if (CKeyMgr::Get_Instance()->Key_Down('R'))
		{	
			if (dynamic_cast<CSkill_UI*>(CUIMgr::Get_Instance()->Get_SkillUI())->Get_CheckBool()
				&& m_tStat.fCurMp >dynamic_cast<CSkill_UI*>(CUIMgr::Get_Instance()->Get_SkillUI())->Get_SpendMp())
			{
				switch (dynamic_cast<CSkill_UI*>(CUIMgr::Get_Instance()->Get_SkillUI())->Get_CurSkill())
				{
				case 1:
					CSkillMgr::Get_Instance()->Use_Skill(L"EXPLOSION");
					CSoundMgr::Get_Instance()->StopSound(SOUND_SKILL);
					CSoundMgr::Get_Instance()->PlaySound(L"Explosion.mp3", SOUND_SKILL, m_fSound);
					dynamic_cast<CSkill_UI*>(CUIMgr::Get_Instance()->Get_SkillUI())->Set_CheckBool();
					break;
				case 3:
					CSkillMgr::Get_Instance()->Use_Skill(L"TORNADO");
					CSoundMgr::Get_Instance()->StopSound(SOUND_SKILL);
					CSoundMgr::Get_Instance()->PlaySound(L"Tornado.wav", SOUND_SKILL, m_fSound);
					//CSoundMgr::Get_Instance()->PlaySound(L"Explosion.mp3", SOUND_SKILL, m_fSound);
					dynamic_cast<CSkill_UI*>(CUIMgr::Get_Instance()->Get_SkillUI())->Set_CheckBool();
					break;
				case 2:
					CSkillMgr::Get_Instance()->Use_Skill(L"STORM");
					CSoundMgr::Get_Instance()->StopSound(SOUND_SKILL);
					CSoundMgr::Get_Instance()->PlaySound(L"Thunder.mp3", SOUND_SKILL, m_fSound);
					dynamic_cast<CSkill_UI*>(CUIMgr::Get_Instance()->Get_SkillUI())->Set_CheckBool();
					break;				
				}
				m_pFrameKey = L"Fighter_CRY";
				m_eCurState = CRY;
				m_tStat.fCurMp -= dynamic_cast<CSkill_UI*>(CUIMgr::Get_Instance()->Get_SkillUI())->Get_SpendMp();
			}
			
		}
		if (CKeyMgr::Get_Instance()->Key_Down('T'))
		{
			CUIMgr::Get_Instance()->Get_SkillUI()->Set_Show();
		}
		if (CKeyMgr::Get_Instance()->Key_Down('W'))
		{
			dynamic_cast<CSkill_UI*>(CUIMgr::Get_Instance()->Get_SkillUI())->Rot_Icon(false);

		}
		if (CKeyMgr::Get_Instance()->Key_Down('E'))
		{
			dynamic_cast<CSkill_UI*>(CUIMgr::Get_Instance()->Get_SkillUI())->Rot_Icon(true);
		}
		/////////////////////////////////
		//Weapon UPdate
		Weapon_Update();
		Update_Shadow();
		Update_ColRect();
		Update_Rect();
		Update_ShadowRect();	
	}

	return OBJ_NOEVENT;
}

void CPlayer::Late_Update(void)
{
	//m_tPosin.x = long(m_tInfo.fX + m_fDistance * cosf(m_fAngle * PI / 180.f));
	//m_tPosin.y = long(m_tInfo.fY - m_fDistance * sinf(m_fAngle * PI / 180.f));
	if (m_eCurState == DEAD && m_tFrame.iFrameEnd == m_tFrame.iFrameStart)
		m_bMoveFrame = false;
	Damaged();
	Motion_Change();
	if(m_bMoveFrame)
		Move_Frame();
	//Weapon
	m_pWeapon->Late_Update();
	m_pShield->Late_Update();
}

void CPlayer::Render(HDC hDC)
{
	int iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	HDC	hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);

	HDC	hShadowDC = CBmpMgr::Get_Instance()->Find_Image(L"Player_SHADOW");

	//충돌 범위용
	//Rectangle(hDC, m_tColRect.left + iScrollX, m_tColRect.top, m_tColRect.right + iScrollX, m_tColRect.bottom);

	//Ellipse(hDC, m_tShadowRect.left + iScrollX, m_tShadowRect.top, m_tShadowRect.right + iScrollX, m_tShadowRect.bottom);
	//Rectangle(hDC, m_tColRect.left + iScrollX, m_tColRect.top, m_tColRect.right + iScrollX, m_tColRect.bottom);
		//Character Render
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
	
	if (m_eUnbeatable == HITED) //맞은 상태
	{
		if (m_BlinkTIme + 100 < GetTickCount())
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
				RGB(255, 0, 255));	// 제거하고자 하는 색상
			m_BlinkTIme = GetTickCount();
			if (!m_bPlayerDead)
			{
				m_pShield->Render(hDC);
				m_pWeapon->Render(hDC);
			}

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
			RGB(255, 0, 255));	// 제거하고자 하는 색상
		//Weapon Render
		if (!m_bPlayerDead)
		{
			m_pShield->Render(hDC);
			m_pWeapon->Render(hDC);
		}
	}
	//
	//TCHAR		szBuff[255] = L"";
	//swprintf_s(szBuff, L" x: %3f y: %3f ", m_tInfo.fX /*+ iScrollX*/, m_tInfo.fY);
	//TextOut(hDC, 180, 30, szBuff, lstrlen(szBuff));


	//Rectangle(hDC, m_Atk_Rect.left + iScrollX, m_Atk_Rect.top, m_Atk_Rect.right + iScrollX, m_Atk_Rect.bottom);
	m_Atk_Rect = { 0, 0, 0, 0 };


	

	/*Rectangle(hDC,
		m_tRect.left + iScrollX,
		m_tRect.top,
		m_tRect.right + iScrollX,
		m_tRect.bottom);*/

		//MoveToEx(hDC, (int)m_tInfo.fX + iScrollX, (int)m_tInfo.fY, nullptr);
		//LineTo(hDC, (int)m_tPosin.x + iScrollX, (int)m_tPosin.y);

}

void CPlayer::Release(void)
{
	Safe_Delete(m_pWeapon);
	Safe_Delete(m_pShield);

}
//여기부터시작 else if문으로만 되잇음 전체 else if문으로 되잇음
void CPlayer::Key_Input(void)
{

	float	fY = 0.f;

	if (m_eCurState == DAMAGED)
	{
		return;
	}

	if (m_eCurState == DEAD)
		return;

	if (CKeyMgr::Get_Instance()->Key_Down('Q'))
	{
		if (m_bMuJuck == true)
		{
			m_bMuJuck = false;
		}
		else {
			m_bMuJuck = true;
		}
		if (m_eUnbeatable == CHEAT )
		{
			m_eUnbeatable = NOHITED;
			return;
		}
		m_eUnbeatable = CHEAT;
		
	}

	if (CKeyMgr::Get_Instance()->Key_Pressing('D'))
	{//블락킹
		m_bBlocking = true;
		m_dwAttackTime = GetTickCount();

		
		m_eUnbeatable = CHEAT;

		if (m_eDir == DIR_LEFT)
		{
			m_pFrameKey = L"Player_BLOCKING_LEFT";
		}
		else if (m_eDir == DIR_RIGHT)
		{
			m_pFrameKey = L"Player_BLOCKING_RIGHT";
		}
		return;
	}
	else {
		m_bBlocking = false;
		if (m_eUnbeatable == CHEAT)
		{
			m_eUnbeatable = NOHITED;
			return;
		}
	}

	if (!m_bJump && !m_bAttack)
	{
		if (CKeyMgr::Get_Instance()->Key_Pressing(VK_LEFT))
		{//왼쪽 이동
			m_pFrameKey = L"Player_LEFT";
			m_eDir = DIR_LEFT;
			m_eCurState = WALK;
			if (CKeyMgr::Get_Instance()->Key_Pressing(VK_UP))
			{
				m_tInfo.fX -= m_fSpeed / sqrtf(2.f);
				m_tInfo.fY -= m_fSpeed / sqrtf(2.f);
			}
			else if (CKeyMgr::Get_Instance()->Key_Pressing(VK_DOWN))
			{
				m_tInfo.fX -= m_fSpeed / sqrtf(2.f);
				m_tInfo.fY += m_fSpeed / sqrtf(2.f);
			}
			else {
				m_tInfo.fX -= m_fSpeed ;
			}
		}
		else if (CKeyMgr::Get_Instance()->Key_Pressing(VK_RIGHT))
		{//오른쪽 이동
			m_eDir = DIR_RIGHT;
			m_pFrameKey = L"Player_RIGHT";
			m_eCurState = WALK;
			if (CKeyMgr::Get_Instance()->Key_Pressing(VK_UP))
			{
				m_tInfo.fX += m_fSpeed / sqrtf(2.f);
				m_tInfo.fY -= m_fSpeed / sqrtf(2.f);
			}
			else if (CKeyMgr::Get_Instance()->Key_Pressing(VK_DOWN))
			{
				m_tInfo.fX += m_fSpeed / sqrtf(2.f);
				m_tInfo.fY += m_fSpeed / sqrtf(2.f);
			}
			else {
				m_tInfo.fX += m_fSpeed;
			}
		}
		else if (CKeyMgr::Get_Instance()->Key_Pressing(VK_UP))
		{ //위로 이동
			m_tInfo.fY -= m_fSpeed;
			if (m_eDir == DIR_LEFT)
			{
				m_pFrameKey = L"Player_LEFT";
			}
			else if (m_eDir == DIR_RIGHT)
			{
				m_pFrameKey = L"Player_RIGHT";
			}
			m_eCurState = WALK;
		}
		else if (CKeyMgr::Get_Instance()->Key_Pressing(VK_DOWN))
		{ //아래이동
			m_tInfo.fY += m_fSpeed;
			if (m_eDir == DIR_LEFT)
			{
				m_pFrameKey = L"Player_LEFT";
			}
			else if (m_eDir == DIR_RIGHT)
			{
				m_pFrameKey = L"Player_RIGHT";
			}
			m_eCurState = WALK;
		}
		else //Idle State
		{
			if (m_eDir == DIR_LEFT)
			{
				m_pFrameKey = L"Player_IDLE_LEFT";
			}
			else if (m_eDir == DIR_RIGHT)
			{
				m_pFrameKey = L"Player_IDLE_RIGHT";
			}
			else {
				m_pFrameKey = L"Player_IDLE_RIGHT";
			}
			m_eCurState = IDLE;
		}



		if (CKeyMgr::Get_Instance()->Key_Down('A'))
		{//공격 시작
			m_eCurState = ATTACK;
			m_dwAttackTime = GetTickCount();
			m_bAttack = true;
			if (m_eDir == DIR_LEFT)
			{
				m_pFrameKey = L"Player_ATTACK_LEFT";
				m_eDir = DIR_LEFT;
			}
			else if (m_eDir == DIR_RIGHT)
			{
				m_pFrameKey = L"Player_ATTACK_RIGHT";
				m_eDir = DIR_RIGHT;
			}
		}

		if (CKeyMgr::Get_Instance()->Key_Down(VK_SPACE))
		{ //점프 시작
			m_oldY = m_tInfo.fY;

			m_eCurState = JUMP;
			m_bJump = true;

			if (m_eDir == DIR_LEFT)
			{
				m_pFrameKey = L"Player_JUMP_LEFT";
				m_eDir = DIR_LEFT;
			}
			else if (m_eDir == DIR_RIGHT)
			{
				m_pFrameKey = L"Player_JUMP_RIGHT";
				m_eDir = DIR_RIGHT;
			}

		}
	}
	//-----------------------------------
		//점프 중일 떄
	if (m_bJump)
	{
		m_bBlocking = false;
		if (CKeyMgr::Get_Instance()->Key_Pressing(VK_LEFT))
		{
			if (!m_bAttack)
			{
				if (CKeyMgr::Get_Instance()->Key_Down('A'))
				{// 점프 공격 시작
					m_eCurState = ATTACK;
					m_bAttack = true;
					m_dwAttackTime = GetTickCount();
					if (m_eDir == DIR_LEFT)
					{
						m_pFrameKey = L"Player_JUMPATTACK_LEFT";
						m_eDir = DIR_LEFT;
					}
				}
				else {
					m_pFrameKey = L"Player_JUMP_LEFT";

				}
			}
			m_tInfo.fX -= m_fSpeed;
			m_eDir = DIR_LEFT;
		}
		else if (CKeyMgr::Get_Instance()->Key_Pressing(VK_RIGHT))
		{
			if (!m_bAttack)
			{
				if (CKeyMgr::Get_Instance()->Key_Down('A'))
				{// 점프 공격 시작
					m_eCurState = ATTACK;
					m_dwAttackTime = GetTickCount();
					m_bAttack = true;
					if (m_eDir == DIR_RIGHT)
					{
						m_pFrameKey = L"Player_JUMPATTACK_RIGHT";
						m_eDir = DIR_RIGHT;
					}
				}
				else {
					m_pFrameKey = L"Player_JUMP_RIGHT";
				}
			}
			m_tInfo.fX += m_fSpeed;
			m_eDir = DIR_RIGHT;
		}
		else if (!m_bAttack)
		{
			if (CKeyMgr::Get_Instance()->Key_Down('A'))
			{// 점프 공격 시작
				m_eCurState = ATTACK;
				m_dwAttackTime = GetTickCount();
				m_bAttack = true;
				if (m_eDir == DIR_LEFT)
				{
					m_pFrameKey = L"Player_JUMPATTACK_LEFT";
					m_eDir = DIR_LEFT;
				}

				else if (m_eDir == DIR_RIGHT)
				{
					m_pFrameKey = L"Player_JUMPATTACK_RIGHT";
					m_eDir = DIR_RIGHT;
				}
			}

		}
	}
}


void CPlayer::Offset(void)
{
	int iOffsetMinX = 400.f;
	int iOffsetMaxX = 410.f;

	int iOffsetMinY = 100.f;
	int iOffsetMaxY = 500.f;

	if (!CStageHandler::Get_Instance()->Get_HasStage())
	{
		return;
	}

	if (CStageHandler::Get_Instance()->Get_PhaseLock())
	{
		//iOffsetMinX = CStageHandler::Get_Instance()->Get_PhaseX().fLeft;
		//iOffsetMaxX = CStageHandler::Get_Instance()->Get_PhaseX().fRight;
		iOffsetMinX = 0;
		iOffsetMaxX = 800;
		//나중에 세로 추가.
	}
	else
	{
		int iOffsetMinX = 400.f;
		int iOffsetMaxX = 410.f;

		int iOffsetMinY = 100.f;
		int iOffsetMaxY = 500.f;
	}

	int iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	// 플레이어가 왼쪽을 향하는 경우
	if (iOffsetMinX > m_tInfo.fX + iScrollX)
		CScrollMgr::Get_Instance()->Set_ScrollX(m_fSpeed);

	if (iOffsetMaxX < m_tInfo.fX + iScrollX)
		CScrollMgr::Get_Instance()->Set_ScrollX(-m_fSpeed);

	// 플레이어가 왼쪽을 향하는 경우
	if (iOffsetMinY > m_tInfo.fY + iScrollY)
		CScrollMgr::Get_Instance()->Set_ScrollY(m_fSpeed);

	if (iOffsetMaxY < m_tInfo.fY + iScrollY)
		CScrollMgr::Get_Instance()->Set_ScrollY(-m_fSpeed);
}

void CPlayer::Motion_Change(void)
{
	if (m_ePreState != m_eCurState)
	{
		switch (m_eCurState)
		{
		case CPlayer::IDLE:
			m_tFrame.iFrameStart = 0;
			//m_tFrame.iFrameEnd = 3;
			m_tFrame.iFrameEnd = 0;
			m_tFrame.iMotion = 0;

			m_tFrame.dwFrameSpeed = 200;
			m_tFrame.dwFrameTime = GetTickCount();
			break;

		case CPlayer::WALK:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 7;
			m_tFrame.iMotion = 0;

			m_tFrame.dwFrameSpeed = 100;
			m_tFrame.dwFrameTime = GetTickCount();
			break;

		case CPlayer::ATTACK:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 2;
			m_tFrame.iMotion = 0;

			m_tFrame.dwFrameSpeed = 100;
			m_tFrame.dwFrameTime = GetTickCount();
			break;

		case CPlayer::BLOCKING:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 1;
			m_tFrame.iMotion = 0;
			m_tFrame.dwFrameSpeed = 100;
			m_tFrame.dwFrameTime = GetTickCount();
			break;

		case CPlayer::JUMP:
			m_tFrame.iFrameStart = 1;
			m_tFrame.iFrameEnd = 1;
			m_tFrame.iMotion = 0;

			m_tFrame.dwFrameSpeed = 200;
			m_tFrame.dwFrameTime = GetTickCount();
			break;

		case CPlayer::DAMAGED:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 2;
			m_tFrame.iMotion = 0;

			m_tFrame.dwFrameSpeed = 200;
			m_tFrame.dwFrameTime = GetTickCount();
			break;


		case CRY:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 0;
			m_tFrame.iMotion = 0;

			m_tFrame.dwFrameSpeed = 200;
			m_tFrame.dwFrameTime = GetTickCount();
			break;

		case CPlayer::DEAD:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 4;
			m_tFrame.iMotion = 0;

			m_tFrame.dwFrameSpeed = 200;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		}

		m_ePreState = m_eCurState;
	}

}

void CPlayer::Update_Shadow(void)
{
	if (m_bJump)
	{
		m_tShadowInfo.fX = m_tInfo.fX;
	}
	else
	{
		m_tShadowInfo.fX = m_tInfo.fX;
		m_tShadowInfo.fY = m_tRect.bottom -20;
	}
}

void CPlayer::Attacking(void)
{
	if (m_bAttack)
	{
		CSoundMgr::Get_Instance()->PlaySound(L"Weapon_Swing.wav", SOUND_EFFECT, m_fSound);
		RECT Atk_Rect;
		if (m_eDir == DIR_RIGHT)
		{
			m_Atk_Rect.left = m_tInfo.fX + m_tInfo.fCX*0.5f;
			m_Atk_Rect.top = m_tInfo.fY + m_tInfo.fCY*0.3f;
			m_Atk_Rect.right = m_tInfo.fX + m_tInfo.fCX;
			m_Atk_Rect.bottom = m_tInfo.fY + m_tInfo.fCY*0.5f;
		}
		else
		{
			m_Atk_Rect.right = m_tInfo.fX - m_tInfo.fCX*0.5f;
			m_Atk_Rect.top = m_tInfo.fY + m_tInfo.fCY*0.3f;
			m_Atk_Rect.left = m_tInfo.fX - m_tInfo.fCX;
			m_Atk_Rect.bottom = m_tInfo.fY + m_tInfo.fCY*0.5f;
		}
		//Rect생성해서 충돌 처리 하기.
		if (m_tFrame.iFrameStart == 1 && m_iAtkCount == 0)
		{
			CCollisionMgr::Collision_AtkRect(this, m_Atk_Rect, CObjMgr::Get_Instance()->Get_Monster());
			++m_iAtkCount;
		}

		if (m_dwAttackTime + 400 < GetTickCount())
		{
			m_iAtkCount = 0;
			m_bAttack = false;
		}
	}

}

void CPlayer::Damaged(void)
{//1. 맞아서 무적인 상황 2. 맞은적 없을 때 3. 무적치트중

	if (m_bIsCol && !m_bMuJuck)
	{
		if (m_eUnbeatable == CHEAT)
		{
			return;
		}
		switch (m_eUnbeatable)
		{
		case NOHITED:
		{
			Reduce_Hp(m_iDamage);
			if (m_tStat.fCurHp <= 0.f /*&& !m_bPlayerDead*/)
			{
				m_tStat.fCurHp = 0.f;
				m_eCurState = DEAD;
				m_bPlayerDead = true;
				m_pFrameKey = L"Player_DAMAGED_RIGHT";
				return;
			}
			m_eCurState = DAMAGED;
			if (m_eDir == DIR_RIGHT)
			{
				m_pFrameKey = L"Player_DAMAGED_RIGHT";
			}
			else
			{
				m_pFrameKey = L"Player_DAMAGED_LEFT";
			}
			//m_bIsCol = false;
			m_eUnbeatable = HITED;
			m_dwUnbeatTime = GetTickCount();
			m_BlinkTIme = GetTickCount();
			break;
		}
		case HITED:
		{
			if (m_dwUnbeatTime + 2000 < GetTickCount())
			{// 2초뒤 무적 해제
				m_eUnbeatable = NOHITED;
				m_bIsCol = false;
			}
			if (m_eCurState == DAMAGED)
			{
				if (m_eDir == DIR_RIGHT)
				{
					m_tInfo.fX -= 2;
				}
				else
				{
					m_tInfo.fX += 2;
				}

				if (m_tFrame.iFrameStart == m_tFrame.iFrameEnd)
				{
					
					m_eCurState = IDLE;
				}
					
			}
			//
		}
		break;

		case CHEAT:
			m_bIsCol = false;
			break;
		}

	}
}
void CPlayer::Weapon_Update(void)
{
	/*case CPlayer::JUMP:
		m_tFrame.iFrameStart = 0;
		m_tFrame.iFrameEnd = 4;
		m_tFrame.iMotion = 0;

		m_tFrame.dwFrameSpeed = 200;
		m_tFrame.dwFrameTime = GetTickCount();
		break;*/
	if (m_pWeapon)
	{
		if (m_bJump)//점프중
		{
			if (m_eDir == DIR_RIGHT)
			{
				m_pWeapon->Set_Pos(m_tInfo.fX - 17.5f, m_tInfo.fY - 14.f);
				m_pShield->Set_Pos(m_tInfo.fX + 25.f, m_tInfo.fY - 0.f);

			}
			else if (m_eDir == DIR_LEFT)
			{
				m_pWeapon->Set_Pos(m_tInfo.fX + 17.5f, m_tInfo.fY - 14.f);
				m_pShield->Set_Pos(m_tInfo.fX - 25.f, m_tInfo.fY - 0.f);
			}
		}
		else {//
			if (m_eDir == DIR_RIGHT)
			{
				if (m_bBlocking)
				{//방패 사용시
					m_pWeapon->Set_Pos(m_tInfo.fX - 21.5f, m_tInfo.fY + 10.f);
					m_pShield->Set_Pos(m_tInfo.fX + 25.f, m_tInfo.fY - 5.f);
				}
				else {//사용안할 때
					m_pWeapon->Set_Pos(m_tInfo.fX - 17.5f, m_tInfo.fY - 24.f);
					m_pShield->Set_Pos(m_tInfo.fX + 25.f, m_tInfo.fY - 0.f);
				}

			}
			else if (m_eDir == DIR_LEFT)
			{
				if (m_bBlocking)
				{
					m_pWeapon->Set_Pos(m_tInfo.fX + 21.5f, m_tInfo.fY + 10.f);
					m_pShield->Set_Pos(m_tInfo.fX - 25.f, m_tInfo.fY - 5.f);
				}
				else {
					m_pWeapon->Set_Pos(m_tInfo.fX + 17.5f, m_tInfo.fY - 24.f);
					m_pShield->Set_Pos(m_tInfo.fX - 25.f, m_tInfo.fY - 0.f);
				}
			}
		}

		m_pWeapon->Set_Dir(m_eDir);
		m_pWeapon->Set_Motion(m_eCurState);
		m_pWeapon->Update();

		m_pShield->Set_Dir(m_eDir);
		m_pShield->Set_Motion(m_eCurState);
		m_pShield->Update();
	}

}

void CPlayer::Shield_Update(void)
{

}

void CPlayer::Insert_Bmp(void)
{//Walk
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/Fighter_Left.bmp", L"Player_LEFT");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/Fighter_Right.bmp", L"Player_RIGHT");
	//Idle
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/Fighter_Idle_Left.bmp", L"Player_IDLE_LEFT");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/Fighter_Idle_Right.bmp", L"Player_IDLE_RIGHT");
	//Jump
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/Fighter_Jump_Left1.bmp", L"Player_JUMP_LEFT");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/Fighter_Jump_Right.bmp", L"Player_JUMP_RIGHT");
	//Attack
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/Fighter_Attack_Left.bmp", L"Player_ATTACK_LEFT");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/Fighter_Attack_Right.bmp", L"Player_ATTACK_RIGHT");
	//JumpAttack
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/Fighter_JumpAttack_Left.bmp", L"Player_JUMPATTACK_LEFT");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/Fighter_JumpAttack_Right.bmp", L"Player_JUMPATTACK_RIGHT");

	//Blocking
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/Fighter_Blocking_Left.bmp", L"Player_BLOCKING_LEFT");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/Fighter_Blocking_Right.bmp", L"Player_BLOCKING_RIGHT");

	//Damaged
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/Fighter_Damaged_Left.bmp", L"Player_DAMAGED_LEFT");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/Fighter_Damaged_Right.bmp", L"Player_DAMAGED_RIGHT");

	//cry
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/Fighter_Cry.bmp", L"Fighter_CRY");
	//shadow
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/Fighter_Shadow.bmp", L"Player_SHADOW");
}

void CPlayer::Update_MP(void)
{
	if (m_tStat.fCurMp < m_tStat.fMaxMp && m_ManaUpdate +1000 < GetTickCount())
	{
		m_tStat.fCurMp += 5.f;
		m_ManaUpdate = GetTickCount();
	}
	else if (m_tStat.fCurMp >= m_tStat.fMaxMp)
	{
		m_tStat.fCurMp = m_tStat.fMaxMp;
	}
}

void CPlayer::Save_Player(void)
{
	HANDLE		hFile = CreateFile(L"../Data/Player.dat", GENERIC_WRITE, NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
		return;

	CWeapon::SWORD SwordID = m_pWeapon->Get_eSword();
	CShield::SHIELD ShieldID = m_pShield->Get_eShield();
	STAT StatInfo = m_tStat;
	DWORD dwByte = 0;

	WriteFile(hFile, &(SwordID), sizeof(CWeapon::SWORD), &dwByte, nullptr);
	WriteFile(hFile, &ShieldID, sizeof(CShield::SHIELD), &dwByte, nullptr);
	WriteFile(hFile, &StatInfo, sizeof(STAT), &dwByte, nullptr);


	CloseHandle(hFile);
}

void CPlayer::Load_Player(void)
{
	HANDLE		hFile = CreateFile(L"../Data/Player.dat", GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
		return;
	CWeapon::SWORD SwordID = m_pWeapon->Get_eSword();
	CShield::SHIELD ShieldID = m_pShield->Get_eShield();
	STAT StatInfo = m_tStat;
	DWORD dwByte = 0;

	ReadFile(hFile, &SwordID, sizeof(CWeapon::SWORD), &dwByte, nullptr);
	ReadFile(hFile, &ShieldID, sizeof(CShield::SHIELD), &dwByte, nullptr);
	ReadFile(hFile, &StatInfo, sizeof(STAT), &dwByte, nullptr);

	m_tStat.fMaxHp = StatInfo.fMaxHp;
	m_tStat.fCurHp = StatInfo.fMaxHp;
	m_pWeapon->Set_eSword(SwordID);
	m_pShield->Set_eShield(ShieldID);

	CloseHandle(hFile);
}

void CPlayer::Jumping(void)
{
	float	fY = 0.f;

	if (m_bJump)
	{
		m_tInfo.fY -= m_fJumpPower * m_fTime - (9.8f * m_fTime * m_fTime * 0.5f);
		
		m_fTime += 0.15f;
		if (m_fTime > 5.f)
			m_fTime = 5.f;

		if (m_oldY < m_tInfo.fY)
		{
			m_bJump = false;
			m_fTime = 0.f;
			m_tInfo.fY = m_oldY;
		}
	}
}


