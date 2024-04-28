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
	// X��ǥ�� ��� ������ ����
	if (m_bOnGround == false) // ����
	{
		// ���� ���� �ִ��� Ȯ��
		if (CLineManager::Get_Instance()->OnLine(m_vPos.fX, m_vPos.fY, fTargetLineY, m_bOnCurve,
			&m_pCurrentCurve, m_bOnCircle, &m_pCurrentCircle))
		{
			// Ȯ�� ���� ���� ���� ��̾����� Ȯ��
			/*if (m_bOnCurve)
			{
				m_bOnGround = true;
				m_vPos.fY = (m_pCurrentCurve)->Get_CurrentY(m_vPos.fX);
				return;
			}*/

			// �ؿ� ���� ������ �ִٸ� �ϴ� �߷� ����
			m_f_yVelocity += -GRAVITY * g_fDeltaTime;
			m_vPos.fY -= m_f_yVelocity;

			// �߷� ������ y�� ���� ������ �Ʒ��ִٸ� �������� ������ �ŵ��� �ø�
			if (m_vPos.fY > fTargetLineY)
			{
				m_vPos.fY = fTargetLineY;
				m_bOnGround = true;
				m_f_yVelocity = 0.f;
				// TODO
				// check crealurve X range
				if (m_pCurrentCurve) // ���� ����Ű�� Ŀ�갡 ������
				{
					m_bOnCurve = true;
					m_bOnGround = true;
					m_vPos.fY = (m_pCurrentCurve)->Get_CurrentY(m_vPos.fX);
					return;
				}
			}
		}
		else // �����ε� ���� ���� ������ �߷� ����
		{
			m_f_yVelocity += -GRAVITY * g_fDeltaTime;
			m_vPos.fY -= m_f_yVelocity;
		}
	}
	else // OnGround
	{
		if (m_bOnCurve) // � Ÿ�� �ִٸ�
		{
#pragma region Ŀ�� Ÿ�ٰ� ������ ���
			// � Ÿ�ٰ� ���� �ٱ����� ������
			if (m_vPos.fX < m_pCurrentCurve->Get_LeftX())
			{
				m_bOnCurve = false;

				// �ӵ� �����ų� �������� ������
				if ( m_bOnGround && (
					abs(m_f_xVelocity) < 155 || m_vPos.fY >= m_pCurrentCurve->Get_LowesetY()))
				{
					m_vPos = Vector2(m_pCurrentCurve->Get_LeftX(), m_pCurrentCurve->Get_LeftY());
				}
				//if (m_pCurrentCurve->IfOnY_AccelRange(m_vPos.fX)) //Ŀ���� ��ܳ��ʿ� �ִٸ�
				//{
				//	// �ӵ� ������ ����
				//	if (abs(m_f_xVelocity) < 155)
				//	{
				//		m_vPos = Vector2(m_pCurrentCurve->Get_LeftX(), m_pCurrentCurve->Get_LeftY());										
				//	}
				//}
				//else // Ŀ���� ��� ������ �ƴ϶��
				//{
				//	m_vPos = Vector2(m_pCurrentCurve->Get_LeftX(), m_pCurrentCurve->Get_LeftY());															
				//}
				m_pCurrentCurve = nullptr;
				return;
			}
			// � Ÿ�ٰ� ������ �ٱ����� ������ �����ʿ� ����
			else if (m_vPos.fX > m_pCurrentCurve->Get_RightX())
			{
				m_bOnCurve = false;
				// �ӵ� �����ų� �������� ������
				if (abs(m_f_xVelocity) < m_fRunMaxSpeed - 50 || m_vPos.fY >= m_pCurrentCurve->Get_LowesetY())
				{
					m_vPos = Vector2(m_pCurrentCurve->Get_RightX(), m_pCurrentCurve->Get_RightY());
				}
				//if (m_pCurrentCurve->IfOnY_AccelRange(m_vPos.fX)) //Ŀ���� ��ܳ��ʿ� �ִٸ�
				//{
				//	// �ӵ� ������ ����
				//	if (abs(m_f_xVelocity) < 155)
				//	{
				//		m_vPos = Vector2(m_pCurrentCurve->Get_RightX(), m_pCurrentCurve->Get_RightY());
				//	}
				//}
				//else // Ŀ���� ��� ������ �ƴ϶��
				//{
				//	m_vPos = Vector2(m_pCurrentCurve->Get_RightX(), m_pCurrentCurve->Get_RightX());
				//}
				m_pCurrentCurve = nullptr;
				return;
			}
#pragma endregion

			// ��Ÿ��
			if (abs(m_f_xVelocity) < m_fRunMaxSpeed - 50) // �Ϲ� ��Ÿ��
			{
				m_vPos.fY = (m_pCurrentCurve)->Get_CurrentY(m_vPos.fX);
			}
			else if (m_bAccelTriggered == false) //������ y�Ǽ�
			{				
				m_vPos.fY = (m_pCurrentCurve)->Get_CurrentY(m_vPos.fX, m_f_yVelocity, m_bAccelTriggered);
			}
			else // ������ �Ǽ� ������
			{
				m_vPos.fY = (m_pCurrentCurve)->Get_CurrentY(m_vPos.fX);
			}
		}
#pragma region ��
		else if (m_bOnCircle) // ���� Ÿ�� �ִ� ���
		{
			// � Ÿ�ٰ� ���� �ٱ����� ������
			if (m_vPos.fX < m_pCurrentCircle->Get_LeftX())
			{
				m_bOnCircle = false;

				// �ӵ� ������
				if (m_bOnGround && (abs(m_f_xVelocity) < 155))
				{
					m_vPos = Vector2(m_pCurrentCircle->Get_LeftX(), m_pCurrentCircle->Get_BottomY());
				}

				m_pCurrentCircle = nullptr;
				return;
			}
			// � Ÿ�ٰ� ������ �ٱ����� ������ �����ʿ� ����
			else if (m_vPos.fX > m_pCurrentCircle->Get_RightX() + 2.f)
			{
				m_bOnCircle = false;
				// �ӵ� ������
				if (m_bOnGround && abs(m_f_xVelocity) < 155)
				{
					//m_vPos = Vector2(m_pCurrentCircle->Get_RightX(), m_pCurrentCircle->get());
				}

				m_pCurrentCircle = nullptr;
				return;
			}

			// ��Ÿ��
			m_vPos.fY = m_pCurrentCircle->Get_CurrentY(m_vPos.fX, m_f_xVelocity, m_bRidingCircle);
			//if (abs(m_f_xVelocity) < 155.f) // �Ϲ� ��Ÿ��
			//{
			//	m_vPos.fY = (m_pCurrentCircle)->Get_CurrentY(m_vPos.fX,);
			//}
			//else if (m_bAccelTriggered == false) //������ y�Ǽ�
			//{
			//	m_vPos.fY = (m_pCurrentCircle)->Get_CurrentY(m_vPos.fX, m_f_yVelocity, m_bAccelTriggered);
			//}
			//else // ������ �Ǽ� ������
			//{
			//	m_vPos.fY = (m_pCurrentCircle)->Get_CurrentY(m_vPos.fX,);
			//}
		}
#pragma endregion ��
		else  // ���� �ɾ�� �ִٸ�
		{
			if (CLineManager::Get_Instance()->OnLine(m_vPos.fX, m_vPos.fY, fTargetLineY, m_bOnCurve,
				&m_pCurrentCurve, m_bOnCircle, &m_pCurrentCircle)) // ���� ���� ������
			{
				// ����� Ȯ��
				if (m_bOnCurve)
				{
					m_vPos.fY = (m_pCurrentCurve)->Get_CurrentY(m_vPos.fX,m_f_yVelocity);
				}
				else // �����̸�
				{
					// �÷��̾ ���߿� ������
					if (m_vPos.fY < fTargetLineY) 
					{
						m_bOnGround = false;
					}
				}

			}
			else // ���� ���� ������
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

void CPlayer::UpdateMoveY() // ���� �Ⱦ���
{
	if (m_bOnGround == false)
	{
		m_f_yVelocity += -GRAVITY * g_fDeltaTime;
		m_vPos.fY -= m_f_yVelocity;
	}
		//UpdateInAir();
}

void CPlayer::UpdateInAir() // ���߿� ���� �� // �Ⱦ���
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

	if (m_bIsSpinning) // ������ ����
	{
		fDeltaSpeed = 0.02f;
		UpdateAnimState(PLAYER_ANIM_SPIN);
		// ������, ���ڸ� ���Ǵ׿��� �Ʒ�Ű�� ����



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


		if (CKeyManager::Get_Instance()->InputNothing()) // �ƹ��͵� ������ ���� ����
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
		// ������ �Ʒ�Ű + �����̽� ������ ���ڸ� ���Ǵ�
		if (m_bOnGround)
		{
			if (m_bSpinReady && CKeyManager::Get_Instance()->Key_Up(VK_DOWN))
			{

				if (abs(m_f_xVelocity) <= 0.1f)
				{
					m_bSpinReady = false;
					m_bIsSpinning = true; // ��μ� real spinning
					m_f_xVelocity = m_fSpinningPower * m_vDir.fX;
				}

			}
			// ���ɸ���� �ƴ� ����
			else if (m_bSpinReady == false)
			{
				if (CKeyManager::Get_Instance()->Key_Down(VK_DOWN))
				{
					m_bSpinReady = true;
					UpdateAnimState(PLAYER_ANIM_LOOK_DOWN);
				}
				else if (CKeyManager::Get_Instance()->Key_Down(VK_SPACE))
				{
					// ����
					fTargetLineY = m_vPos.fY;
					m_bOnGround = false;
					m_f_yVelocity = m_fJumpPower;

					UpdateAnimState(PLAYER_ANIM_SPIN); // jump���
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


				if (CKeyManager::Get_Instance()->InputNothing()) // �ƹ��͵� ������ ���� ����
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
		Set_ScrollXPlus(-1.f * m_f_xVelocity * g_fDeltaTime); // �� �����ӿ��� ������ �Ÿ�
	}
	else if (m_vPos.fX + iScrollX > iOffsetMaxX)
	{
		CScrollManager::Get_Instance()->
			Set_ScrollXPlus(-1.f * m_f_xVelocity * g_fDeltaTime); // �� �����ӿ��� ������ �Ÿ�
	}
}

void CPlayer::UpdateAnimState(PLAYER_ANIM_STATE _eAnimState)
{
	m_ePrevAnimState = m_eCurrAnimState;
	m_eCurrAnimState = _eAnimState;
}
