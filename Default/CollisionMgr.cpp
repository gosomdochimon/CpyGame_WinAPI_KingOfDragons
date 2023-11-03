#include "stdafx.h"
#include "CollisionMgr.h"
#include "Monster.h"

CCollisionMgr::CCollisionMgr()
{
}


CCollisionMgr::~CCollisionMgr()
{
}

void CCollisionMgr::Collision_Rect(list<CObj*> _Dest, list<CObj*> _Sour)
{
	RECT		rc{};


	for (auto& Dest : _Dest)
	{
		for (auto& Sour : _Sour)
		{
			if (IntersectRect(&rc, &(Dest->Get_Rect()), &(Sour->Get_Rect())))
			{
				Dest->Set_Dead();
				Sour->Set_Dead();
			}
		}
	}


}

void CCollisionMgr::Collision_Rect_Ex(list<CObj*> _Dest, list<CObj*> _Sour)
{
	for (auto& Dest : _Dest)
	{
		for (auto& Sour : _Sour)
		{
			float	fWidth = 0.f;
			float	fHeight = 0.f;

			if (Check_Rect(Dest, Sour, &fWidth, &fHeight))
			{
				// ���� �浹
				if (fWidth > fHeight)
				{
					// �� �浹
					if (Dest->Get_Info().fY < Sour->Get_Info().fY)
					{
						Dest->Set_PosY(-fHeight);
					}
					// �� �浹
					else
					{
						Dest->Set_PosY(fHeight);
					}
				}

				// �¿� �浹
				else
				{
					// �� �浹
					if (Dest->Get_Info().fX < Sour->Get_Info().fX)
					{
						Dest->Set_PosX(-fWidth);
					}
					// �� �浹
					else
					{
						Dest->Set_PosX(fWidth);
					}
				}

			}
		}
	}
}

void CCollisionMgr::Collision_Sphere(list<CObj*> _Dest, list<CObj*> _Sour)
{
	for (auto& Dest : _Dest)
	{
		for (auto& Sour : _Sour)
		{
			if (Check_Sphere(Dest, Sour))
			{
				Dest->Set_Dead();
				Sour->Set_Dead();
			}
		}
	}
}

bool CCollisionMgr::Check_Sphere(CObj * pDest, CObj * pSour)
{
	// abs : ���밪�� ������ִ� �Լ�
	float	fWidth  = fabs(pDest->Get_Info().fX - pSour->Get_Info().fX);
	float	fHeight = fabs(pDest->Get_Info().fY - pSour->Get_Info().fY);

	// sqrt : ��Ʈ�� �����ִ� �Լ�
	float	fDiagonal = sqrtf(fWidth * fWidth + fHeight * fHeight);

	float	fRadius = (pDest->Get_Info().fCX + pSour->Get_Info().fCX) * 0.5f;

	return fRadius >= fDiagonal;	// �浹�� �� ���
}

bool CCollisionMgr::Check_Rect(CObj* pDest, CObj* pSour, float* _pX, float* _pY)
{
	float		fWidth = abs(pDest->Get_Info().fX - pSour->Get_Info().fX);
	float		fHeight = abs(pDest->Get_Info().fY - pSour->Get_Info().fY);

	float		fRadiusX = (pDest->Get_Info().fCX + pSour->Get_Info().fCX) * 0.5f;
	float		fRadiusY = (pDest->Get_Info().fCY + pSour->Get_Info().fCY) * 0.5f;

	if ((fRadiusX > fWidth) && (fRadiusY > fHeight))
	{
		*_pX = fRadiusX - fWidth;
		*_pY = fRadiusY - fHeight;

		return true;
	}

	return false;
}

void CCollisionMgr::Collision_AtkRect(CObj* _This, RECT AtkRect, list<CObj*> _Target)
{
	RECT rc{};

	for (auto& Target : _Target)
	{		//Ÿ�� �� Y���� ���� ����������Ƽ� ���ݷ�Ʈ �浹 �� bCol=true

		if (_This->Get_Jump() == true && Target->Get_Jump() == true)
		{//�� �� ���� �� �� 
			if (IntersectRect(&rc, &AtkRect, &(Target->Get_ColRect())))
			{//�������� ������ �浹üũ 
				Target->Set_Damage(_This->Get_Atk());
				Target->Set_Col(true);
			}
		}
		else if (_This->Get_Jump() != true && Target->Get_Jump() != true)
		{//�Ѵ� ���� �ƴ� �� 
			if (IntersectRect(&rc, &AtkRect, &(Target->Get_ShadowRect())))
			{
				//Target->Set_Target(_This);
				Target->Set_Damage(_This->Get_Atk());
				Target->Set_Col(true);
			}
		}
		else if (_This->Get_Jump() && !(Target->Get_Jump()))
		{
			if (dynamic_cast<CMonster*>(Target)->Get_MonsterID() == MONSTER_BOSS)
			{
				if (IntersectRect(&rc, &AtkRect, &(Target->Get_ColRect())))
				{//�������� ������ �浹üũ 
					Target->Set_Damage(_This->Get_Atk());
					Target->Set_Col(true);
				}
			}
		}
		else if (Target->Get_Jump() && !(_This->Get_Jump()))
		{//�ڽ��� �������ϰ� ���� ������ ��Ȳ
			if (IntersectRect(&rc, &AtkRect, &(Target->Get_ColRect())))
			{//�������� ������ �浹üũ 
				if (Target->Get_Info().fY <= AtkRect.top)
				{
					Target->Set_Damage(_This->Get_Atk());
					Target->Set_Col(true);
				}
				
			}
		}
		
	}
}

void CCollisionMgr::Collision_Skill(CObj * _This, RECT AtkRect, list<CObj*> _Target)
{
	RECT rc{};

	for (auto& Target : _Target)
	{
		if (IntersectRect(&rc, &AtkRect, &(Target->Get_ShadowRect())))

		{
			Target->Set_Damage(_This->Get_Atk());
			Target->Set_Col(true);
		}
	}
}

bool CCollisionMgr::Collision_OneAtkRect(CObj * _This, RECT AtkRect, list<CObj*> _Target)
{
	RECT rc{};

	for (auto& Target : _Target)
	{		//Ÿ�� �� Y���� ���� ����������Ƽ� ���ݷ�Ʈ �浹 �� bCol=true

		if (_This->Get_Jump() == true && _This->Get_Jump() == true)
		{//�� �� ���� �� �� 
			if (IntersectRect(&rc, &AtkRect, &(Target->Get_ColRect())))
			{//�������� ������ �浹üũ 
				Target->Set_Damage(_This->Get_Atk());
				Target->Set_Col(true);
				return true;
			}
		}
		else if (_This->Get_Jump() != true && Target->Get_Jump() != true)
		{//�Ѵ� ���� �ƴ� �� 
			if (IntersectRect(&rc, &AtkRect, &(Target->Get_ShadowRect())))
			{
				//Target->Set_Target(_This);
				Target->Set_Damage(_This->Get_Atk());
				Target->Set_Col(true);
				return true;
			}
		}
		else if (_This->Get_Jump() && !(Target->Get_Jump()))
		{
			if (dynamic_cast<CMonster*>(Target)->Get_MonsterID() == MONSTER_BOSS)
			{
				if (IntersectRect(&rc, &AtkRect, &(Target->Get_ColRect())))
				{//�������� ������ �浹üũ 
					Target->Set_Damage(_This->Get_Atk());
					Target->Set_Col(true);
					return true;
				}
			}
		}
		else if (Target->Get_Jump() && !(_This->Get_Jump()))
		{//�ڽ��� �������ϰ� ���� ������ ��Ȳ
			if (IntersectRect(&rc, &AtkRect, &(Target->Get_ColRect())))
			{//�������� ������ �浹üũ 
				Target->Set_Damage(_This->Get_Atk());
				Target->Set_Col(true);
				return true;
			}
		}

	}
	return false;
}
