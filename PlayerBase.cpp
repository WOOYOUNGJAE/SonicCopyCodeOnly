#include "stdafx.h"
#include "PlayerBase.h"
#include  "GameStateManager.h"
#include "BmpManager.h"
#include "KeyManager.h"
#include "DebugManager.h"
#include "DiagonalSpring.h"
#include "LineManager.h"
#include "MechaSonic.h"
#include "ObjManager.h"
#include "Ring.h"
#include "ScrollManager.h"
#include "SoundManager.h"
#include "Spring.h"
#include "UIManager.h"


CPlayerBase::CPlayerBase() :m_bSpinReady(false), m_bIsSpinning(false),
                            m_bPressDown(false), m_fSpinningPower(1000.f),
                            m_fRunMaxSpeed(800), m_fRunMinSpeed(-800),
                            m_fSpinMaxSpeed(1000), m_fSpinMinSpeed(-1000), fTargetLineY(0.f), m_bOnCurve(false),
                            m_pCurrentCurve(nullptr), m_bAccelTriggered(false), m_bOnCircle(false), m_pCurrentCircle(nullptr),
                            m_bRidingCircle(false), m_iOffsetInterval(150), m_eCurrAnimState(PLAYER_ANIM_IDLE), m_ePrevAnimState(PLAYER_ANIM_IDLE),
                            m_fWalkMin(10.f), m_fRunMin(450.f), m_pCurrentTargetLine(nullptr), m_bRidingConnected(false),
                            m_bIsJumping(false), m_fAngle(0),
                            m_fCurrKnockBackDistance(0.f)
{
	m_iAnimAdjustCX = 15;
	m_iAnimAdjustCY = 9;
	m_eID = ID_PLAYER;
	m_fJumpPower = 5.f;
	m_fDefaultSpeed = 10.f;
	m_vScale = Vector2{ float(32),float(43) };
	m_vDir = Vector2(1.f, 0.f);
	//m_vPos = { 100, 100 };


	m_bOnGround = false;

	m_pFrameKey = L"SonicR0";
}

CPlayerBase::~CPlayerBase()
{
}

void CPlayerBase::Initialize()
{
	
#pragma region DebugInit
	CDebugManager::Get_Instance()->AddDebugLog(
		L"DefaultSpeed", CMyString::FloatToString(L"Default Speed : ", 0.f));
	CDebugManager::Get_Instance()->AddDebugLog(
		L"XVelocity", CMyString::FloatToString(L"X Velocity : ", 0.f));
	CDebugManager::Get_Instance()->AddDebugLog(
		L"SpinMode", L"Spin Mode : ");
	CDebugManager::Get_Instance()->AddDebugLog(
		L"PlayerX", L"PlayerX : ");
	CDebugManager::Get_Instance()->AddDebugLog(
		L"PlayerY", L"PlayerY : ");
#pragma endregion


	m_anim.Initialize(m_pFrameKey);
	m_anim.Set_FrameSize(64);
	m_anim.Get_FrameInfo().iFrameStart = 0;
	m_anim.Get_FrameInfo().iFrameEnd = 0;
	m_anim.Get_FrameInfo().iMotion = 0;
	m_anim.Get_FrameInfo().dwInterval = 50.f;
	
}

bool CPlayerBase::Update()
{
	if (m_bIsAttacked)
	{
		UpdateAnimState(PLAYER_ANIM_ATTACKED); 
 		if (CLineManager::Get_Instance()->LineCheck(m_vPos.fX, m_vPos.fY, fTargetLineY))
		{

			m_f_yVelocity += -GRAVITY * g_fDeltaTime * 0.5f;
			m_vPos.fY -= m_f_yVelocity;
			m_vPos.fX += m_f_xVelocity * g_fDeltaTime;



			// 밑에 밟을 직선이 있다면 일단 중력 적용

			// 중력 적용후 y가 밟을 땅보다 아래있다면 누구보다 빠르게 거따가 올림
			if (m_f_yVelocity < 0 && m_vPos.fY + m_vScale.fY * 0.5f > fTargetLineY)
			{
				m_vPos.fY = fTargetLineY;
				m_f_yVelocity = 0.f;
				m_bIsJumping = false;
				m_bOnGround = true;
				m_bIsAttacked = false;
			}
		}


		
		return OBJ_ALIVE;
	}
	m_fCurrKnockBackDistance = 0.f;

	InputKey();

	UpdateMove();

#pragma region Move


	// X좌표가 모두 결정된 상태
	if (m_bOnGround == false) // 공중
	{
		// 밟을 땅이 있는지 확인
		/*if (CLineManager::Get_Instance()->OnLine(m_vPos.fX, m_vPos.fY, fTargetLineY, m_bOnCurve,
			&m_pCurrentCurve, m_bOnCircle, &m_pCurrentCircle))*/

		if (CLineManager::Get_Instance()->LineCheck(m_vPos.fX, m_vPos.fY, fTargetLineY))
		{
			// 확인 도중 밟을 땅이 곡선이었음을 확인
			/*if (m_bOnCurve)
			{
				m_bOnGround = true;
				m_vPos.fY = (m_pCurrentCurve)->Get_CurrentY(m_vPos.fX);
				return;
			}*/

			m_f_yVelocity += -GRAVITY * g_fDeltaTime;
			m_vPos.fY -= m_f_yVelocity;
			// 밑에 밟을 직선이 있다면 일단 중력 적용

			// 중력 적용후 y가 밟을 땅보다 아래있다면 누구보다 빠르게 거따가 올림
			if (m_f_yVelocity < 0 && m_vPos.fY + m_vScale.fY * 0.5f > fTargetLineY)
			{
				m_bIsJumping = false;
				m_vPos.fY = fTargetLineY;
				m_bOnGround = true;
				m_f_yVelocity = 0.f;
				// TODO
				// check crealurve X range
				if (m_pCurrentCurve) // 현재 가리키는 커브가 있으면
				{
					m_bOnCurve = true;
					m_bOnGround = true;
					m_vPos.fY = (m_pCurrentCurve)->Get_CurrentY(m_vPos.fX);
					return OBJ_ALIVE;
				}
			}
		}
		else // 공중인데 밟을 땅이 없으면 중력 적용
		{
			m_f_yVelocity += -GRAVITY * g_fDeltaTime;
			m_vPos.fY -= m_f_yVelocity;
		}

		UpdateMoveX();
	}
	else // OnGround
	{
		m_bIsJumping = false;
		if (CLineManager::Get_Instance()->LineCheck(m_vPos.fX, m_vPos.fY, fTargetLineY))
		{
			if (m_pCurrentTargetLine->Get_LineID() == LINE_ID_CONNECTED)
			{
				LineOnConnected();
			}
			else if (m_pCurrentTargetLine->Get_LineID() == LINE_ID_CIRCLE)
			{
				LineOnCircle();
			}
			else if (m_pCurrentTargetLine->Get_LineID() == LINE_ID_CURVE)
			{
				LineOnCurve();
			}
			else
			{

			}
		}
		else // 땅 없을 때
			m_bOnGround = false;

	}
#pragma endregion Move

	if (m_bIsSpinning)
		CheckSpinFinish();

	return OBJ_ALIVE;
}

void CPlayerBase::LateUpdate()
 {
#pragma region DebugLog
	CDebugManager::Get_Instance()->ChangeDebugLog(L"DefaultSpeed",
		CMyString::FloatToString(L"DefaultSpeed: ", m_fDefaultSpeed));
	CDebugManager::Get_Instance()->ChangeDebugLog(L"XVelocity",
		CMyString::FloatToString(L"XVelocity: ", m_f_xVelocity));
	CDebugManager::Get_Instance()->ChangeDebugLog(L"SpinMode",
		CMyString::FloatToString(L"Spin Mode: ", (float)m_bIsSpinning));
	CDebugManager::Get_Instance()->ChangeDebugLog(L"PlayerX",
		CMyString::FloatToString(L"PlayerX: ", m_vPos.fX));
	CDebugManager::Get_Instance()->ChangeDebugLog(L"PlayerY",
		CMyString::FloatToString(L"PlayerY: ", m_vPos.fY));
#pragma endregion
	

	 LateUpdateOffset();
	 
		
	 //UpdateAnimState(m_eCurrAnimState);
	CheckMotionChanged();
	CObj::LateUpdateAnimFrame();
	CObj::LateUpdateRECT();
}

void CPlayerBase::Render(HDC _dc)
{
	/*if (m_bIsSpinning || m_bSpinReady)
	{
		Ellipse(_dc, m_RECT.left + iScrollX,
			m_RECT.top,
			m_RECT.right + iScrollX,
			m_RECT.bottom);
		return;
	}*/

	CObj::Render(_dc);
	//m_anim.Render(_dc, m_RECT);
	m_anim.AdjustRender(_dc, m_RECT,m_iAnimAdjustCX, m_iAnimAdjustCY);

	

	//iScrollX = CScrollManager::Get_Instance()->Get_ScrollX();

	/*Rectangle(_dc,
		static_cast<int>(m_vPos.fX - m_vScale.fX + iScrollX),
		static_cast<int>(m_vPos.fY - m_vScale.fY),
		static_cast<int>(m_vPos.fX + m_vScale.fX + iScrollX),
		static_cast<int>(m_vPos.fY + m_vScale.fY)
	);

	m_anim.Render(_dc, m_RECT);*/
	//TextOut(_dc, 10, 90, CMyString::FloatToString3(L"xVelocity : ", m_f_xVelocity).c_str(), 18);
}

void CPlayerBase::Release()
{
}

void CPlayerBase::OnCollisionStay(CObj* _pOther, MY_DIR _eDir, float _fDiffCX, float _fDiffCY)
{
	if (_pOther->Get_ID() == ID_WALL)
	{
		switch (_eDir)
		{
		case DIR_RIGHT:
			m_vPos.fX -= _fDiffCX;
			m_i_xVelocity = 0.f;
			if (m_bIsSpinning)
			{
				m_f_xVelocity = 0.f;
				m_bIsSpinning = false;
				CKeyManager::Get_Instance()->ResetAllHorizonAxis();
				UpdateAnimState(PLAYER_ANIM_IDLE);
			}
			break;
		case DIR_LEFT:
			m_vPos.fX += _fDiffCX;
			m_i_xVelocity = 0.f;
			if (m_bIsSpinning)
			{
				m_f_xVelocity = 0.f;
				m_bIsSpinning = false;
				CKeyManager::Get_Instance()->ResetAllHorizonAxis();
				UpdateAnimState(PLAYER_ANIM_IDLE);
			}
			break;
		}
	}
	else if (_pOther->Get_ID() == ID_SPIKE)
	{
		switch (_eDir)
		{
		case DIR_RIGHT:
			m_vPos.fX -= _fDiffCX;
			if (m_bIsSpinning)
			{
				m_f_xVelocity = 0.f;
				m_bIsSpinning = false;
				CKeyManager::Get_Instance()->ResetAllHorizonAxis();
				UpdateAnimState(PLAYER_ANIM_IDLE);
			}
			break;
		case DIR_LEFT:
			m_vPos.fX += _fDiffCX;
			if (m_bIsSpinning)
			{
				m_f_xVelocity = 0.f;
				m_bIsSpinning = false;
				CKeyManager::Get_Instance()->ResetAllHorizonAxis();
				UpdateAnimState(PLAYER_ANIM_IDLE);
			}
			break;

		}
	}
	else if (_pOther->Get_ID() == ID_SPRING)
	{
		switch (dynamic_cast<CSpring*>(_pOther)->Get_SpringDir())
		{
		case DIR_RIGHT:
			{
				switch (_eDir)
				{
				case DIR_UP:
					m_vPos.fY -= _fDiffCY;
					break;
				case DIR_LEFT:
				m_vPos.fX += _fDiffCX;
					break;
				case DIR_DOWN:
					m_vPos.fY += _fDiffCY;
					break;
				}
			}
			break;
		case DIR_LEFT:
			switch (_eDir)
			{
			case DIR_UP:
				m_vPos.fY -= _fDiffCY;
				break;
			case DIR_RIGHT:
				m_vPos.fX -= _fDiffCX;
				break;
			case DIR_DOWN:
				m_vPos.fY += _fDiffCY;
				break;
			}
			break;
		case DIR_UP:
			switch (_eDir)
			{
			case DIR_UP:
				break;
			case DIR_RIGHT:
				m_vPos.fX -= _fDiffCX;
				if (m_bIsSpinning)
				{
					m_f_xVelocity = 0.f;
					m_bIsSpinning = false;
					CKeyManager::Get_Instance()->ResetAllHorizonAxis();
					UpdateAnimState(PLAYER_ANIM_IDLE);
				}
				break;
			case DIR_LEFT:
				m_vPos.fX += _fDiffCX;
				if (m_bIsSpinning)
				{
					m_f_xVelocity = 0.f;
					m_bIsSpinning = false;
					CKeyManager::Get_Instance()->ResetAllHorizonAxis();
					UpdateAnimState(PLAYER_ANIM_IDLE);
				}
				break;
			case DIR_DOWN:
				m_vPos.fY += _fDiffCY;
				break;
			}
			break;
		case DIR_DOWN:
			switch (_eDir)
			{
			case DIR_UP:
				m_vPos.fY -= _fDiffCY;
				break;
			case DIR_RIGHT:
				m_vPos.fX -= _fDiffCX;
				break;
			case DIR_LEFT:
				m_vPos.fX += _fDiffCX;
				break;
			case DIR_DOWN:
				break;
			}
			break;
		}
	}
	else if (_pOther->Get_ID() == ID_DIAGONALSPRING)
	{
		switch (dynamic_cast<CDiagonalSpring*>(_pOther)->Get_SpringDir())
		{
		case DIR_RU:
			if (_eDir == DIR_RIGHT)
			{
				if (m_bIsSpinning)
				{
					m_f_xVelocity = 0.f;
					m_bIsSpinning = false;
					CKeyManager::Get_Instance()->ResetAllHorizonAxis();
					UpdateAnimState(PLAYER_ANIM_IDLE);
				}
				m_vPos.fX -= _fDiffCX;
			}
			else if (_eDir == DIR_UP)
			{
				if (m_bIsSpinning)
				{
					m_f_xVelocity = 0.f;
					m_bIsSpinning = false;
					CKeyManager::Get_Instance()->ResetAllHorizonAxis();
					UpdateAnimState(PLAYER_ANIM_IDLE);
				}
				m_vPos.fY += _fDiffCY;
			}
			break;
		case DIR_LU:
			if (_eDir == DIR_LEFT)
			{
				if (m_bIsSpinning)
				{
					m_f_xVelocity = 0.f;
					m_bIsSpinning = false;
					CKeyManager::Get_Instance()->ResetAllHorizonAxis();
					UpdateAnimState(PLAYER_ANIM_IDLE);
				}
				m_vPos.fX += _fDiffCX;
			}
			else if (_eDir == DIR_UP)
			{
				if (m_bIsSpinning)
				{
					m_f_xVelocity = 0.f;
					m_bIsSpinning = false;
					CKeyManager::Get_Instance()->ResetAllHorizonAxis();
					UpdateAnimState(PLAYER_ANIM_IDLE);
				}
				m_vPos.fY += _fDiffCY;
			}
			break;
		case DIR_RD:
			if (_eDir == DIR_RIGHT)
				m_vPos.fX -= _fDiffCX;
			else if (_eDir == DIR_DOWN)
				m_vPos.fY -= _fDiffCY;
			break;
		case DIR_LD:
			if (_eDir == DIR_LEFT)
				m_vPos.fX += _fDiffCX;
			else if (_eDir == DIR_DOWN)
				m_vPos.fY -= _fDiffCY;
			break;
		}
	}
	else if (_pOther->Get_ID() == ID_ITEM)
	{
	if (_pOther->Get_Anim().Get_FrameInfo().iFrameEnd == 0)
		return;
  	if (m_bIsSpinning || m_bIsJumping)
		return;

		switch (_eDir)
		{
		case DIR_RIGHT:
			m_vPos.fX -= _fDiffCX;
			break;
		case DIR_LEFT:
			m_vPos.fX += _fDiffCX;
			break;
		}
	}
}

void CPlayerBase::OnCollisionEnter(CObj* _pOther, MY_DIR _eDir, float _fDiffCX, float _fDiffCY)
{
	if (_pOther->Get_ID() == ID_RING)
	{
		if (dynamic_cast<CRing*>(_pOther)->Get_IsSpawned() == false)
			return;


		CGameStateManager::Get_Instance()->Set_RingCountPlus();
		//CGameStateManager::Get_Instance()->Set_CurrentScorePlus();
		_pOther->Set_FrameKey(L"Shine");
		_pOther->Get_Anim().ChangeAnimSheet(L"Shine");
		_pOther->Get_Anim().Get_FrameInfo().iFrameStart = 0;
		_pOther->Get_Anim().Get_FrameInfo().iFrameEnd = 3;
		_pOther->Get_Anim().Get_FrameInfo().iMotion = 0;
		_pOther->Get_Anim().Get_FrameInfo().dwInterval = 1000.f;

		//sound
		CSoundManager::Get_Instance()->StopSound(SOUND_EFFECT);
		CSoundManager::Get_Instance()->PlaySoundW(L"SE_Ringb.wav", SOUND_EFFECT, 1.f);
	}
	else if (_pOther->Get_ID() == ID_ITEM)
	{
		if (_pOther->Get_Anim().Get_FrameInfo().iFrameEnd == 0)
			return;

		// 스핀하고 있으면 무조건 깨기
		if (m_bIsSpinning)
		{
			_pOther->Get_Anim().Get_FrameInfo().iFrameStart = 0;
			_pOther->Get_Anim().Get_FrameInfo().iFrameEnd = 0;
			_pOther->Get_Anim().Get_FrameInfo().iMotion = 5;

			CGameStateManager::Get_Instance()->Set_CurrentScorePlus();
			CUIManager::Get_Instance()->m_pInventory->PushItem((CItemObj*)(_pOther));

			//CGameStateManager::Get_Instance()->Set_RingCountPlus(10);

			//sound
			CSoundManager::Get_Instance()->StopSound(SOUND_EFFECT);
			CSoundManager::Get_Instance()->PlaySoundW(L"SE_Itembox.wav", SOUND_EFFECT, 1.f);
		}
		else
		{
			if (m_bIsJumping)
			{
				// knockback
				m_vPos.fY -= _fDiffCY;
				if (m_vPos.fX <= _pOther->Get_Pos().fX)
					m_f_xVelocity = -80.f;
				else
					m_f_xVelocity = 80.f;
				_pOther->Get_Anim().Get_FrameInfo().iFrameStart = 0;
				_pOther->Get_Anim().Get_FrameInfo().iFrameEnd = 0;
				_pOther->Get_Anim().Get_FrameInfo().iMotion = 5;

				//CGameStateManager::Get_Instance()->Set_RingCountPlus(10);
				CUIManager::Get_Instance()->m_pInventory->PushItem((CItemObj*)(_pOther));
				CGameStateManager::Get_Instance()->Set_CurrentScorePlus();
				 
				//sound
				CSoundManager::Get_Instance()->StopSound(SOUND_EFFECT);
				CSoundManager::Get_Instance()->PlaySoundW(L"SE_Itembox.wav", SOUND_EFFECT, 1.f);
			}
		}
	}
	else if (_pOther->Get_ID() == ID_SPIKE)
	{
		if (_eDir == DIR_DOWN && (m_eCurrAnimState != PLAYER_ANIM_ATTACKED))
		{
			m_vPos.fY -= _fDiffCY;
			m_bIsAttacked = true;

			m_bIsJumping = true;
			m_bOnGround = false;
			m_bRidingConnected = false;
			m_f_yVelocity = m_fJumpPower * 0.3f;

			if (m_vPos.fX <= _pOther->Get_Pos().fX)
				m_f_xVelocity = -80.f;
			else
				m_f_xVelocity = 80.f;

			CObjManager::Get_Instance()->DropRings();

			//sound
			CSoundManager::Get_Instance()->StopSound(SOUND_EFFECT);
			CSoundManager::Get_Instance()->PlaySoundW(L"S3K_3E.wav", SOUND_EFFECT, 1.f);
		}
	}
	else if (_pOther->Get_ID() == ID_MECHASONIC)
	{
		if (dynamic_cast<CMechaSonic*>(_pOther)->Get_State() == BOSS_STATE_END)
			return;

		if (!m_bIsJumping) // 점프중이 아니면 이미 맞은 상황, 스핀 공격 제외하고는 무조건 피격
		{
			if (m_bIsSpinning)
			{
				if ((dynamic_cast<CMechaSonic*>(_pOther)->Get_AnimState() == BOSS_ANIM_SPINNING))
					return;

				CGameStateManager::Get_Instance()->Set_CurrentScorePlus();

				m_f_xVelocity = 0.f;
				m_bIsSpinning = false;
				CKeyManager::Get_Instance()->ResetAllHorizonAxis();

				dynamic_cast<CMovableObject*>(_pOther)->Set_HPMinus();

				//KnockBack
				m_vPos.fY -= _fDiffCY;
				if (m_vPos.fX <= _pOther->Get_Pos().fX)
					m_f_xVelocity = -80.f;
				else
					m_f_xVelocity = 80.f;

				// effect
				dynamic_cast<CMovableObject*>(_pOther)->Set_Attacked(true);
				dynamic_cast<CMechaSonic*>(_pOther)->Set_AttackedStartTime(GetTickCount());

				//sound
				CSoundManager::Get_Instance()->StopSound(SOUND_EFFECT);
				CSoundManager::Get_Instance()->PlaySoundW(L"MonsterHit.wav", SOUND_EFFECT, 1.f);
			}
			else if (m_eCurrAnimState != PLAYER_ANIM_ATTACKED)
			{
				--m_iCurrentHP;

				//KnockBack
				m_vPos.fY -= _fDiffCY;
				if (m_vPos.fX <= _pOther->Get_Pos().fX)
					m_f_xVelocity = -80.f;
				else
					m_f_xVelocity = 80.f;

				m_bIsAttacked = true;

				m_bIsJumping = true;
				m_bOnGround = false;
				m_bRidingConnected = false;
				m_f_yVelocity = m_fJumpPower * 0.3f;

				//sound
				CSoundManager::Get_Instance()->StopSound(SOUND_EFFECT);
				CSoundManager::Get_Instance()->PlaySoundW(L"S3K_3E.wav", SOUND_EFFECT, 1.f);

			}
		}
		else // 점프 중에는 
		{
			// 보스가 스핀하고 있지 않고 충돌 방향이 위가 아니면 공격 성공
			if ((dynamic_cast<CMechaSonic*>(_pOther)->Get_AnimState() != BOSS_ANIM_SPINNING) &&
				_eDir != DIR_UP)
			{
				dynamic_cast<CMovableObject*>(_pOther)->Set_HPMinus();

				CGameStateManager::Get_Instance()->Set_CurrentScorePlus();

				//KnockBack
				m_vPos.fY -= _fDiffCY;
				if (m_vPos.fX <= _pOther->Get_Pos().fX)
					m_f_xVelocity = -80.f;
				else
					m_f_xVelocity = 80.f;

				// effect
				dynamic_cast<CMovableObject*>(_pOther)->Set_Attacked(true);
				dynamic_cast<CMechaSonic*>(_pOther)->Set_AttackedStartTime(GetTickCount());

				//sound
				CSoundManager::Get_Instance()->StopSound(SOUND_EFFECT);
				CSoundManager::Get_Instance()->PlaySoundW(L"MonsterHit.wav", SOUND_EFFECT, 1.f);
			}
			// 그외에는 이미 맞은 상황 아니라면 피격
			else if (m_eCurrAnimState != PLAYER_ANIM_ATTACKED)
			{
				--m_iCurrentHP;

				//KnockBack
				m_vPos.fY -= _fDiffCY;
				if (m_vPos.fX <= _pOther->Get_Pos().fX)
					m_f_xVelocity = -80.f;
				else
					m_f_xVelocity = 80.f;

				m_bIsAttacked = true;

				m_bIsJumping = true;
				m_bOnGround = false;
				m_bRidingConnected = false;
				m_f_yVelocity = m_fJumpPower * 0.3f;

				//sound
				CSoundManager::Get_Instance()->StopSound(SOUND_EFFECT);
				CSoundManager::Get_Instance()->PlaySoundW(L"S3K_3E.wav", SOUND_EFFECT, 1.f);
			}

		}
	}
}

void CPlayerBase::OnCollisionExit(CObj* _pOther)
{
	int a = 1;
}

void CPlayerBase::UpdateMove()
{
	ClampVelocity();



	//UpdateMoveX();
	//UpdateMoveY();
}

void CPlayerBase::UpdateMoveX()
{
	if (m_bRidingCircle)
		return;

	m_vPos.fX += m_f_xVelocity * g_fDeltaTime;
}

void CPlayerBase::UpdateMoveY() // 아직 안쓰임
{
	if (m_bRidingCircle)
		return;

	m_vPos.fY -= m_f_xVelocity * g_fDeltaTime;

}

void CPlayerBase::UpdateInAir() // 공중에 있을 때 // 안쓰임
{
	m_vPos.fX +=
		m_vDir.fX * m_f_xVelocity * g_fDeltaTime;
}

void CPlayerBase::CheckMotionChanged()
{
	if (m_eCurrAnimState != m_ePrevAnimState)
	{
 		switch (m_eCurrAnimState)
		{
		case PLAYER_ANIM_IDLE:

			Set_Scale_AdjustXY(41, 43, 10, 10);
			m_anim.Get_FrameInfo().iFrameStart = 0;
			m_anim.Get_FrameInfo().iFrameEnd = 0;
			m_anim.Get_FrameInfo().iMotion = 0;
			m_anim.Get_FrameInfo().dwInterval = 200.f;
			break;
		case PLAYER_ANIM_WALK:
			Set_Scale_AdjustXY(41, 43, 10, 10);
			m_anim.Get_FrameInfo().iFrameStart = 0;
			m_anim.Get_FrameInfo().iFrameEnd = 7;
			m_anim.Get_FrameInfo().iMotion = 1;
			m_anim.Get_FrameInfo().dwInterval = 200.f;
			break;
		case PLAYER_ANIM_RUN:
			Set_Scale_AdjustXY(41, 43, 10, 10);
			m_anim.Get_FrameInfo().iFrameStart = 0;
			m_anim.Get_FrameInfo().iFrameEnd = 3;
			m_anim.Get_FrameInfo().iMotion = 2;
			m_anim.Get_FrameInfo().dwInterval = 200.f;
			break;
		case PLAYER_ANIM_SPIN:
			Set_Scale_AdjustXY(33, 33, 15, 18);
			m_anim.Get_FrameInfo().iFrameStart = 0;
			m_anim.Get_FrameInfo().iFrameEnd = 7;
			m_anim.Get_FrameInfo().iMotion = 3;
			m_anim.Get_FrameInfo().dwInterval = 50.f;
			break;
		case PLAYER_ANIM_STAY_SPIN:
			Set_Scale_AdjustXY(33, 33, 15, 18);
			m_anim.Get_FrameInfo().iFrameStart = 0;
			m_anim.Get_FrameInfo().iFrameEnd = 5;
			m_anim.Get_FrameInfo().iMotion = 4;
			m_anim.Get_FrameInfo().dwInterval = 200.f;
			break;
		case PLAYER_ANIM_LOOK_UP:
			Set_Scale_AdjustXY(32, 43, 13, 10);
			m_anim.Get_FrameInfo().iFrameStart = 0;
			m_anim.Get_FrameInfo().iFrameEnd = 1;
			m_anim.Get_FrameInfo().iMotion = 6;
			m_anim.Get_FrameInfo().dwInterval = 200.f;
			break;
		case PLAYER_ANIM_LOOK_DOWN:
			Set_Scale_AdjustXY(41, 43, 10, 10);
			m_anim.Get_FrameInfo().iFrameStart = 0;
			m_anim.Get_FrameInfo().iFrameEnd = 1;
			m_anim.Get_FrameInfo().iMotion = 7;
			m_anim.Get_FrameInfo().dwInterval = 200.f;
			break;
		case PLAYER_ANIM_SUDDEN_STOP:
			Set_Scale_AdjustXY(41, 43, 10, 10);
			m_anim.Get_FrameInfo().iFrameStart = 0;
			m_anim.Get_FrameInfo().iFrameEnd = 3;
			m_anim.Get_FrameInfo().iMotion = 9;
			m_anim.Get_FrameInfo().dwInterval = 200.f;
			break;
		case PLAYER_ANIM_ATTACKED:
			Set_Scale_AdjustXY(41, 43, 10, 10);
			m_anim.Get_FrameInfo().iFrameStart = 0;
			m_anim.Get_FrameInfo().iFrameEnd = 0;
			m_anim.Get_FrameInfo().iMotion = 10;
			m_anim.Get_FrameInfo().dwInterval = 200.f;
			break;
		case PLAYER_ANIM_FLY:
			Set_Scale_AdjustXY(41, 43, 10, 10);
			m_anim.Get_FrameInfo().iFrameStart = 0;
			m_anim.Get_FrameInfo().iFrameEnd = 0;
			m_anim.Get_FrameInfo().iMotion = 0;
			m_anim.Get_FrameInfo().dwInterval = 10000.f;
			break;

		default:
			break;
		}

		m_anim.Get_FrameInfo().dwTime = GetTickCount();
	}
}

void CPlayerBase::LineOnConnected()
{
	// 땅의 상단에 있지만 아직 발이 안닿았기 때문에 중력 적용
	if (m_bRidingConnected == false)
	{
 		m_f_yVelocity += -GRAVITY * g_fDeltaTime;
		m_vPos.fY -= m_f_yVelocity;

		// 중력 적용후 y가 밟을 땅보다 아래있다면 누구보다 빠르게 거따가 올림
		if (m_vPos.fY + m_vScale.fY * 0.5f > fTargetLineY)
		{
			m_bRidingConnected = true;
			m_vPos.fY = fTargetLineY - m_vScale.fY * 0.5f;
			m_bOnGround = true;
			m_f_yVelocity = 0.f;
		}
		//UpdateMoveX();
	}
	else // 타고 있는 경우
	{
		/*Vector2 vLineDir = 
			dynamic_cast<CConnectedLine*>(m_pCurrentTargetLine)->ReturnTangentVector(m_vPos.fX);

		m_vDir = vLineDir;*/
		m_vPos.fX += m_f_xVelocity * g_fDeltaTime;
		m_vPos.fX = int(m_vPos.fX);


		/*if (m_pCurrentTargetLine->Get_LeftX() < m_vPos.fX &&
			m_vPos.fX < m_pCurrentTargetLine->Get_RightX())*/
		if (dynamic_cast<CConnectedLine*>(m_pCurrentTargetLine)->Get_LeftX() < m_vPos.fX &&
			m_vPos.fX < dynamic_cast<CConnectedLine*>(m_pCurrentTargetLine)->Get_RightX())
			m_vPos.fY = dynamic_cast<CConnectedLine*>(m_pCurrentTargetLine)->LineEquation(m_vPos.fX) - m_vScale.fY * 0.5f;
		else
		{
			m_bOnGround = false;
			m_bRidingConnected = false;
		}
	}

	if (m_pCurrentTargetLine)
	{
		m_fAngle = m_pCurrentTargetLine->Get_CurrentAngle(m_vPos.fX);
		if (m_bIsRight)
		{
			if (m_fAngle < 23)
				m_anim.ChangeAnimSheet(L"SonicR0");
			else if (m_fAngle < 68)
				m_anim.ChangeAnimSheet(L"SonicR45");
			else if (m_fAngle < 113)
				m_anim.ChangeAnimSheet(L"SonicR90");
			else if (m_fAngle < 158)
				m_anim.ChangeAnimSheet(L"SonicR135");
			else if (m_fAngle < 203)
				m_anim.ChangeAnimSheet(L"SonicR180");
			else if (m_fAngle < 248)
				m_anim.ChangeAnimSheet(L"SonicR225");
			else if (m_fAngle < 293)
				m_anim.ChangeAnimSheet(L"SonicR270");
			else if (m_fAngle < 335)
				m_anim.ChangeAnimSheet(L"SonicR315");
			else
				m_anim.ChangeAnimSheet(L"SonicR0");
		}
		else
		{
			if (m_fAngle < 23)
				m_anim.ChangeAnimSheet(L"SonicL0");
			else if (m_fAngle < 68)
				m_anim.ChangeAnimSheet(L"SonicL45");
			else if (m_fAngle < 113)
				m_anim.ChangeAnimSheet(L"SonicL90");
			else if (m_fAngle < 158)
				m_anim.ChangeAnimSheet(L"SonicL135");
			else if (m_fAngle < 203)
				m_anim.ChangeAnimSheet(L"SonicL180");
			else if (m_fAngle < 248)
				m_anim.ChangeAnimSheet(L"SonicL225");
			else if (m_fAngle < 293)
				m_anim.ChangeAnimSheet(L"SonicL270");
			else if (m_fAngle < 335)
				m_anim.ChangeAnimSheet(L"SonicL315");
			else
				m_anim.ChangeAnimSheet(L"SonicL0");

		}
	}
}

void CPlayerBase::LineOnCurve()
{
#pragma region 커브 타다가 끝났을 경우
	// 곡선 타다가 왼쪽 바깥으로 나가면
	if (m_vPos.fX < m_pCurrentCurve->Get_LeftX())
	{
		m_bOnCurve = false;

		// 속도 느리거나 내려가고 있으면
		if (m_bOnGround && (
			abs(m_f_xVelocity) < 155 || m_vPos.fY >= m_pCurrentCurve->Get_LowesetY()))
		{
			m_vPos = Vector2(m_pCurrentCurve->Get_LeftX(), m_pCurrentCurve->Get_LeftY());
		}
		m_pCurrentCurve = nullptr;
		return;
	}
	// 곡선 타다가 오른쪽 바깥으로 나가면 오른쪽에 고정
	else if (m_vPos.fX > m_pCurrentCurve->Get_RightX())
	{
		m_bOnCurve = false;
		// 속도 느리거나 내려가고 있으면
		if (abs(m_f_xVelocity) < m_fRunMaxSpeed - 50 || m_vPos.fY >= m_pCurrentCurve->Get_LowesetY())
		{
			m_vPos = Vector2(m_pCurrentCurve->Get_RightX(), m_pCurrentCurve->Get_RightY());
		}
		m_pCurrentCurve = nullptr;
		return;
	}
#pragma endregion

	// 선타기
	if (abs(m_f_xVelocity) < m_fRunMaxSpeed - 50) // 일반 선타기
	{
		m_vPos.fY = (m_pCurrentCurve)->Get_CurrentY(m_vPos.fX);
	}
	else if (m_bAccelTriggered == false) //빠르면 y악셀
	{
		m_vPos.fY = (m_pCurrentCurve)->Get_CurrentY(m_vPos.fX, m_f_yVelocity, m_bAccelTriggered);
	}
	else // 빠른데 악셀 했으면
	{
		m_vPos.fY = (m_pCurrentCurve)->Get_CurrentY(m_vPos.fX);
	}
}

void CPlayerBase::LineOnCircle()
{
	if (m_bOnCircle == false)
	{
		m_f_yVelocity += -GRAVITY * g_fDeltaTime;
		m_vPos.fY -= m_f_yVelocity;

		// 중력 적용후 y가 밟을 땅보다 아래있다면 누구보다 빠르게 거따가 올림
		if (m_vPos.fY + m_vScale.fY * 0.5f > fTargetLineY)
		{
			m_bIsJumping = false;
			m_bRidingConnected = true;
			m_vPos.fY = fTargetLineY - m_vScale.fY * 0.5f;
			m_bOnGround = true;
			m_f_yVelocity = 0.f;
		}
	}
	else // onCircle
	{
		if (m_bRidingCircle == false)
		{
			m_vPos.fX += m_f_xVelocity * g_fDeltaTime;
			m_vPos.fX = int(m_vPos.fX);

			//m_vPos.fY = m_pCurrentCircle->Get_CurrentY(m_vPos.fX, m_f_xVelocity, m_bRidingCircle);// -m_vScale.fY * 0.5f;
			m_vPos.fY = m_pCurrentCircle->Get_CurrentY(m_vPos.fX, m_f_xVelocity, m_bRidingCircle) - m_vScale.fY * 0.5f;
		}
		else 
		{
			m_vPos.fY = m_pCurrentCircle->Get_CurrentY(m_vPos.fX, m_f_xVelocity, m_bRidingCircle);

			if (m_fAngle < 23)
			{
				m_anim.ChangeAnimSheet(L"SonicR0");
				m_vPos.fY -= m_vScale.fY * 0.5f;
			}
			else if (m_fAngle < 68)
			{
				m_anim.ChangeAnimSheet(L"SonicR45");
				m_vPos.fX -= m_vScale.fX * 0.25f;
				m_vPos.fY -= m_vScale.fY * 0.25f;				
			}
			else if (m_fAngle < 113)
			{
				m_vPos.fX -= m_vScale.fX * 0.5f;				
				m_anim.ChangeAnimSheet(L"SonicR90");
			}
			else if (m_fAngle < 158)
			{
				m_vPos.fX -= m_vScale.fX * 0.25f;
				m_vPos.fY += m_vScale.fY * 0.25f;
				m_anim.ChangeAnimSheet(L"SonicR135");
			}
			else if (m_fAngle < 203)
			{
				
				m_vPos.fY += m_vScale.fY * 0.5f;
				m_anim.ChangeAnimSheet(L"SonicR180");
			}
			else if (m_fAngle < 248)
			{
				m_vPos.fX += m_vScale.fX * 0.25f;
				m_vPos.fY += m_vScale.fY * 0.25f;
				m_anim.ChangeAnimSheet(L"SonicR225");
			}
			else if (m_fAngle < 293)
			{
				
				m_vPos.fX += m_vScale.fX * 0.5f;
				m_anim.ChangeAnimSheet(L"SonicR270");
			}
			else if (m_fAngle < 335)
			{
				m_vPos.fX += m_vScale.fX * 0.25f;
				m_vPos.fY -= m_vScale.fY * 0.25f;
				m_anim.ChangeAnimSheet(L"SonicR315");
			}
			else
			{
				m_anim.ChangeAnimSheet(L"SonicR0");
				m_vPos.fY -= m_vScale.fY * 0.5f;
			}
		}

	}

	// 곡선 타다가 왼쪽 바깥으로 나가면
	if (m_vPos.fX < m_pCurrentCircle->Get_LeftX())
	{
		m_bOnCircle = false;

		// 속도 느리면
		if (m_bOnGround && (abs(m_f_xVelocity) < 155))
		{
			m_vPos = Vector2(m_pCurrentCircle->Get_LeftX(), m_pCurrentCircle->Get_BottomY() - m_vScale.fY * 0.5f);
		}

		m_pCurrentCircle = nullptr;
		return;
	}
	// 곡선 타다가 오른쪽 바깥으로 나가면 오른쪽에 고정
	else if (m_vPos.fX > m_pCurrentCircle->Get_RightX() + 2.f)
	{
		m_bOnCircle = false;
		// 속도 느리면
		if (m_bOnGround && abs(m_f_xVelocity) < 155)
		{
			//m_vPos = Vector2(m_pCurrentCircle->Get_RightX(), m_pCurrentCircle->get());
		}

		m_pCurrentCircle = nullptr;
		return;
	}

	// 선타기
	//m_vPos.fY = m_pCurrentCircle->Get_CurrentY(m_vPos.fX, m_f_xVelocity, m_bRidingCircle);
	//if (abs(m_f_xVelocity) < 155.f) // 일반 선타기
	//{
	//	m_vPos.fY = (m_pCurrentCircle)->Get_CurrentY(m_vPos.fX,);
	//}
	//else if (m_bAccelTriggered == false) //빠르면 y악셀
	//{
	//	m_vPos.fY = (m_pCurrentCircle)->Get_CurrentY(m_vPos.fX, m_f_yVelocity, m_bAccelTriggered);
	//}
	//else // 빠른데 악셀 했으면
	//{
	//	m_vPos.fY = (m_pCurrentCircle)->Get_CurrentY(m_vPos.fX,);
	//}
}

bool CPlayerBase::CheckUnderFeet(float _fPosX, float _fPosY, float& _refTargetY)
{
	//CLineManager::Get_Instance()->OnLine();
	return true;

}

void CPlayerBase::ClampVelocity()
{
	// Clamp X_Velocity
	if (m_bIsSpinning)
		CMyMath::fClamp(m_f_xVelocity, m_fSpinMinSpeed, m_fSpinMaxSpeed);
	else
		CMyMath::fClamp(m_f_xVelocity, m_fRunMinSpeed, m_fRunMaxSpeed);
}

void CPlayerBase::InputKey()
{
	float fDeltaSpeed = 0.03f;

	if (m_bIsSpinning) // 구르는 상태
	{
		fDeltaSpeed = 0.02f;
		UpdateAnimState(PLAYER_ANIM_SPIN);
		// 땅에서, 제자리 스피닝에서 아래키를 떼면


		// key input
		if (CKeyManager::Get_Instance()->Key_Pressing(VK_RIGHT))
		{
			m_vDir.fX = 1.f;
			m_f_xVelocity += m_fDefaultSpeed *
				CKeyManager::Get_Instance()->Get_Axis_Horizontal(VK_RIGHT, fDeltaSpeed);
		}
		else if (CKeyManager::Get_Instance()->Key_Pressing(VK_LEFT))
		{
			m_vDir.fX = -1.f;
			m_f_xVelocity += m_fDefaultSpeed *
				CKeyManager::Get_Instance()->Get_Axis_Horizontal(VK_LEFT, fDeltaSpeed);
		}
		else if (CKeyManager::Get_Instance()->Key_Down(VK_SPACE) && !m_bIsJumping)
		{
			// 점프
			fTargetLineY = m_vPos.fY;
			m_bIsJumping = true;
			m_bOnGround = false;
			m_bRidingConnected = false;
			m_f_yVelocity = m_fJumpPower;

			//UpdateAnimState(PLAYER_ANIM_SPIN); // jump대신

			//sound
			CSoundManager::Get_Instance()->StopSound(SOUND_PLAYER);
			CSoundManager::Get_Instance()->PlaySoundW(L"jump.wav", SOUND_PLAYER, 1.f);
		}

		if (CKeyManager::Get_Instance()->InputNothing()) // 아무것도 누르지 않은 상태
		{
			//m_f_xVelocity = CMyMath::Lerp(m_f_xVelocity, 0.f, fDeltaSpeed);// *2.f);

			m_f_xVelocity = CMyMath::Lerp(m_f_xVelocity, 0.f, g_fDeltaTime);
		}

	}
	else // non spin
	{
		fDeltaSpeed = g_fDeltaTime * 0.4f;
		// 땅에서 아래키 + 스페이스 눌러서 제자리 스피닝
		if (m_bOnGround)
		{
			if (m_bSpinReady == false)
			{
				if (CKeyManager::Get_Instance()->Key_Pressing(VK_DOWN))
				{
					UpdateAnimState(PLAYER_ANIM_LOOK_DOWN);
					if (CKeyManager::Get_Instance()->Key_Down(VK_SPACE))
					{
						m_bSpinReady = true;
						UpdateAnimState(PLAYER_ANIM_STAY_SPIN);
						CSoundManager::Get_Instance()->StopSound(SOUND_PLAYER);
						CSoundManager::Get_Instance()->PlaySoundW(L"Rolling.wav", SOUND_PLAYER, 1.f);
					}
				}
				else if (CKeyManager::Get_Instance()->Key_Down(VK_SPACE) && !m_bIsJumping )
				{
					// 점프
					fTargetLineY = m_vPos.fY;
					m_bIsJumping = true;
					m_bOnGround = false;
					m_bRidingConnected = false;
					m_f_yVelocity = m_fJumpPower;

					UpdateAnimState(PLAYER_ANIM_SPIN); // jump대신

					//sound
					CSoundManager::Get_Instance()->StopSound(SOUND_PLAYER);
					CSoundManager::Get_Instance()->PlaySoundW(L"jump.wav", SOUND_PLAYER, 1.f);
				}

				// key input
				else if (CKeyManager::Get_Instance()->Key_Pressing(VK_RIGHT))
				{
					m_vDir.fX = 1.f;

					/*if (m_f_xVelocity < -400)
					{
						m_f_xVelocity += m_fDefaultSpeed *
							CKeyManager::Get_Instance()->Get_Axis_Horizontal2(VK_RIGHT, g_fDeltaTime * 0.1f);
					}
					else if (m_f_xVelocity < -200)
					{
						m_f_xVelocity += m_fDefaultSpeed *
							CKeyManager::Get_Instance()->Get_Axis_Horizontal(VK_RIGHT, g_fDeltaTime * 0.1f);
					}
					else if (m_f_xVelocity < 200)
					{
						m_f_xVelocity += m_fDefaultSpeed *
							CKeyManager::Get_Instance()->Get_Axis_Horizontal3(VK_RIGHT, g_fDeltaTime * 0.4f);
					}
					else if (m_f_xVelocity < 400)
					{
						m_f_xVelocity += m_fDefaultSpeed *
							CKeyManager::Get_Instance()->Get_Axis_Horizontal(VK_RIGHT, g_fDeltaTime * 0.1f);
					}
					else
					{
						m_f_xVelocity += m_fDefaultSpeed *
							CKeyManager::Get_Instance()->Get_Axis_Horizontal2(VK_RIGHT, g_fDeltaTime * 0.1f);
					}*/

					if (abs(m_f_xVelocity) < 200)
					{
						m_f_xVelocity += m_fDefaultSpeed *
							CKeyManager::Get_Instance()->Get_Axis_Horizontal3(VK_RIGHT, g_fDeltaTime * 0.4f);
					}					
					else if (abs(m_f_xVelocity) < 400)
					{
						m_f_xVelocity += m_fDefaultSpeed *
							CKeyManager::Get_Instance()->Get_Axis_Horizontal(VK_RIGHT, g_fDeltaTime * 0.1f);
					}
					else
						m_f_xVelocity += m_fDefaultSpeed *
						//CKeyManager::Get_Instance()->Get_Axis_Horizontal(VK_RIGHT, g_fDeltaTime * 0.1f);
						CKeyManager::Get_Instance()->Get_Axis_Horizontal2(VK_RIGHT, g_fDeltaTime * 0.3f);



					//anim
					m_bIsRight = true;
					//m_anim.ChangeAnimSheet(L"SonicR0");

					if (abs(m_f_xVelocity) < m_fWalkMin)
						UpdateAnimState(PLAYER_ANIM_IDLE);
					else if (abs(m_f_xVelocity) < m_fRunMin)
						UpdateAnimState((PLAYER_ANIM_WALK));
					else
						UpdateAnimState(PLAYER_ANIM_RUN);
				}
				else if (CKeyManager::Get_Instance()->Key_Pressing(VK_LEFT))
				{
					m_vDir.fX = -1.f;

					if (abs(m_f_xVelocity) < 200)
					{
						m_f_xVelocity += m_fDefaultSpeed *
							CKeyManager::Get_Instance()->Get_Axis_Horizontal3(VK_LEFT, g_fDeltaTime * 0.4f);
					}
					else if (abs(m_f_xVelocity) < 400)
					{
						m_f_xVelocity += m_fDefaultSpeed *
							CKeyManager::Get_Instance()->Get_Axis_Horizontal(VK_LEFT, g_fDeltaTime * 0.1f);
					}
					else
						m_f_xVelocity += m_fDefaultSpeed *
						CKeyManager::Get_Instance()->Get_Axis_Horizontal2(VK_LEFT, g_fDeltaTime * 0.1f);
					

					m_bIsRight = false;
					m_anim.ChangeAnimSheet(L"SonicL0");
					if (abs(m_f_xVelocity) < m_fWalkMin)
						UpdateAnimState(PLAYER_ANIM_IDLE);
					else if (abs(m_f_xVelocity) < m_fRunMin)
						UpdateAnimState((PLAYER_ANIM_WALK));
					else
						UpdateAnimState(PLAYER_ANIM_RUN);
				}


				if (CKeyManager::Get_Instance()->InputNothing()) // 아무것도 누르지 않은 상태
				{
					//m_f_xVelocity = CMyMath::Lerp(m_f_xVelocity, 0.f, fDeltaSpeed);// *2.f);

					m_f_xVelocity = CMyMath::Lerp(m_f_xVelocity, 0.f, fDeltaSpeed * 40.f);

					
					if (abs(m_f_xVelocity) < m_fWalkMin)
						UpdateAnimState(PLAYER_ANIM_IDLE);
					else if (abs(m_f_xVelocity) < m_fRunMin)
						UpdateAnimState((PLAYER_ANIM_WALK));
					else
						UpdateAnimState(PLAYER_ANIM_RUN);

					//UpdateAnimState(m_eCurrAnimState);
				}
			}
			else // spin ready True
			{
				UpdateAnimState(PLAYER_ANIM_STAY_SPIN);
				if (m_bSpinReady && CKeyManager::Get_Instance()->Key_Up(VK_DOWN))
				{
					m_bSpinReady = false;
					m_bIsSpinning = true; // 비로소 real spinning
					m_f_xVelocity = m_fSpinningPower * m_vDir.fX;
					if (abs(m_f_xVelocity) <= 0.1f)
					{
					}

				}
			}
		}
		else // 공중에서
		{
			// key input
			if (CKeyManager::Get_Instance()->Key_Pressing(VK_RIGHT))
			{
				m_vDir.fX = 1.f;
				m_f_xVelocity += m_fDefaultSpeed *
					CKeyManager::Get_Instance()->Get_Axis_Horizontal(VK_RIGHT, fDeltaSpeed);

				m_bIsRight = true;
				m_anim.ChangeAnimSheet(L"SonicR0");
			}
			else if (CKeyManager::Get_Instance()->Key_Pressing(VK_LEFT))
			{
				m_vDir.fX = -1.f;
				m_f_xVelocity += m_fDefaultSpeed *
					CKeyManager::Get_Instance()->Get_Axis_Horizontal(VK_LEFT, fDeltaSpeed);

				m_bIsRight = true;
				m_anim.ChangeAnimSheet(L"SonicL0");
			}


			if (CKeyManager::Get_Instance()->InputNothing()) // 아무것도 누르지 않은 상태
			{
				//m_f_xVelocity = CMyMath::Lerp(m_f_xVelocity, 0.f, fDeltaSpeed);// *2.f);
				
				//m_f_xVelocity = CMyMath::Lerp(m_f_xVelocity, 0.f, fDeltaSpeed * 50.f);
				m_f_xVelocity = CMyMath::Lerp(m_f_xVelocity, 0.f, fDeltaSpeed);
			}
				if (m_eCurrAnimState != PLAYER_ANIM_FLY)
    				UpdateAnimState(m_eCurrAnimState);
		}
	}
}

void CPlayerBase::CheckSpinFinish()
{
	if (abs(m_f_xVelocity) < 5)
	{
		m_f_xVelocity = 0.f;
		m_bIsSpinning = false;
	}

}

void CPlayerBase::LateUpdateOffset()
{
	int iOffsetMinX = m_iOffsetInterval;
	int iOffsetMaxX = WINCX - m_iOffsetInterval;

	int iOffseval; // 상
	int iOffsetMaxY = WINCY - m_iOffsetInterval; // 하

	if (m_vPos.fX > 7600)
		CScrollManager::Get_Instance()->Set_Fixed(true);

	if (CScrollManager::Get_Instance()->Get_IfFixed())
		return;

	iScrollX = CScrollManager::Get_Instance()->Get_ScrollX();
	iScrollY = CScrollManager::Get_Instance()->Get_ScrollY();

	//if (m_vPos.fX + iScrollX < iOffsetMinX)
	//{
	//	CScrollManager::Get_Instance()->
	//		Set_ScrollXPlus(-1.f * m_f_xVelocity * g_fDeltaTime); // 현 프레임에서 움직인 거리
	//}
	//else if (m_vPos.fX + iScrollX > iOffsetMaxX)
	//{
	//	CScrollManager::Get_Instance()->
	//		Set_ScrollXPlus(-1.f * m_f_xVelocity * g_fDeltaTime); // 현 프레임에서 움직인 거리
	//}
	if ((m_vPos.fX + iScrollX) < m_iOffsetInterval)
	{
		CScrollManager::Get_Instance()->
			Set_ScrollXPlus( (m_iOffsetInterval - (m_vPos.fX + iScrollX))); // 현 프레임에서 움직인 거리
	}
	else if (m_vPos.fX + iScrollX > WINCX - m_iOffsetInterval)
	{
		CScrollManager::Get_Instance()->
			Set_ScrollXPlus((WINCX - m_iOffsetInterval - (m_vPos.fX + iScrollX))); // 현 프레임에서 움직인 거리
	}

	if ((m_vPos.fY + iScrollY) < (m_iOffsetInterval - 30)) // 위 한계 이상으로
	{
		CScrollManager::Get_Instance()->
			Set_ScrollYPlus((m_iOffsetInterval - 30) - (m_vPos.fY + iScrollY)); // 현 프레임에서 움직인 거리
	}
	else if (m_vPos.fY + iScrollY > WINCY - (m_iOffsetInterval - 30))
	{
		CScrollManager::Get_Instance()->
			Set_ScrollYPlus((WINCY - (m_iOffsetInterval - 30) - (m_vPos.fY + iScrollY))); // 현 프레임에서 움직인 거리
	}


}

void CPlayerBase::UpdateAnimState(PLAYER_ANIM_STATE _eAnimState)
{
	m_ePrevAnimState = m_eCurrAnimState;
	m_eCurrAnimState = _eAnimState;
}
