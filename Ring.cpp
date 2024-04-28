#include "stdafx.h"
#include "Ring.h"

#include "LineManager.h"

#define RING_GRAVITY 5

CRing::CRing() : m_fXSpeed(3.f), m_fYSpeed(3.f), m_bSpawned(true), m_fAccTime(0.f), m_fRingGravity(0.f),
m_iBounceCount(0)
{
	m_eID = ID_RING;
	m_vScale = Vector2{ 16,16 };
	m_pFrameKey = L"Ring";
	m_fMoveAngle = 1.f;

	//m_bSpawned = false;
}

CRing::~CRing()
{
}

void CRing::Initialize()
{
	m_anim.Initialize(m_pFrameKey);
	m_anim.Set_FrameSize(16);
	m_anim.Get_FrameInfo().iFrameStart = 0;
	m_anim.Get_FrameInfo().iFrameEnd = 3;
	m_anim.Get_FrameInfo().iMotion = 0;
	m_anim.Get_FrameInfo().dwInterval = 200.f;
}

bool CRing::Update()
{
	if (m_bAlive == false)
		return OBJ_DEAD;
	//TODO
	// Èð»Ñ·ÁÁú ¶§ ¾÷µ¥ÀÌÆ®
	if (m_bSpawned == false)
	{
		m_fAccTime += g_fDeltaTime;
		m_vPos.fX += (cosf(m_fMoveAngle) * m_fXSpeed * m_fAccTime);
		m_vPos.fY -= (sinf(m_fMoveAngle) * m_fYSpeed * m_fAccTime - m_fRingGravity * m_fAccTime * m_fAccTime);
		// array<AChild> a;
		// array<AParent> a;
	}


	if (m_pFrameKey == L"Shine" && CObj::AnimOneCycle())
		m_bAlive = false;

	return OBJ_ALIVE;
}

void CRing::LateUpdate()
{
	CObj::LateUpdateAnimFrame();
	CObj::LateUpdateRECT();
}

void CRing::Render(HDC _dc)
{
	CObj::Render(_dc);
	m_anim.Render(_dc, m_RECT);
}

void CRing::Release()
{
}

void CRing::OnCollisionStay(CObj* _pOther, MY_DIR _eDir, float _fDiffCX, float _fDiffCY)
{
	
}
void CRing::OnCollisionEnter(CObj* _pOther, MY_DIR _eDir, float _fDiffCX, float _fDiffCY)
{
	if (m_bSpawned == false && (_pOther->Get_ID() == ID_WALL))
	{		
		if (m_iBounceCount <= 0) // Ã³À½ Æ¨±ä°Å¸é
		{
			switch (_eDir)
			{
			case DIR_RIGHT:
				m_vPos.fX -= _fDiffCX;
				break;
			case DIR_DOWN:
				m_vPos.fY -= _fDiffCY;
				break;
			case DIR_LEFT:
				m_vPos.fX += _fDiffCX;
				break;
			case DIR_UP:
				m_vPos.fY += _fDiffCY;
				break;
			}
			//m_fMoveAngle -= PI * 0.5f;
			m_fAccTime = 0.f;
			m_fRingGravity *= 0.8f;
			++m_iBounceCount;
		}
		else
		 {  
			m_bSpawned = true;
			m_iBounceCount = 0;
		}
	}

	/*if (_pOther->Get_ID() == ID_PLAYER)
	{
		m_pFrameKey = L"Shine";
		m_anim.ChangeAnimSheet(L"Shine");
		m_anim.Get_FrameInfo().iFrameStart = 0;
		m_anim.Get_FrameInfo().iFrameEnd = 3;
		m_anim.Get_FrameInfo().iMotion = 0;
		m_anim.Get_FrameInfo().dwInterval = 1000.f;
	}*/
}


void CRing::TriggerCollected()
{
	
}

void CRing::StartShineEffectAnim()
{
	m_pFrameKey = L"Shine";
}
