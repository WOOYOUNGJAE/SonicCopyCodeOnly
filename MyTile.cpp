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
		_dc,	// ������� ���� ��� dc
		(int)m_RECT.left + iScrollX,
		(int)m_RECT.top + iScrollY,
		TILECX, // ��Ʈ���� ���� ������
		TILECX, // ��Ʈ���� ���� ������

		// Bmp �� DC
		hMemDC, // �����  �̹��� dc
		TILECX* m_iDrawIDX, // ��Ʈ���� ��� ���� x��ǥ
		TILECX* m_iDrawIDY,	// ��Ʈ���� ��� ���� Y��ǥ
		TILECX, // Sheet �� ���� ������
		TILECX, // Sheet �� ���� ������
		RGB(255, 0, 255)); // ������ �ȼ� ����

}

void CMyTile::Release()
{
}
