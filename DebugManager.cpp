#include "stdafx.h"
#include "DebugManager.h"
#include "MyString.h"

CDebugManager* CDebugManager::m_pInstance = nullptr;

CDebugManager::CDebugManager():m_bDebugActive(false), m_iInterval(20)
{
}

CDebugManager::~CDebugManager()
{
}

void CDebugManager::Initialize()
{
	//m_bDebugActive = true;
	m_bDebugActive = false;
}

void CDebugManager::Update()
{
}

void CDebugManager::LateUpdate()
{
}

void CDebugManager::Render(HDC _dc)
{
	if (!m_bDebugActive || m_mapLog.empty())
		return;


	int i = 0;
	for (auto iter = m_mapLog.begin(); iter != m_mapLog.end(); ++iter)
	{
		++i;
		PrintLog(_dc, i, iter);
	}
	
}

void CDebugManager::Release()
{
}

void CDebugManager::AddDebugLog(wstring _key, wstring _strInput)
{
	m_mapLog.emplace(_key, _strInput);
}

void CDebugManager::ChangeDebugLog(wstring _key, wstring _strInput)
{
	m_mapLog[_key] = _strInput;
}

void CDebugManager::DeleteDebugLog(wstring _key)
{
	m_mapLog.erase(_key);
}

void CDebugManager::PrintLog(HDC _dc, int _iOrder, map<wstring, wstring>::iterator _iter)
{
	TextOut(_dc, 10, 5 + m_iInterval * _iOrder, (*_iter).second.c_str(), lstrlen((*_iter).second.c_str()));
}