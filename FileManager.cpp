#include "stdafx.h"
#include "FileManager.h"

CFileManager* CFileManager::m_pInstance = nullptr;

CFileManager::CFileManager()
{
}

CFileManager::~CFileManager()
{
}

void CFileManager::PushDataPath(const TCHAR* _key, const TCHAR* _path)
{
	m_dataMap.emplace(_key, _path);
}

const TCHAR* CFileManager::GetDataPath(const TCHAR* _key)
{
	auto iter = find_if(m_dataMap.begin(), m_dataMap.end(), CTagFinder(_key));

	if (iter == m_dataMap.end())
		return nullptr;

	return iter->second;
}
