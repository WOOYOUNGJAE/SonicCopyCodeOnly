#include "stdafx.h"
#include "Player.h"

#include "BmpManager.h"
#include "KeyManager.h"
#include "DebugManager.h"
#include "LineManager.h"
#include "ScrollManager.h"


CPlayer::CPlayer() :m_bSpinReady(false), m_bIsSpinning(false),
m_bPressDown(false), m_fSpinningPower(500.f),
m_fRunMaxSpeed(500), m_fRunMinSpeed(-500),
m_fSpinMaxSpeed(500), m_fSpinMinSpeed(-500), fTargetLineY(0.f), m_bOnCurve(false),
m_pCurrentCurve(nullptr), m_bAccelTriggered(false), m_bOnCircle(false), m_pCurrentCircle(nullptr),
m_bRidingCircle(false), m_iOffsetInterval(150), m_eCurrAnimState(PLAYER_ANIM_IDLE), m_ePrevAnimState(PLAYER_ANIM_IDLE),
m_fWalkMin(10.f), m_fRunMin(100.f)
{
	m_fJumpPower = 5.f;
	m_fDefaultSpeed = 30.f;
	m_vScale = Vector2{ 40, 60};
	m_vDir = Vector2(1.f, 0.f);
	//m_vPos = { 100, 100 };


	m_bOnGround = true;

	m_pFrameKey = L"SonicR0";
}

CPlayer::~CPlayer()
{
}

void CPlayer::Initialize()
{
	CDebugManager::Get_Instance()->AddDebugLog(
L"DefaultSpeed", CMyString::FloatToString(L"Default Speed : ", 0.f) );
	CDebugManager::Get_Instance()->AddDebugLog(
L"XVelocity", CMyString::FloatToString(L"X Velocity : ", 0.f));
	CDebugManager::Get_Instance()->AddDebugLog(
L"SpinMode", L"Spin Mode : ");

	m_anim.Initialize(m_pFrameKey);
	m_anim.Set_FrameSize(64);
	m_anim.Get_FrameInfo().iFrameStart = 0;
	m_anim.Get_FrameInfo().iFrameEnd = 0;
	m_anim.Get_FrameInfo().iMotion = 0;
	m_anim.Get_FrameInfo().dwInterval = 50.f;
}

bool CPlayer::Update()
{
	InputKey();

	UpdateMove();

	CObj::LateUpdateRECT();
	return OBJ_ALIVE;
}

void CPlayer::LateUpdate()
{
#pragma region DebugLog
	CDebugManager::Get_Instance()->ChangeDebugLog(L"DefaultSpeed",
		CMyString::FloatToString(L"DefaultSpeed: ", m_fDefaultSpeed));
	CDebugManager::Get_Instance()->ChangeDebugLog(L"XVelocity", 
		CMyString::FloatToString(L"XVelocity: ", m_f_xVelocity));
	CDebugManager::Get_Instance()->ChangeDebugLog(L"SpinMode", 
		CMyString::FloatToString(L"Spin Mode: ", (float)m_bIsSpinning));
#pragma endregion
	// X좌표가 모두 결정된 상태
	if (m_bOnGround == false) // 공중
	{
		// 밟을 땅이 있는지 확인
		if (CLineManager::Get_Instance()->OnLine(m_vPos.fX, m_vPos.fY, fTargetLineY, m_bOnCurve,
			&m_pCurrentCurve, m_bOnCircle, &m_pCurrentCircle))
		{
			// 확인 도중 밟을 땅이 곡선이었음을 확인
			/*if (m_bOnCurve)
			{
				m_bOnGround = true;
				m_vPos.fY = (m_pCurrentCurve)->Get_CurrentY(m_vPos.fX);
				return;
			}*/

			// 밑에 밟을 직선이 있다면 일단 중력 적용
			m_f_yVelocity += -GRAVITY * g_fDeltaTime;
			m_vPos.fY -= m_f_yVelocity;

			// 중력 적용후 y가 밟을 땅보다 아래있다면 누구보다 빠르게 거따가 올림
			if (m_vPos.fY > fTargetLineY)
			{
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
					return;
				}
			}
		}
		else // 공중인데 밟을 땅이 없으면 중력 적용
		{
			m_f_yVelocity += -GRAVITY * g_fDeltaTime;
			m_vPos.fY -= m_f_yVelocity;
		}
	}
	else // OnGround
	{
		if (m_bOnCurve) // 곡선 타고 있다면
		{
#pragma region 커브 타다가 끝났을 경우
			// 곡선 타다가 왼쪽 바깥으로 나가면
			if (m_vPos.fX < m_pCurrentCurve->Get_LeftX())
			{
				m_bOnCurve = false;

				// 속도 느리거나 내려가고 있으면
				if ( m_bOnGround && (
					abs(m_f_xVelocity) < 155 || m_vPos.fY >= m_pCurrentCurve->Get_LowesetY()))
				{
					m_vPos = Vector2(m_pCurrentCurve->Get_LeftX(), m_pCurrentCurve->Get_LeftY());
				}
				//if (m_pCurrentCurve->IfOnY_AccelRange(m_vPos.fX)) //커브의 상단끝쪽에 있다면
				//{
				//	// 속도 느리면 보정
				//	if (abs(m_f_xVelocity) < 155)
				//	{
				//		m_vPos = Vector2(m_pCurrentCurve->Get_LeftX(), m_pCurrentCurve->Get_LeftY());										
				//	}
				//}
				//else // 커브의 상단 끝쪽이 아니라면
				//{
				//	m_vPos = Vector2(m_pCurrentCurve->Get_LeftX(), m_pCurrentCurve->Get_LeftY());															
				//}
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
				//if (m_pCurrentCurve->IfOnY_AccelRange(m_vPos.fX)) //커브의 상단끝쪽에 있다면
				//{
				//	// 속도 느리면 보정
				//	if (abs(m_f_xVelocity) < 155)
				//	{
				//		m_vPos = Vector2(m_pCurrentCurve->Get_RightX(), m_pCurrentCurve->Get_RightY());
				//	}
				//}
				//else // 커브의 상단 끝쪽이 아니라면
				//{
				//	m_vPos = Vector2(m_pCurrentCurve->Get_RightX(), m_pCurrentCurve->Get_RightX());
				//}
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
#pragma region 링
		else if (m_bOnCircle) // 원을 타고 있는 경우
		{
			// 곡선 타다가 왼쪽 바깥으로 나가면
			if (m_vPos.fX < m_pCurrentCircle->Get_LeftX())
			{
				m_bOnCircle = false;

				// 속도 느리면
				if (m_bOnGround && (abs(m_f_xVelocity) < 155))
				{
					m_vPos = Vector2(m_pCurrentCircle->Get_LeftX(), m_pCurrentCircle->Get_BottomY());
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
			m_vPos.fY = m_pCurrentCircle->Get_CurrentY(m_vPos.fX, m_f_xVelocity, m_bRidingCircle);
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
#pragma endregion 링
		else  // 직선 걸어가고 있다면
		{
			if (CLineManager::Get_Instance()->OnLine(m_vPos.fX, m_vPos.fY, fTargetLineY, m_bOnCurve,
				&m_pCurrentCurve, m_bOnCircle, &m_pCurrentCircle)) // 밟을 땅이 있으면
			{
				// 곡선인지 확인
				if (m_bOnCurve)
				{
					m_vPos.fY = (m_pCurrentCurve)->Get_CurrentY(m_vPos.fX,m_f_yVelocity);
				}
				else // 직선이면
				{
					// 플레이어가 공중에 있으면
					if (m_vPos.fY < fTargetLineY) 
					{
						m_bOnGround = false;
					}
				}

			}
			else // 밟을 땅이 없으면
				m_bOnGround = false;
		}
	}

	if (m_bIsSpinning)
		CheckSpinFinish();

	LateUpdateOffset();

	CheckMotionChanged();
	CObj::LateUpdateAnimFrame();
}

void CPlayer::Render(HDC _dc)
{
	if (m_bIsSpinning || m_bSpinReady)
	{
		Ellipse(_dc, m_RECT.left + iScrollX,
			m_RECT.top,
			m_RECT.right + iScrollX,
			m_RECT.bottom);
		return;
	}

	CObj::Render(_dc);

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

void CPlayer::Release()
{
}

void CPlayer::OnCollisionStay(CObj* _pOther, MY_DIR _eDir, float _fDiffCX, float _fDiffCY)
{
	
}

void CPlayer::UpdateMove()
{
	ClampVelocity();

	UpdateMoveX();
	//UpdateMoveY();
}

void CPlayer::UpdateMoveX()
{
	if (m_bRidingCircle)
		return;

	m_vPos.fX += m_f_xVelocity * g_fDeltaTime;
	int(m_vPos.fX);
}

void CPlayer::UpdateMoveY() // 아직 안쓰임
{
	if (m_bOnGround == false)
	{
		m_f_yVelocity += -GRAVITY * g_fDeltaTime;
		m_vPos.fY -= m_f_yVelocity;
	}
		//UpdateInAir();
}

void CPlayer::UpdateInAir() // 공중에 있을 때 // 안쓰임
{
}

void CPlayer::CheckMotionChanged()
{
	if (m_eCurrAnimState != m_ePrevAnimState)
	{
		switch (m_eCurrAnimState)
		{
		case PLAYER_ANIM_IDLE:
			m_anim.Get_FrameInfo().iFrameStart = 0;
			m_anim.Get_FrameInfo().iFrameEnd = 0;
			m_anim.Get_FrameInfo().iMotion = 0;
			m_anim.Get_FrameInfo().dwInterval = 200.f;
			break;
		case PLAYER_ANIM_WALK:
			m_anim.Get_FrameInfo().iFrameStart = 0;
			m_anim.Get_FrameInfo().iFrameEnd = 7;
			m_anim.Get_FrameInfo().iMotion = 1;
			m_anim.Get_FrameInfo().dwInterval = 200.f;
			break;
		case PLAYER_ANIM_RUN:
			m_anim.Get_FrameInfo().iFrameStart = 0;
			m_anim.Get_FrameInfo().iFrameEnd = 3;
			m_anim.Get_FrameInfo().iMotion = 2;
			m_anim.Get_FrameInfo().dwInterval = 200.f;
			break;
		case PLAYER_ANIM_SPIN:
			m_anim.Get_FrameInfo().iFrameStart = 0;
			m_anim.Get_FrameInfo().iFrameEnd = 7;
			m_anim.Get_FrameInfo().iMotion = 3;
			m_anim.Get_FrameInfo().dwInterval = 200.f;
			break;
		case PLAYER_ANIM_STAY_SPIN:
			m_anim.Get_FrameInfo().iFrameStart = 0;
			m_anim.Get_FrameInfo().iFrameEnd = 5;
			m_anim.Get_FrameInfo().iMotion = 4;
			m_anim.Get_FrameInfo().dwInterval = 200.f;
			break;
		case PLAYER_ANIM_LOOK_UP:
			m_anim.Get_FrameInfo().iFrameStart = 0;
			m_anim.Get_FrameInfo().iFrameEnd = 1;
			m_anim.Get_FrameInfo().iMotion = 6;
			m_anim.Get_FrameInfo().dwInterval = 200.f;
			break;
		case PLAYER_ANIM_LOOK_DOWN:
			m_anim.Get_FrameInfo().iFrameStart = 0;
			m_anim.Get_FrameInfo().iFrameEnd = 1;
			m_anim.Get_FrameInfo().iMotion = 7;
			m_anim.Get_FrameInfo().dwInterval = 200.f;
			break;
		case PLAYER_ANIM_SUDDEN_STOP:
			m_anim.Get_FrameInfo().iFrameStart = 0;
			m_anim.Get_FrameInfo().iFrameEnd = 3;
			m_anim.Get_FrameInfo().iMotion = 8;
			m_anim.Get_FrameInfo().dwInterval = 200.f;
			break;

		default:
			break;
		}

		m_anim.Get_FrameInfo().dwTime = GetTickCount();
	}
}

void CPlayer::ClampVelocity()
{
	// Clamp X_Velocity
	if (m_bIsSpinning)
		CMyMath::fClamp(m_f_xVelocity, m_fSpinMinSpeed, m_fSpinMaxSpeed);
	else
		CMyMath::fClamp(m_f_xVelocity, m_fRunMinSpeed, m_fRunMaxSpeed);
}

void CPlayer::InputKey()
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


		if (CKeyManager::Get_Instance()->InputNothing()) // 아무것도 누르지 않은 상태
		{
			m_f_xVelocity = CMyMath::Lerp(m_f_xVelocity, 0.f, fDeltaSpeed);// *2.f);
			
			m_f_xVelocity = CMyMath::Lerp(m_f_xVelocity, 0.f, fDeltaSpeed * 2.f);


			/*m_f_xVelocity += m_fDefaultSpeed *
				CKeyManager::Get_Instance()->Get_Axis_Horizontal(VK_NOINPUT, 0.4f);*/
		}

	}
	else // non spin
	{
		fDeltaSpeed = g_fDeltaTime * 0.04f;
		// 땅에서 아래키 + 스페이스 눌러서 제자리 스피닝
		if (m_bOnGround)
		{
			if (m_bSpinReady && CKeyManager::Get_Instance()->Key_Up(VK_DOWN))
			{

				if (abs(m_f_xVelocity) <= 0.1f)
				{
					m_bSpinReady = false;
					m_bIsSpinning = true; // 비로소 real spinning
					m_f_xVelocity = m_fSpinningPower * m_vDir.fX;
				}

			}
			// 스핀모션이 아닌 상태
			else if (m_bSpinReady == false)
			{
				if (CKeyManager::Get_Instance()->Key_Down(VK_DOWN))
				{
					m_bSpinReady = true;
					UpdateAnimState(PLAYER_ANIM_LOOK_DOWN);
				}
				else if (CKeyManager::Get_Instance()->Key_Down(VK_SPACE))
				{
					// 점프
					fTargetLineY = m_vPos.fY;
					m_bOnGround = false;
					m_f_yVelocity = m_fJumpPower;

					UpdateAnimState(PLAYER_ANIM_SPIN); // jump대신
				}

				// key input
				else if (CKeyManager::Get_Instance()->Key_Pressing(VK_RIGHT))
				{
					m_vDir.fX = 1.f;
					m_f_xVelocity += m_fDefaultSpeed *
						CKeyManager::Get_Instance()->Get_Axis_Horizontal(VK_RIGHT, fDeltaSpeed);

					m_anim.ChangeAnimSheet(L"SonicR0");

					if (abs(m_f_xVelocity) < m_fWalkMin)
						UpdateAnimState(PLAYER_ANIM_IDLE);
					else if (abs(m_f_xVelocity) < m_fRunMin)
						UpdateAnimState((PLAYER_ANIM_WALK));
					else
						UpdateAnimState(PLAYER_ANIM_RUN);
					//UpdateAnimState(PLAYER_ANIM_WALK);
				}
				else if (CKeyManager::Get_Instance()->Key_Pressing(VK_LEFT))
				{
					m_vDir.fX = -1.f;
					m_f_xVelocity += m_fDefaultSpeed *
						CKeyManager::Get_Instance()->Get_Axis_Horizontal(VK_LEFT, fDeltaSpeed);

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

					m_f_xVelocity = CMyMath::Lerp(m_f_xVelocity, 0.f, fDeltaSpeed * 50.f);

					if (abs(m_f_xVelocity) < m_fWalkMin)
						UpdateAnimState(PLAYER_ANIM_IDLE);
					else if (abs(m_f_xVelocity) < m_fRunMin)
						UpdateAnimState((PLAYER_ANIM_WALK));
					else
						UpdateAnimState(PLAYER_ANIM_RUN);
					//UpdateAnimState(PLAYER_ANIM_IDLE);
					/*m_f_xVelocity += m_fDefaultSpeed *
						CKeyManager::Get_Instance()->Get_Axis_Horizontal(VK_NOINPUT, 0.4f);*/
				}
			}
		}
	}
}

void CPlayer::CheckSpinFinish()
{
	if (abs(m_f_xVelocity) < 10)
	{
		m_f_xVelocity = 0.f;
		m_bIsSpinning = false;
	}

}

void CPlayer::LateUpdateOffset()
{
	int iOffsetMinX = m_iOffsetInterval;
	int iOffsetMaxX = WINCX - m_iOffsetInterval;

	iScrollX = CScrollManager::Get_Instance()->Get_ScrollX();

	if (m_vPos.fX + iScrollX < iOffsetMinX)
	{
		CScrollManager::Get_Instance()->
		Set_ScrollXPlus(-1.f * m_f_xVelocity * g_fDeltaTime); // 현 프레임에서 움직인 거리
	}
	else if (m_vPos.fX + iScrollX > iOffsetMaxX)
	{
		CScrollManager::Get_Instance()->
			Set_ScrollXPlus(-1.f * m_f_xVelocity * g_fDeltaTime); // 현 프레임에서 움직인 거리
	}
}

void CPlayer::UpdateAnimState(PLAYER_ANIM_STATE _eAnimState)
{
	m_ePrevAnimState = m_eCurrAnimState;
	m_eCurrAnimState = _eAnimState;
}
