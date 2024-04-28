#include "stdafx.h"
#include "ScrollManager.h"
#include "DebugManager.h"
#include "MyString.h"

CScrollManager* CScrollManager::m_pInstance = nullptr;

CScrollManager::CScrollManager() : m_fScrollX(0.f), m_fScrollY(0.f), m_bScrollFixed(false)
{
}

void CScrollManager::Initiailize()
{
	CDebugManager::Get_Instance()->AddDebugLog(L"ScrollX", L"Scroll X : ");
	CDebugManager::Get_Instance()->AddDebugLog(L"ScrollY", L"Scroll Y : ");
}

void CScrollManager::Update()
{
	CDebugManager::Get_Instance()->ChangeDebugLog(L"ScrollX", 
		CMyString::FloatToString(L"Scroll X : ", m_fScrollX));
	CDebugManager::Get_Instance()->ChangeDebugLog(L"ScrollY", 
		CMyString::FloatToString(L"Scroll Y : ", m_fScrollY));
}
