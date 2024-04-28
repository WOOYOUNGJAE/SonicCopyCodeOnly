#pragma once
#include "Include.h"
/**
 * \brief Bmp������ Ŭ������ ����
 */
class CMyBmp
{
public:
	CMyBmp();
	~CMyBmp();

public:
	HBITMAP Get_Bitmap() { return m_hBitMap; }

	HDC Get_MemDC() { return m_hMemDC; }
	void Load_Bmp(const TCHAR* _path);
	void Release();


private:
	HDC m_hMemDC;
	HBITMAP m_hBitMap;
	HBITMAP m_hOldMap;
};

