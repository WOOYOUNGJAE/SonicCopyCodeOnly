#include "stdafx.h"
#include "MyAnimation.h"
#include "BmpManager.h"
#include "ScrollManager.h"

CMyAnimation::CMyAnimation() : m_pBmp(nullptr)
{
	ZeroMemory(&m_tAnimBitmapInfo, sizeof(AnimBitmapInfo));
	ZeroMemory(&m_tFrameInfo, sizeof(FrameInfo));
}

CMyAnimation::~CMyAnimation()
{
}

void CMyAnimation::Initialize()
{
}

void CMyAnimation::Initialize(const TCHAR* _key)
{
	m_tFrameInfo.dwTime = GetTickCount();
	m_pBmp = CBmpManager::Get_Instance()->Get_BMP(_key);
}

void CMyAnimation::Update()
{
}

void CMyAnimation::Render(HDC _dc)
{
	HDC hMemDC = (m_pBmp)->Get_MemDC(); // bmp매니저 안거치고 직접 받아옴
	/*const int iRectWidth = 64;
	const int iRectHeight =64;*/

	GdiTransparentBlt(
		// 기존 창 DC
		_dc,	// 복사받을 최종 출력 dc
		0, // DC내에서의 x좌표
		0,// DC내에서의 y좌표
		WINCX,// 비트맵의 가로 사이즈
		WINCY, // 비트맵의 세로 사이즈

		// Bmp 의 DC
		hMemDC, // 출력할  이미지 dc
		m_tFrameInfo.iFrameStart * m_tAnimBitmapInfo.iFrameCX, // 비트맵의 출력 시작 x좌표
		m_tFrameInfo.iMotion * m_tAnimBitmapInfo.iFrameCY,	// 비트맵의 출력 시작 Y좌표
		m_tAnimBitmapInfo.iFrameCX, // Sheet 의 가로 사이즈
		m_tAnimBitmapInfo.iFrameCY, // Sheet 의 세로 사이즈
		RGB(255, 0, 255)); // 제거할 픽셀 색상값
}

void CMyAnimation::AdjustRender(HDC _dc, const RECT& _rect, int _iAdjustCX, int _iAdjustCY)
{
	const int iScrollX = (int)CScrollManager::Get_Instance()->Get_ScrollX();
	const int iScrollY = (int)CScrollManager::Get_Instance()->Get_ScrollY();


	HDC hMemDC = (m_pBmp)->Get_MemDC(); // bmp매니저 안거치고 직접 받아옴
	//HDC hMemDC = CBmpManager::Get_Instance()->Find_Img(L"SonicR0");; // bmp매니저 안거치고 직접 받아옴

	const int iRectWidth = _rect.right - _rect.left;
	const int iRectHeight = _rect.bottom - _rect.top;
	/*const int iRectWidth = 64;
	const int iRectHeight =64;*/
	GdiTransparentBlt(
		// 기존 창 DC
		_dc,	// 복사받을 최종 출력 dc
		(int)_rect.left + iScrollX, // DC내에서의 x좌표
		(int)_rect.top + iScrollY,// DC내에서의 y좌표
		iRectWidth, // 비트맵의 가로 사이즈
		iRectHeight, // 비트맵의 세로 사이즈

		// Bmp 의 DC
		hMemDC, // 출력할  이미지 dc
		m_tFrameInfo.iFrameStart * m_tAnimBitmapInfo.iFrameCX + _iAdjustCX, // 비트맵의 출력 시작 x좌표
		m_tFrameInfo.iMotion * m_tAnimBitmapInfo.iFrameCY + _iAdjustCY,	// 비트맵의 출력 시작 Y좌표
		//m_tAnimBitmapInfo.iFrameCX - _iAdjustCX * 2, // Sheet 의 가로 사이즈
		//m_tAnimBitmapInfo.iFrameCY - _iAdjustCY * 2, // Sheet 의 세로 사이즈
		iRectWidth, // Sheet 의 가로 사이즈
		iRectHeight,// Sheet 의 세로 사이즈
		RGB(255, 0, 255) ); // 제거할 픽셀 색상값
		//RGB(0, 0, 0)); // 제거할 픽셀 색상값
}

void CMyAnimation::Render(HDC _dc, const RECT& _rect)
{
	const int iScrollX = (int)CScrollManager::Get_Instance()->Get_ScrollX();
	const int iScrollY = (int)CScrollManager::Get_Instance()->Get_ScrollY();


	HDC hMemDC = (m_pBmp)->Get_MemDC(); // bmp매니저 안거치고 직접 받아옴
	//HDC hMemDC = CBmpManager::Get_Instance()->Find_Img(L"SonicR0");; // bmp매니저 안거치고 직접 받아옴

	const int iRectWidth = _rect.right - _rect.left;
	const int iRectHeight = _rect.bottom - _rect.top;
	/*const int iRectWidth = 64;
	const int iRectHeight =64;*/
	GdiTransparentBlt(
		// 기존 창 DC
		_dc,	// 복사받을 최종 출력 dc
		(int)_rect.left + iScrollX, // DC내에서의 x좌표
		(int)_rect.top + iScrollY,// DC내에서의 y좌표
		iRectWidth, // 비트맵의 가로 사이즈
		iRectHeight, // 비트맵의 세로 사이즈

		// Bmp 의 DC
		hMemDC, // 출력할  이미지 dc
		m_tFrameInfo.iFrameStart * m_tAnimBitmapInfo.iFrameCX , // 비트맵의 출력 시작 x좌표
		m_tFrameInfo.iMotion* m_tAnimBitmapInfo.iFrameCY,	// 비트맵의 출력 시작 Y좌표
		m_tAnimBitmapInfo.iFrameCX , // Sheet 의 가로 사이즈
		m_tAnimBitmapInfo.iFrameCY , // Sheet 의 세로 사이즈
		RGB(255, 0, 255) ); // 제거할 픽셀 색상값
		//RGB(0, 0, 0) ); // 제거할 픽셀 색상값

}

void CMyAnimation::Render(HDC _dc, const Vector2& _vPos, const Vector2& _vScale)
{
}

void CMyAnimation::Release()
{
}

void CMyAnimation::ChangeAnimSheet(const TCHAR* _key)
{
	m_pBmp = CBmpManager::Get_Instance()->Get_BMP(_key);
}
