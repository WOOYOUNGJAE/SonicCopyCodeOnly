#pragma once
#include "PlayerInventory.h"

class CGameStateManager
{
private:
	CGameStateManager();
	~CGameStateManager();


public:
	static CGameStateManager* Get_Instance(void)
	{
		if (nullptr == m_pInstance)
			m_pInstance = new CGameStateManager;

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

	void Initialize() { m_inventory.Initialize(); }

	int Get_RingCount(){ return m_iRingCount; }
	int Get_CurrentScore(){ return m_iCurrentScore; }
	int Get_LifeCount() { return m_iLifeCount; }

	void Set_RingCountPlus() { ++m_iRingCount; }
	void Set_RingCountPlus(int _iAdding) { m_iRingCount += _iAdding; }
	void Set_CurrentScorePlus() { m_iCurrentScore += 100; }
	void Set_LifeCount(bool _bPlus)
	{
		if (_bPlus)
			++m_iLifeCount;
		else
			--m_iLifeCount;

		if (m_iLifeCount < 0)
			m_iLifeCount = 0;
	}
	void ResetRingCount() { m_iRingCount = 0; }

	void Render(HDC _dc);
private:
	static CGameStateManager* m_pInstance;

	int m_iTotalScore;
	int m_iCurrentScore;
	int m_iRingCount;
	int m_iTime;
	int m_iLifeCount;

	CPlayerInventory m_inventory;
};

