#include "stdafx.h"
#include "Spike.h"

#include "BmpManager.h"
#include "ScrollManager.h"

CSpike::CSpike()
{
	m_eID = ID_SPIKE;

	m_vPos = {WINCX * 0.5f + 60.f, WINCY * 0.5f - 40.f };
	m_vScale = { 31, 31 };
}

CSpike::~CSpike()
{
}

void CSpike::Initialize()
{
}

bool CSpike::Update()
{
	return OBJ_ALIVE;
}

void CSpike::LateUpdate()
{
	CObj::LateUpdateRECT();
}

void CSpike::Render(HDC _dc)
{
	iScrollX = CScrollManager::Get_Instance()->Get_ScrollX();
	iScrollY = CScrollManager::Get_Instance()->Get_ScrollY();


	/*Rectangle(_dc,
		static_cast<int>(m_vPos.fX - m_vScale.fX * 0.5f + iScrollX),
		static_cast<int>(m_vPos.fY - m_vScale.fY * 0.5f + iScrollY),
		static_cast<int>(m_vPos.fX + m_vScale.fX * 0.5f + iScrollX),
		static_cast<int>(m_vPos.fY + m_vScale.fY * 0.5f + iScrollY)
	);*/

	HDC hMemDC = CBmpManager::Get_Instance()->Find_Img(L"Spike");

	GdiTransparentBlt(
		// 기존 창 DC
		_dc,	// 복사받을 최종 출력 dc
		(int)m_RECT.left + iScrollX, // DC내에서의 x좌표
		(int)m_RECT.top + iScrollY,// DC내에서의 y좌표
		31, // 비트맵의 가로 사이즈
		31, // 비트맵의 세로 사이즈

		// Bmp 의 DC
		hMemDC, // 출력할  이미지 dc
		0,
		0,
		31,
		31,
		RGB(255, 0, 255)); // 제거할 픽셀 색상값
		//RGB(0, 0, 0)); // 제거할 픽셀 색상값
}

void CSpike::Release()
{
}

void CSpike::OnCollisionStay(CObj* _pOther, MY_DIR _eDir, float _fDiffCX, float _fDiffCY)
{
}
