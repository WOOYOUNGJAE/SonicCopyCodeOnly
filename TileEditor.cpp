#include "stdafx.h"
#include "TileEditor.h"
#include "UIManager.h"
#include "KeyManager.h"
#include "TileManager.h"
#include "ScrollManager.h"

CTileEditor::CTileEditor() : m_eCurrentTileMap(TILEMAP_END), m_iCurrentPage(0) , m_iCurrentTileIDX(0),
m_iCurrentTileIDY(0), m_bTileSorting(false)
{
	m_eCurrentTileMap = TILEMAP_MUSHROOM;
}

CTileEditor::~CTileEditor()
{
	Release();
}

void CTileEditor::Initialize()
{
	CTileManager::Get_Instance()->Initialize();
}

void CTileEditor::Update()
{
	CTileManager::Get_Instance()->Update();

	// �پ��ϰ�
	if (CKeyManager::Get_Instance()->Key_Pressing(VK_LBUTTON))
	{		
		POINT	pt;
		GetCursorPos(&pt);
		ScreenToClient(g_hWnd, &pt);


		if (m_bTileSorting) // Ÿ�� ���� â ��� Ÿ�� ���� ����
		{

			if (m_eCurrentTileMap == TILEMAP_MUSHROOM)
			{
				if (pt.y > 512)
				{
					if (pt.x < 160)
						CUIManager::Get_Instance()->Set_TileSelectPage(0);
					else if (pt.x < 320)
						CUIManager::Get_Instance()->Set_TileSelectPage(1);
					else if (pt.x < 480)
						CUIManager::Get_Instance()->Set_TileSelectPage(2);
					else if (pt.x < 640)
						CUIManager::Get_Instance()->Set_TileSelectPage(3);
					else
						CUIManager::Get_Instance()->Set_TileSelectPage(4);

					return;
				}

				m_iCurrentPage = CUIManager::Get_Instance()->Get_TileSelectPage();
				
				int	i = pt.y / TILECY; // ���콺 y��ǥ / ���� ������ == ������ �� // 0 ~ 4
				int	j = pt.x / TILECX; // ���콺 x��ǥ / ���� ������ == ������ �� // 0 ~ 5

				int	iIndex = i * 5 + j; // �Ϸķ� ������ �ε�����

				int iIndexFinal = iIndex + m_iCurrentPage * 20;

				m_iCurrentTileIDY = iIndexFinal / 8 ;
				m_iCurrentTileIDX = iIndexFinal -  m_iCurrentTileIDY * 8; // ���� Ÿ�ϸ� x�ε���
				
			}
		}
		else // picking
		{
			pt.x -= (int)CScrollManager::Get_Instance()->Get_ScrollX();
			pt.y -= (int)CScrollManager::Get_Instance()->Get_ScrollY();

			CTileManager::Get_Instance()->Pick(pt, m_iCurrentTileIDX, m_iCurrentTileIDY, 0);
		}
	}
	else if (CKeyManager::Get_Instance()->Key_Down('I')) // Ÿ�� sorting On/Off
	{
		m_bTileSorting = !m_bTileSorting;
	}
}

void CTileEditor::LateUpdate()
{
}

void CTileEditor::Render(HDC _dc)
{
}

void CTileEditor::Release()
{
}
