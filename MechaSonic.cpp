#include "stdafx.h"
#include "MechaSonic.h"

#include "Factory.h"
#include "KeyManager.h"
#include "ObjManager.h"
#include "SceneManager.h"
#include "SoundManager.h"
#include "UIManager.h"
#include "Wall.h"

CMechaSonic::CMechaSonic() : m_eState(BOSS_BORN), m_ePrevAnimState(BOSS_ANIM_STANDING),
                             m_eCurrAnimState(BOSS_ANIM_STANDING), m_pTarget(nullptr),
                             // Time
                             m_dwTargetStartTime(0), m_dwTargetAccTime(4000 /*5000*/),
                             m_iWallCollisionCount(0), m_iWallCollisionMax(10/*20*/), m_bAnimPause(false), m_bAnimCycled(false),
                             m_bWaitStart(false), m_dwRushDownWaitStartTime(0), m_dwRushDownWaitAccTime(500/*3000*/), m_bRushDownComplete(false),
                             m_fRushDownSpeed(200.f), m_dwAttackedStartTime(0), m_dwAttackedAccTime0(200), m_dwAttackedAccTime1(400),
                             m_dwAttackedAccTime2(600), m_pDownWall(nullptr), m_bClearTick(false),
attack0(true)
{
	m_iAnimAdjustCX = 12;
	m_iAnimAdjustCY = 1;
	m_eID = ID_MECHASONIC;
	m_vScale = Vector2{ float(64 - 21),float(64 - m_iAnimAdjustCY) };
	m_vDir = Vector2(-1.f, 0.f);
	m_f_xVelocity = m_f_yVelocity = 0.f;
	m_fDefaultSpeed = 500.f;
	m_pFrameKey = L"MechaSonicL";

	m_iMaxHP =  20;
	m_iCurrentHP = 8;
}

CMechaSonic::~CMechaSonic()
{
}

void CMechaSonic::Initialize()
{
	m_vBornPos = Vector2(WINCX - m_vScale.fX * 1.5f, WINCY - m_vScale.fY * 1.5f);
	m_anim.Initialize(m_pFrameKey);
	m_anim.Set_FrameSize(64);
	m_anim.Get_FrameInfo().iFrameStart = 0;
	m_anim.Get_FrameInfo().iFrameEnd = 0;
	m_anim.Get_FrameInfo().iMotion = 0;
	m_anim.Get_FrameInfo().dwInterval = 50.f;

	m_pTarget = CObjManager::Get_Instance()->Get_Player();

	CObj* pTestWall = CFactory<CWall>::Create(WINCX * 0.5f, WINCY - 39 * 0.5f);
	CObjManager::Get_Instance()->AddObject(ID_WALL, pTestWall);
	dynamic_cast<CWall*>(pTestWall)->WallInit(WINCX, 39);
	dynamic_cast<CWall*>(pTestWall)->Set_BossGround(true);
	m_pDownWall = pTestWall;
}

bool CMechaSonic::Update()
{
	CheckAlive();

	if (m_bAlive == false)
		return OBJ_DEAD;

	switch (m_eState)
	{
	case BOSS_BORN:
		UpdateBorn();
		break;
	case BOSS_TARGETING:
		UpdateTargeting();
		break;
	case BOSS_ATTACK0:
		UpdateAttack0();
		break;
	case BOSS_ATTACK1:
		UpdateAttack1();
		break;
	default:
		break;
	}

	if (CKeyManager::Get_Instance()->Key_Down(VK_F2))
	{
		Set_HPMinus();
		Set_Attacked(true);
		Set_AttackedStartTime(GetTickCount());
	}

	return OBJ_ALIVE;
}

void CMechaSonic::LateUpdate()
{
	LateUpdateRECT();
	CheckMotionChanged();
	if (IfLookingRight())
		m_anim.ChangeAnimSheet(L"MechaSonicR");
	else
		m_anim.ChangeAnimSheet(L"MechaSonicL");

	if (m_bIsAttacked)
	{
		if (m_dwAttackedStartTime + m_dwAttackedAccTime2 < GetTickCount()) // 빡 역순, 끝
		{
			if (IfLookingRight())
				m_anim.ChangeAnimSheet(L"MechaSonicR");
			else
				m_anim.ChangeAnimSheet(L"MechaSonicL");

			m_bIsAttacked = false;
		}
		else if (m_dwAttackedStartTime + m_dwAttackedAccTime1 < GetTickCount()) // 깜
		{
			if (IfLookingRight())
				m_anim.ChangeAnimSheet(L"MechaSonicWhiteR");
			else
				m_anim.ChangeAnimSheet(L"MechaSonicWhiteL");
		}
		else if (m_dwAttackedStartTime + m_dwAttackedAccTime0 < GetTickCount()) //빡
		{
			if (IfLookingRight())
				m_anim.ChangeAnimSheet(L"MechaSonicR");
			else
				m_anim.ChangeAnimSheet(L"MechaSonicL");
		}
		else
		{
			if (IfLookingRight())
				m_anim.ChangeAnimSheet(L"MechaSonicWhiteR");
			else
				m_anim.ChangeAnimSheet(L"MechaSonicWhiteL");
		}
	}


	m_bAnimCycled = LateUpdateAnimFrame();
	if (m_bAnimCycled)
		int a = 1;
}

void CMechaSonic::Render(HDC _dc)
{
	CObj::Render(_dc);
	//m_anim.Render(_dc, m_RECT);
	m_anim.AdjustRender(_dc, m_RECT, m_iAnimAdjustCX, m_iAnimAdjustCY);

	//HP bar
	CUIManager::Get_Instance()->Render_BossHPBar(_dc);
}

void CMechaSonic::Release()
{
	
}

void CMechaSonic::OnCollisionStay(CObj* _pOther, MY_DIR _eDir, float _fDiffCX, float _fDiffCY)
{
	if (m_eState != BOSS_ATTACK0 && m_eState != BOSS_ATTACK1 && _pOther->Get_ID() == ID_WALL)
	{
		switch (_eDir)
		{
		case DIR_UP:
			m_vPos.fY += _fDiffCY;
			break;
		case DIR_RIGHT:
			m_vPos.fX -= _fDiffCX;
			break;
		case DIR_DOWN:
			m_vPos.fY -= _fDiffCY;
			break;
		case DIR_LEFT:
			m_vPos.fX += _fDiffCX;
			break;
		}
	}
}

void CMechaSonic::OnCollisionEnter(CObj* _pOther, MY_DIR _eDir, float _fDiffCX, float _fDiffCY)
{
	if (m_eState == BOSS_ATTACK0 && _pOther->Get_ID() == ID_WALL)
	{
			switch (_eDir)
			{
			case DIR_UP:
				m_vPos.fY += _fDiffCY;
				m_vDir.fY = -m_vDir.fY;
				break;
			case DIR_RIGHT:
				m_vPos.fX -= _fDiffCX;
				m_vDir.fX = -m_vDir.fX;
				break;
			case DIR_DOWN:
				m_vPos.fY -= _fDiffCY;
				m_vDir.fY = -m_vDir.fY;

				//sound
				CSoundManager::Get_Instance()->StopSound(SOUND_MONSTER);
				CSoundManager::Get_Instance()->PlaySoundW(L"BossCrash.wav", SOUND_MONSTER, 1.f);
				dynamic_cast<CWall*>(m_pDownWall)->Set_ShakeTrigger(true);
				break;
			case DIR_LEFT:
				m_vPos.fX += _fDiffCX;
				m_vDir.fX = -m_vDir.fX;
				break;
			}
			++m_iWallCollisionCount;
	}
	else if (m_eState == BOSS_ATTACK1 && _pOther->Get_ID() == ID_WALL && _eDir == DIR_DOWN)
	{
		m_bRushDownComplete = true;
		//m_vPos.fY = WINCY - m_vScale.fY * 0.5f - _fDiffCY;
		m_vPos.fY = m_pDownWall->Get_Pos().fY - m_pDownWall->Get_Scale().fY * 0.5f -
			m_vScale.fY * 0.5f;

		//sound
		CSoundManager::Get_Instance()->StopSound(SOUND_EFFECT);
		CSoundManager::Get_Instance()->PlaySoundW(L"BossCrash.wav", SOUND_EFFECT, 1.f);
		dynamic_cast<CWall*>(m_pDownWall)->Set_ShakeTrigger(true);
	}


}

void CMechaSonic::OnCollisionExit(CObj* _pOther)
{
}

void CMechaSonic::CheckMotionChanged()
{
	if (m_eCurrAnimState != m_ePrevAnimState)
	{
		switch (m_eCurrAnimState)
		{
		case BOSS_ANIM_STANDING:
			Set_Scale_AdjustXY(48, 63, 8, 1);
			m_anim.Get_FrameInfo().iFrameStart = 0;
			m_anim.Get_FrameInfo().iFrameEnd = 0;
			m_anim.Get_FrameInfo().iMotion = 0;
			m_anim.Get_FrameInfo().dwInterval = 200.f;
			break;
		case BOSS_ANIM_SIT:
			m_anim.Get_FrameInfo().iFrameStart = 0;
			Set_Scale_AdjustXY(48, 63, 8, 1);
			m_anim.Get_FrameInfo().iFrameEnd = 1;
			m_anim.Get_FrameInfo().iMotion = 1;
			m_anim.Get_FrameInfo().dwInterval = 1000.f;
			break;
		case BOSS_ANIM_SPINSTART:
		Set_Scale_AdjustXY(48, 63, 8, 1);
			m_anim.Get_FrameInfo().iFrameStart = 0;
			m_anim.Get_FrameInfo().iFrameEnd = 4;
			m_anim.Get_FrameInfo().iMotion = 2;
			m_anim.Get_FrameInfo().dwInterval = 500.f;
			break;
		case BOSS_ANIM_SPINNING:
			Set_Scale_AdjustXY(48, 48, 8, 15);
			m_anim.Get_FrameInfo().iFrameStart = 0;
			m_anim.Get_FrameInfo().iFrameEnd = 2;
			m_anim.Get_FrameInfo().iMotion = 3;
			m_anim.Get_FrameInfo().dwInterval = 10.f;
			break;
		case BOSS_ANIM_SPINEND:
			Set_Scale_AdjustXY(48, 63, 8, 1);
			m_anim.Get_FrameInfo().iFrameStart = 0;
			m_anim.Get_FrameInfo().iFrameEnd = 4;
			m_anim.Get_FrameInfo().iMotion = 4;
			m_anim.Get_FrameInfo().dwInterval = 200.f;
			break;
		case BOSS_ANIM_REVERSE:
			Set_Scale_AdjustXY(48, 63, 8, 1);
			m_anim.Get_FrameInfo().iFrameStart = 0;
			m_anim.Get_FrameInfo().iFrameEnd = 7;
			m_anim.Get_FrameInfo().iMotion = 5;
			m_anim.Get_FrameInfo().dwInterval = 200.f;
			break;
		case BOSS_ANIM_SLIDING:
			Set_Scale_AdjustXY(48, 63, 8, 1);
			m_anim.Get_FrameInfo().iFrameStart = 0;
			m_anim.Get_FrameInfo().iFrameEnd = 2;
			m_anim.Get_FrameInfo().iMotion = 6;
			m_anim.Get_FrameInfo().dwInterval = 100.f;
			break;
		case BOSS_ANIM_DYING:
			Set_Scale_AdjustXY(48, 63, 8, 1);
			m_anim.Get_FrameInfo().iFrameStart = 0;
			m_anim.Get_FrameInfo().iFrameEnd = 3;
			m_anim.Get_FrameInfo().iMotion = 7;
			m_anim.Get_FrameInfo().dwInterval = 200.f;
			break;

		default:
			break;
		}

		m_anim.Get_FrameInfo().dwTime = GetTickCount();
	}
}

void CMechaSonic::CheckAlive()
{
	if (m_iCurrentHP > 0)
		return;

	m_vPos.fY = m_pDownWall->Get_Pos().fY - m_pDownWall->Get_Scale().fY * 0.5f -
		m_vScale.fY * 0.5f;
	UpdateAnimState(BOSS_ANIM_DYING);

	if (!m_bClearTick)
	{
		m_eState = BOSS_STATE_END;
		CUIManager::Get_Instance()->Set_UIState(UI_BOSS_END);
		CSoundManager::Get_Instance()->StopSound(SOUND_BGM);
		CSoundManager::Get_Instance()->PlaySoundW(L"ClearBgm.wav", SOUND_MONSTER, 1.f);
		//CSoundManager::Get_Instance()->PlayBGM(L"ClearBgm.wav", SOUND_BGM);
		m_bClearTick = true;		
	}

	//CSoundManager::Get_Instance()->PlaySoundW(L"ClearBgm", SOUND_EFFECT , 1.f);
}

void CMechaSonic::UpdateAnimState(BOSS_ANIM_STATE _eAnimState)
{
	m_ePrevAnimState = m_eCurrAnimState;
	m_eCurrAnimState = _eAnimState;
}

void CMechaSonic::UpdateBorn()
{
	m_vPos = CMyMath::vecLerp(m_vPos, m_vBornPos, g_fDeltaTime * 1.f);

	if (CMyMath::GetDistanceSquare(m_vPos, m_vBornPos) < 1.f)
	{
		m_vPos = m_vBornPos;
		m_dwTargetStartTime = GetTickCount();
		UpdateAnimState(BOSS_ANIM_SIT);
		m_eState = BOSS_TARGETING;
	}
}

void CMechaSonic::UpdateTargeting()
{
	if (m_bAnimCycled)
	{
		if (m_anim.Get_FrameInfo().iMotion == (int)BOSS_ANIM_SIT ||
			m_anim.Get_FrameInfo().iMotion == (int)BOSS_ANIM_SLIDING)
		{
			UpdateAnimState(BOSS_ANIM_SPINSTART);
		}
		else if (m_anim.Get_FrameInfo().iMotion == (int)BOSS_ANIM_SPINSTART)
		{
			UpdateAnimState(BOSS_ANIM_SPINNING);
		}
		else if (m_anim.Get_FrameInfo().iMotion == (int)BOSS_ANIM_SPINEND)
		{
			UpdateAnimState(BOSS_ANIM_SIT);
		}
	}
	else 
		UpdateAnimState(m_eCurrAnimState);


	// Target Complete
	if (m_dwTargetStartTime + m_dwTargetAccTime < GetTickCount())
	{
		//int iRandom = (int)GetTickCount() % 2;
		int iRandom = rand() % 2;
		//iRandom = 1;


		switch (attack0)
		{
		case true:
			m_eState = BOSS_ATTACK0;
			UpdateAnimState(BOSS_ANIM_SPINSTART);
			RenewTargetPos();
			return;
		case false:
			m_eState = BOSS_ATTACK1;
			UpdateAnimState(BOSS_ANIM_STANDING);
			RenewTargetPos();
			return;
		default:
			{
			m_eState = BOSS_STATE_END;
			}
		}
		/*switch (iRandom)
		{
		case 0:
			m_eState = BOSS_ATTACK0;
			UpdateAnimState(BOSS_ANIM_SPINSTART);
			RenewTargetPos();
			return;
		case 1:
			m_eState = BOSS_ATTACK1;
			UpdateAnimState(BOSS_ANIM_STANDING);
			RenewTargetPos();
			return;
		default:
			{
			m_eState = BOSS_STATE_END;
			}
		}*/
	}
}

void CMechaSonic::UpdateAttack0()
{
	m_vPos.fX += m_vDir.fX * m_fDefaultSpeed * g_fDeltaTime;
	m_vPos.fY += m_vDir.fY * m_fDefaultSpeed * g_fDeltaTime;

	UpdateAnimState(BOSS_ANIM_SPINNING);
	if (m_iWallCollisionCount > m_iWallCollisionMax)
	{
		m_iWallCollisionCount = 0;
		m_vTargetPos = m_pTarget->Get_Pos();
		m_vDir = m_vTargetPos - m_vPos;
		m_vDir.Normalize();
		UpdateAnimState(BOSS_ANIM_SPINEND);
		Set_Scale_AdjustXY(48, 63, 12, 1);
		m_eState = BOSS_TARGETING;
		//m_eState = BOSS_ATTACK0;
		//m_eState = BOSS_ATTACK1;

		m_dwTargetStartTime = GetTickCount();
		attack0 = !attack0;
	}
}

void CMechaSonic::UpdateAttack1()
{
	UpdateAnimState(m_eCurrAnimState);
	if (!m_bWaitStart) // 플레이어 머리 위로
	{
		// 발 높이가 ?보다 낮다면
		if (abs(m_vPos.fY - (-100)) >= 7.f)
			m_vPos.fY = CMyMath::Lerp(m_vPos.fY, -100, g_fDeltaTime * 10.f);
		else
		{
			m_vPos.fX = m_vTargetPos.fX;
			m_bWaitStart = true;
			m_dwRushDownWaitStartTime = GetTickCount();

			RenewTargetPos();
		}

	}
	else if (m_bWaitStart && !m_bRushDownComplete &&
		m_dwRushDownWaitStartTime + m_dwRushDownWaitAccTime < GetTickCount())
	{
		m_vPos.fY += m_fRushDownSpeed * g_fDeltaTime;
	}
	else if (m_bWaitStart && m_bRushDownComplete) // 땅 찍은 이후
	{
		// TODO
		// Shake Wall

		if (m_bAnimCycled)
		{
			if (m_eCurrAnimState == BOSS_ANIM_STANDING)
				UpdateAnimState(BOSS_ANIM_SLIDING);
			else if (m_eCurrAnimState == BOSS_ANIM_SLIDING) // 최종으로 끝내기
			{
				m_bWaitStart = false;
				m_bRushDownComplete = false;
				m_vTargetPos = m_pTarget->Get_Pos();
				m_vDir = m_vTargetPos - m_vPos;
				m_vDir.Normalize();
				m_eState = BOSS_TARGETING;
				//m_eState = BOSS_ATTACK0;
				m_dwTargetStartTime = GetTickCount();
				attack0 = !attack0;
			}
		}
	}

}

bool CMechaSonic::IfLookingRight()
{
	return m_vDir.fX > 0.f;
}
