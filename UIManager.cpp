#include "stdafx.h"
#include "UIManager.h"
#include "BmpManager.h"
#include "GameStateManager.h"
#include "DebugManager.h"
#include "Factory.h"
#include "ItemObj.h"
#include "KeyManager.h"
#include "MovableObject.h"
#include "ObjManager.h"
#include "SoundManager.h"


CUIManager* CUIManager::m_pInstance = nullptr;

CUIManager::CUIManager() : m_iTileSelectPage(0), m_eState(UI_STATE_END), m_iCurrentBarPoint(0),
m_iCurrTopX(WINCX), m_iCurrMiddleX(WINCX), m_iCurrBottomX(WINCX), m_dwNumberWaitStartTime(0),
m_dwNumberTimeInterval(80), m_bNumberAnimTick(false), m_iCurrOnesNum(0), m_iCurrTensNum(0),
m_bTotalTick(false), m_iCurrHunidsNum(0), m_bInventoryOn(false), m_pFocusingObj(nullptr)
{
	m_iCurrentBarPoint = WINCX * 0.3f + 9 * 20;
	ZeroMemory(&m_tRECT_SelectEditMode, sizeof(RECT));
	m_eState = UI_PLAY;
	//m_eState = UI_END;
}

CUIManager::~CUIManager()
{
}

void CUIManager::Initialize()
{
	CBmpManager::Get_Instance()->Insert_Bmp(L"Numbers5",L"./Resources/Images/UI/Numbers5.bmp");
	CBmpManager::Get_Instance()->Insert_Bmp(L"End1",L"./Resources/Images/UI/End1.bmp");
	CBmpManager::Get_Instance()->Insert_Bmp(L"End2",L"./Resources/Images/UI/End2.bmp");
	CBmpManager::Get_Instance()->Insert_Bmp(L"Inventory",L"./Resources/Images/UI/Inventory.bmp");
	CDebugManager::Get_Instance()->AddDebugLog(L"MouseX", L"");
	CDebugManager::Get_Instance()->AddDebugLog(L"MouseY", L"");
}

void CUIManager::Update()
{
	POINT pt{};
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);
	CDebugManager::Get_Instance()->ChangeDebugLog(L"MouseX", CMyString::FloatToString(L"X: ",
		pt.x));
	CDebugManager::Get_Instance()->ChangeDebugLog(L"MouseY", CMyString::FloatToString(L"X: ",
		pt.y));


	if (m_bInventoryOn == false)
		return;

	POINT InvenPt{};

	list<CItemObj*>& refList = m_pInventory->Get_ItemList();
	if (CKeyManager::Get_Instance()->Key_Down('L'))
	{
		GetCursorPos(&InvenPt);
		ScreenToClient(g_hWnd, &InvenPt);
	}
	else if (CKeyManager::Get_Instance()->Key_Down('C'))
	{
		if (m_pFocusingObj && ChangePt.x < WINCX - 216) // ������
		{
			ITEM_ID ePrevItemID = m_pFocusingObj->Get_ItemID();
			m_pInventory->PopItem(m_pFocusingObj, ePrevItemID);
			Vector2 DropPos = CObjManager::Get_Instance()->Get_Player()->Get_Pos();
			CObj* pTmp = CFactory<CItemObj>::Create(DropPos.fX, DropPos.fY + 10);
			CObjManager::Get_Instance()->AddObject(ID_ITEM, pTmp);
			dynamic_cast<CItemObj*>(pTmp)->Set_ItemID(ePrevItemID);
		}

		else if (m_pFocusingObj &&(  ChangePt.x != 0 || ChangePt.y != 0))
		{
			// ���� �� �ִ��� ���콺 ��ġ��
			if (ChangePt.x >= WINCX - 216 + 27 && ChangePt.y >= 26)
			{
				int iIndex = -1;

				auto iter = refList.begin();
#pragma region indexing
				if (26 <= ChangePt.y && ChangePt.y < 91) // ù ° ��
				{
					if (ChangePt.x < WINCX - 216 + 27 + 65) // ù ĭ
					{
						iIndex = 0;
					}
					else if (ChangePt.x < WINCX - 216 + 27 + 65 + 65) // �� ĭ
					{
						iIndex = 1;
					}
					else if (ChangePt.x < WINCX - 216 + 27 + 65 + 65 + 65) // �� ĭ
					{
						iIndex = 2;
					}

				}
				else if (26 + 65 <= ChangePt.y && ChangePt.y < 91 + 65) // �� ° ��
				{
					if (ChangePt.x < WINCX - 216 + 27 + 65) // ù ĭ
					{
						iIndex = 3;
					}
					else if (ChangePt.x < WINCX - 216 + 27 + 65 + 65) // �� ĭ
					{
						iIndex = 4;
					}
					else if (InvenPt.x < WINCX - 216 + 27 + 65 + 65 + 65) // �� ĭ
					{
						iIndex = 5;
					}

				}
				else if (26 + 65 + 65 <= ChangePt.y && ChangePt.y < 91 + 65 + 65) // �� ° ��
				{
					if (ChangePt.x < WINCX - 216 + 27 + 65) // ù ĭ
					{
						iIndex = 6;
					}
					else if (ChangePt.x < WINCX - 216 + 27 + 65 + 65) // �� ĭ
					{
						iIndex = 7;
					}
					else if (ChangePt.x < WINCX - 216 + 27 + 65 + 65 + 65) // �� ĭ
					{
						iIndex = 8;
					}

				}
				else if (26 + 65 + 65 + 65 <= ChangePt.y && ChangePt.y < 91 + 65 + 65 + 65) // �� ° ��
				{
					if (ChangePt.x < WINCX - 216 + 27 + 65) // ù ĭ
					{
						iIndex = 9;
					}
					else if (ChangePt.x < WINCX - 216 + 27 + 65 + 65) // �� ĭ
					{
						iIndex = 10;
					}
					else if (ChangePt.x < WINCX - 216 + 27 + 65 + 65 + 65) // �� ĭ
					{
						iIndex = 11;
					}

				}

				for (int i = 0; (i < iIndex && iter != refList.end()); ++i)
					++iter;
#pragma endregion

				if (iter != refList.end() && *iter) // �� �ε����� ���� ������
				{
					m_pInventory->SwapItem(m_pFocusingObj->Get_ItemID(),
						(*iter)->Get_ItemID());
				}

			}
		}
		ChangePt = {};
		m_pFocusingObj = nullptr;
	}
	if (CKeyManager::Get_Instance()->Key_Pressing(VK_LBUTTON))
	{
		GetCursorPos(&ChangePt);
		ScreenToClient(g_hWnd, &ChangePt);
	}

	if (!m_pFocusingObj && (ChangePt.x != 0 || ChangePt.y != 0))
	{
		// Choose to focus
		if (ChangePt.x >= WINCX - 216 + 27 && ChangePt.y >= 26)
		{
			int iIndex = -1;

			auto iter = refList.begin();
#pragma region indexing
			if (26 <= ChangePt.y && ChangePt.y < 91) // ù ° ��
			{
				if (ChangePt.x < WINCX - 216 + 27 + 65) // ù ĭ
				{
					iIndex = 0;
				}
				else if (ChangePt.x < WINCX - 216 + 27 + 65 + 65) // �� ĭ
				{
					iIndex = 1;
				}
				else if (ChangePt.x < WINCX - 216 + 27 + 65 + 65 + 65) // �� ĭ
				{
					iIndex = 2;
				}

			}
			else if (26 + 65 <= ChangePt.y && ChangePt.y < 91 + 65) // �� ° ��
			{
				if (ChangePt.x < WINCX - 216 + 27 + 65) // ù ĭ
				{
					iIndex = 3;
				}
				else if (ChangePt.x < WINCX - 216 + 27 + 65 + 65) // �� ĭ
				{
					iIndex = 4;
				}
				else if (InvenPt.x < WINCX - 216 + 27 + 65 + 65 + 65) // �� ĭ
				{
					iIndex = 5;
				}

			}
			else if (26 + 65 + 65 <= ChangePt.y && ChangePt.y < 91 + 65 + 65) // �� ° ��
			{
				if (ChangePt.x < WINCX - 216 + 27 + 65) // ù ĭ
				{
					iIndex = 6;
				}
				else if (ChangePt.x < WINCX - 216 + 27 + 65 + 65) // �� ĭ
				{
					iIndex = 7;
				}
				else if (ChangePt.x < WINCX - 216 + 27 + 65 + 65 + 65) // �� ĭ
				{
					iIndex = 8;
				}

			}
			else if (26 + 65 + 65 + 65 <= ChangePt.y && ChangePt.y < 91 + 65 + 65 + 65) // �� ° ��
			{
				if (ChangePt.x < WINCX - 216 + 27 + 65) // ù ĭ
				{
					iIndex = 9;
				}
				else if (ChangePt.x < WINCX - 216 + 27 + 65 + 65) // �� ĭ
				{
					iIndex = 10;
				}
				else if (ChangePt.x < WINCX - 216 + 27 + 65 + 65 + 65) // �� ĭ
				{
					iIndex = 11;
				}

			}

			for (int i = 0; (i < iIndex && iter != refList.end()); ++i)
				++iter;
#pragma endregion

			if (iter != refList.end() && *iter) // �� �ε����� ���� ������
			{
				m_pFocusingObj = (*iter);
			}
		}
	}
	else if (m_pFocusingObj)
	{

	}







	// �κ��丮 â �ۿ������� ����
	if (InvenPt.x < WINCX - 216 + 27 || InvenPt.y < 26)
		return;

	int iIndex = -1;

	auto iter = refList.begin();
	if (26 <= InvenPt.y && InvenPt.y < 91) // ù ° ��
	{
		if (InvenPt.x < WINCX - 216 + 27 + 65) // ù ĭ
		{
			iIndex = 0;
		}
		else if (InvenPt.x < WINCX - 216 + 27 + 65 + 65) // �� ĭ
		{
			iIndex = 1;
		}
		else if (InvenPt.x < WINCX - 216 + 27 + 65 + 65 + 65) // �� ĭ
		{
			iIndex = 2;
		}

	}
	else if (26 + 65 <= InvenPt.y && InvenPt.y < 91 + 65) // �� ° ��
	{
		if (InvenPt.x < WINCX - 216 + 27 + 65) // ù ĭ
		{
			iIndex = 3;
		}
		else if (InvenPt.x < WINCX - 216 + 27 + 65 + 65) // �� ĭ
		{
			iIndex = 4;
		}
		else if (InvenPt.x < WINCX - 216 + 27 + 65 + 65 + 65) // �� ĭ
		{
			iIndex = 5;
		}

	}
	else if (26 + 65 + 65 <= InvenPt.y && InvenPt.y < 91 + 65 + 65) // �� ° ��
	{
		if (InvenPt.x < WINCX - 216 + 27 + 65) // ù ĭ
		{
			iIndex = 6;
		}
		else if (InvenPt.x < WINCX - 216 + 27 + 65 + 65) // �� ĭ
		{
			iIndex = 7;
		}
		else if (InvenPt.x < WINCX - 216 + 27 + 65 + 65 + 65) // �� ĭ
		{
			iIndex = 8;
		}

	}
	else if (26 + 65 + 65 + 65 <= InvenPt.y && InvenPt.y < 91 + 65 + 65 + 65) // �� ° ��
	{
		if (InvenPt.x < WINCX - 216 + 27 + 65) // ù ĭ
		{
			iIndex = 9;
		}
		else if (InvenPt.x < WINCX - 216 + 27 + 65 + 65) // �� ĭ
		{
			iIndex = 10;
		}
		else if (InvenPt.x < WINCX - 216 + 27 + 65 + 65 + 65) // �� ĭ
		{
			iIndex = 11;
		}

	}

	for (int i = 0; i < iIndex && iter != refList.end(); ++i)
		++iter;

	if (iter != refList.end() && *iter) // �� �ε����� ���� ������
	{
		(*iter)->ActiveItem();
		m_pInventory->PopItem(*iter, (*iter)->Get_ItemID());
	}
}
void CUIManager::LateUpdate()
{
}

void CUIManager::RenderPlay(HDC _dc)
{
	// Score , Rings
	HDC hMemDC = CBmpManager::Get_Instance()->Find_Img(L"UI");
	int iRectWidth = 48;
	int iRectHeight = 25;

	GdiTransparentBlt(
		// ���� â DC
		_dc,	// ������� ���� ��� dc
		0, // DC�������� x��ǥ
		15,// DC�������� y��ǥ
		iRectWidth, // ��Ʈ���� ���� ������
		iRectHeight, // ��Ʈ���� ���� ������

		// Bmp �� DC
		hMemDC, // �����  �̹��� dc
		0, // ��Ʈ���� ��� ���� x��ǥ
		0,	// ��Ʈ���� ��� ���� Y��ǥ
		48, // Sheet �� ���� ������
		25, // Sheet �� ���� ������
		RGB(255, 0, 255)); // ������ �ȼ� ����

	// SONIC X (Life)
	GdiTransparentBlt(
		// ���� â DC
		_dc,	// ������� ���� ��� dc
		7, // DC�������� x��ǥ
		WINCY - 25,// DC�������� y��ǥ
		iRectWidth, // ��Ʈ���� ���� ������
		iRectHeight, // ��Ʈ���� ���� ������

		// Bmp �� DC
		hMemDC, // �����  �̹��� dc
		48, // ��Ʈ���� ��� ���� x��ǥ
		0,	// ��Ʈ���� ��� ���� Y��ǥ
		48, // Sheet �� ���� ������
		25, // Sheet �� ���� ������
		RGB(255, 0, 255)); // ������ �ȼ� ����

	hMemDC = CBmpManager::Get_Instance()->Find_Img(L"SmallNumber");
	GdiTransparentBlt(
		// ���� â DC
		_dc,	// ������� ���� ��� dc
		40, // DC�������� x��ǥ
		WINCY - 12,// DC�������� y��ǥ
		8, // ��Ʈ���� ���� ������
		6, // ��Ʈ���� ���� ������

		// Bmp �� DC
		hMemDC, // �����  �̹��� dc
		8 * CGameStateManager::Get_Instance()->Get_LifeCount(), // ��Ʈ���� ��� ���� x��ǥ
		0,	// ��Ʈ���� ��� ���� Y��ǥ
		8, // Sheet �� ���� ������
		6, // Sheet �� ���� ������
		RGB(255, 0, 255)); // ������ �ȼ� ����

	// RingCount
	int iRingCount = CGameStateManager::Get_Instance()->Get_RingCount();
	int iNumOnes = iRingCount % 10; // ���� �ڸ� ����
	int iNumTens = iRingCount % 100 / 10; // ���� �ڸ� ����
	int iNumHundreds = iRingCount / 100;
	hMemDC = CBmpManager::Get_Instance()->Find_Img(L"Number");

	// ���� �ڸ�
	GdiTransparentBlt(
		// ���� â DC
		_dc,	// ������� ���� ��� dc
		100, // DC�������� x��ǥ
		28,// DC�������� y��ǥ
		8, // ��Ʈ���� ���� ������
		11, // ��Ʈ���� ���� ������

		// Bmp �� DC
		hMemDC, // �����  �̹��� dc
		iNumOnes * 8, // ��Ʈ���� ��� ���� x��ǥ
		0,	// ��Ʈ���� ��� ���� Y��ǥ
		8, // Sheet �� ���� ������
		11, // Sheet �� ���� ������
		RGB(255, 0, 255)); // ������ �ȼ� ����
	// ���� �ڸ�
	if (iNumTens > 0)
		GdiTransparentBlt(
		// ���� â DC
		_dc,	// ������� ���� ��� dc
		92, // DC�������� x��ǥ
		28,// DC�������� y��ǥ
		8, // ��Ʈ���� ���� ������
		11, // ��Ʈ���� ���� ������
		// Bmp �� DC
		hMemDC, // �����  �̹��� dc
		iNumTens * 8, // ��Ʈ���� ��� ���� x��ǥ
		0,	// ��Ʈ���� ��� ���� Y��ǥ
		8, // Sheet �� ���� ������
		11, // Sheet �� ���� ������
		RGB(255, 0, 255)); // ������ �ȼ� ����

	if (iNumHundreds > 0)
		GdiTransparentBlt(
			// ���� â DC
			_dc,	// ������� ���� ��� dc
			84, // DC�������� x��ǥ
			28,// DC�������� y��ǥ
			8, // ��Ʈ���� ���� ������
			11, // ��Ʈ���� ���� ������
			// Bmp �� DC
			hMemDC, // �����  �̹��� dc
			iNumHundreds * 8, // ��Ʈ���� ��� ���� x��ǥ
			0,	// ��Ʈ���� ��� ���� Y��ǥ
			8, // Sheet �� ���� ������
			11, // Sheet �� ���� ������
			RGB(255, 0, 255)); // ������ �ȼ� ����
	// SCORE---------------------------

	int iScoreCount = CGameStateManager::Get_Instance()->Get_CurrentScore();
	int iScoreOnes = iScoreCount % 1000 / 100; // ���� �ڸ� ����
	int iScoreTens = iScoreCount / 1000; // ���� �ڸ� ����


	//score 1���ڸ�
	GdiTransparentBlt(
		// ���� â DC
		_dc,	// ������� ���� ��� dc
		100, // DC�������� x��ǥ
		16,// DC�������� y��ǥ
		8, // ��Ʈ���� ���� ������
		11, // ��Ʈ���� ���� ������
		// Bmp �� DC
		hMemDC, // �����  �̹��� dc
		0, // ��Ʈ���� ��� ���� x��ǥ
		0,	// ��Ʈ���� ��� ���� Y��ǥ
		8, // Sheet �� ���� ������
		11, // Sheet �� ���� ������
		RGB(255, 0, 255)); // ������ �ȼ� ����

	if (iScoreCount <= 0)
		return;

	// 10�� �ڸ�
	GdiTransparentBlt(
		// ���� â DC
		_dc,	// ������� ���� ��� dc
		92, // DC�������� x��ǥ
		16,// DC�������� y��ǥ
		8, // ��Ʈ���� ���� ������
		11, // ��Ʈ���� ���� ������
		// Bmp �� DC
		hMemDC, // �����  �̹��� dc
		0, // ��Ʈ���� ��� ���� x��ǥ
		0,	// ��Ʈ���� ��� ���� Y��ǥ
		8, // Sheet �� ���� ������
		11, // Sheet �� ���� ������
		RGB(255, 0, 255)); // ������ �ȼ� ����

	// score	100�� �ڸ�
	GdiTransparentBlt(
		// ���� â DC
		_dc,	// ������� ���� ��� dc
		84, // DC�������� x��ǥ
		16,// DC�������� y��ǥ
		8, // ��Ʈ���� ���� ������
		11, // ��Ʈ���� ���� ������

		// Bmp �� DC
		hMemDC, // �����  �̹��� dc
		iScoreOnes * 8, // ��Ʈ���� ��� ���� x��ǥ
		0,	// ��Ʈ���� ��� ���� Y��ǥ
		8, // Sheet �� ���� ������
		11, // Sheet �� ���� ������
		RGB(255, 0, 255)); // ������ �ȼ� ����
	// ���� �ڸ�
	if (iScoreTens > 0)
		GdiTransparentBlt(
			// ���� â DC
			_dc,	// ������� ���� ��� dc
			76, // DC�������� x��ǥ
			16,// DC�������� y��ǥ
			8, // ��Ʈ���� ���� ������
			11, // ��Ʈ���� ���� ������
			// Bmp �� DC
			hMemDC, // �����  �̹��� dc
			iScoreTens * 8, // ��Ʈ���� ��� ���� x��ǥ
			0,	// ��Ʈ���� ��� ���� Y��ǥ
			8, // Sheet �� ���� ������
			11, // Sheet �� ���� ������
			RGB(255, 0, 255)); // ������ �ȼ� ����
}

void CUIManager::RenderEnd(HDC _dc)
{
	// Total Point
	HDC hMemDC = CBmpManager::Get_Instance()->Find_Img(L"End1");
	int iDestX = 200 -105;
	int iDestY = 150 - 58;

	// Total Top
	if (abs(m_iCurrTopX - iDestX) > 1)
	{
		m_iCurrTopX = CMyMath::Lerp(m_iCurrTopX, iDestX, g_fDeltaTime * 1.2f);
	}
	else
		m_iCurrTopX = iDestX;

	GdiTransparentBlt(
			_dc,	// ������� ���� ��� dc
			m_iCurrTopX, // DC�������� x��ǥ
			iDestY,// DC�������� y��ǥ
			211, // ��Ʈ���� ���� ������
			50, // ��Ʈ���� ���� ������

			// Bmp �� DC
			hMemDC, // �����  �̹��� dc
			0, // ��Ʈ���� ��� ���� x��ǥ
			0,	// ��Ʈ���� ��� ���� Y��ǥ
			211, // Sheet �� ���� ������
			50, // Sheet �� ���� ������
			RGB(255, 0, 255)); // ������ �ȼ� ����

	if (abs(m_iCurrTopX - iDestX) > 1)
		return;

	// Total Middle
	if (abs(m_iCurrMiddleX - iDestX) > 1)
		m_iCurrMiddleX = CMyMath::Lerp(m_iCurrMiddleX, iDestX, g_fDeltaTime * 1.f);
	else
		m_iCurrMiddleX = iDestX;
	GdiTransparentBlt(
		_dc,	// ������� ���� ��� dc
		m_iCurrMiddleX, // DC�������� x��ǥ
		iDestY + 50,// DC�������� y��ǥ
		211, // ��Ʈ���� ���� ������
		50, // ��Ʈ���� ���� ������

		// Bmp �� DC
		hMemDC, // �����  �̹��� dc
		0, // ��Ʈ���� ��� ���� x��ǥ
		50,	// ��Ʈ���� ��� ���� Y��ǥ
		211, // Sheet �� ���� ������
		50, // Sheet �� ���� ������
		RGB(255, 0, 255)); // ������ �ȼ� ����

	if (abs(m_iCurrMiddleX - iDestX) > 1)
		return;

	// Total Down
	if (abs(m_iCurrBottomX - iDestX) > 1)
		m_iCurrBottomX = CMyMath::Lerp(m_iCurrBottomX, iDestX, g_fDeltaTime * 1.3f);
	else
		m_iCurrBottomX = iDestX;

	GdiTransparentBlt(
		_dc,	// ������� ���� ��� dc
		m_iCurrBottomX, // DC�������� x��ǥ
		iDestY + 100,// DC�������� y��ǥ
		211, // ��Ʈ���� ���� ������
		17, // ��Ʈ���� ���� ������

		// Bmp �� DC
		hMemDC, // �����  �̹��� dc
		0, // ��Ʈ���� ��� ���� x��ǥ
		100,	// ��Ʈ���� ��� ���� Y��ǥ
		211, // Sheet �� ���� ������
		17, // Sheet �� ���� ������
		RGB(255, 0, 255)); // ������ �ȼ� ����

	if (abs(m_iCurrBottomX - iDestX) > 1)
		return;

	if (!m_bNumberAnimTick)
	{
		m_bNumberAnimTick = true;
		m_dwNumberWaitStartTime = GetTickCount();
	}
	//--------------------------- ���� ������ ����
	hMemDC = CBmpManager::Get_Instance()->Find_Img(L"Number");

	// scoreCount
	int iScoreCount = CGameStateManager::Get_Instance()->Get_CurrentScore();
	int iScoreOnes = iScoreCount % 1000 / 100; // ���� �ڸ� ����
	int iScoreTens = iScoreCount / 1000; // ���� �ڸ� ����

	//score 1���ڸ�
	GdiTransparentBlt(
		// ���� â DC
		_dc,	// ������� ���� ��� dc
		263, // DC�������� x��ǥ
		150,// DC�������� y��ǥ
		8, // ��Ʈ���� ���� ������
		11, // ��Ʈ���� ���� ������
		// Bmp �� DC
		hMemDC, // �����  �̹��� dc
		0, // ��Ʈ���� ��� ���� x��ǥ
		0,	// ��Ʈ���� ��� ���� Y��ǥ
		8, // Sheet �� ���� ������
		11, // Sheet �� ���� ������
		RGB(255, 0, 255)); // ������ �ȼ� ����

	if (iScoreCount > 0)
	{
		// 10�� �ڸ�
		GdiTransparentBlt(
			// ���� â DC
			_dc,	// ������� ���� ��� dc
			255, // DC�������� x��ǥ
			150,// DC�������� y��ǥ
			8, // ��Ʈ���� ���� ������
			11, // ��Ʈ���� ���� ������
			// Bmp �� DC
			hMemDC, // �����  �̹��� dc
			0, // ��Ʈ���� ��� ���� x��ǥ
			0,	// ��Ʈ���� ��� ���� Y��ǥ
			8, // Sheet �� ���� ������
			11, // Sheet �� ���� ������
			RGB(255, 0, 255)); // ������ �ȼ� ����

	// score	100�� �ڸ�
	GdiTransparentBlt(
		// ���� â DC
		_dc,	// ������� ���� ��� dc
		247, // DC�������� x��ǥ
		150,// DC�������� y��ǥ
		8, // ��Ʈ���� ���� ������
		11, // ��Ʈ���� ���� ������

		// Bmp �� DC
		hMemDC, // �����  �̹��� dc
		iScoreOnes * 8, // ��Ʈ���� ��� ���� x��ǥ
		0,	// ��Ʈ���� ��� ���� Y��ǥ
		8, // Sheet �� ���� ������
		11, // Sheet �� ���� ������
		RGB(255, 0, 255)); // ������ �ȼ� ����
	}
	// ���� �ڸ�
	if (iScoreTens > 0)
		GdiTransparentBlt(
			// ���� â DC
			_dc,	// ������� ���� ��� dc
			239, // DC�������� x��ǥ
			150,// DC�������� y��ǥ
			8, // ��Ʈ���� ���� ������
			11, // ��Ʈ���� ���� ������
			// Bmp �� DC
			hMemDC, // �����  �̹��� dc
			iScoreTens * 8, // ��Ʈ���� ��� ���� x��ǥ
			0,	// ��Ʈ���� ��� ���� Y��ǥ
			8, // Sheet �� ���� ������
			11, // Sheet �� ���� ������
			RGB(255, 0, 255)); // ������ �ȼ� ����


	// RingCount
	int iRingCount = CGameStateManager::Get_Instance()->Get_RingCount();
	int iNumOnes = iRingCount % 10; // ���� �ڸ� ����
	int iNumTens = iRingCount % 100 / 10; // ���� �ڸ� ����
	int iNumHundreds = iRingCount / 100;

	if (m_iCurrOnesNum != iNumOnes &&
		m_dwNumberWaitStartTime + m_dwNumberTimeInterval < GetTickCount())
	{
		m_dwNumberWaitStartTime = GetTickCount();
		++m_iCurrOnesNum;
		CSoundManager::Get_Instance()->StopSound(SOUND_EFFECT);
		CSoundManager::Get_Instance()->PlaySoundW(L"NumberCounting.wav", SOUND_EFFECT, 1.f);
	}

	// ���� �ڸ�
	GdiTransparentBlt(
		// ���� â DC
		_dc,	// ������� ���� ��� dc
		263, // DC�������� x��ǥ
		167,// DC�������� y��ǥ
		8, // ��Ʈ���� ���� ������
		11, // ��Ʈ���� ���� ������

		// Bmp �� DC
		hMemDC, // �����  �̹��� dc
		m_iCurrOnesNum * 8, // ��Ʈ���� ��� ���� x��ǥ
		0,	// ��Ʈ���� ��� ���� Y��ǥ
		8, // Sheet �� ���� ������
		11, // Sheet �� ���� ������
		RGB(255, 0, 255)); // ������ �ȼ� ����

	// ���� �ڸ�
	if ((iNumTens > 0 && m_iCurrOnesNum == iNumOnes))
	{
		if (m_iCurrTensNum != iNumTens &&
			m_dwNumberWaitStartTime + m_dwNumberTimeInterval < GetTickCount())
		{
			m_dwNumberWaitStartTime = GetTickCount();
			++m_iCurrTensNum;
			CSoundManager::Get_Instance()->StopSound(SOUND_EFFECT);
			CSoundManager::Get_Instance()->PlaySoundW(L"NumberCounting.wav", SOUND_EFFECT, 1.f);
		}
		GdiTransparentBlt(
			// ���� â DC
			_dc,	// ������� ���� ��� dc
			255, // DC�������� x��ǥ
			167,// DC�������� y��ǥ
			8, // ��Ʈ���� ���� ������
			11, // ��Ʈ���� ���� ������

			// Bmp �� DC
			hMemDC, // �����  �̹��� dc
			m_iCurrTensNum * 8, // ��Ʈ���� ��� ���� x��ǥ
			0,	// ��Ʈ���� ��� ���� Y��ǥ
			8, // Sheet �� ���� ������
			11, // Sheet �� ���� ������
			RGB(255, 0, 255)); // ������ �ȼ� ����

		if ((iNumHundreds > 0 && m_iCurrTensNum == iNumTens))
		{
			if (m_iCurrHunidsNum != iNumHundreds &&
				m_dwNumberWaitStartTime + m_dwNumberTimeInterval < GetTickCount())
			{
				m_dwNumberWaitStartTime = GetTickCount();
				++m_iCurrHunidsNum;
				CSoundManager::Get_Instance()->StopSound(SOUND_EFFECT);
				CSoundManager::Get_Instance()->PlaySoundW(L"NumberCounting.wav", SOUND_EFFECT, 1.f);
			}
			GdiTransparentBlt(
				// ���� â DC
				_dc,	// ������� ���� ��� dc
				247, // DC�������� x��ǥ
				167,// DC�������� y��ǥ
				8, // ��Ʈ���� ���� ������
				11, // ��Ʈ���� ���� ������

				// Bmp �� DC
				hMemDC, // �����  �̹��� dc
				m_iCurrHunidsNum * 8, // ��Ʈ���� ��� ���� x��ǥ
				0,	// ��Ʈ���� ��� ���� Y��ǥ
				8, // Sheet �� ���� ������
				11, // Sheet �� ���� ������
				RGB(255, 0, 255)); // ������ �ȼ� ����
		}


		if (m_iCurrHunidsNum == iNumHundreds)
		{
			int iTotalScore = iScoreCount + iRingCount * 100;
			int iTotalHundreds = iTotalScore % 1000 / 100;
			int iTotalThousands = iTotalScore % 10000 / 1000;
			int iTotalTenThousands = iTotalScore % 100000 / 10000;


			// 1�� �ڸ� 0
			GdiTransparentBlt(
				// ���� â DC
				_dc,	// ������� ���� ��� dc
				263, // DC�������� x��ǥ
				196,// DC�������� y��ǥ
				8, // ��Ʈ���� ���� ������
				11, // ��Ʈ���� ���� ������

				// Bmp �� DC
				hMemDC, // �����  �̹��� dc
				0, // ��Ʈ���� ��� ���� x��ǥ
				0,	// ��Ʈ���� ��� ���� Y��ǥ
				8, // Sheet �� ���� ������
				11, // Sheet �� ���� ������
				RGB(255, 0, 255)); // ������ �ȼ� ����
			// 10 �� �ڸ� 0
			GdiTransparentBlt(
				// ���� â DC
				_dc,	// ������� ���� ��� dc
				255, // DC�������� x��ǥ
				196,// DC�������� y��ǥ
				8, // ��Ʈ���� ���� ������
				11, // ��Ʈ���� ���� ������

				// Bmp �� DC
				hMemDC, // �����  �̹��� dc
				0, // ��Ʈ���� ��� ���� x��ǥ
				0,	// ��Ʈ���� ��� ���� Y��ǥ
				8, // Sheet �� ���� ������
				11, // Sheet �� ���� ������
				RGB(255, 0, 255)); // ������ �ȼ� ����

			// 100�� �ڸ�
			GdiTransparentBlt(
				// ���� â DC
				_dc,	// ������� ���� ��� dc
				247, // DC�������� x��ǥ
				196,// DC�������� y��ǥ
				8, // ��Ʈ���� ���� ������
				11, // ��Ʈ���� ���� ������

				// Bmp �� DC
				hMemDC, // �����  �̹��� dc
				iTotalHundreds * 8, // ��Ʈ���� ��� ���� x��ǥ
				0,	// ��Ʈ���� ��� ���� Y��ǥ
				8, // Sheet �� ���� ������
				11, // Sheet �� ���� ������
				RGB(255, 0, 255)); // ������ �ȼ� ����

			if (iTotalScore >= 1000)
				// 100�� �ڸ�
				GdiTransparentBlt(
					// ���� â DC
					_dc,	// ������� ���� ��� dc
					239, // DC�������� x��ǥ
					196,// DC�������� y��ǥ
					8, // ��Ʈ���� ���� ������
					11, // ��Ʈ���� ���� ������

					// Bmp �� DC
					hMemDC, // �����  �̹��� dc
					iTotalThousands * 8, // ��Ʈ���� ��� ���� x��ǥ
					0,	// ��Ʈ���� ��� ���� Y��ǥ
					8, // Sheet �� ���� ������
					11, // Sheet �� ���� ������
					RGB(255, 0, 255)); // ������ �ȼ� ����

			if (iTotalScore >= 10000)
				// 100�� �ڸ�
				GdiTransparentBlt(
					// ���� â DC
					_dc,	// ������� ���� ��� dc
					231, // DC�������� x��ǥ
					196,// DC�������� y��ǥ
					8, // ��Ʈ���� ���� ������
					11, // ��Ʈ���� ���� ������

					// Bmp �� DC
					hMemDC, // �����  �̹��� dc
					iTotalTenThousands * 8, // ��Ʈ���� ��� ���� x��ǥ
					0,	// ��Ʈ���� ��� ���� Y��ǥ
					8, // Sheet �� ���� ������
					11, // Sheet �� ���� ������
					RGB(255, 0, 255)); // ������ �ȼ� ����

			if (!m_bTotalTick)
			{
				m_bTotalTick = true;
				CSoundManager::Get_Instance()->StopSound(SOUND_EFFECT);
				CSoundManager::Get_Instance()->PlaySoundW(L"SE_Totalring_Decision.wav", SOUND_EFFECT, 1.f);
			}
		}
	}



}

void CUIManager::RenderBossEnd(HDC _dc)
{// Total Point
	HDC hMemDC = CBmpManager::Get_Instance()->Find_Img(L"End2");
	int iDestX = 200 - 105;
	int iDestY = 150 - 58;

	// Total Top
	if (abs(m_iCurrTopX - iDestX) > 1)
	{
		m_iCurrTopX = CMyMath::Lerp(m_iCurrTopX, iDestX, g_fDeltaTime * 1.2f);
	}
	else
		m_iCurrTopX = iDestX;

	GdiTransparentBlt(
		_dc,	// ������� ���� ��� dc
		m_iCurrTopX, // DC�������� x��ǥ
		iDestY,// DC�������� y��ǥ
		211, // ��Ʈ���� ���� ������
		50, // ��Ʈ���� ���� ������

		// Bmp �� DC
		hMemDC, // �����  �̹��� dc
		0, // ��Ʈ���� ��� ���� x��ǥ
		0,	// ��Ʈ���� ��� ���� Y��ǥ
		211, // Sheet �� ���� ������
		50, // Sheet �� ���� ������
		RGB(255, 0, 255)); // ������ �ȼ� ����

	if (abs(m_iCurrTopX - iDestX) > 1)
		return;

	// Total Middle
	if (abs(m_iCurrMiddleX - iDestX) > 1)
		m_iCurrMiddleX = CMyMath::Lerp(m_iCurrMiddleX, iDestX, g_fDeltaTime * 1.f);
	else
		m_iCurrMiddleX = iDestX;
	GdiTransparentBlt(
		_dc,	// ������� ���� ��� dc
		m_iCurrMiddleX, // DC�������� x��ǥ
		iDestY + 50,// DC�������� y��ǥ
		211, // ��Ʈ���� ���� ������
		50, // ��Ʈ���� ���� ������

		// Bmp �� DC
		hMemDC, // �����  �̹��� dc
		0, // ��Ʈ���� ��� ���� x��ǥ
		50,	// ��Ʈ���� ��� ���� Y��ǥ
		211, // Sheet �� ���� ������
		50, // Sheet �� ���� ������
		RGB(255, 0, 255)); // ������ �ȼ� ����

	if (abs(m_iCurrMiddleX - iDestX) > 1)
		return;

	// Total Down
	if (abs(m_iCurrBottomX - iDestX) > 1)
		m_iCurrBottomX = CMyMath::Lerp(m_iCurrBottomX, iDestX, g_fDeltaTime * 1.3f);
	else
		m_iCurrBottomX = iDestX;

	GdiTransparentBlt(
		_dc,	// ������� ���� ��� dc
		m_iCurrBottomX, // DC�������� x��ǥ
		iDestY + 100,// DC�������� y��ǥ
		211, // ��Ʈ���� ���� ������
		17, // ��Ʈ���� ���� ������

		// Bmp �� DC
		hMemDC, // �����  �̹��� dc
		0, // ��Ʈ���� ��� ���� x��ǥ
		100,	// ��Ʈ���� ��� ���� Y��ǥ
		211, // Sheet �� ���� ������
		17, // Sheet �� ���� ������
		RGB(255, 0, 255)); // ������ �ȼ� ����

	if (abs(m_iCurrBottomX - iDestX) > 1)
		return;

	if (!m_bNumberAnimTick)
	{
		m_bNumberAnimTick = true;
		m_dwNumberWaitStartTime = GetTickCount();
	}
	//--------------------------- ���� ������ ����
	hMemDC = CBmpManager::Get_Instance()->Find_Img(L"Number");

	// scoreCount
	int iScoreCount = CGameStateManager::Get_Instance()->Get_CurrentScore();
	int iScoreOnes = iScoreCount % 1000 / 100; // ���� �ڸ� ����
	int iScoreTens = iScoreCount / 1000; // ���� �ڸ� ����

	//score 1���ڸ�
	GdiTransparentBlt(
		// ���� â DC
		_dc,	// ������� ���� ��� dc
		263, // DC�������� x��ǥ
		150,// DC�������� y��ǥ
		8, // ��Ʈ���� ���� ������
		11, // ��Ʈ���� ���� ������
		// Bmp �� DC
		hMemDC, // �����  �̹��� dc
		0, // ��Ʈ���� ��� ���� x��ǥ
		0,	// ��Ʈ���� ��� ���� Y��ǥ
		8, // Sheet �� ���� ������
		11, // Sheet �� ���� ������
		RGB(255, 0, 255)); // ������ �ȼ� ����

	if (iScoreCount > 0)
	{		
		// 10�� �ڸ�
		GdiTransparentBlt(
			// ���� â DC
			_dc,	// ������� ���� ��� dc
			255, // DC�������� x��ǥ
			150,// DC�������� y��ǥ
			8, // ��Ʈ���� ���� ������
			11, // ��Ʈ���� ���� ������
			// Bmp �� DC
			hMemDC, // �����  �̹��� dc
			0, // ��Ʈ���� ��� ���� x��ǥ
			0,	// ��Ʈ���� ��� ���� Y��ǥ
			8, // Sheet �� ���� ������
			11, // Sheet �� ���� ������
			RGB(255, 0, 255)); // ������ �ȼ� ����
	}

	// score	100�� �ڸ�
	GdiTransparentBlt(
		// ���� â DC
		_dc,	// ������� ���� ��� dc
		247, // DC�������� x��ǥ
		150,// DC�������� y��ǥ
		8, // ��Ʈ���� ���� ������
		11, // ��Ʈ���� ���� ������

		// Bmp �� DC
		hMemDC, // �����  �̹��� dc
		iScoreOnes * 8, // ��Ʈ���� ��� ���� x��ǥ
		0,	// ��Ʈ���� ��� ���� Y��ǥ
		8, // Sheet �� ���� ������
		11, // Sheet �� ���� ������
		RGB(255, 0, 255)); // ������ �ȼ� ����
	// ���� �ڸ�
	if (iScoreTens > 0)
		GdiTransparentBlt(
			// ���� â DC
			_dc,	// ������� ���� ��� dc
			239, // DC�������� x��ǥ
			150,// DC�������� y��ǥ
			8, // ��Ʈ���� ���� ������
			11, // ��Ʈ���� ���� ������
			// Bmp �� DC
			hMemDC, // �����  �̹��� dc
			iScoreTens * 8, // ��Ʈ���� ��� ���� x��ǥ
			0,	// ��Ʈ���� ��� ���� Y��ǥ
			8, // Sheet �� ���� ������
			11, // Sheet �� ���� ������
			RGB(255, 0, 255)); // ������ �ȼ� ����


	// RingCount
	int iRingCount = CGameStateManager::Get_Instance()->Get_RingCount();
	int iNumOnes = iRingCount % 10; // ���� �ڸ� ����
	int iNumTens = iRingCount / 10; // ���� �ڸ� ����

	if (m_iCurrOnesNum != iNumOnes &&
		m_dwNumberWaitStartTime + m_dwNumberTimeInterval < GetTickCount())
	{
		m_dwNumberWaitStartTime = GetTickCount();
		++m_iCurrOnesNum;
		CSoundManager::Get_Instance()->StopSound(SOUND_EFFECT);
		CSoundManager::Get_Instance()->PlaySoundW(L"NumberCounting.wav", SOUND_EFFECT, 1.f);
	}

	// ���� �ڸ�
	GdiTransparentBlt(
		// ���� â DC
		_dc,	// ������� ���� ��� dc
		263, // DC�������� x��ǥ
		167,// DC�������� y��ǥ
		8, // ��Ʈ���� ���� ������
		11, // ��Ʈ���� ���� ������

		// Bmp �� DC
		hMemDC, // �����  �̹��� dc
		m_iCurrOnesNum * 8, // ��Ʈ���� ��� ���� x��ǥ
		0,	// ��Ʈ���� ��� ���� Y��ǥ
		8, // Sheet �� ���� ������
		11, // Sheet �� ���� ������
		RGB(255, 0, 255)); // ������ �ȼ� ����

	// ���� �ڸ�
	if ((iNumTens > 0 && m_iCurrOnesNum == iNumOnes))
	{
		if (m_iCurrTensNum != iNumTens &&
			m_dwNumberWaitStartTime + m_dwNumberTimeInterval < GetTickCount())
		{
			m_dwNumberWaitStartTime = GetTickCount();
			++m_iCurrTensNum;
			CSoundManager::Get_Instance()->StopSound(SOUND_EFFECT);
			CSoundManager::Get_Instance()->PlaySoundW(L"NumberCounting.wav", SOUND_EFFECT, 1.f);
		}
		GdiTransparentBlt(
			// ���� â DC
			_dc,	// ������� ���� ��� dc
			255, // DC�������� x��ǥ
			167,// DC�������� y��ǥ
			8, // ��Ʈ���� ���� ������
			11, // ��Ʈ���� ���� ������

			// Bmp �� DC
			hMemDC, // �����  �̹��� dc
			m_iCurrTensNum * 8, // ��Ʈ���� ��� ���� x��ǥ
			0,	// ��Ʈ���� ��� ���� Y��ǥ
			8, // Sheet �� ���� ������
			11, // Sheet �� ���� ������
			RGB(255, 0, 255)); // ������ �ȼ� ����

		if (m_iCurrTensNum == iNumTens)
		{
			int iTotalScore = iScoreCount + iRingCount * 100;
			int iTotalHundreds = iTotalScore % 1000 / 100;
			int iTotalThousands = iTotalScore % 10000 / 1000;
			int iTotalTenThousands = iTotalScore % 100000 / 10000;
			

			// 1�� �ڸ� 0
			GdiTransparentBlt(
				// ���� â DC
				_dc,	// ������� ���� ��� dc
				263, // DC�������� x��ǥ
				196,// DC�������� y��ǥ
				8, // ��Ʈ���� ���� ������
				11, // ��Ʈ���� ���� ������

				// Bmp �� DC
				hMemDC, // �����  �̹��� dc
				0, // ��Ʈ���� ��� ���� x��ǥ
				0,	// ��Ʈ���� ��� ���� Y��ǥ
				8, // Sheet �� ���� ������
				11, // Sheet �� ���� ������
				RGB(255, 0, 255)); // ������ �ȼ� ����
			// 10 �� �ڸ� 0
			GdiTransparentBlt(
				// ���� â DC
				_dc,	// ������� ���� ��� dc
				255, // DC�������� x��ǥ
				196,// DC�������� y��ǥ
				8, // ��Ʈ���� ���� ������
				11, // ��Ʈ���� ���� ������

				// Bmp �� DC
				hMemDC, // �����  �̹��� dc
				0, // ��Ʈ���� ��� ���� x��ǥ
				0,	// ��Ʈ���� ��� ���� Y��ǥ
				8, // Sheet �� ���� ������
				11, // Sheet �� ���� ������
				RGB(255, 0, 255)); // ������ �ȼ� ����

			// 100�� �ڸ�
			GdiTransparentBlt(
				// ���� â DC
				_dc,	// ������� ���� ��� dc
				247, // DC�������� x��ǥ
				196,// DC�������� y��ǥ
				8, // ��Ʈ���� ���� ������
				11, // ��Ʈ���� ���� ������

				// Bmp �� DC
				hMemDC, // �����  �̹��� dc
				iTotalHundreds * 8, // ��Ʈ���� ��� ���� x��ǥ
				0,	// ��Ʈ���� ��� ���� Y��ǥ
				8, // Sheet �� ���� ������
				11, // Sheet �� ���� ������
				RGB(255, 0, 255)); // ������ �ȼ� ����

			if (iTotalScore >= 1000)
			// 100�� �ڸ�
			GdiTransparentBlt(
				// ���� â DC
				_dc,	// ������� ���� ��� dc
				239, // DC�������� x��ǥ
				196,// DC�������� y��ǥ
				8, // ��Ʈ���� ���� ������
				11, // ��Ʈ���� ���� ������

				// Bmp �� DC
				hMemDC, // �����  �̹��� dc
				iTotalThousands * 8, // ��Ʈ���� ��� ���� x��ǥ
				0,	// ��Ʈ���� ��� ���� Y��ǥ
				8, // Sheet �� ���� ������
				11, // Sheet �� ���� ������
				RGB(255, 0, 255)); // ������ �ȼ� ����

			if (iTotalScore >= 10000)
			// 100�� �ڸ�
			GdiTransparentBlt(
				// ���� â DC
				_dc,	// ������� ���� ��� dc
				231, // DC�������� x��ǥ
				196,// DC�������� y��ǥ
				8, // ��Ʈ���� ���� ������
				11, // ��Ʈ���� ���� ������

				// Bmp �� DC
				hMemDC, // �����  �̹��� dc
				iTotalTenThousands * 8, // ��Ʈ���� ��� ���� x��ǥ
				0,	// ��Ʈ���� ��� ���� Y��ǥ
				8, // Sheet �� ���� ������
				11, // Sheet �� ���� ������
				RGB(255, 0, 255)); // ������ �ȼ� ����

			if (!m_bTotalTick)
			{
				m_bTotalTick = true;
				CSoundManager::Get_Instance()->StopSound(SOUND_EFFECT);
				CSoundManager::Get_Instance()->PlaySoundW(L"SE_Totalring_Decision.wav", SOUND_EFFECT, 1.f);								
			}
		}
	}

}

void CUIManager::RenderInventory(HDC _dc)
{
	if (m_bInventoryOn == false)
		return;

	HDC hMemDC = CBmpManager::Get_Instance()->Find_Img(L"Inventory");
	GdiTransparentBlt(
		// ���� â DC
		_dc,	// ������� ���� ��� dc
		WINCX - 216, // DC�������� x��ǥ
		0,// DC�������� y��ǥ
		216, // ��Ʈ���� ���� ������
		WINCY, // ��Ʈ���� ���� ������

		// Bmp �� DC
		hMemDC, // �����  �̹��� dc
		0, // ��Ʈ���� ��� ���� x��ǥ
		0,	// ��Ʈ���� ��� ���� Y��ǥ
		216, // Sheet �� ���� ������
		WINCY, // Sheet �� ���� ������
		RGB(255, 0, 255)); // ������ �ȼ� ����

	// ���� ������
	hMemDC = CBmpManager::Get_Instance()->Find_Img(L"Item");

	int iIndex = 0;
	list<CItemObj*>& refList = m_pInventory->Get_ItemList();
	int iInventoryLeftTopX = WINCX - 216 + 27;
	int iInventoryLeftTopY = 26;
	int iInterval = 65;

	
	for (auto iter = refList.begin(); iter != refList.end(); ++iter, ++iIndex)
	{
		if (*iter == nullptr)
		{
			continue;
		}
		else
		{
			if (*iter == m_pFocusingObj)
				continue;

			int iItemID = (*iter)->Get_ItemID();
			int i = iIndex / 3; // ����
			int j = iIndex - i * 3; // ���� , ��

			GdiTransparentBlt(
				// ���� â DC
				_dc,	// ������� ���� ��� dc
				iInventoryLeftTopX + j * iInterval, // DC�������� x��ǥ
				iInventoryLeftTopY + i * iInterval,// DC�������� y��ǥ
				52, // ��Ʈ���� ���� ������
				52, // ��Ʈ���� ���� ������

				// Bmp �� DC
				hMemDC, // �����  �̹��� dc
				56, // ��Ʈ���� ��� ���� x��ǥ
				32 * iItemID,	// ��Ʈ���� ��� ���� Y��ǥ
				28, // Sheet �� ���� ������
				32, // Sheet �� ���� ������
				RGB(255, 0, 255)); // ������ �ȼ� ����
		}
	}

	if (m_pFocusingObj)
	{
		GdiTransparentBlt(
			// ���� â DC
			_dc,	// ������� ���� ��� dc
			ChangePt.x-14, // DC�������� x��ǥ
			ChangePt.y-16,// DC�������� y��ǥ
			52, // ��Ʈ���� ���� ������
			52, // ��Ʈ���� ���� ������

			// Bmp �� DC
			hMemDC, // �����  �̹��� dc
			56, // ��Ʈ���� ��� ���� x��ǥ
			32 * m_pFocusingObj->Get_ItemID(),	// ��Ʈ���� ��� ���� Y��ǥ
			28, // Sheet �� ���� ������
			32, // Sheet �� ���� ������
			RGB(255, 0, 255)); // ������ �ȼ� ����
	}

}

void CUIManager::Render(HDC _dc)
{
	switch (m_eState)
	{
	case UI_PLAY:
		RenderPlay(_dc);
		break;
	case UI_END:
		RenderEnd(_dc);
		break;
	case UI_BOSS_END:
		RenderBossEnd(_dc);
		break;
	}
}

void CUIManager::Release()
{
}


void CUIManager::Render_SelectEditMode_Mushroom(HDC _dc)
{
	HDC hMemDC = CBmpManager::Get_Instance()->Find_Img(L"TileMap_Mushroom");
	int iTmp = m_iTileSelectPage * 20; // == i * 8 + j  // i == 2, j == 6
	int dim = (iTmp / 8);
	int col = (iTmp - 8* dim); //�� 7 //0: 0,0  1:

	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 5; ++j)
		{
			if (col > 7)
			{
				col = 0;
				dim++;
			}
			TranspBlt(_dc, hMemDC, TILECX, j * TILECX, i * TILECX, col++, dim);
		}
	}

	for (int i = 0; i < 5; ++i)
	{
		Rectangle(_dc, i * 160, 512, (i+1) * 160, 600);		
	}
}

void CUIManager::Render_BossHPBar(HDC _dc)
{
	CObj* pBoss = CObjManager::Get_Instance()->Get_ObjList()[ID_MECHASONIC].front();
	int iMaxHP = dynamic_cast<CMovableObject*>(pBoss)->Get_MaxHP();
	int iCurrentHP = dynamic_cast<CMovableObject*>(pBoss)->Get_CurrentHP();

	if (CKeyManager::Get_Instance()->Key_Down(VK_F1))
		dynamic_cast<CMovableObject*>(pBoss)->Set_HPMinus();


	int iStartPosX = WINCX * 0.3f;
	int iStartPosY = WINCY * 0.1f;
	int iUnitHPWidth = 9; // hp 1�� ũ��
	int iHeight = 9; // �� ����

	m_iCurrentBarPoint = CMyMath::Lerp(m_iCurrentBarPoint,
		iStartPosX + iUnitHPWidth * iCurrentHP, g_fDeltaTime * 0.8f);

	RECT fullRECT = { iStartPosX, iStartPosY, iStartPosX + iUnitHPWidth * iMaxHP, iStartPosY + iHeight };
	RECT currentRECT = { iStartPosX, iStartPosY,
		m_iCurrentBarPoint,
		iStartPosY + iHeight };

	Rectangle(_dc, fullRECT.left, fullRECT.top, fullRECT.right, fullRECT.bottom);
	Rectangle(_dc, currentRECT.left, currentRECT.top, currentRECT.right, currentRECT.bottom);

	SetDCBrushColor(_dc, RGB(77, 77, 77)); // dark Gray
	FillRect(_dc, &fullRECT, (HBRUSH)GetStockObject(DC_BRUSH));
	SetDCBrushColor(_dc, RGB(255, 70, 70));
	FillRect(_dc, &currentRECT, (HBRUSH)GetStockObject(DC_BRUSH));
}

void CUIManager::TranspBlt(HDC _dc, HDC _hMemDC, int _iSize, int _DCX, int _DCY, int _BitmapIndxX, int _BitmapIndxY)
{
	GdiTransparentBlt(
		_dc,	// ������� ���� ��� dc
		_DCX, // 128 * i
		_DCY,
		_iSize, // ��Ʈ���� ���� ������
		_iSize, // ��Ʈ���� ���� ������

		// Bmp �� DC
		_hMemDC, // �����  �̹��� dc
		_iSize * _BitmapIndxX, // ��Ʈ���� ��� ���� x��ǥ
		_iSize * _BitmapIndxY,	// ��Ʈ���� ��� ���� Y��ǥ
		_iSize, // Sheet �� ���� ������
		_iSize, // Sheet �� ���� ������
		RGB(255, 0, 255)); // ������ �ȼ� ����
}
