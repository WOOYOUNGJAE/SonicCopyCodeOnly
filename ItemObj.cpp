#include "stdafx.h"
#include "ItemObj.h"

#include "GameStateManager.h"
#include "SoundManager.h"

CItemObj::CItemObj() : m_eItemID(ITEM_RING)
{
	m_eID = ID_ITEM;
	m_vScale = { 28, 32 };
	m_pFrameKey = L"Item";
}

CItemObj::~CItemObj()
{
}

void CItemObj::Initialize()
{
	m_anim.Initialize(m_pFrameKey);
	m_anim.Set_FrameSize(28, 32);
	m_anim.Get_FrameInfo().iFrameStart = 0;
	m_anim.Get_FrameInfo().iFrameEnd = 3;
	m_anim.Get_FrameInfo().iMotion = 0;
	m_anim.Get_FrameInfo().dwInterval = 200.f;
}

bool CItemObj::Update()
{
	return OBJ_ALIVE;
}

void CItemObj::LateUpdate()
{
	CObj::LateUpdateAnimFrame();
	CObj::LateUpdateRECT();
}

void CItemObj::Render(HDC _dc)
{
	CObj::Render(_dc);
	m_anim.Render(_dc, m_RECT);
}

void CItemObj::Release()
{
}

void CItemObj::OnCollisionStay(CObj* _pOther, MY_DIR _eDir, float _fDiffCX, float _fDiffCY)
{
}

void CItemObj::OnCollisionEnter(CObj* _pOther, MY_DIR _eDir, float _fDiffCX, float _fDiffCY)
{
}

void CItemObj::ActiveItem()
{
	switch (m_eItemID)
	{
	case ITEM_RING:
		CGameStateManager::Get_Instance()->Set_RingCountPlus(10);
		CSoundManager::Get_Instance()->StopSound(SOUND_EFFECT);
		CSoundManager::Get_Instance()->PlaySoundW(L"SE_Totalring_Decision.wav", SOUND_EFFECT, 1.f);
		break;
	case ITEM_LIFE:
		CGameStateManager::Get_Instance()->Set_LifeCount(true);
		CSoundManager::Get_Instance()->StopSound(SOUND_EFFECT);
		CSoundManager::Get_Instance()->PlaySoundW(L"SE_Point.wav", SOUND_EFFECT, 1.f);
		break;

	default:
		CSoundManager::Get_Instance()->StopSound(SOUND_EFFECT);
		CSoundManager::Get_Instance()->PlaySoundW(L"SE_Point.wav", SOUND_EFFECT, 1.f);
		break;
	}
}
