#include "stdafx.h"
#include "BmpManager.h"

CBmpManager* CBmpManager::m_pInstance = nullptr;

CBmpManager::CBmpManager()
{
}

CBmpManager::~CBmpManager()
{
	Relesae();
}

CMyBmp* CBmpManager::Get_BMP(const TCHAR* _key)
{
	auto iter = find_if(m_mapBit.begin(), m_mapBit.end(), CTagFinder(_key));

	// �� ã������
	if (iter == m_mapBit.end())
	{
		return nullptr;
	}

	return iter->second;
}

void CBmpManager::Insert_Bmp(const TCHAR* _key, const TCHAR* _path)
{
	auto iter = find_if(m_mapBit.begin(), m_mapBit.end(), CTagFinder(_key));

	// �� ã������ (���� ������ �ϴ� ��Ȳ)
	if (iter == m_mapBit.end())
	{
		CMyBmp* pBmp = new CMyBmp;
		pBmp->Load_Bmp(_path); // Initializing

		m_mapBit.emplace(_key, pBmp);
	}
}

HDC CBmpManager::Find_Img(const TCHAR* _key)
{
	auto iter = find_if(m_mapBit.begin(), m_mapBit.end(), CTagFinder(_key));

	// �� ã������
	if (iter == m_mapBit.end())
	{
		return nullptr;
	}

	return iter->second->Get_MemDC();
}

void CBmpManager::Relesae()
{
}

void CBmpManager::ChangeWhite(const TCHAR* _path, const TCHAR* _newPath)
{
	HBITMAP hBitmap = (HBITMAP)LoadImage(NULL, _path, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	// �̹��� ������ �����ɴϴ�.
	BITMAP bitmap;
	GetObject(hBitmap, sizeof(BITMAP), &bitmap);

	// �ȼ��� �о���� ���� DIB ������ �����ɴϴ�.
	BITMAPINFOHEADER bmiHeader;
	ZeroMemory(&bmiHeader, sizeof(BITMAPINFOHEADER));
	bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmiHeader.biWidth = bitmap.bmWidth;
	bmiHeader.biHeight = bitmap.bmHeight;
	bmiHeader.biPlanes = 1;
	bmiHeader.biBitCount = 32;
	bmiHeader.biCompression = BI_RGB;

	// DIB ������ ������� DIB ������ �����մϴ�.
	void* pBits;
	HDC hdc = CreateCompatibleDC(NULL);
	HBITMAP hDib = CreateDIBSection(hdc, (BITMAPINFO*)&bmiHeader, DIB_RGB_COLORS, &pBits, NULL, 0);
	SelectObject(hdc, hDib);

	// �̹����� DIB ���ǿ� �����մϴ�.
	HDC hdcMem = CreateCompatibleDC(hdc);
	SelectObject(hdcMem, hBitmap);
	BitBlt(hdc, 0, 0, bitmap.bmWidth, bitmap.bmHeight, hdcMem, 0, 0, SRCCOPY);

	// �̹����� �����մϴ�.
	for (int y = 0; y < bitmap.bmHeight; y++) {
		for (int x = 0; x < bitmap.bmWidth; x++) {
			// �ȼ��� ������ �����ɴϴ�.
			COLORREF color = ((COLORREF*)pBits)[y * bitmap.bmWidth + x];

			// ������ RGB(255, 0, 0)�� �ƴϸ� �Ͼ������ �����մϴ�.
			if ((color != RGB(255, 0, 0)) && (color != RGB(255, 0, 255)))
			{
				((COLORREF*)pBits)[y * bitmap.bmWidth + x] = RGB(255, 255, 255);
			}
		}
	}

	// ����� �̹����� ���Ϸ� �����մϴ�.
	BITMAPFILEHEADER bmfHeader;
	ZeroMemory(&bmfHeader, sizeof(BITMAPFILEHEADER));
	bmfHeader.bfType = 0x4d42;
	bmfHeader.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + bitmap.bmWidthBytes * bitmap.bmHeight;
	bmfHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
	DWORD dwBytesWritten;
	HANDLE hFile = CreateFile(_newPath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	WriteFile(hFile, &bmfHeader, sizeof(BITMAPFILEHEADER), &dwBytesWritten, NULL);
	WriteFile(hFile, &bmiHeader, sizeof(BITMAPINFOHEADER), &dwBytesWritten, NULL);
	WriteFile(hFile, pBits, bitmap.bmWidthBytes * bitmap.bmHeight, &dwBytesWritten, NULL);
	CloseHandle(hFile);

	// �޸𸮸� �����մϴ�.
	DeleteObject(hBitmap);
	DeleteObject(hDib);
	DeleteDC(hdc);
	DeleteDC(hdcMem);


}
