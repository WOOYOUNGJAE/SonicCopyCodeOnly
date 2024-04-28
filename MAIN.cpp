#include "stdafx.h"
#include "MAIN.h"
#include "SceneManager.h"
#include "DebugManager.h"
#include "ScrollManager.h"
#include "AnimationManager.h"
#include "GameStateManager.h"
#include "SoundManager.h"
#include "UIManager.h"
#include "TileManager.h"

CMAIN::CMAIN() : m_dwTime(GetTickCount()), m_iFPS(0)
{
	ZeroMemory(m_szFPS, sizeof(TCHAR) * 32);

}

CMAIN::~CMAIN()
{
	Release();
}

void CMAIN::Initialize()
{
	m_dc = GetDC(g_hWnd);

	m_subDC = CreateCompatibleDC(m_dc);
	HBITMAP hBitmap = CreateCompatibleBitmap(m_dc, WINCX, WINCY);

	DeleteObject(SelectObject(m_subDC, hBitmap));


	// Init Managers
	CSoundManager::Get_Instance()->Initialize();
	CScrollManager::Get_Instance()->Initiailize();
	CAnimationManager::Get_Instance()->Initialize();
	CSceneManager::Get_Instance()->Initialize();
	CDebugManager::Get_Instance()->Initialize();
	CTileManager::Get_Instance()->Initialize();
	CUIManager::Get_Instance()->Initialize();

	CDebugManager::Get_Instance()->AddDebugLog(L"Deltatime", L"");
}

void CMAIN::Update()
{
	CScrollManager::Get_Instance()->Update();
	CSceneManager::Get_Instance()->Update();
	CDebugManager::Get_Instance()->ChangeDebugLog(L"Deltatime", CMyString::FloatToString(L"DeltaTime : ", g_fDeltaTime));
	CUIManager::Get_Instance()->Update();
}

void CMAIN::LateUpdate()
{
	CSceneManager::Get_Instance()->LateUpdate();
}

void CMAIN::Render()
{
	Rectangle(m_subDC, 0, 0, WINCX, WINCY);

	CSceneManager::Get_Instance()->Render(m_subDC);

	++m_iFPS;
	if (m_dwTime + 1000 < GetTickCount())
	{
		swprintf_s(m_szFPS, L"FPS : %d", m_iFPS);

		SetWindowText(g_hWnd, m_szFPS);

		m_iFPS = 0;
		m_dwTime = GetTickCount();
	}
	
	CDebugManager::Get_Instance()->Render(m_subDC);
	BitBlt(m_dc, 0, 0, WINCX, WINCY, m_subDC, 0, 0, SRCCOPY);
}

void CMAIN::Release()
{
	// Release Managers
	CSceneManager::Get_Instance()->Release();
}
