#include "stdafx.h"
#include "GameStateManager.h"

CGameStateManager* CGameStateManager::m_pInstance = nullptr;

CGameStateManager::CGameStateManager(): m_iRingCount(0), m_iTotalScore(0), m_iCurrentScore(0),
m_iLifeCount(0)
{
}

CGameStateManager::~CGameStateManager()
{
}

void CGameStateManager::Render(HDC _dc)
{
	// if on
	m_inventory.Render(_dc);
}
