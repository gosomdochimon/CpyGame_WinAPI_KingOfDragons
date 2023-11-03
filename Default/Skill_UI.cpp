#include "stdafx.h"
#include "Skill_UI.h"
#include "ObjMgr.h"
#include "BmpMgr.h"
#include "KeyMgr.h"
#include "ScrollMgr.h"
CSkill_UI::CSkill_UI()
{
}


CSkill_UI::~CSkill_UI()
{
}

void CSkill_UI::Initialize(void)
{
	m_pPlayer = CObjMgr::Get_Instance()->Get_Player();
	Insert_Bmp();
	Register_Map();
	Find_FameKey();
	m_tInfo = { m_pPlayer->Get_Info().fX , m_pPlayer->Get_Info().fY, 120, 120 };
	m_pFrameKey = L"SkillBar_UI";
	m_eId = UI_SKILL;
	m_bShow = false;
	//m_dwCoolTime = GetTickCount();

	m_iCurSkill = 1;
	m_tIconPos[0] = { m_pPlayer->Get_Info().fX - 47.f , m_pPlayer->Get_Info().fY, 16, 16 };
	m_tIconPos[1] = { m_pPlayer->Get_Info().fX , m_pPlayer->Get_Info().fY - 47.f, 16, 16 };
	m_tIconPos[2] = { m_pPlayer->Get_Info().fX + 47.f , m_pPlayer->Get_Info().fY, 16, 16 };
}

int CSkill_UI::Update(void)
{
	CoolTimeDown();
	Update_InfoPos();
	Update_Rect();
	Update_IconRect();

	Find_FameKey();
	return 0;
}

void CSkill_UI::Late_Update(void)
{
}

void CSkill_UI::Render(HDC hDC)
{
	int iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	HDC	hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);

	for (int i = 0; i < 4; i++)
	{
		m_hDC[i] = CBmpMgr::Get_Instance()->Find_Image(m_pIconKey[i]);
	}
	//HDC hIconDC1 = CBmpMgr::Get_Instance()->Find_Image(m_pIconKey[0]);
	//HDC hIconDC1 = CBmpMgr::Get_Instance()->Find_Image(m_pIconKey[1]);
	//HDC hIconDC1 = CBmpMgr::Get_Instance()->Find_Image(m_pIconKey[2]);

	if (m_bShow)
	{
		//스킬인벤토리
		GdiTransparentBlt(hDC, 					// 복사 받을, 최종적으로 그림을 그릴 DC
			int(m_tRect.left + iScrollX),	// 2,3 인자 :  복사받을 위치 X, Y
			int(m_tRect.top + iScrollY),
			int(m_tInfo.fCX),				// 4,5 인자 : 복사받을 가로, 세로 길이
			int(m_tInfo.fCY),
			hMemDC,							// 비트맵을 가지고 있는 DC
			0,								// 비트맵 출력 시작 좌표, X,Y
			0,
			(int)m_tInfo.fCX,				// 복사할 비트맵의 가로, 세로 길이
			(int)m_tInfo.fCY,
			RGB(255, 0, 255));	// 제거하고자 하는 색상

		//스킬 아이콘들
		for (int i = 0; i < 4; ++i)
		{
			GdiTransparentBlt(hDC, 					// 복사 받을, 최종적으로 그림을 그릴 DC
				int(m_tIconRect[i].left + iScrollX),	// 2,3 인자 :  복사받을 위치 X, Y
				int(m_tIconRect[i].top + iScrollY),
				int(m_tIconPos[i].fCX),				// 4,5 인자 : 복사받을 가로, 세로 길이
				int(m_tIconPos[i].fCY),
				m_hDC[i],							// 비트맵을 가지고 있는 DC
				0,								// 비트맵 출력 시작 좌표, X,Y
				0,
				(int)m_tIconPos[i].fCX,				// 복사할 비트맵의 가로, 세로 길이
				(int)m_tIconPos[i].fCY,
				RGB(255, 0, 255));	// 제거하고자 하는 색상

			//Rectangle(hDC, m_tIconRect[i].left + iScrollX, m_tIconRect[i].top + iScrollY, m_tIconRect[i].right + iScrollX, m_tIconRect[i].bottom + iScrollY);
		}
	}
	//쿨타임 표시
	auto inveniter = find_if(m_mapInven.begin(), m_mapInven.end(), CInt_Finder(0));

	auto imapiter = find_if(m_mapSkillInfo.begin(), m_mapSkillInfo.end(), CInt_Finder(inveniter->second));

	/*TCHAR		szBuff[255] = L"";
	swprintf_s(szBuff, L" curskill: %3d  cooltime: %3f", m_iCurSkill, imapiter->second.fCoolTIme);
	TextOut(hDC, 180, 60, szBuff, lstrlen(szBuff));*/
}

void CSkill_UI::Release(void)
{
}

void CSkill_UI::Rot_Icon(bool _Rot)
{
	if (m_bShow)
	{
		if (_Rot == true)
		{
			auto invenrev = find_if(m_mapInven.begin(), m_mapInven.end(), CInt_Finder(InvenEnd));
			m_iBackSkill = invenrev->second;

			for (int i = 0; i < 4; i++)
			{
				auto inveniter = find_if(m_mapInven.begin(), m_mapInven.end(), CInt_Finder(i));

				m_iNextSkill = inveniter->second;
				inveniter->second = m_iBackSkill;
				m_iBackSkill = m_iNextSkill;
				if (i == 0)
				{
					m_iCurSkill = inveniter->second;
				}
			}

			//만약 주력이 none일 시 다음 꺼로 돌리기.
			if (m_iCurSkill == 0)
			{
				auto invenrev = find_if(m_mapInven.begin(), m_mapInven.end(), CInt_Finder(InvenEnd));
				m_iBackSkill = invenrev->second;

				for (int i = 0; i < 4; i++)
				{
					auto inveniter = find_if(m_mapInven.begin(), m_mapInven.end(), CInt_Finder(i));

					m_iNextSkill = inveniter->second;
					inveniter->second = m_iBackSkill;
					m_iBackSkill = m_iNextSkill;
					if (i == 0)
					{
						m_iCurSkill = inveniter->second;
					}
				}
			}
		}
		else
		{
			auto invenrev = find_if(m_mapInven.begin(), m_mapInven.end(), CInt_Finder(InvenStart));
			m_iBackSkill = invenrev->second;

			for (int i = 0; i < 4; i++)
			{
				int j = 0;
				int x = ReversRot;
				if (x - i < 0)
				{
					j = InvenEnd + (x - i);
				}
				else {
					j = x - i;
				}
				auto inveniter = find_if(m_mapInven.begin(), m_mapInven.end(), CInt_Finder(j));

				m_iNextSkill = inveniter->second;
				inveniter->second = m_iBackSkill;
				m_iBackSkill = m_iNextSkill;
				if (j == 0)
				{
					m_iCurSkill = inveniter->second;
				}

				//만약 주력이 none일 시 다음 꺼로 돌리기.

				if (m_iCurSkill == 0)
				{
					auto invenrev = find_if(m_mapInven.begin(), m_mapInven.end(), CInt_Finder(InvenStart));
					m_iBackSkill = invenrev->second;

					for (int i = 0; i < 4; i++)
					{
						int j = 0;
						int x = ReversRot;
						if (x - i < 0)
						{
							j = InvenEnd + (x - i);
						}
						else {
							j = x - i;
						}
						auto inveniter = find_if(m_mapInven.begin(), m_mapInven.end(), CInt_Finder(j));

						m_iNextSkill = inveniter->second;
						inveniter->second = m_iBackSkill;
						m_iBackSkill = m_iNextSkill;
						if (j == 0)
						{
							m_iCurSkill = inveniter->second;
						}
					}
				}
			}
		}
	}
}

void CSkill_UI::Insert_Bmp(void)
{
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/SkillBar_UI.bmp", L"SkillBar_UI");
	//아이콘
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/Icon_Explosion.bmp", L"Icon_Explosion");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/Icon_Tornado.bmp", L"Icon_Tornado");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/Icon_Storm.bmp", L"Icon_Storm");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/Icon_None.bmp", L"Icon_None");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/Icon_Skull.bmp", L"Icon_Skull");
}

void CSkill_UI::Register_Map(void)
{
	m_mapInven.emplace(0, 1);
	m_mapInven.emplace(1, 2);
	m_mapInven.emplace(2, 3);
	m_mapInven.emplace(3, 0);

	SKILLINFO Info_Explosion{ 20, 5, 0, GetTickCount(), true, L"EXPLOSION", L"Icon_Explosion" };
	SKILLINFO Info_Storm{ 30, 10, 0, GetTickCount(), true,L"STORM", L"Icon_Storm" };
	SKILLINFO Info_Tornado{ 15, 7, 0, GetTickCount(), true, L"TORNADO", L"Icon_Tornado" };
	SKILLINFO Info_None{ 0, 0, 0 , GetTickCount(), false, L"None", L"Icon_None" };

	m_mapSkillInfo.emplace(1, Info_Explosion);
	m_mapSkillInfo.emplace(2, Info_Storm);
	m_mapSkillInfo.emplace(3, Info_Tornado);
	m_mapSkillInfo.emplace(0, Info_None);
	//m_mapIcon.emplace(1, L"Icon_Explosion");
	//m_mapIcon.emplace(2, L"Icon_Tornado");
	//m_mapIcon.emplace(3, L"Icon_Storm");
	//m_mapIcon.emplace(4, L"Icon_None");
	//m_mapIcon.emplace(5, L"Icon_Skull");
}

void CSkill_UI::Find_FameKey(void)
{
	/*auto		iter = find_if(m_mapBit.begin(), m_mapBit.end(), CTag_Finder(pImageKey));
	if (iter == m_mapBit.end())
		return nullptr;
	return iter->second->Get_MemDC();*/

	/*for (int i = 0; i < 3; i++)
	{
		auto inveniter = find_if(m_mapInven.begin(), m_mapInven.end(), CInt_Finder(i+1));
		if (inveniter == m_mapInven.end())
			return;

		auto iconiter = find_if(m_mapIcon.begin(), m_mapIcon.end(), CInt_Finder(inveniter->second));
		if (iconiter == m_mapIcon.end())
			return;

		m_pIconKey[i] = iconiter->second;
	}*/

	for (int i = 0; i < 4; i++)
	{
		auto inveniter = find_if(m_mapInven.begin(), m_mapInven.end(), CInt_Finder(i));
		if (inveniter == m_mapInven.end())
			return;

		auto imapiter = find_if(m_mapSkillInfo.begin(), m_mapSkillInfo.end(), CInt_Finder(inveniter->second));
		if (imapiter == m_mapSkillInfo.end())
			return;

		m_pIconKey[i] = imapiter->second.SkillIcon;
	}
	//해야할 것 맵 저거 바꿔서 다 적용하기.
}

void CSkill_UI::Update_InfoPos()
{
	m_tInfo.fX = m_pPlayer->Get_Info().fX;
	m_tInfo.fY = m_pPlayer->Get_Info().fY;
	m_tIconPos[2] = { m_pPlayer->Get_Info().fX , m_pPlayer->Get_Info().fY + 47.f, 16, 16 };
	m_tIconPos[3] = { m_pPlayer->Get_Info().fX - 47.f , m_pPlayer->Get_Info().fY, 16, 16 };
	m_tIconPos[1] = { m_pPlayer->Get_Info().fX + 47.f , m_pPlayer->Get_Info().fY, 16, 16 };
	m_tIconPos[0] = { m_pPlayer->Get_Info().fX , m_pPlayer->Get_Info().fY - 47.f, 16, 16 };
}

void CSkill_UI::Update_IconRect(void)
{
	for (int i = 0; i < 4; ++i)
	{
		m_tIconRect[i].left = m_tIconPos[i].fX - m_tIconPos[i].fCX*0.5f;
		m_tIconRect[i].top = m_tIconPos[i].fY - m_tIconPos[i].fCY*0.5f;
		m_tIconRect[i].right = m_tIconPos[i].fX + m_tIconPos[i].fCX*0.5f;
		m_tIconRect[i].bottom = m_tIconPos[i].fY + m_tIconPos[i].fCY*0.5f;
	}
}

void CSkill_UI::Init_SkillInfo(void)
{
	//SKILLINFO Info{ 1, 10, 30, L""}
}

void CSkill_UI::CoolTimeDown(void)
{
	for (int i = 0; i < 4; ++i)
	{
		auto inveniter = find_if(m_mapInven.begin(), m_mapInven.end(), CInt_Finder(i));

		auto imapiter = find_if(m_mapSkillInfo.begin(), m_mapSkillInfo.end(), CInt_Finder(inveniter->second));
		if (imapiter == m_mapSkillInfo.end())
			return;

		if (imapiter->second.bCanUse == false)
		{
			if (imapiter->second.fCoolTIme >= imapiter->second.fMaxCoolTIme) {
				imapiter->second.bCanUse = true;
				imapiter->second.fCoolTIme = 0.f;
			}
			else {
				if (imapiter->second.m_dwCoolTime + 1000 <= GetTickCount())
				{
					imapiter->second.fCoolTIme += 1.f;
					imapiter->second.m_dwCoolTime = GetTickCount();
				}

			}

		}

	}
}

void CSkill_UI::Set_CheckBool(void)
{
	auto inveniter = find_if(m_mapInven.begin(), m_mapInven.end(), CInt_Finder(0));
	auto imapiter = find_if(m_mapSkillInfo.begin(), m_mapSkillInfo.end(), CInt_Finder(inveniter->second));
	if (imapiter == m_mapSkillInfo.end())
		return;

	imapiter->second.bCanUse = !(imapiter->second.bCanUse);
	imapiter->second.m_dwCoolTime = GetTickCount();
}

bool CSkill_UI::Get_CheckBool(void)
{
	auto inveniter = find_if(m_mapInven.begin(), m_mapInven.end(), CInt_Finder(0));

	auto imapiter = find_if(m_mapSkillInfo.begin(), m_mapSkillInfo.end(), CInt_Finder(inveniter->second));


	return imapiter->second.bCanUse;
}

float CSkill_UI::Get_Cooltime(void)
{
	auto inveniter = find_if(m_mapInven.begin(), m_mapInven.end(), CInt_Finder(0));

	auto imapiter = find_if(m_mapSkillInfo.begin(), m_mapSkillInfo.end(), CInt_Finder(inveniter->second));
	return imapiter->second.fCoolTIme;
}

float CSkill_UI::Get_MaxCooltime(void)
{
	auto inveniter = find_if(m_mapInven.begin(), m_mapInven.end(), CInt_Finder(0));

	auto imapiter = find_if(m_mapSkillInfo.begin(), m_mapSkillInfo.end(), CInt_Finder(inveniter->second));
	return imapiter->second.fMaxCoolTIme;
}

const TCHAR * CSkill_UI::Get_Icon(void)
{
	auto inveniter = find_if(m_mapInven.begin(), m_mapInven.end(), CInt_Finder(0));

	auto imapiter = find_if(m_mapSkillInfo.begin(), m_mapSkillInfo.end(), CInt_Finder(inveniter->second));


	return imapiter->second.SkillIcon;
}

float CSkill_UI::Get_SpendMp(void)
{
	auto inveniter = find_if(m_mapInven.begin(), m_mapInven.end(), CInt_Finder(0));

	auto imapiter = find_if(m_mapSkillInfo.begin(), m_mapSkillInfo.end(), CInt_Finder(inveniter->second));


	return imapiter->second.fSpendMp;
}
