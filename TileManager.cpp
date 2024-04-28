#include "stdafx.h"
#include "TileManager.h"
#include "BmpManager.h"
#include "Factory.h"
#include "MyTile.h"
#include "ScrollManager.h"

CTileManager* CTileManager::m_pInstance = nullptr;

CTileManager::CTileManager()
{
	m_vecMushroomTile.reserve(TILEX * TILEY);
}

CTileManager::~CTileManager()
{
}

void CTileManager::Initialize()
{
	// Insert Bmp
	CBmpManager::Get_Instance()->Insert_Bmp(L"TileMap_Mushroom",
		L"./Resources/Images/TileMap/MushroomTile0.bmp");

	

}

void CTileManager::Update()
{
	for (auto& iter : m_vecMushroomTile)
		iter->Update();
}

void CTileManager::LateUpdate()
{
	for (auto& iter : m_vecMushroomTile)
		iter->LateUpdate();
}

void CTileManager::Render(HDC _dc)
{
	// picking 과 유사하게 일렬로 세우고 인덱스로 쓰는 방식
	// 스크롤이 없었다면 0,0이 디폴트, 바뀐 위치 보정해야 하기 때문에 스크롤을 한칸 사이즈로 나눔
	int	iCullX = abs((int)CScrollManager::Get_Instance()->Get_ScrollX() / TILECX);
	int	iCullY = abs((int)CScrollManager::Get_Instance()->Get_ScrollY() / TILECY);

	int	iCullEndX = iCullX + WINCX / TILECX + 2;
	int	iCullEndY = iCullY + WINCY / TILECY + 2;

	for (int i = iCullY; i < iCullEndY; ++i)
	{
		for (int j = iCullX; j < iCullEndX; ++j)
		{
			int		iIndex = i * TILEX + j;

			if (0 > iIndex || size_t(iIndex) >= m_vecMushroomTile.size())
				continue;

			m_vecMushroomTile[iIndex]->Render(_dc);
		}
	}
}

void CTileManager::Release()
{
	for_each(m_vecMushroomTile.begin(), m_vecMushroomTile.end(), CDeleteObj());
	m_vecMushroomTile.clear();
	m_vecMushroomTile.shrink_to_fit();
}

void CTileManager::Pick(POINT _pt, int _iDrawID, int _iOption)
{
	int	i = _pt.y / TILECY; // 마우스 y좌표 / 세로 사이즈 == 현재의 행
	int	j = _pt.x / TILECX; // 마우스 x좌표 / 세로 사이즈 == 현재의 열

	int	iIndex = i * TILEX + j; // 일렬로 세워서 인덱스로

	if (0 > iIndex || size_t(iIndex) >= m_vecMushroomTile.size())
		return;

	dynamic_cast<CMyTile*>(m_vecMushroomTile[iIndex])->Set_DrawIDX(_iDrawID);
	dynamic_cast<CMyTile*>(m_vecMushroomTile[iIndex])->Set_Option(_iOption);
}

void CTileManager::Pick(POINT _pt, int _iDrawIDX, int _iDrawIDY, int _iOption)
{
	int	i = _pt.y / TILECY; // 마우스 y좌표 / 세로 사이즈 == 현재의 행
	int	j = _pt.x / TILECX; // 마우스 x좌표 / 세로 사이즈 == 현재의 열

	int	iIndex = i * TILEX + j; // 일렬로 세워서 인덱스로

	if (0 > iIndex || size_t(iIndex) >= m_vecMushroomTile.size())
		return;

	dynamic_cast<CMyTile*>(m_vecMushroomTile[iIndex])->Set_DrawIDX(_iDrawIDX);
	dynamic_cast<CMyTile*>(m_vecMushroomTile[iIndex])->Set_DrawIDY(_iDrawIDY);
	dynamic_cast<CMyTile*>(m_vecMushroomTile[iIndex])->Set_Option(_iOption);
}

void CTileManager::SaveTile_Mushroom(const TCHAR* _path)
{
	HANDLE		hFile = CreateFile(_path, GENERIC_WRITE, NULL, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
		return;

	int	iDrawIDX = 0;
	int	iDrawIDY = 0;
	int iOption = 0;
	Vector2 vPos{};
	Vector2 vScale{};
	/*float fPosX = 0;
	float fPosY = 0;
	float fScaleX = 0;
	float fScaleY = 0;*/
	DWORD		dwByte = 0;

	for (auto& iter : m_vecMushroomTile)
	{
		iDrawIDX= dynamic_cast<CMyTile*>(iter)->Get_DrawIDX();
		iDrawIDY= dynamic_cast<CMyTile*>(iter)->Get_DrawIDY();
		iOption = dynamic_cast<CMyTile*>(iter)->Get_Option();

		

		WriteFile(hFile, &(iter->Get_Pos()), sizeof(Vector2), &dwByte, nullptr);
		WriteFile(hFile, &(iter->Get_Scale()), sizeof(Vector2), &dwByte, nullptr);

		WriteFile(hFile, &iDrawIDX, sizeof(int), &dwByte, nullptr);
		WriteFile(hFile, &iDrawIDY, sizeof(int), &dwByte, nullptr);
		WriteFile(hFile, &iOption, sizeof(int), &dwByte, nullptr);
	}

	CloseHandle(hFile);
}

void CTileManager::LoadTile_Mushroom(const TCHAR* _path)
{
	HANDLE		hFile = CreateFile(_path, GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
		return;

	int	iDrawIDX = 0;
	int	iDrawIDY = 0;
	int iOption = 0;
	Vector2 vPos{};
	Vector2 vScale{};

	DWORD		dwByte = 0;

	Release();

	while (true)
	{
		ReadFile (hFile, &vPos, sizeof(Vector2), &dwByte, nullptr);
		ReadFile (hFile, &vScale, sizeof(Vector2), &dwByte, nullptr);
		ReadFile (hFile, &iDrawIDX, sizeof(int), &dwByte, nullptr);
		ReadFile (hFile, &iDrawIDY, sizeof(int), &dwByte, nullptr);
		ReadFile (hFile, &iOption, sizeof(int), &dwByte, nullptr);
		

		if (0 == dwByte)
			break;

		CObj* pObj = CFactory<CMyTile>::Create(vPos.fX, vPos.fY);
		dynamic_cast<CMyTile*>(pObj)->Set_DrawIDX(iDrawIDX);
		dynamic_cast<CMyTile*>(pObj)->Set_DrawIDY(iDrawIDY);
		dynamic_cast<CMyTile*>(pObj)->Set_Option(iOption);

		m_vecMushroomTile.push_back(pObj);
	}

	CloseHandle(hFile);
}

void CTileManager::CreateDefaultMushroom()
{
	// 절대 좌표 지정 후 생성, 행 채우고 다음 열 채움
	for (int i = 0; i < TILEY; ++i)
	{
		for (int j = 0; j < TILEX; ++j)
		{
			float fX = (TILECX * j) + (float)(TILECX >> 1);
			float fY = (TILECY * i) + (float)(TILECY >> 1);

			CObj* pObj = CFactory<CMyTile>::Create(fX, fY);
			m_vecMushroomTile.push_back(pObj);
		}
	}
}