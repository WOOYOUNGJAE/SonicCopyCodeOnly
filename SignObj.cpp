#include "stdafx.h"
#include "SignObj.h"

#include "UIManager.h"

CSignObj::CSignObj() : m_fLerpSpeed(0.5f), m_fLerpAcc(0.f), m_iAnimCycle(0)
{ 
	m_eID = ID_OTHER;
	m_pFrameKey = L"Sign";
	m_vScale = { 48, 47 };
	m_vTargetLandingPos = { WINCX * 0.1f, WINCY * 0.7f };
}

CSignObj::~CSignObj()
{
}

void CSignObj::Initialize()
{
	CObj::Initialize();
	m_anim.Initialize(m_pFrameKey);
	m_anim.Set_FrameSize(48, 47);
	m_anim.Get_FrameInfo().iFrameStart = 0;
	m_anim.Get_FrameInfo().iFrameEnd = 8;
	m_anim.Get_FrameInfo().iMotion = 0;
	m_anim.Get_FrameInfo().dwInterval = 50.f;
	
}

bool CSignObj::Update()
{
	if (m_fLerpAcc <= 1.f)
	{
		m_fLerpAcc += m_fLerpSpeed * g_fDeltaTime;
		m_vPos = CMyMath::vecLerp(m_vStartPos, m_vTargetLandingPos, m_fLerpAcc);
	}

	return OBJ_ALIVE;
}

void CSignObj::LateUpdate()
{
	CObj::LateUpdateRECT();
	if (m_iAnimCycle <=5 && CObj::LateUpdateAnimFrame())
	{
		++m_iAnimCycle;
		if (m_iAnimCycle >= 5)
		{
			m_anim.Get_FrameInfo().iFrameStart = m_anim.Get_FrameInfo().iFrameEnd;
			CUIManager::Get_Instance()->Set_UIState(UI_END);
		}
	}
	
}

void CSignObj::Render(HDC _dc)
{
	CObj::Render(_dc);
	m_anim.Render(_dc, m_RECT);
}

void CSignObj::Release()
{
}