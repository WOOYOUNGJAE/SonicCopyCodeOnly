#include "stdafx.h"
#include "MyTile.h"

#include "BmpManager.h"
#include "ScrollManager.h"

CMyTile::CMyTile() : m_iDrawIDX(1), m_iDrawIDY(0), m_iOption(0)
{
}

CMyTile::~CMyTile()
{
}

void CMyTile::Initialize()
{
	m_vScale = { 128.f, 128.f };
}

bool CMyTile::Update()
{
	LateUpdateRECT();

	return OBJ_ALIVE;
}

void CMyTile::LateUpdate()
{
}

void CMyTile::Render(HDC _dc)
{
	if (m_iDrawIDX > 1)
		int a = 11;

	int iScrollX = (int)CScrollManager::Get_Instance()->Get_ScrollX();
	int iScrollY = (int)CScrollManager::Get_Instance()->Get_ScrollY();

	HDC hMemDC = CBmpManager::Get_Instance()->Find_Img(L"TileMap_Mushroom");

	//BitBlt(_dc,
	//	(int)m_RECT.left + iScrollX,
	//	(int)m_RECT.top + iScrollY,
	//	128, 128,
	//	hMemDC,
	//	128 * m_iDrawIDX,
	//	128 * m_iDrawIDY,
	//	SRCCOPY);

	GdiTransparentBlt(
		_dc,	// 복사받을 최종 출력 dc
		(int)m_RECT.left + iScrollX,
		(int)m_RECT.top + iScrollY,
		TILECX, // 비트맵의 가로 사이즈
		TILECX, // 비트맵의 세로 사이즈

		// Bmp 의 DC
		hMemDC, // 출력할  이미지 dc
		TILECX* m_iDrawIDX, // 비트맵의 출력 시작 x좌표
		TILECX* m_iDrawIDY,	// 비트맵의 출력 시작 Y좌표
		TILECX, // Sheet 의 가로 사이즈
		TILECX, // Sheet 의 세로 사이즈
		RGB(255, 0, 255)); // 제거할 픽셀 색상값

}

void CMyTile::Release()
{
}
