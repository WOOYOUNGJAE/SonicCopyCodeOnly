#pragma once
#include "Include.h"
class CFileManager
{
private:
	CFileManager();
	~CFileManager();

public:
	// SingleTon
	static CFileManager* Get_Instance()
	{
		if (m_pInstance == nullptr)
		{
			m_pInstance = new CFileManager;
		}
		return m_pInstance;
	}
	static void Destroy_Instance()
	{
		if (m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}

	void PushDataPath(const TCHAR* _key, const TCHAR* _path);
	const TCHAR* GetDataPath(const TCHAR* _key);

private:
	static CFileManager* m_pInstance;

	map<const TCHAR*, const TCHAR*> m_dataMap;
};

