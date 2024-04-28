#include "stdafx.h"
#include "Wall.h"

#include "BmpManager.h"
#include "ScrollManager.h"

CWall::CWall() : m_bShakeTriggered(false), m_fAmplitude(10.f), m_fLerpT(0.f), m_fStaticPosY(0.f),
m_iShakeCount(0), m_bIsBossGround(false)
{
	m_eID = ID_WALL;
	m_bIsStatic = true; // 못움직이게
}

CWall::~CWall()
{
}

void CWall::Set_Pos(float _x, float _y)
{
	CObj::Set_Pos(_x, _y);
	m_vStaticPos = m_vPos;
	m_fStaticPosY = m_vPos.fY;
}

void CWall::Initialize()
{
	
}

bool CWall::Update()
{
	

	return OBJ_ALIVE;
}

void CWall::LateUpdate()
{
}

void CWall::Render(HDC _dc)
{
	iScrollX = CScrollManager::Get_Instance()->Get_ScrollX();
	iScrollY = CScrollManager::Get_Instance()->Get_ScrollY();

	if (m_bShakeTriggered)
		ShakeSelf();

	/*Rectangle(_dc,
		static_cast<int>(m_vPos.fX - m_vScale.fX * 0.5f + iScrollX),
		static_cast<int>(m_vPos.fY - m_vScale.fY * 0.5f + iScrollY),
		static_cast<int>(m_vPos.fX + m_vScale.fX * 0.5f + iScrollX),
		static_cast<int>(m_vPos.fY + m_vScale.fY * 0.5f + iScrollY)
	);*/

	if (m_bIsBossGround)
	{
		HDC hMemDC = CBmpManager::Get_Instance()->Find_Img(L"BossStageFloor");

		BitBlt(_dc,
			static_cast<int>(m_vPos.fX - m_vScale.fX * 0.5f + iScrollX),
			static_cast<int>(m_vPos.fY - m_vScale.fY * 0.5f + iScrollY),
			WINCX,
			39,
			hMemDC,
			0,
			0,
			SRCCOPY);
	}
	m_vPos = m_vStaticPos;
}

void CWall::Release()
{
}

void CWall::WallInit(Vector2 _vScale)
{
	m_vScale = _vScale;
}

void CWall::WallInit(float _fCX, float _fCY)
{
	m_vScale = Vector2(_fCX, _fCY);
}

void CWall::OnCollisionStay(CObj* _pOther, MY_DIR _eDir, float _fDiffCX, float _fDiffCY)
{
}

void CWall::ShakeSelf()
{
	switch (m_iShakeCount)
	{
	case 0:
		m_vPos.fY = m_fStaticPosY + m_fAmplitude;
		break;
	case 1:
		m_vPos.fY = m_fStaticPosY - m_fAmplitude;
		break;
	case 2:
		m_vPos.fY = m_fStaticPosY + m_fAmplitude;// *0.5f;
		break;
	case 3:
		m_vPos.fY = m_fStaticPosY - m_fAmplitude;// *0.5f;
		break;
	case 4:
		m_vPos.fY = m_fStaticPosY + m_fAmplitude * 0.5f;
		break;
	case 5:
		m_vPos.fY = m_fStaticPosY - m_fAmplitude * 0.5f;
		break;
	case 6:
		m_vPos.fY = m_fStaticPosY + m_fAmplitude * 0.25f;
		break;
	case 7:
		m_vPos.fY = m_fStaticPosY - m_fAmplitude * 0.25f;
		break;
	case 8:
		m_vPos.fY = m_fStaticPosY + m_fAmplitude * 0.25f;
		break;
	case 9:
		m_vPos.fY = m_fStaticPosY - m_fAmplitude * 0.25f;
		break;

	default:
		m_iShakeCount = 0;
		m_bShakeTriggered = false;
		return;

	}

	++m_iShakeCount;
}
