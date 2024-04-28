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
	HDC hMemDC = (m_pBmp)->Get_MemDC(); // bmp�Ŵ��� �Ȱ�ġ�� ���� �޾ƿ�
	/*const int iRectWidth = 64;
	const int iRectHeight =64;*/

	GdiTransparentBlt(
		// ���� â DC
		_dc,	// ������� ���� ��� dc
		0, // DC�������� x��ǥ
		0,// DC�������� y��ǥ
		WINCX,// ��Ʈ���� ���� ������
		WINCY, // ��Ʈ���� ���� ������

		// Bmp �� DC
		hMemDC, // �����  �̹��� dc
		m_tFrameInfo.iFrameStart * m_tAnimBitmapInfo.iFrameCX, // ��Ʈ���� ��� ���� x��ǥ
		m_tFrameInfo.iMotion * m_tAnimBitmapInfo.iFrameCY,	// ��Ʈ���� ��� ���� Y��ǥ
		m_tAnimBitmapInfo.iFrameCX, // Sheet �� ���� ������
		m_tAnimBitmapInfo.iFrameCY, // Sheet �� ���� ������
		RGB(255, 0, 255)); // ������ �ȼ� ����
}

void CMyAnimation::AdjustRender(HDC _dc, const RECT& _rect, int _iAdjustCX, int _iAdjustCY)
{
	const int iScrollX = (int)CScrollManager::Get_Instance()->Get_ScrollX();
	const int iScrollY = (int)CScrollManager::Get_Instance()->Get_ScrollY();


	HDC hMemDC = (m_pBmp)->Get_MemDC(); // bmp�Ŵ��� �Ȱ�ġ�� ���� �޾ƿ�
	//HDC hMemDC = CBmpManager::Get_Instance()->Find_Img(L"SonicR0");; // bmp�Ŵ��� �Ȱ�ġ�� ���� �޾ƿ�

	const int iRectWidth = _rect.right - _rect.left;
	const int iRectHeight = _rect.bottom - _rect.top;
	/*const int iRectWidth = 64;
	const int iRectHeight =64;*/
	GdiTransparentBlt(
		// ���� â DC
		_dc,	// ������� ���� ��� dc
		(int)_rect.left + iScrollX, // DC�������� x��ǥ
		(int)_rect.top + iScrollY,// DC�������� y��ǥ
		iRectWidth, // ��Ʈ���� ���� ������
		iRectHeight, // ��Ʈ���� ���� ������

		// Bmp �� DC
		hMemDC, // �����  �̹��� dc
		m_tFrameInfo.iFrameStart * m_tAnimBitmapInfo.iFrameCX + _iAdjustCX, // ��Ʈ���� ��� ���� x��ǥ
		m_tFrameInfo.iMotion * m_tAnimBitmapInfo.iFrameCY + _iAdjustCY,	// ��Ʈ���� ��� ���� Y��ǥ
		//m_tAnimBitmapInfo.iFrameCX - _iAdjustCX * 2, // Sheet �� ���� ������
		//m_tAnimBitmapInfo.iFrameCY - _iAdjustCY * 2, // Sheet �� ���� ������
		iRectWidth, // Sheet �� ���� ������
		iRectHeight,// Sheet �� ���� ������
		RGB(255, 0, 255) ); // ������ �ȼ� ����
		//RGB(0, 0, 0)); // ������ �ȼ� ����
}

void CMyAnimation::Render(HDC _dc, const RECT& _rect)
{
	const int iScrollX = (int)CScrollManager::Get_Instance()->Get_ScrollX();
	const int iScrollY = (int)CScrollManager::Get_Instance()->Get_ScrollY();


	HDC hMemDC = (m_pBmp)->Get_MemDC(); // bmp�Ŵ��� �Ȱ�ġ�� ���� �޾ƿ�
	//HDC hMemDC = CBmpManager::Get_Instance()->Find_Img(L"SonicR0");; // bmp�Ŵ��� �Ȱ�ġ�� ���� �޾ƿ�

	const int iRectWidth = _rect.right - _rect.left;
	const int iRectHeight = _rect.bottom - _rect.top;
	/*const int iRectWidth = 64;
	const int iRectHeight =64;*/
	GdiTransparentBlt(
		// ���� â DC
		_dc,	// ������� ���� ��� dc
		(int)_rect.left + iScrollX, // DC�������� x��ǥ
		(int)_rect.top + iScrollY,// DC�������� y��ǥ
		iRectWidth, // ��Ʈ���� ���� ������
		iRectHeight, // ��Ʈ���� ���� ������

		// Bmp �� DC
		hMemDC, // �����  �̹��� dc
		m_tFrameInfo.iFrameStart * m_tAnimBitmapInfo.iFrameCX , // ��Ʈ���� ��� ���� x��ǥ
		m_tFrameInfo.iMotion* m_tAnimBitmapInfo.iFrameCY,	// ��Ʈ���� ��� ���� Y��ǥ
		m_tAnimBitmapInfo.iFrameCX , // Sheet �� ���� ������
		m_tAnimBitmapInfo.iFrameCY , // Sheet �� ���� ������
		RGB(255, 0, 255) ); // ������ �ȼ� ����
		//RGB(0, 0, 0) ); // ������ �ȼ� ����

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
