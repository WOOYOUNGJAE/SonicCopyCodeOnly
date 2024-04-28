#include "stdafx.h"
#include "MovableObject.h"

CMovableObject::CMovableObject() : m_bOnGround(false), m_fDefaultSpeed(0.f),
m_f_xVelocity(0.f) , m_f_yVelocity(0.f),
m_fJumpPower(0.f), m_fAirAccTime(0.f), m_iMaxHP(1), m_iCurrentHP(1), m_bIsAttacked(false)
{
	ZeroMemory(&m_vDir, sizeof(Vector2));
}

CMovableObject::~CMovableObject()
{
}

void CMovableObject::Set_Scale_AdjustXY(float _fX, float _fY, float _adjustX, float _adjustY)
{
	m_vScale = { _fX, _fY };
	m_iAnimAdjustCX = _adjustX;
	m_iAnimAdjustCY = _adjustY;
}

void CMovableObject::Initialize()
{
}

bool CMovableObject::Update()
{
	if (m_bAlive == false)
		return OBJ_DEAD;

	return OBJ_ALIVE;
}

void CMovableObject::LateUpdate()
{
}

void CMovableObject::Render(HDC _dc)
{
}

void CMovableObject::Release()
{
}

void CMovableObject::OnCollisionStay(CObj* _pOther, MY_DIR _eDir, float _fDiffCX, float _fDiffCY)
{
}

void CMovableObject::UpdateMove()
{
}

void CMovableObject::UpdateMoveX()
{
}

void CMovableObject::UpdateMoveY()
{
}

void CMovableObject::UpdateInAir()
{
}

void CMovableObject::CheckAlive()
{
	if (m_iCurrentHP <= 0)
		m_bAlive = false;
}
