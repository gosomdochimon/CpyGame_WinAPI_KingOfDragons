#include "stdafx.h"
#include "Monster.h"
#include "ObjMgr.h"
CMonster::CMonster() 
	: m_dwAttackTime(GetTickCount()), m_ReadyTime(GetTickCount()), m_oldY(0.f), m_bAttack(false),
	m_BlinkTIme(GetTickCount())
{
}


CMonster::~CMonster()
{
}

void CMonster::Initialize(void)
{
	

	
	m_fSpeed = 5.f;
}

int CMonster::Update(void)
{
	if (m_bDead)
		return OBJ_DEAD;

#pragma region 복습
	/*float	fWidth = 0.f, fHeight = 0.f;

	fWidth = m_pTarget->Get_Info().fX - m_tInfo.fX;
	fHeight = m_pTarget->Get_Info().fY - m_tInfo.fY;

	float	fDiagonal = sqrtf(fWidth * fWidth + fHeight * fHeight);

	// 역함수에 의해서 발생하는 각도는 라디안 값

	float	fRadian = acosf(fWidth / fDiagonal);

	if (m_pTarget->Get_Info().fY > m_tInfo.fY)
	fRadian = 2 * PI - fRadian;


	// Radian To Degree
	m_fAngle = fRadian * 180.f / PI;

	/ *if (m_pTarget->Get_Info().fY > m_tInfo.fY)	// 몬스터의 위치가 플레이어보다 높이 있다면
	m_fAngle *= -1.f;* /
	// Degree To Radian
	m_tInfo.fX += m_fSpeed * cosf(m_fAngle * PI / 180.f);
	m_tInfo.fY -= m_fSpeed * sinf(m_fAngle * PI / 180.f);*/

#pragma endregion 복습

	

	Update_Rect();
	
	return OBJ_NOEVENT;
}

void CMonster::Late_Update(void)
{
	/*if (100 >= m_tRect.left || WINCX - 100 <= m_tRect.right)
		m_fSpeed *= -1.f;*/
}

void CMonster::Render(HDC hDC)
{
	Rectangle(hDC,
		m_tRect.left,
		m_tRect.top,
		m_tRect.right,
		m_tRect.bottom);
}

void CMonster::Release(void)
{
	
}

void CMonster::Guide_Move()
{
	float fWidth = m_pTarget->Get_ShadowInfo().fX - m_tShadowInfo.fX;
	float fHeight = m_pTarget->Get_ShadowInfo().fY - m_tShadowInfo.fY;

	float fDiagnal = sqrtf(fWidth* fWidth + fHeight*fHeight);

	float fRadian = acosf(fWidth / fDiagnal);

	if (m_tShadowInfo.fY < m_pTarget->Get_ShadowInfo().fY)
		fRadian = 2 * PI - fRadian;

	m_fAngle = fRadian;
	m_tShadowInfo.fX += m_fSpeed * cosf(m_fAngle);
	m_tShadowInfo.fY -= m_fSpeed * sinf(m_fAngle);
}

void CMonster::Guide_Enemy()
{
	float fWidth = m_pTarget->Get_ShadowInfo().fX - m_tShadowInfo.fX;
	float fHeight = m_pTarget->Get_ShadowInfo().fY - m_tShadowInfo.fY;

	float fDiagnal = sqrtf(fWidth* fWidth + fHeight*fHeight);

	float fRadian = acosf(fWidth / fDiagnal);

	if (m_tShadowInfo.fY < m_pTarget->Get_ShadowInfo().fY)
		fRadian = 2 * PI - fRadian;

	m_fAngle = fRadian;
}

void CMonster::Update_Shadow(void)
{
	if (m_bJump)
	{
		  m_tInfo.fX = m_tShadowInfo.fX;
	}
	else
	{
		m_tInfo.fX = m_tShadowInfo.fX;
		m_tInfo.fY = m_tShadowInfo.fY - m_tInfo.fCY*0.4f ;
	}
}

