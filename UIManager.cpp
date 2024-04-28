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
		if (m_pFocusingObj && ChangePt.x < WINCX - 216) // 버리기
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
			// 지금 뭐 있는지 마우스 위치에
			if (ChangePt.x >= WINCX - 216 + 27 && ChangePt.y >= 26)
			{
				int iIndex = -1;

				auto iter = refList.begin();
#pragma region indexing
				if (26 <= ChangePt.y && ChangePt.y < 91) // 첫 째 줄
				{
					if (ChangePt.x < WINCX - 216 + 27 + 65) // 첫 칸
					{
						iIndex = 0;
					}
					else if (ChangePt.x < WINCX - 216 + 27 + 65 + 65) // 둘 칸
					{
						iIndex = 1;
					}
					else if (ChangePt.x < WINCX - 216 + 27 + 65 + 65 + 65) // 셋 칸
					{
						iIndex = 2;
					}

				}
				else if (26 + 65 <= ChangePt.y && ChangePt.y < 91 + 65) // 둘 째 줄
				{
					if (ChangePt.x < WINCX - 216 + 27 + 65) // 첫 칸
					{
						iIndex = 3;
					}
					else if (ChangePt.x < WINCX - 216 + 27 + 65 + 65) // 둘 칸
					{
						iIndex = 4;
					}
					else if (InvenPt.x < WINCX - 216 + 27 + 65 + 65 + 65) // 셋 칸
					{
						iIndex = 5;
					}

				}
				else if (26 + 65 + 65 <= ChangePt.y && ChangePt.y < 91 + 65 + 65) // 셋 째 줄
				{
					if (ChangePt.x < WINCX - 216 + 27 + 65) // 첫 칸
					{
						iIndex = 6;
					}
					else if (ChangePt.x < WINCX - 216 + 27 + 65 + 65) // 둘 칸
					{
						iIndex = 7;
					}
					else if (ChangePt.x < WINCX - 216 + 27 + 65 + 65 + 65) // 셋 칸
					{
						iIndex = 8;
					}

				}
				else if (26 + 65 + 65 + 65 <= ChangePt.y && ChangePt.y < 91 + 65 + 65 + 65) // 넷 째 줄
				{
					if (ChangePt.x < WINCX - 216 + 27 + 65) // 첫 칸
					{
						iIndex = 9;
					}
					else if (ChangePt.x < WINCX - 216 + 27 + 65 + 65) // 둘 칸
					{
						iIndex = 10;
					}
					else if (ChangePt.x < WINCX - 216 + 27 + 65 + 65 + 65) // 셋 칸
					{
						iIndex = 11;
					}

				}

				for (int i = 0; (i < iIndex && iter != refList.end()); ++i)
					++iter;
#pragma endregion

				if (iter != refList.end() && *iter) // 그 인덱스에 뭐라도 있으면
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
			if (26 <= ChangePt.y && ChangePt.y < 91) // 첫 째 줄
			{
				if (ChangePt.x < WINCX - 216 + 27 + 65) // 첫 칸
				{
					iIndex = 0;
				}
				else if (ChangePt.x < WINCX - 216 + 27 + 65 + 65) // 둘 칸
				{
					iIndex = 1;
				}
				else if (ChangePt.x < WINCX - 216 + 27 + 65 + 65 + 65) // 셋 칸
				{
					iIndex = 2;
				}

			}
			else if (26 + 65 <= ChangePt.y && ChangePt.y < 91 + 65) // 둘 째 줄
			{
				if (ChangePt.x < WINCX - 216 + 27 + 65) // 첫 칸
				{
					iIndex = 3;
				}
				else if (ChangePt.x < WINCX - 216 + 27 + 65 + 65) // 둘 칸
				{
					iIndex = 4;
				}
				else if (InvenPt.x < WINCX - 216 + 27 + 65 + 65 + 65) // 셋 칸
				{
					iIndex = 5;
				}

			}
			else if (26 + 65 + 65 <= ChangePt.y && ChangePt.y < 91 + 65 + 65) // 셋 째 줄
			{
				if (ChangePt.x < WINCX - 216 + 27 + 65) // 첫 칸
				{
					iIndex = 6;
				}
				else if (ChangePt.x < WINCX - 216 + 27 + 65 + 65) // 둘 칸
				{
					iIndex = 7;
				}
				else if (ChangePt.x < WINCX - 216 + 27 + 65 + 65 + 65) // 셋 칸
				{
					iIndex = 8;
				}

			}
			else if (26 + 65 + 65 + 65 <= ChangePt.y && ChangePt.y < 91 + 65 + 65 + 65) // 넷 째 줄
			{
				if (ChangePt.x < WINCX - 216 + 27 + 65) // 첫 칸
				{
					iIndex = 9;
				}
				else if (ChangePt.x < WINCX - 216 + 27 + 65 + 65) // 둘 칸
				{
					iIndex = 10;
				}
				else if (ChangePt.x < WINCX - 216 + 27 + 65 + 65 + 65) // 셋 칸
				{
					iIndex = 11;
				}

			}

			for (int i = 0; (i < iIndex && iter != refList.end()); ++i)
				++iter;
#pragma endregion

			if (iter != refList.end() && *iter) // 그 인덱스에 뭐라도 있으면
			{
				m_pFocusingObj = (*iter);
			}
		}
	}
	else if (m_pFocusingObj)
	{

	}







	// 인벤토리 창 밖에있으면 리턴
	if (InvenPt.x < WINCX - 216 + 27 || InvenPt.y < 26)
		return;

	int iIndex = -1;

	auto iter = refList.begin();
	if (26 <= InvenPt.y && InvenPt.y < 91) // 첫 째 줄
	{
		if (InvenPt.x < WINCX - 216 + 27 + 65) // 첫 칸
		{
			iIndex = 0;
		}
		else if (InvenPt.x < WINCX - 216 + 27 + 65 + 65) // 둘 칸
		{
			iIndex = 1;
		}
		else if (InvenPt.x < WINCX - 216 + 27 + 65 + 65 + 65) // 셋 칸
		{
			iIndex = 2;
		}

	}
	else if (26 + 65 <= InvenPt.y && InvenPt.y < 91 + 65) // 둘 째 줄
	{
		if (InvenPt.x < WINCX - 216 + 27 + 65) // 첫 칸
		{
			iIndex = 3;
		}
		else if (InvenPt.x < WINCX - 216 + 27 + 65 + 65) // 둘 칸
		{
			iIndex = 4;
		}
		else if (InvenPt.x < WINCX - 216 + 27 + 65 + 65 + 65) // 셋 칸
		{
			iIndex = 5;
		}

	}
	else if (26 + 65 + 65 <= InvenPt.y && InvenPt.y < 91 + 65 + 65) // 셋 째 줄
	{
		if (InvenPt.x < WINCX - 216 + 27 + 65) // 첫 칸
		{
			iIndex = 6;
		}
		else if (InvenPt.x < WINCX - 216 + 27 + 65 + 65) // 둘 칸
		{
			iIndex = 7;
		}
		else if (InvenPt.x < WINCX - 216 + 27 + 65 + 65 + 65) // 셋 칸
		{
			iIndex = 8;
		}

	}
	else if (26 + 65 + 65 + 65 <= InvenPt.y && InvenPt.y < 91 + 65 + 65 + 65) // 넷 째 줄
	{
		if (InvenPt.x < WINCX - 216 + 27 + 65) // 첫 칸
		{
			iIndex = 9;
		}
		else if (InvenPt.x < WINCX - 216 + 27 + 65 + 65) // 둘 칸
		{
			iIndex = 10;
		}
		else if (InvenPt.x < WINCX - 216 + 27 + 65 + 65 + 65) // 셋 칸
		{
			iIndex = 11;
		}

	}

	for (int i = 0; i < iIndex && iter != refList.end(); ++i)
		++iter;

	if (iter != refList.end() && *iter) // 그 인덱스에 뭐라도 있으면
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
		// 기존 창 DC
		_dc,	// 복사받을 최종 출력 dc
		0, // DC내에서의 x좌표
		15,// DC내에서의 y좌표
		iRectWidth, // 비트맵의 가로 사이즈
		iRectHeight, // 비트맵의 세로 사이즈

		// Bmp 의 DC
		hMemDC, // 출력할  이미지 dc
		0, // 비트맵의 출력 시작 x좌표
		0,	// 비트맵의 출력 시작 Y좌표
		48, // Sheet 의 가로 사이즈
		25, // Sheet 의 세로 사이즈
		RGB(255, 0, 255)); // 제거할 픽셀 색상값

	// SONIC X (Life)
	GdiTransparentBlt(
		// 기존 창 DC
		_dc,	// 복사받을 최종 출력 dc
		7, // DC내에서의 x좌표
		WINCY - 25,// DC내에서의 y좌표
		iRectWidth, // 비트맵의 가로 사이즈
		iRectHeight, // 비트맵의 세로 사이즈

		// Bmp 의 DC
		hMemDC, // 출력할  이미지 dc
		48, // 비트맵의 출력 시작 x좌표
		0,	// 비트맵의 출력 시작 Y좌표
		48, // Sheet 의 가로 사이즈
		25, // Sheet 의 세로 사이즈
		RGB(255, 0, 255)); // 제거할 픽셀 색상값

	hMemDC = CBmpManager::Get_Instance()->Find_Img(L"SmallNumber");
	GdiTransparentBlt(
		// 기존 창 DC
		_dc,	// 복사받을 최종 출력 dc
		40, // DC내에서의 x좌표
		WINCY - 12,// DC내에서의 y좌표
		8, // 비트맵의 가로 사이즈
		6, // 비트맵의 세로 사이즈

		// Bmp 의 DC
		hMemDC, // 출력할  이미지 dc
		8 * CGameStateManager::Get_Instance()->Get_LifeCount(), // 비트맵의 출력 시작 x좌표
		0,	// 비트맵의 출력 시작 Y좌표
		8, // Sheet 의 가로 사이즈
		6, // Sheet 의 세로 사이즈
		RGB(255, 0, 255)); // 제거할 픽셀 색상값

	// RingCount
	int iRingCount = CGameStateManager::Get_Instance()->Get_RingCount();
	int iNumOnes = iRingCount % 10; // 일의 자리 숫자
	int iNumTens = iRingCount % 100 / 10; // 십의 자리 숫자
	int iNumHundreds = iRingCount / 100;
	hMemDC = CBmpManager::Get_Instance()->Find_Img(L"Number");

	// 일의 자리
	GdiTransparentBlt(
		// 기존 창 DC
		_dc,	// 복사받을 최종 출력 dc
		100, // DC내에서의 x좌표
		28,// DC내에서의 y좌표
		8, // 비트맵의 가로 사이즈
		11, // 비트맵의 세로 사이즈

		// Bmp 의 DC
		hMemDC, // 출력할  이미지 dc
		iNumOnes * 8, // 비트맵의 출력 시작 x좌표
		0,	// 비트맵의 출력 시작 Y좌표
		8, // Sheet 의 가로 사이즈
		11, // Sheet 의 세로 사이즈
		RGB(255, 0, 255)); // 제거할 픽셀 색상값
	// 십의 자리
	if (iNumTens > 0)
		GdiTransparentBlt(
		// 기존 창 DC
		_dc,	// 복사받을 최종 출력 dc
		92, // DC내에서의 x좌표
		28,// DC내에서의 y좌표
		8, // 비트맵의 가로 사이즈
		11, // 비트맵의 세로 사이즈
		// Bmp 의 DC
		hMemDC, // 출력할  이미지 dc
		iNumTens * 8, // 비트맵의 출력 시작 x좌표
		0,	// 비트맵의 출력 시작 Y좌표
		8, // Sheet 의 가로 사이즈
		11, // Sheet 의 세로 사이즈
		RGB(255, 0, 255)); // 제거할 픽셀 색상값

	if (iNumHundreds > 0)
		GdiTransparentBlt(
			// 기존 창 DC
			_dc,	// 복사받을 최종 출력 dc
			84, // DC내에서의 x좌표
			28,// DC내에서의 y좌표
			8, // 비트맵의 가로 사이즈
			11, // 비트맵의 세로 사이즈
			// Bmp 의 DC
			hMemDC, // 출력할  이미지 dc
			iNumHundreds * 8, // 비트맵의 출력 시작 x좌표
			0,	// 비트맵의 출력 시작 Y좌표
			8, // Sheet 의 가로 사이즈
			11, // Sheet 의 세로 사이즈
			RGB(255, 0, 255)); // 제거할 픽셀 색상값
	// SCORE---------------------------

	int iScoreCount = CGameStateManager::Get_Instance()->Get_CurrentScore();
	int iScoreOnes = iScoreCount % 1000 / 100; // 일의 자리 숫자
	int iScoreTens = iScoreCount / 1000; // 십의 자리 숫자


	//score 1의자리
	GdiTransparentBlt(
		// 기존 창 DC
		_dc,	// 복사받을 최종 출력 dc
		100, // DC내에서의 x좌표
		16,// DC내에서의 y좌표
		8, // 비트맵의 가로 사이즈
		11, // 비트맵의 세로 사이즈
		// Bmp 의 DC
		hMemDC, // 출력할  이미지 dc
		0, // 비트맵의 출력 시작 x좌표
		0,	// 비트맵의 출력 시작 Y좌표
		8, // Sheet 의 가로 사이즈
		11, // Sheet 의 세로 사이즈
		RGB(255, 0, 255)); // 제거할 픽셀 색상값

	if (iScoreCount <= 0)
		return;

	// 10의 자리
	GdiTransparentBlt(
		// 기존 창 DC
		_dc,	// 복사받을 최종 출력 dc
		92, // DC내에서의 x좌표
		16,// DC내에서의 y좌표
		8, // 비트맵의 가로 사이즈
		11, // 비트맵의 세로 사이즈
		// Bmp 의 DC
		hMemDC, // 출력할  이미지 dc
		0, // 비트맵의 출력 시작 x좌표
		0,	// 비트맵의 출력 시작 Y좌표
		8, // Sheet 의 가로 사이즈
		11, // Sheet 의 세로 사이즈
		RGB(255, 0, 255)); // 제거할 픽셀 색상값

	// score	100의 자리
	GdiTransparentBlt(
		// 기존 창 DC
		_dc,	// 복사받을 최종 출력 dc
		84, // DC내에서의 x좌표
		16,// DC내에서의 y좌표
		8, // 비트맵의 가로 사이즈
		11, // 비트맵의 세로 사이즈

		// Bmp 의 DC
		hMemDC, // 출력할  이미지 dc
		iScoreOnes * 8, // 비트맵의 출력 시작 x좌표
		0,	// 비트맵의 출력 시작 Y좌표
		8, // Sheet 의 가로 사이즈
		11, // Sheet 의 세로 사이즈
		RGB(255, 0, 255)); // 제거할 픽셀 색상값
	// 십의 자리
	if (iScoreTens > 0)
		GdiTransparentBlt(
			// 기존 창 DC
			_dc,	// 복사받을 최종 출력 dc
			76, // DC내에서의 x좌표
			16,// DC내에서의 y좌표
			8, // 비트맵의 가로 사이즈
			11, // 비트맵의 세로 사이즈
			// Bmp 의 DC
			hMemDC, // 출력할  이미지 dc
			iScoreTens * 8, // 비트맵의 출력 시작 x좌표
			0,	// 비트맵의 출력 시작 Y좌표
			8, // Sheet 의 가로 사이즈
			11, // Sheet 의 세로 사이즈
			RGB(255, 0, 255)); // 제거할 픽셀 색상값
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
			_dc,	// 복사받을 최종 출력 dc
			m_iCurrTopX, // DC내에서의 x좌표
			iDestY,// DC내에서의 y좌표
			211, // 비트맵의 가로 사이즈
			50, // 비트맵의 세로 사이즈

			// Bmp 의 DC
			hMemDC, // 출력할  이미지 dc
			0, // 비트맵의 출력 시작 x좌표
			0,	// 비트맵의 출력 시작 Y좌표
			211, // Sheet 의 가로 사이즈
			50, // Sheet 의 세로 사이즈
			RGB(255, 0, 255)); // 제거할 픽셀 색상값

	if (abs(m_iCurrTopX - iDestX) > 1)
		return;

	// Total Middle
	if (abs(m_iCurrMiddleX - iDestX) > 1)
		m_iCurrMiddleX = CMyMath::Lerp(m_iCurrMiddleX, iDestX, g_fDeltaTime * 1.f);
	else
		m_iCurrMiddleX = iDestX;
	GdiTransparentBlt(
		_dc,	// 복사받을 최종 출력 dc
		m_iCurrMiddleX, // DC내에서의 x좌표
		iDestY + 50,// DC내에서의 y좌표
		211, // 비트맵의 가로 사이즈
		50, // 비트맵의 세로 사이즈

		// Bmp 의 DC
		hMemDC, // 출력할  이미지 dc
		0, // 비트맵의 출력 시작 x좌표
		50,	// 비트맵의 출력 시작 Y좌표
		211, // Sheet 의 가로 사이즈
		50, // Sheet 의 세로 사이즈
		RGB(255, 0, 255)); // 제거할 픽셀 색상값

	if (abs(m_iCurrMiddleX - iDestX) > 1)
		return;

	// Total Down
	if (abs(m_iCurrBottomX - iDestX) > 1)
		m_iCurrBottomX = CMyMath::Lerp(m_iCurrBottomX, iDestX, g_fDeltaTime * 1.3f);
	else
		m_iCurrBottomX = iDestX;

	GdiTransparentBlt(
		_dc,	// 복사받을 최종 출력 dc
		m_iCurrBottomX, // DC내에서의 x좌표
		iDestY + 100,// DC내에서의 y좌표
		211, // 비트맵의 가로 사이즈
		17, // 비트맵의 세로 사이즈

		// Bmp 의 DC
		hMemDC, // 출력할  이미지 dc
		0, // 비트맵의 출력 시작 x좌표
		100,	// 비트맵의 출력 시작 Y좌표
		211, // Sheet 의 가로 사이즈
		17, // Sheet 의 세로 사이즈
		RGB(255, 0, 255)); // 제거할 픽셀 색상값

	if (abs(m_iCurrBottomX - iDestX) > 1)
		return;

	if (!m_bNumberAnimTick)
	{
		m_bNumberAnimTick = true;
		m_dwNumberWaitStartTime = GetTickCount();
	}
	//--------------------------- 점수 보여줄 차례
	hMemDC = CBmpManager::Get_Instance()->Find_Img(L"Number");

	// scoreCount
	int iScoreCount = CGameStateManager::Get_Instance()->Get_CurrentScore();
	int iScoreOnes = iScoreCount % 1000 / 100; // 일의 자리 숫자
	int iScoreTens = iScoreCount / 1000; // 십의 자리 숫자

	//score 1의자리
	GdiTransparentBlt(
		// 기존 창 DC
		_dc,	// 복사받을 최종 출력 dc
		263, // DC내에서의 x좌표
		150,// DC내에서의 y좌표
		8, // 비트맵의 가로 사이즈
		11, // 비트맵의 세로 사이즈
		// Bmp 의 DC
		hMemDC, // 출력할  이미지 dc
		0, // 비트맵의 출력 시작 x좌표
		0,	// 비트맵의 출력 시작 Y좌표
		8, // Sheet 의 가로 사이즈
		11, // Sheet 의 세로 사이즈
		RGB(255, 0, 255)); // 제거할 픽셀 색상값

	if (iScoreCount > 0)
	{
		// 10의 자리
		GdiTransparentBlt(
			// 기존 창 DC
			_dc,	// 복사받을 최종 출력 dc
			255, // DC내에서의 x좌표
			150,// DC내에서의 y좌표
			8, // 비트맵의 가로 사이즈
			11, // 비트맵의 세로 사이즈
			// Bmp 의 DC
			hMemDC, // 출력할  이미지 dc
			0, // 비트맵의 출력 시작 x좌표
			0,	// 비트맵의 출력 시작 Y좌표
			8, // Sheet 의 가로 사이즈
			11, // Sheet 의 세로 사이즈
			RGB(255, 0, 255)); // 제거할 픽셀 색상값

	// score	100의 자리
	GdiTransparentBlt(
		// 기존 창 DC
		_dc,	// 복사받을 최종 출력 dc
		247, // DC내에서의 x좌표
		150,// DC내에서의 y좌표
		8, // 비트맵의 가로 사이즈
		11, // 비트맵의 세로 사이즈

		// Bmp 의 DC
		hMemDC, // 출력할  이미지 dc
		iScoreOnes * 8, // 비트맵의 출력 시작 x좌표
		0,	// 비트맵의 출력 시작 Y좌표
		8, // Sheet 의 가로 사이즈
		11, // Sheet 의 세로 사이즈
		RGB(255, 0, 255)); // 제거할 픽셀 색상값
	}
	// 십의 자리
	if (iScoreTens > 0)
		GdiTransparentBlt(
			// 기존 창 DC
			_dc,	// 복사받을 최종 출력 dc
			239, // DC내에서의 x좌표
			150,// DC내에서의 y좌표
			8, // 비트맵의 가로 사이즈
			11, // 비트맵의 세로 사이즈
			// Bmp 의 DC
			hMemDC, // 출력할  이미지 dc
			iScoreTens * 8, // 비트맵의 출력 시작 x좌표
			0,	// 비트맵의 출력 시작 Y좌표
			8, // Sheet 의 가로 사이즈
			11, // Sheet 의 세로 사이즈
			RGB(255, 0, 255)); // 제거할 픽셀 색상값


	// RingCount
	int iRingCount = CGameStateManager::Get_Instance()->Get_RingCount();
	int iNumOnes = iRingCount % 10; // 일의 자리 숫자
	int iNumTens = iRingCount % 100 / 10; // 십의 자리 숫자
	int iNumHundreds = iRingCount / 100;

	if (m_iCurrOnesNum != iNumOnes &&
		m_dwNumberWaitStartTime + m_dwNumberTimeInterval < GetTickCount())
	{
		m_dwNumberWaitStartTime = GetTickCount();
		++m_iCurrOnesNum;
		CSoundManager::Get_Instance()->StopSound(SOUND_EFFECT);
		CSoundManager::Get_Instance()->PlaySoundW(L"NumberCounting.wav", SOUND_EFFECT, 1.f);
	}

	// 일의 자리
	GdiTransparentBlt(
		// 기존 창 DC
		_dc,	// 복사받을 최종 출력 dc
		263, // DC내에서의 x좌표
		167,// DC내에서의 y좌표
		8, // 비트맵의 가로 사이즈
		11, // 비트맵의 세로 사이즈

		// Bmp 의 DC
		hMemDC, // 출력할  이미지 dc
		m_iCurrOnesNum * 8, // 비트맵의 출력 시작 x좌표
		0,	// 비트맵의 출력 시작 Y좌표
		8, // Sheet 의 가로 사이즈
		11, // Sheet 의 세로 사이즈
		RGB(255, 0, 255)); // 제거할 픽셀 색상값

	// 십의 자리
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
			// 기존 창 DC
			_dc,	// 복사받을 최종 출력 dc
			255, // DC내에서의 x좌표
			167,// DC내에서의 y좌표
			8, // 비트맵의 가로 사이즈
			11, // 비트맵의 세로 사이즈

			// Bmp 의 DC
			hMemDC, // 출력할  이미지 dc
			m_iCurrTensNum * 8, // 비트맵의 출력 시작 x좌표
			0,	// 비트맵의 출력 시작 Y좌표
			8, // Sheet 의 가로 사이즈
			11, // Sheet 의 세로 사이즈
			RGB(255, 0, 255)); // 제거할 픽셀 색상값

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
				// 기존 창 DC
				_dc,	// 복사받을 최종 출력 dc
				247, // DC내에서의 x좌표
				167,// DC내에서의 y좌표
				8, // 비트맵의 가로 사이즈
				11, // 비트맵의 세로 사이즈

				// Bmp 의 DC
				hMemDC, // 출력할  이미지 dc
				m_iCurrHunidsNum * 8, // 비트맵의 출력 시작 x좌표
				0,	// 비트맵의 출력 시작 Y좌표
				8, // Sheet 의 가로 사이즈
				11, // Sheet 의 세로 사이즈
				RGB(255, 0, 255)); // 제거할 픽셀 색상값
		}


		if (m_iCurrHunidsNum == iNumHundreds)
		{
			int iTotalScore = iScoreCount + iRingCount * 100;
			int iTotalHundreds = iTotalScore % 1000 / 100;
			int iTotalThousands = iTotalScore % 10000 / 1000;
			int iTotalTenThousands = iTotalScore % 100000 / 10000;


			// 1의 자리 0
			GdiTransparentBlt(
				// 기존 창 DC
				_dc,	// 복사받을 최종 출력 dc
				263, // DC내에서의 x좌표
				196,// DC내에서의 y좌표
				8, // 비트맵의 가로 사이즈
				11, // 비트맵의 세로 사이즈

				// Bmp 의 DC
				hMemDC, // 출력할  이미지 dc
				0, // 비트맵의 출력 시작 x좌표
				0,	// 비트맵의 출력 시작 Y좌표
				8, // Sheet 의 가로 사이즈
				11, // Sheet 의 세로 사이즈
				RGB(255, 0, 255)); // 제거할 픽셀 색상값
			// 10 의 자리 0
			GdiTransparentBlt(
				// 기존 창 DC
				_dc,	// 복사받을 최종 출력 dc
				255, // DC내에서의 x좌표
				196,// DC내에서의 y좌표
				8, // 비트맵의 가로 사이즈
				11, // 비트맵의 세로 사이즈

				// Bmp 의 DC
				hMemDC, // 출력할  이미지 dc
				0, // 비트맵의 출력 시작 x좌표
				0,	// 비트맵의 출력 시작 Y좌표
				8, // Sheet 의 가로 사이즈
				11, // Sheet 의 세로 사이즈
				RGB(255, 0, 255)); // 제거할 픽셀 색상값

			// 100의 자리
			GdiTransparentBlt(
				// 기존 창 DC
				_dc,	// 복사받을 최종 출력 dc
				247, // DC내에서의 x좌표
				196,// DC내에서의 y좌표
				8, // 비트맵의 가로 사이즈
				11, // 비트맵의 세로 사이즈

				// Bmp 의 DC
				hMemDC, // 출력할  이미지 dc
				iTotalHundreds * 8, // 비트맵의 출력 시작 x좌표
				0,	// 비트맵의 출력 시작 Y좌표
				8, // Sheet 의 가로 사이즈
				11, // Sheet 의 세로 사이즈
				RGB(255, 0, 255)); // 제거할 픽셀 색상값

			if (iTotalScore >= 1000)
				// 100의 자리
				GdiTransparentBlt(
					// 기존 창 DC
					_dc,	// 복사받을 최종 출력 dc
					239, // DC내에서의 x좌표
					196,// DC내에서의 y좌표
					8, // 비트맵의 가로 사이즈
					11, // 비트맵의 세로 사이즈

					// Bmp 의 DC
					hMemDC, // 출력할  이미지 dc
					iTotalThousands * 8, // 비트맵의 출력 시작 x좌표
					0,	// 비트맵의 출력 시작 Y좌표
					8, // Sheet 의 가로 사이즈
					11, // Sheet 의 세로 사이즈
					RGB(255, 0, 255)); // 제거할 픽셀 색상값

			if (iTotalScore >= 10000)
				// 100의 자리
				GdiTransparentBlt(
					// 기존 창 DC
					_dc,	// 복사받을 최종 출력 dc
					231, // DC내에서의 x좌표
					196,// DC내에서의 y좌표
					8, // 비트맵의 가로 사이즈
					11, // 비트맵의 세로 사이즈

					// Bmp 의 DC
					hMemDC, // 출력할  이미지 dc
					iTotalTenThousands * 8, // 비트맵의 출력 시작 x좌표
					0,	// 비트맵의 출력 시작 Y좌표
					8, // Sheet 의 가로 사이즈
					11, // Sheet 의 세로 사이즈
					RGB(255, 0, 255)); // 제거할 픽셀 색상값

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
		_dc,	// 복사받을 최종 출력 dc
		m_iCurrTopX, // DC내에서의 x좌표
		iDestY,// DC내에서의 y좌표
		211, // 비트맵의 가로 사이즈
		50, // 비트맵의 세로 사이즈

		// Bmp 의 DC
		hMemDC, // 출력할  이미지 dc
		0, // 비트맵의 출력 시작 x좌표
		0,	// 비트맵의 출력 시작 Y좌표
		211, // Sheet 의 가로 사이즈
		50, // Sheet 의 세로 사이즈
		RGB(255, 0, 255)); // 제거할 픽셀 색상값

	if (abs(m_iCurrTopX - iDestX) > 1)
		return;

	// Total Middle
	if (abs(m_iCurrMiddleX - iDestX) > 1)
		m_iCurrMiddleX = CMyMath::Lerp(m_iCurrMiddleX, iDestX, g_fDeltaTime * 1.f);
	else
		m_iCurrMiddleX = iDestX;
	GdiTransparentBlt(
		_dc,	// 복사받을 최종 출력 dc
		m_iCurrMiddleX, // DC내에서의 x좌표
		iDestY + 50,// DC내에서의 y좌표
		211, // 비트맵의 가로 사이즈
		50, // 비트맵의 세로 사이즈

		// Bmp 의 DC
		hMemDC, // 출력할  이미지 dc
		0, // 비트맵의 출력 시작 x좌표
		50,	// 비트맵의 출력 시작 Y좌표
		211, // Sheet 의 가로 사이즈
		50, // Sheet 의 세로 사이즈
		RGB(255, 0, 255)); // 제거할 픽셀 색상값

	if (abs(m_iCurrMiddleX - iDestX) > 1)
		return;

	// Total Down
	if (abs(m_iCurrBottomX - iDestX) > 1)
		m_iCurrBottomX = CMyMath::Lerp(m_iCurrBottomX, iDestX, g_fDeltaTime * 1.3f);
	else
		m_iCurrBottomX = iDestX;

	GdiTransparentBlt(
		_dc,	// 복사받을 최종 출력 dc
		m_iCurrBottomX, // DC내에서의 x좌표
		iDestY + 100,// DC내에서의 y좌표
		211, // 비트맵의 가로 사이즈
		17, // 비트맵의 세로 사이즈

		// Bmp 의 DC
		hMemDC, // 출력할  이미지 dc
		0, // 비트맵의 출력 시작 x좌표
		100,	// 비트맵의 출력 시작 Y좌표
		211, // Sheet 의 가로 사이즈
		17, // Sheet 의 세로 사이즈
		RGB(255, 0, 255)); // 제거할 픽셀 색상값

	if (abs(m_iCurrBottomX - iDestX) > 1)
		return;

	if (!m_bNumberAnimTick)
	{
		m_bNumberAnimTick = true;
		m_dwNumberWaitStartTime = GetTickCount();
	}
	//--------------------------- 점수 보여줄 차례
	hMemDC = CBmpManager::Get_Instance()->Find_Img(L"Number");

	// scoreCount
	int iScoreCount = CGameStateManager::Get_Instance()->Get_CurrentScore();
	int iScoreOnes = iScoreCount % 1000 / 100; // 일의 자리 숫자
	int iScoreTens = iScoreCount / 1000; // 십의 자리 숫자

	//score 1의자리
	GdiTransparentBlt(
		// 기존 창 DC
		_dc,	// 복사받을 최종 출력 dc
		263, // DC내에서의 x좌표
		150,// DC내에서의 y좌표
		8, // 비트맵의 가로 사이즈
		11, // 비트맵의 세로 사이즈
		// Bmp 의 DC
		hMemDC, // 출력할  이미지 dc
		0, // 비트맵의 출력 시작 x좌표
		0,	// 비트맵의 출력 시작 Y좌표
		8, // Sheet 의 가로 사이즈
		11, // Sheet 의 세로 사이즈
		RGB(255, 0, 255)); // 제거할 픽셀 색상값

	if (iScoreCount > 0)
	{		
		// 10의 자리
		GdiTransparentBlt(
			// 기존 창 DC
			_dc,	// 복사받을 최종 출력 dc
			255, // DC내에서의 x좌표
			150,// DC내에서의 y좌표
			8, // 비트맵의 가로 사이즈
			11, // 비트맵의 세로 사이즈
			// Bmp 의 DC
			hMemDC, // 출력할  이미지 dc
			0, // 비트맵의 출력 시작 x좌표
			0,	// 비트맵의 출력 시작 Y좌표
			8, // Sheet 의 가로 사이즈
			11, // Sheet 의 세로 사이즈
			RGB(255, 0, 255)); // 제거할 픽셀 색상값
	}

	// score	100의 자리
	GdiTransparentBlt(
		// 기존 창 DC
		_dc,	// 복사받을 최종 출력 dc
		247, // DC내에서의 x좌표
		150,// DC내에서의 y좌표
		8, // 비트맵의 가로 사이즈
		11, // 비트맵의 세로 사이즈

		// Bmp 의 DC
		hMemDC, // 출력할  이미지 dc
		iScoreOnes * 8, // 비트맵의 출력 시작 x좌표
		0,	// 비트맵의 출력 시작 Y좌표
		8, // Sheet 의 가로 사이즈
		11, // Sheet 의 세로 사이즈
		RGB(255, 0, 255)); // 제거할 픽셀 색상값
	// 십의 자리
	if (iScoreTens > 0)
		GdiTransparentBlt(
			// 기존 창 DC
			_dc,	// 복사받을 최종 출력 dc
			239, // DC내에서의 x좌표
			150,// DC내에서의 y좌표
			8, // 비트맵의 가로 사이즈
			11, // 비트맵의 세로 사이즈
			// Bmp 의 DC
			hMemDC, // 출력할  이미지 dc
			iScoreTens * 8, // 비트맵의 출력 시작 x좌표
			0,	// 비트맵의 출력 시작 Y좌표
			8, // Sheet 의 가로 사이즈
			11, // Sheet 의 세로 사이즈
			RGB(255, 0, 255)); // 제거할 픽셀 색상값


	// RingCount
	int iRingCount = CGameStateManager::Get_Instance()->Get_RingCount();
	int iNumOnes = iRingCount % 10; // 일의 자리 숫자
	int iNumTens = iRingCount / 10; // 십의 자리 숫자

	if (m_iCurrOnesNum != iNumOnes &&
		m_dwNumberWaitStartTime + m_dwNumberTimeInterval < GetTickCount())
	{
		m_dwNumberWaitStartTime = GetTickCount();
		++m_iCurrOnesNum;
		CSoundManager::Get_Instance()->StopSound(SOUND_EFFECT);
		CSoundManager::Get_Instance()->PlaySoundW(L"NumberCounting.wav", SOUND_EFFECT, 1.f);
	}

	// 일의 자리
	GdiTransparentBlt(
		// 기존 창 DC
		_dc,	// 복사받을 최종 출력 dc
		263, // DC내에서의 x좌표
		167,// DC내에서의 y좌표
		8, // 비트맵의 가로 사이즈
		11, // 비트맵의 세로 사이즈

		// Bmp 의 DC
		hMemDC, // 출력할  이미지 dc
		m_iCurrOnesNum * 8, // 비트맵의 출력 시작 x좌표
		0,	// 비트맵의 출력 시작 Y좌표
		8, // Sheet 의 가로 사이즈
		11, // Sheet 의 세로 사이즈
		RGB(255, 0, 255)); // 제거할 픽셀 색상값

	// 십의 자리
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
			// 기존 창 DC
			_dc,	// 복사받을 최종 출력 dc
			255, // DC내에서의 x좌표
			167,// DC내에서의 y좌표
			8, // 비트맵의 가로 사이즈
			11, // 비트맵의 세로 사이즈

			// Bmp 의 DC
			hMemDC, // 출력할  이미지 dc
			m_iCurrTensNum * 8, // 비트맵의 출력 시작 x좌표
			0,	// 비트맵의 출력 시작 Y좌표
			8, // Sheet 의 가로 사이즈
			11, // Sheet 의 세로 사이즈
			RGB(255, 0, 255)); // 제거할 픽셀 색상값

		if (m_iCurrTensNum == iNumTens)
		{
			int iTotalScore = iScoreCount + iRingCount * 100;
			int iTotalHundreds = iTotalScore % 1000 / 100;
			int iTotalThousands = iTotalScore % 10000 / 1000;
			int iTotalTenThousands = iTotalScore % 100000 / 10000;
			

			// 1의 자리 0
			GdiTransparentBlt(
				// 기존 창 DC
				_dc,	// 복사받을 최종 출력 dc
				263, // DC내에서의 x좌표
				196,// DC내에서의 y좌표
				8, // 비트맵의 가로 사이즈
				11, // 비트맵의 세로 사이즈

				// Bmp 의 DC
				hMemDC, // 출력할  이미지 dc
				0, // 비트맵의 출력 시작 x좌표
				0,	// 비트맵의 출력 시작 Y좌표
				8, // Sheet 의 가로 사이즈
				11, // Sheet 의 세로 사이즈
				RGB(255, 0, 255)); // 제거할 픽셀 색상값
			// 10 의 자리 0
			GdiTransparentBlt(
				// 기존 창 DC
				_dc,	// 복사받을 최종 출력 dc
				255, // DC내에서의 x좌표
				196,// DC내에서의 y좌표
				8, // 비트맵의 가로 사이즈
				11, // 비트맵의 세로 사이즈

				// Bmp 의 DC
				hMemDC, // 출력할  이미지 dc
				0, // 비트맵의 출력 시작 x좌표
				0,	// 비트맵의 출력 시작 Y좌표
				8, // Sheet 의 가로 사이즈
				11, // Sheet 의 세로 사이즈
				RGB(255, 0, 255)); // 제거할 픽셀 색상값

			// 100의 자리
			GdiTransparentBlt(
				// 기존 창 DC
				_dc,	// 복사받을 최종 출력 dc
				247, // DC내에서의 x좌표
				196,// DC내에서의 y좌표
				8, // 비트맵의 가로 사이즈
				11, // 비트맵의 세로 사이즈

				// Bmp 의 DC
				hMemDC, // 출력할  이미지 dc
				iTotalHundreds * 8, // 비트맵의 출력 시작 x좌표
				0,	// 비트맵의 출력 시작 Y좌표
				8, // Sheet 의 가로 사이즈
				11, // Sheet 의 세로 사이즈
				RGB(255, 0, 255)); // 제거할 픽셀 색상값

			if (iTotalScore >= 1000)
			// 100의 자리
			GdiTransparentBlt(
				// 기존 창 DC
				_dc,	// 복사받을 최종 출력 dc
				239, // DC내에서의 x좌표
				196,// DC내에서의 y좌표
				8, // 비트맵의 가로 사이즈
				11, // 비트맵의 세로 사이즈

				// Bmp 의 DC
				hMemDC, // 출력할  이미지 dc
				iTotalThousands * 8, // 비트맵의 출력 시작 x좌표
				0,	// 비트맵의 출력 시작 Y좌표
				8, // Sheet 의 가로 사이즈
				11, // Sheet 의 세로 사이즈
				RGB(255, 0, 255)); // 제거할 픽셀 색상값

			if (iTotalScore >= 10000)
			// 100의 자리
			GdiTransparentBlt(
				// 기존 창 DC
				_dc,	// 복사받을 최종 출력 dc
				231, // DC내에서의 x좌표
				196,// DC내에서의 y좌표
				8, // 비트맵의 가로 사이즈
				11, // 비트맵의 세로 사이즈

				// Bmp 의 DC
				hMemDC, // 출력할  이미지 dc
				iTotalTenThousands * 8, // 비트맵의 출력 시작 x좌표
				0,	// 비트맵의 출력 시작 Y좌표
				8, // Sheet 의 가로 사이즈
				11, // Sheet 의 세로 사이즈
				RGB(255, 0, 255)); // 제거할 픽셀 색상값

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
		// 기존 창 DC
		_dc,	// 복사받을 최종 출력 dc
		WINCX - 216, // DC내에서의 x좌표
		0,// DC내에서의 y좌표
		216, // 비트맵의 가로 사이즈
		WINCY, // 비트맵의 세로 사이즈

		// Bmp 의 DC
		hMemDC, // 출력할  이미지 dc
		0, // 비트맵의 출력 시작 x좌표
		0,	// 비트맵의 출력 시작 Y좌표
		216, // Sheet 의 가로 사이즈
		WINCY, // Sheet 의 세로 사이즈
		RGB(255, 0, 255)); // 제거할 픽셀 색상

	// 안의 아이템
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
			int i = iIndex / 3; // 세로
			int j = iIndex - i * 3; // 가로 , 열

			GdiTransparentBlt(
				// 기존 창 DC
				_dc,	// 복사받을 최종 출력 dc
				iInventoryLeftTopX + j * iInterval, // DC내에서의 x좌표
				iInventoryLeftTopY + i * iInterval,// DC내에서의 y좌표
				52, // 비트맵의 가로 사이즈
				52, // 비트맵의 세로 사이즈

				// Bmp 의 DC
				hMemDC, // 출력할  이미지 dc
				56, // 비트맵의 출력 시작 x좌표
				32 * iItemID,	// 비트맵의 출력 시작 Y좌표
				28, // Sheet 의 가로 사이즈
				32, // Sheet 의 세로 사이즈
				RGB(255, 0, 255)); // 제거할 픽셀 색상
		}
	}

	if (m_pFocusingObj)
	{
		GdiTransparentBlt(
			// 기존 창 DC
			_dc,	// 복사받을 최종 출력 dc
			ChangePt.x-14, // DC내에서의 x좌표
			ChangePt.y-16,// DC내에서의 y좌표
			52, // 비트맵의 가로 사이즈
			52, // 비트맵의 세로 사이즈

			// Bmp 의 DC
			hMemDC, // 출력할  이미지 dc
			56, // 비트맵의 출력 시작 x좌표
			32 * m_pFocusingObj->Get_ItemID(),	// 비트맵의 출력 시작 Y좌표
			28, // Sheet 의 가로 사이즈
			32, // Sheet 의 세로 사이즈
			RGB(255, 0, 255)); // 제거할 픽셀 색상
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
	int col = (iTmp - 8* dim); //열 7 //0: 0,0  1:

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
	int iUnitHPWidth = 9; // hp 1당 크기
	int iHeight = 9; // 바 높이

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
		_dc,	// 복사받을 최종 출력 dc
		_DCX, // 128 * i
		_DCY,
		_iSize, // 비트맵의 가로 사이즈
		_iSize, // 비트맵의 세로 사이즈

		// Bmp 의 DC
		_hMemDC, // 출력할  이미지 dc
		_iSize * _BitmapIndxX, // 비트맵의 출력 시작 x좌표
		_iSize * _BitmapIndxY,	// 비트맵의 출력 시작 Y좌표
		_iSize, // Sheet 의 가로 사이즈
		_iSize, // Sheet 의 세로 사이즈
		RGB(255, 0, 255)); // 제거할 픽셀 색상값
}
