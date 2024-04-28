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
		// ���� â DC
		_dc,	// ������� ���� ��� dc
		(int)m_RECT.left + iScrollX, // DC�������� x��ǥ
		(int)m_RECT.top + iScrollY,// DC�������� y��ǥ
		31, // ��Ʈ���� ���� ������
		31, // ��Ʈ���� ���� ������

		// Bmp �� DC
		hMemDC, // �����  �̹��� dc
		0,
		0,
		31,
		31,
		RGB(255, 0, 255)); // ������ �ȼ� ����
		//RGB(0, 0, 0)); // ������ �ȼ� ����
}

void CSpike::Release()
{
}

void CSpike::OnCollisionStay(CObj* _pOther, MY_DIR _eDir, float _fDiffCX, float _fDiffCY)
{
}
