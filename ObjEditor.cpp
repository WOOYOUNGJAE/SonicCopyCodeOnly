#include "stdafx.h"
#include "ObjEditor.h"
#include "DiagonalSpring.h"
#include "BmpManager.h"
#include "Factory.h"
#include "ItemObj.h"
#include "KeyManager.h"
#include "Ring.h"
#include "ScrollManager.h"
#include "Spike.h"
#include "Spring.h"
#include "Wall.h"

CObjEditor::CObjEditor() : m_pCurrentObj(nullptr), m_eSpringDir(DIR_UP), _eEditor(OBJ_EDIT_END),
m_eDiagonalSpringDir(DIR_RU)
{
	memset(m_pCurrentObjName, 0, sizeof(TCHAR) * 32);
	ZeroMemory(&m_ptWallLU, sizeof(POINT));
	ZeroMemory(&m_ptWallRD, sizeof(POINT));
}

CObjEditor::~CObjEditor()
{
}

void CObjEditor::Initialize()
{
}

void CObjEditor::Update()
{
	m_iScrollX = CScrollManager::Get_Instance()->Get_ScrollX();
	m_iScrollY = CScrollManager::Get_Instance()->Get_ScrollY();

	POINT	pt;
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	// Ŭ���� �ߴµ�
	if (CKeyManager::Get_Instance()->Key_Down('L'))
	{
		// ����
		if (pt.x >= WINCX - 32)
		{
			if (pt.y < 32)
				_eEditor = EDIT_SPRING;
			else if (pt.y >= 32 && pt.y < 64)
				_eEditor = EDIT_WALL;
			else if (pt.y >= 64 && pt.y < 96)
				_eEditor = EDIT_RING;
			else if (pt.y >= 96 && pt.y < 128)
				_eEditor = EDIT_SPIKE;
			else if (pt.y >= 128 && pt.y < 160)
				_eEditor = EDIT_DIAGONALSPRING;
			else if (pt.y >= 160 && pt.y < 192)
				_eEditor = EDIT_ITEM;
		}
	}
	else if (CKeyManager::Get_Instance()->Key_Down(VK_RBUTTON))
		ResetPos();
	else if (CKeyManager::Get_Instance()->Key_Down(VK_RETURN)) // Ȯ�� ������ ���� �� Ǫ��
	{
		//if (lstrcmp(m_pCurrentObjName, L"Spring") == 0)
		if (_eEditor == EDIT_SPRING)
		{
			CObj* pTmpObj = CFactory<CSpring>::Create(m_vCreatePos.fX, m_vCreatePos.fY);
			CObjManager::Get_Instance()->AddObject(ID_SPRING, pTmpObj);
			dynamic_cast<CSpring*>(pTmpObj)->SpringInit(m_eSpringDir);
		}
		else if (_eEditor == EDIT_WALL)
		{
			CObj* pTmpObj = CFactory<CWall>::Create(m_vCreatePos.fX, m_vCreatePos.fY);
			pTmpObj->Set_Scale(abs(m_ptWallLU.x - m_ptWallRD.x), abs(m_ptWallLU.y - m_ptWallRD.y));
			CObjManager::Get_Instance()->AddObject(ID_WALL, pTmpObj);
		}
		else if (_eEditor == EDIT_RING)
		{
			CObj* pTmpObj = CFactory<CRing>::Create(m_vCreatePos.fX, m_vCreatePos.fY);
			CObjManager::Get_Instance()->AddObject(ID_RING, pTmpObj);
		}
		else if (_eEditor == EDIT_SPIKE)
		{
			CObj* pTmpObj = CFactory<CSpike>::Create(m_vCreatePos.fX, m_vCreatePos.fY);
			CObjManager::Get_Instance()->AddObject(ID_SPIKE, pTmpObj);
		}
		else if (_eEditor == EDIT_DIAGONALSPRING)
		{
			CObj* pTmpObj = CFactory<CDiagonalSpring>::Create(m_vCreatePos.fX, m_vCreatePos.fY);
			CObjManager::Get_Instance()->AddObject(ID_DIAGONALSPRING, pTmpObj);
			dynamic_cast<CDiagonalSpring*>(pTmpObj)->SpringInit(m_eDiagonalSpringDir);
		}
		else if (_eEditor == EDIT_ITEM)
		{
			CObj* pTmpObj = CFactory<CItemObj>::Create(m_vCreatePos.fX, m_vCreatePos.fY);
			CObjManager::Get_Instance()->AddObject(ID_ITEM, pTmpObj);
		}
		_eEditor == OBJ_EDIT_END;
		ResetPos();
	}
	else if (CKeyManager::Get_Instance()->Key_Down(VK_PRIOR))
	{
		int iEnum = ((m_eSpringDir)+1);
		if (iEnum >= 4)
			iEnum = 0;

		m_eSpringDir = (MY_DIR)iEnum;
	}
	else if (CKeyManager::Get_Instance()->Key_Down(VK_NEXT))
	{
		int iEnum = ((m_eDiagonalSpringDir)+1);
		if (iEnum >= 4)
			iEnum = 0;

		m_eDiagonalSpringDir = (DIAGONAL_DIR)iEnum;
	}

	switch (_eEditor)
	{
	case EDIT_SPRING:
		UpdateSpring();
		break;
	case EDIT_WALL:
		UpdateWall();
		break;
	case EDIT_RING:
		UpdateRing();
		break;
	case EDIT_SPIKE:
		UpdateSpike();
		break;
	case EDIT_DIAGONALSPRING:
		UpdateDiagonalSpring();
		break;
	case EDIT_ITEM:
		UpdateItem();
		break;
	}
}

void CObjEditor::LateUpdate()
{
}

void CObjEditor::Render(HDC _dc) // ������Ʈ ��ġ Ȯ�� ������ ����
{
	// Yellow Spring

	// ���
	HDC hMemDC = CBmpManager::Get_Instance()->Find_Img(L"Spring");
	GdiTransparentBlt(
		// ���� â DC
		_dc,	// ������� ���� ��� dc
		WINCX - 32, // DC�������� x��ǥ
		//100, // DC�������� x��ǥ
		0,
		32, // ��Ʈ���� ���� ������
		32, // ��Ʈ���� ���� ������

		// Bmp �� DC
		hMemDC, // �����  �̹��� dc
		0, // ��Ʈ���� ��� ���� x��ǥ
		0,	// ��Ʈ���� ��� ���� Y��ǥ
		32, // Sheet �� ���� ������
		32, // Sheet �� ���� ������
		RGB(255, 0, 255)); // ������ �ȼ� ����

	Rectangle(_dc, WINCX - 32, 32, WINCX, 64);

	hMemDC = CBmpManager::Get_Instance()->Find_Img(L"Ring");
	GdiTransparentBlt(
		// ���� â DC
		_dc,	// ������� ���� ��� dc
		WINCX - 32, // DC�������� x��ǥ
		64,
		16, // ��Ʈ���� ���� ������
		16, // ��Ʈ���� ���� ������

		// Bmp �� DC
		hMemDC, // �����  �̹��� dc
		0, // ��Ʈ���� ��� ���� x��ǥ
		0,	// ��Ʈ���� ��� ���� Y��ǥ
		16, // Sheet �� ���� ������
		16, // Sheet �� ���� ������
		RGB(255, 0, 255)); // ������ �ȼ� ����

	hMemDC = CBmpManager::Get_Instance()->Find_Img(L"Spike");
	GdiTransparentBlt(
		// ���� â DC
		_dc,	// ������� ���� ��� dc
		WINCX - 32, // DC�������� x��ǥ
		96,
		31, // ��Ʈ���� ���� ������
		31, // ��Ʈ���� ���� ������

		// Bmp �� DC
		hMemDC, // �����  �̹��� dc
		0, // ��Ʈ���� ��� ���� x��ǥ
		0,	// ��Ʈ���� ��� ���� Y��ǥ
		31, // Sheet �� ���� ������
		31, // Sheet �� ���� ������
		RGB(255, 0, 255)); // ������ �ȼ� ����

	hMemDC = CBmpManager::Get_Instance()->Find_Img(L"DiagonalSpring");
	GdiTransparentBlt(
		// ���� â DC
		_dc,	// ������� ���� ��� dc
		WINCX - 36, // DC�������� x��ǥ
		128,
		36, // ��Ʈ���� ���� ������
		36, // ��Ʈ���� ���� ������

		// Bmp �� DC
		hMemDC, // �����  �̹��� dc
		0, // ��Ʈ���� ��� ���� x��ǥ
		0,	// ��Ʈ���� ��� ���� Y��ǥ
		36, // Sheet �� ���� ������
		36, // Sheet �� ���� ������
		RGB(255, 0, 255)); // ������ �ȼ� ����
	hMemDC = CBmpManager::Get_Instance()->Find_Img(L"Item"); // 28 32
	GdiTransparentBlt(
		// ���� â DC
		_dc,	// ������� ���� ��� dc
		WINCX - 36, // DC�������� x��ǥ
		164,
		28, // ��Ʈ���� ���� ������
		32, // ��Ʈ���� ���� ������

		// Bmp �� DC
		hMemDC, // �����  �̹��� dc
		0, // ��Ʈ���� ��� ���� x��ǥ
		0,	// ��Ʈ���� ��� ���� Y��ǥ
		28, // Sheet �� ���� ������
		32, // Sheet �� ���� ������
		RGB(255, 0, 255)); // ������ �ȼ� ����
	if (_eEditor == OBJ_EDIT_END)
		return;

	
	switch (_eEditor)
	{
	case EDIT_SPRING:
		RenderSpring(_dc);
		break;
	case EDIT_WALL:
		RenderWall(_dc);
		break;
	case EDIT_RING:
		RenderRing(_dc);
		break;
	case EDIT_SPIKE:
		RenderSpike(_dc);
		break;
	case EDIT_DIAGONALSPRING:
		RenderDiagonalSpring(_dc);
		break;
	case EDIT_ITEM:
		RenderItem(_dc);
		break;
	}

	
}

void CObjEditor::Release()
{
}

void CObjEditor::UpdateSpring()
{
	if (CKeyManager::Get_Instance()->Key_Down(VK_LBUTTON))
	{
		POINT	pt;
		GetCursorPos(&pt);
		ScreenToClient(g_hWnd, &pt);

		pt.x -= m_iScrollX;
		pt.y -= m_iScrollY;

		//m_pCurrentObj->Set_Pos(pt.x + iScrollX, pt.y + iScrollY);
		m_vCreatePos = Vector2(pt.x, pt.y); // ��ġ ����		
	}
	else if (CKeyManager::Get_Instance()->Key_Down(VK_PRIOR))
	{
		int iEnum = ((m_eSpringDir)+1);
		if (iEnum >= 4)
			iEnum = 0;

		m_eSpringDir = (MY_DIR)iEnum;
	}
}

void CObjEditor::UpdateDiagonalSpring()
{
	if (CKeyManager::Get_Instance()->Key_Down(VK_LBUTTON))
	{
		POINT	pt;
		GetCursorPos(&pt);
		ScreenToClient(g_hWnd, &pt);

		pt.x -= m_iScrollX;
		pt.y -= m_iScrollY;

		//m_pCurrentObj->Set_Pos(pt.x + iScrollX, pt.y + iScrollY);
		m_vCreatePos = Vector2(pt.x, pt.y); // ��ġ ����		
	}
	else if (CKeyManager::Get_Instance()->Key_Down(VK_NEXT))
	{
		int iEnum = ((m_eDiagonalSpringDir)+1);
		if (iEnum >= 4)
			iEnum = 0;

		m_eDiagonalSpringDir = (DIAGONAL_DIR)iEnum;
	}
}

void CObjEditor::UpdateWall()
{
	if (CKeyManager::Get_Instance()->Key_Down(VK_LBUTTON))
	{
		POINT	pt;
		GetCursorPos(&pt);
		ScreenToClient(g_hWnd, &pt);

		pt.x -= m_iScrollX;
		pt.y -= m_iScrollY;

		m_ptWallLU = { pt.x, pt.y };
		//m_vCreatePos = Vector2(pt.x, pt.y); // ��ġ ����		
	}
	if (CKeyManager::Get_Instance()->Key_Pressing(VK_LBUTTON))
	{
		POINT	pt;
		GetCursorPos(&pt);
		ScreenToClient(g_hWnd, &pt);

		pt.x -= m_iScrollX;
		pt.y -= m_iScrollY;

		m_ptWallRD = { pt.x, pt.y };
	}

	m_vCreatePos = 
	{ (m_ptWallLU.x + m_ptWallRD.x) * 0.5f,
		(m_ptWallLU.y + m_ptWallRD.y) * 0.5f };
}

void CObjEditor::UpdateRing()
{
	if (CKeyManager::Get_Instance()->Key_Down(VK_LBUTTON))
	{
		POINT	pt;
		GetCursorPos(&pt);
		ScreenToClient(g_hWnd, &pt);

		pt.x -= m_iScrollX;
		pt.y -= m_iScrollY;

		//m_pCurrentObj->Set_Pos(pt.x + iScrollX, pt.y + iScrollY);
		m_vCreatePos = Vector2(pt.x, pt.y); // ��ġ ����		
	}
	else if (CKeyManager::Get_Instance()->Key_Down(VK_PRIOR))
	{
		
	}
}

void CObjEditor::UpdateSpike()
{
	if (CKeyManager::Get_Instance()->Key_Down(VK_LBUTTON))
	{
		POINT	pt;
		GetCursorPos(&pt);
		ScreenToClient(g_hWnd, &pt);

		pt.x -= m_iScrollX;
		pt.y -= m_iScrollY;

		//m_pCurrentObj->Set_Pos(pt.x + iScrollX, pt.y + iScrollY);
		m_vCreatePos = Vector2(pt.x, pt.y); // ��ġ ����		
	}
}

void CObjEditor::UpdateItem()
{
	if (CKeyManager::Get_Instance()->Key_Down(VK_LBUTTON))
	{
		POINT	pt;
		GetCursorPos(&pt);
		ScreenToClient(g_hWnd, &pt);

		pt.x -= m_iScrollX;
		pt.y -= m_iScrollY;

		//m_pCurrentObj->Set_Pos(pt.x + iScrollX, pt.y + iScrollY);
		m_vCreatePos = Vector2(pt.x, pt.y); // ��ġ ����		
	}
}

void CObjEditor::RenderSpring(HDC _dc)
{
	POINT	pt;
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);
	if (m_vCreatePos.fX != 0 && m_vCreatePos.fY != 0)
	{
		pt.x = m_vCreatePos.fX;
		pt.y = m_vCreatePos.fY;
	}
	else
	{
		m_iScrollX = 0;
		m_iScrollY = 0;
	}
	int iDefaultFrame = 0;
	switch (m_eSpringDir)
	{
	case DIR_UP:
		iDefaultFrame = 0;
		break;
	case DIR_LEFT:
		iDefaultFrame = 1;
		break;
	case DIR_DOWN:
		iDefaultFrame = 2;
		break;
	case DIR_RIGHT:
		iDefaultFrame = 3;
		break;
	}

	HDC hMemDC = CBmpManager::Get_Instance()->Find_Img(L"Spring");
	GdiTransparentBlt(
		// ���� â DC
		_dc,	// ������� ���� ��� dc
		pt.x - 16 + m_iScrollX, // DC�������� x��ǥ
		pt.y - 16 + m_iScrollY,
		32, // ��Ʈ���� ���� ������
		32, // ��Ʈ���� ���� ������

		// Bmp �� DC
		hMemDC, // �����  �̹��� dc
		0, // ��Ʈ���� ��� ���� x��ǥ
		iDefaultFrame * 32,	// ��Ʈ���� ��� ���� Y��ǥ
		32, // Sheet �� ���� ������
		32, // Sheet �� ���� ������
		RGB(255, 0, 255)); // ������ �ȼ� ����		
	
}

void CObjEditor::RenderDiagonalSpring(HDC _dc)
{
	POINT	pt;
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);
	if (m_vCreatePos.fX != 0 && m_vCreatePos.fY != 0)
	{
		pt.x = m_vCreatePos.fX;
		pt.y = m_vCreatePos.fY;
	}
	else
	{
		m_iScrollX = 0;
		m_iScrollY = 0;
	}
	int iDefaultFrame = 0;
	switch (m_eDiagonalSpringDir)
	{
	case DIR_LU:
		iDefaultFrame = 0;
		break;
	case DIR_RU:
		iDefaultFrame = 1;
		break;
	case DIR_LD:
		iDefaultFrame = 2;
		break;
	case DIR_RD:
		iDefaultFrame = 3;
		break;
	}

	HDC hMemDC = CBmpManager::Get_Instance()->Find_Img(L"DiagonalSpring");
	GdiTransparentBlt(
		// ���� â DC
		_dc,	// ������� ���� ��� dc
		pt.x - 18 + m_iScrollX, // DC�������� x��ǥ
		pt.y - 18 + m_iScrollY,
		36, // ��Ʈ���� ���� ������
		36, // ��Ʈ���� ���� ������

		// Bmp �� DC
		hMemDC, // �����  �̹��� dc
		0, // ��Ʈ���� ��� ���� x��ǥ
		iDefaultFrame * 36,	// ��Ʈ���� ��� ���� Y��ǥ
		36, // Sheet �� ���� ������
		36, // Sheet �� ���� ������
		RGB(255, 0, 255)); // ������ �ȼ� ����
}

void CObjEditor::RenderWall(HDC _dc)
{
	POINT	pt;
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);
	if (m_ptWallRD.x != 0 && m_ptWallRD.y != 0)
	{
		pt.x = m_vCreatePos.fX;
		pt.y = m_vCreatePos.fY;
	}
	else
	{
		m_iScrollX = 0;
		m_iScrollY = 0;
	}

	Rectangle(_dc, m_ptWallLU.x + m_iScrollX,
		m_ptWallLU.y + m_iScrollY,
		m_ptWallRD.x + m_iScrollX,
		m_ptWallRD.y + m_iScrollY);
}

void CObjEditor::RenderRing(HDC _dc)
{
	POINT	pt;
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);
	if (m_vCreatePos.fX != 0 && m_vCreatePos.fY != 0)
	{
		pt.x = m_vCreatePos.fX;
		pt.y = m_vCreatePos.fY;
	}
	else
	{
		m_iScrollX = 0;
		m_iScrollY = 0;
	}
	int iDefaultFrame = 0;

	HDC hMemDC = CBmpManager::Get_Instance()->Find_Img(L"Ring");
	GdiTransparentBlt(
		// ���� â DC
		_dc,	// ������� ���� ��� dc
		pt.x - 8 + m_iScrollX, // DC�������� x��ǥ
		pt.y - 8 + m_iScrollY,
		16, // ��Ʈ���� ���� ������
		16, // ��Ʈ���� ���� ������

		// Bmp �� DC
		hMemDC, // �����  �̹��� dc
		0, // ��Ʈ���� ��� ���� x��ǥ
		0,	// ��Ʈ���� ��� ���� Y��ǥ
		16, // Sheet �� ���� ������
		16, // Sheet �� ���� ������
		RGB(255, 0, 255)); // ������ �ȼ� ����		
}

void CObjEditor::RenderSpike(HDC _dc)
{
	POINT	pt;
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);
	if (m_vCreatePos.fX != 0 && m_vCreatePos.fY != 0)
	{
		pt.x = m_vCreatePos.fX;
		pt.y = m_vCreatePos.fY;
	}
	else
	{
		m_iScrollX = 0;
		m_iScrollY = 0;
	}
	int iDefaultFrame = 0;

	HDC hMemDC = CBmpManager::Get_Instance()->Find_Img(L"Spike");
	GdiTransparentBlt(
		// ���� â DC
		_dc,	// ������� ���� ��� dc
		pt.x - 16 + m_iScrollX, // DC�������� x��ǥ
		pt.y - 16 + m_iScrollY,
		31, // ��Ʈ���� ���� ������
		31, // ��Ʈ���� ���� ������

		// Bmp �� DC
		hMemDC, // �����  �̹��� dc
		0, // ��Ʈ���� ��� ���� x��ǥ
		0,	// ��Ʈ���� ��� ���� Y��ǥ
		31, // Sheet �� ���� ������
		31, // Sheet �� ���� ������
		RGB(255, 0, 255)); // ������ �ȼ� ����		
}

void CObjEditor::RenderItem(HDC _dc)
{
	POINT	pt;
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);
	if (m_vCreatePos.fX != 0 && m_vCreatePos.fY != 0)
	{
		pt.x = m_vCreatePos.fX;
		pt.y = m_vCreatePos.fY;
	}
	else
	{
		m_iScrollX = 0;
		m_iScrollY = 0;
	}
	int iDefaultFrame = 0;

	HDC hMemDC = CBmpManager::Get_Instance()->Find_Img(L"Item");
	GdiTransparentBlt(
		// ���� â DC
		_dc,	// ������� ���� ��� dc
		pt.x - 14 + m_iScrollX, // DC�������� x��ǥ
		pt.y - 16 + m_iScrollY,
		28, // ��Ʈ���� ���� ������
		32, // ��Ʈ���� ���� ������

		// Bmp �� DC
		hMemDC, // �����  �̹��� dc
		0, // ��Ʈ���� ��� ���� x��ǥ
		0,	// ��Ʈ���� ��� ���� Y��ǥ
		28, // Sheet �� ���� ������
		32, // Sheet �� ���� ������
		RGB(255, 0, 255)); // ������ �ȼ� ����		
}
