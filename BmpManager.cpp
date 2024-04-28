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

	// 못 찾았으면
	if (iter == m_mapBit.end())
	{
		return nullptr;
	}

	return iter->second;
}

void CBmpManager::Insert_Bmp(const TCHAR* _key, const TCHAR* _path)
{
	auto iter = find_if(m_mapBit.begin(), m_mapBit.end(), CTagFinder(_key));

	// 못 찾았으면 (새로 만들어야 하는 상황)
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

	// 못 찾았으면
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

	// 이미지 정보를 가져옵니다.
	BITMAP bitmap;
	GetObject(hBitmap, sizeof(BITMAP), &bitmap);

	// 픽셀을 읽어오기 위해 DIB 정보를 가져옵니다.
	BITMAPINFOHEADER bmiHeader;
	ZeroMemory(&bmiHeader, sizeof(BITMAPINFOHEADER));
	bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmiHeader.biWidth = bitmap.bmWidth;
	bmiHeader.biHeight = bitmap.bmHeight;
	bmiHeader.biPlanes = 1;
	bmiHeader.biBitCount = 32;
	bmiHeader.biCompression = BI_RGB;

	// DIB 정보를 기반으로 DIB 섹션을 생성합니다.
	void* pBits;
	HDC hdc = CreateCompatibleDC(NULL);
	HBITMAP hDib = CreateDIBSection(hdc, (BITMAPINFO*)&bmiHeader, DIB_RGB_COLORS, &pBits, NULL, 0);
	SelectObject(hdc, hDib);

	// 이미지를 DIB 섹션에 복사합니다.
	HDC hdcMem = CreateCompatibleDC(hdc);
	SelectObject(hdcMem, hBitmap);
	BitBlt(hdc, 0, 0, bitmap.bmWidth, bitmap.bmHeight, hdcMem, 0, 0, SRCCOPY);

	// 이미지를 변경합니다.
	for (int y = 0; y < bitmap.bmHeight; y++) {
		for (int x = 0; x < bitmap.bmWidth; x++) {
			// 픽셀의 색상을 가져옵니다.
			COLORREF color = ((COLORREF*)pBits)[y * bitmap.bmWidth + x];

			// 색상이 RGB(255, 0, 0)이 아니면 하얀색으로 변경합니다.
			if ((color != RGB(255, 0, 0)) && (color != RGB(255, 0, 255)))
			{
				((COLORREF*)pBits)[y * bitmap.bmWidth + x] = RGB(255, 255, 255);
			}
		}
	}

	// 변경된 이미지를 파일로 저장합니다.
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

	// 메모리를 해제합니다.
	DeleteObject(hBitmap);
	DeleteObject(hDib);
	DeleteDC(hdc);
	DeleteDC(hdcMem);


}
