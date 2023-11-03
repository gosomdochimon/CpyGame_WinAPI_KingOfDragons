#include "stdafx.h"
#include "TileMgr.h"
#include "AbstractFactory.h"
#include "Tile.h"
#include "ScrollMgr.h"

CTileMgr*	CTileMgr::m_pInstance = nullptr;

CTileMgr::CTileMgr()
{
}


CTileMgr::~CTileMgr()
{
	Release();
}

void CTileMgr::Initialize(void)
{
	for (int i = 0; i < TILEY; ++i)
	{
		for (int j = 0; j < TILEX; ++j)
		{
			float	fX = (float)(TILECX * j) + (TILECX >> 1);
			float	fY = (float)(TILECY * i) + (TILECY >> 1);

			CObj* pTile = CAbstractFactory<CTile>::Create(fX, fY);
			m_vecTile.push_back(pTile);
		}
	}

}

void CTileMgr::Update(void)
{
	for (auto& iter : m_vecTile)
		iter->Update();
}

void CTileMgr::Late_Update(void)
{
	for (auto& iter : m_vecTile)
		iter->Late_Update();
}

void CTileMgr::Render(HDC hDC)
{
	int	iMoveX = abs((int)CScrollMgr::Get_Instance()->Get_ScrollX() / TILECX);
	int	iMoveY = abs((int)CScrollMgr::Get_Instance()->Get_ScrollY() / TILECY);

	int	iCullCX = iMoveX + (WINCX / TILECX) + 2;
	int	iCullCY = iMoveY + (WINCY / TILECY) + 2;

	for (int i = iMoveY; i < iCullCY; ++i)
	{
		for (int j = iMoveX; j < iCullCX; ++j)
		{
			int iIndex = i * TILEX + j;

			if(0 > iIndex || m_vecTile.size() <= (size_t)iIndex)
				continue;

			m_vecTile[iIndex]->Render(hDC);
		}
	}
}

void CTileMgr::Release(void)
{
	for_each(m_vecTile.begin(), m_vecTile.end(), CDeleteObj());
	m_vecTile.clear();
}

void CTileMgr::Picking_Tile(POINT& _pt, int _iDrawID, int _iOption)
{
	int	iX = _pt.x / TILECX;
	int	iY = _pt.y / TILECY;

	int	iIndex = iY * TILEX + iX;

	if (0 > iIndex || m_vecTile.size() <= (size_t)iIndex)
		return;

	static_cast<CTile*>(m_vecTile[iIndex])->Set_DrawID(_iDrawID);
	static_cast<CTile*>(m_vecTile[iIndex])->Set_Option(_iOption);
}

void CTileMgr::Save_Tile(void)
{
	HANDLE		hFile = CreateFile(L"../Data/Tile.dat", GENERIC_WRITE, NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
		return;

	int iDrawID = 0, iOption = 0;
	DWORD dwByte = 0;

	for (auto& iter : m_vecTile)
	{
		iDrawID = static_cast<CTile*>(iter)->Get_DrawID();
		iOption = static_cast<CTile*>(iter)->Get_Option();

		WriteFile(hFile, &(iter->Get_Info()), sizeof(INFO), &dwByte, nullptr);
		WriteFile(hFile, &iDrawID, sizeof(int), &dwByte, nullptr);
		WriteFile(hFile, &iOption, sizeof(int), &dwByte, nullptr);
	}

	CloseHandle(hFile);

}

void CTileMgr::Load_Tile(void)
{
	HANDLE		hFile = CreateFile(L"../Data/Tile.dat", GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
		return;

	INFO	tInfo{};
	int		iDrawID = 0, iOption = 0;
	DWORD	dwByte = 0;

	Release();

	while (true)
	{
		ReadFile(hFile, &tInfo, sizeof(INFO), &dwByte, nullptr);
		ReadFile(hFile, &iDrawID, sizeof(int), &dwByte, nullptr);
		ReadFile(hFile, &iOption, sizeof(int), &dwByte, nullptr);

		if (0 == dwByte)
			break;

		CObj*	pTile = CAbstractFactory<CTile>::Create(tInfo.fX, tInfo.fY);
		static_cast<CTile*>(pTile)->Set_DrawID(iDrawID);
		static_cast<CTile*>(pTile)->Set_Option(iOption);

		m_vecTile.push_back(pTile);
	}
	CloseHandle(hFile);


}
