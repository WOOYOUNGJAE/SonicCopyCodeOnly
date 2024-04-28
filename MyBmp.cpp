#include "stdafx.h"
#include "MyBmp.h"

CMyBmp::CMyBmp()
{
}

CMyBmp::~CMyBmp()
{
	Release();
}

void CMyBmp::Load_Bmp(const TCHAR* _path)
{
	HDC hDC = GetDC(g_hWnd);

	//Context DC (장치와 호환) 생성

	m_hMemDC = CreateCompatibleDC(hDC);

	ReleaseDC(g_hWnd, hDC);

	m_hBitMap = (HBITMAP)LoadImage(
		NULL,
		_path,
		IMAGE_BITMAP,
		0,
		0,
		LR_LOADFROMFILE/*파일에서 이미지 로드*/ |
		LR_CREATEDIBSECTION/*로드된 이미지를 DIB로 변환*/
	//Device Independent Bitmap
	);

	// SelectObject : GDI 오브젝트를 선택하기 전에 기존에 가지고 있던 오브젝트를 반환
	m_hOldMap = (HBITMAP)SelectObject(m_hMemDC, m_hBitMap);

}

void CMyBmp::Release()
{
	SelectObject(m_hMemDC, m_hOldMap);
	DeleteObject(m_hBitMap);
	DeleteDC(m_hMemDC);
}
