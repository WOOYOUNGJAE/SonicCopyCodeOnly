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

	//Context DC (��ġ�� ȣȯ) ����

	m_hMemDC = CreateCompatibleDC(hDC);

	ReleaseDC(g_hWnd, hDC);

	m_hBitMap = (HBITMAP)LoadImage(
		NULL,
		_path,
		IMAGE_BITMAP,
		0,
		0,
		LR_LOADFROMFILE/*���Ͽ��� �̹��� �ε�*/ |
		LR_CREATEDIBSECTION/*�ε�� �̹����� DIB�� ��ȯ*/
	//Device Independent Bitmap
	);

	// SelectObject : GDI ������Ʈ�� �����ϱ� ���� ������ ������ �ִ� ������Ʈ�� ��ȯ
	m_hOldMap = (HBITMAP)SelectObject(m_hMemDC, m_hBitMap);

}

void CMyBmp::Release()
{
	SelectObject(m_hMemDC, m_hOldMap);
	DeleteObject(m_hBitMap);
	DeleteDC(m_hMemDC);
}
