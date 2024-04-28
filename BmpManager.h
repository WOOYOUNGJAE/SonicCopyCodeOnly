#pragma once
#include "MyBmp.h"

class CBmpManager
{
private:
	CBmpManager();
	~CBmpManager();

public:
	static CBmpManager* Get_Instance(void)
	{
		if (nullptr == m_pInstance)
			m_pInstance = new CBmpManager;

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

	// getter
	CMyBmp* Get_BMP(const TCHAR* _key);

	void Insert_Bmp(const TCHAR* _key, const TCHAR* _path);
	HDC Find_Img(const TCHAR* _key);
	void Relesae();

	void ChangeWhite(const TCHAR* _path, const TCHAR* _newPath);
private:
	static CBmpManager* m_pInstance;

	map<const TCHAR*, CMyBmp*> m_mapBit;
};