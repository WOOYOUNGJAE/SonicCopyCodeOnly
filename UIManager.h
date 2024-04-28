#pragma once
#include "Include.h"
#include "PlayerInventory.h"

enum UI_STATE
{
	UI_PLAY,
	UI_END,
	UI_BOSS_END,
	UI_STATE_END,
};

class CUIManager
{
private:
	CUIManager();
	~CUIManager();
	
public:
	static CUIManager* Get_Instance(void)
	{
		if (nullptr == m_pInstance)
			m_pInstance = new CUIManager;

		return m_pInstance;
	}
	void	Destroy_Instance(void)
	{
		if (nullptr != m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}

	// setter
	void Set_UIState(UI_STATE _eState) { m_eState = _eState; }
	void Set_Inventory(CPlayerInventory* _pInventory) { m_pInventory = _pInventory; }
	void Set_InventoryOnOff() { m_bInventoryOn = !m_bInventoryOn; }

	void Initialize();
	void Update();
	void LateUpdate();
	void RenderPlay(HDC _dc);
	void RenderEnd(HDC _dc);
	void RenderBossEnd(HDC _dc);
	void RenderInventory(HDC _dc);

	void Render(HDC _dc);
	void Release();

	/**
	 * \brief ¸Ó½¬·ë Å¸ÀÏ¸Ê ½æ³×ÀÏµé º¸¿©ÁÖ´Â
	 */
	void Render_SelectEditMode_Mushroom(HDC _dc);
	void Render_BossHPBar(HDC _dc);
	void TranspBlt(HDC _dc, HDC _hMemDC, int _iSize, int _DCX, int _DCY, int _BitmapIndxX, int _BitmapIndxY);

	int Get_TileSelectPage() { return m_iTileSelectPage; }
	void Set_TileSelectPage(int _iPage) { m_iTileSelectPage = _iPage; }

private:
	static CUIManager* m_pInstance;

	UI_STATE	m_eState;
	RECT m_tRECT_SelectEditMode;
	int m_iTileSelectPage;

	int m_iCurrentBarPoint;

	int m_iCurrTopX;
	int m_iCurrMiddleX;
	int m_iCurrBottomX;
	int m_iCurrOnesNum;
	int m_iCurrTensNum;
	int m_iCurrHunidsNum;
	bool m_bNumberAnimTick;
	bool m_bTotalTick;
	DWORD m_dwNumberWaitStartTime;
	DWORD m_dwNumberTimeInterval;

	bool m_bInventoryOn;
	CItemObj* m_pFocusingObj;
	POINT ChangePt{};
public:
	CPlayerInventory* m_pInventory;
};

