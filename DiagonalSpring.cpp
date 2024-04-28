#include "stdafx.h"
#include "DiagonalSpring.h"

#include "BmpManager.h"
#include "PlayerBase.h"
#include "ScrollManager.h"
#include "SoundManager.h"

CDiagonalSpring::CDiagonalSpring() : m_eSpringDir(DIR_RU), m_fAddingXPower(500), m_fAddingYPower(10), m_iDefaultFrameX(0), m_iDefaultFrameY(0),
                                     m_iTriggeredFrameX(0), m_iColorAdd(0), m_iFrameCX(36), m_iFrameCY(36)
{
	m_strName = L"DiagonalSpring";
	m_eID = ID_DIAGONALSPRING;
	m_pFrameKey = L"DiagonalSpring";
	m_vScale = { 36,36 };
}

CDiagonalSpring::~CDiagonalSpring()
{
}

void CDiagonalSpring::Initialize()
{
}

bool CDiagonalSpring::Update()
{
	return OBJ_ALIVE;
}

void CDiagonalSpring::LateUpdate()
{
}

void CDiagonalSpring::Render(HDC _dc)
{
	const int iScrollX = (int)CScrollManager::Get_Instance()->Get_ScrollX();
	const int iScrollY = (int)CScrollManager::Get_Instance()->Get_ScrollY();


	HDC hMemDC = CBmpManager::Get_Instance()->Find_Img(m_pFrameKey);
	//HDC hMemDC = CBmpManager::Get_Instance()->Find_Img(L"SonicR0");; // bmp�Ŵ��� �Ȱ�ġ�� ���� �޾ƿ�

	const int iRectWidth = (int)m_vScale.fX;
	const int iRectHeight = (int)m_vScale.fY;
	/*const int iRectWidth = 64;
	const int iRectHeight =64;*/

	CObj::Render(_dc);

	GdiTransparentBlt(
		// ���� â DC
		_dc,	// ������� ���� ��� dc
		(int)m_vPos.fX - m_vScale.fX * 0.5f + iScrollX, // DC�������� x��ǥ
		(int)m_vPos.fY - m_vScale.fY * 0.5f + iScrollY,// DC�������� y��ǥ
		iRectWidth, // ��Ʈ���� ���� ������
		iRectHeight, // ��Ʈ���� ���� ������

		// Bmp �� DC
		hMemDC, // �����  �̹��� dc
		m_iDefaultFrameX * m_iFrameCX, // ��Ʈ���� ��� ���� x��ǥ
		m_iDefaultFrameY * m_iFrameCY,	// ��Ʈ���� ��� ���� Y��ǥ
		m_iFrameCX, // Sheet �� ���� ������
		m_iFrameCY, // Sheet �� ���� ������
		RGB(255, 0, 255)); // ������ �ȼ� ����
}

void CDiagonalSpring::Release()
{
}

void CDiagonalSpring::OnCollisionStay(CObj* _pOther, MY_DIR _eDir, float _fDiffCX, float _fDiffCY)
{	// �ߵ� ���� �Ǹ�
	if (_pOther->Get_ID() == ID_PLAYER)
	{
		if (m_eSpringDir == DIR_RU && (_eDir == DIR_UP || _eDir == DIR_RIGHT))
		{
			dynamic_cast<CPlayerBase*>(_pOther)->Set_yVelocity(m_fAddingYPower);
			dynamic_cast<CPlayerBase*>(_pOther)->Set_xVelocity(m_fAddingXPower);
			if (dynamic_cast<CPlayerBase*>(_pOther)->Get_IsRight())
			{
				_pOther->Get_Anim().ChangeAnimSheet(L"SonicR");
			}
			else
			{
				_pOther->Get_Anim().ChangeAnimSheet(L"SonicL");
			}
			dynamic_cast<CPlayerBase*>(_pOther)->Set_OnGround(false);
			CSoundManager::Get_Instance()->StopSound(SOUND_EFFECT);
			CSoundManager::Get_Instance()->PlaySoundW(L"spring.wav", SOUND_EFFECT, 1.f);
			if (dynamic_cast<CPlayerBase*>(_pOther)->Get_CurrentAnimState()
				!= PLAYER_ANIM_FLY)
				dynamic_cast<CPlayerBase*>(_pOther)->UpdateAnimState(PLAYER_ANIM_FLY);
		}
		else if (m_eSpringDir == DIR_LU && (_eDir == DIR_UP || _eDir == DIR_LEFT))
		{
			dynamic_cast<CPlayerBase*>(_pOther)->Set_yVelocity(m_fAddingYPower);
			dynamic_cast<CPlayerBase*>(_pOther)->Set_xVelocity(-m_fAddingXPower);
			if (dynamic_cast<CPlayerBase*>(_pOther)->Get_IsRight())
			{
				_pOther->Get_Anim().ChangeAnimSheet(L"SonicR");
			}
			else
			{
				_pOther->Get_Anim().ChangeAnimSheet(L"SonicL");
			}
			dynamic_cast<CPlayerBase*>(_pOther)->Set_OnGround(false);
			CSoundManager::Get_Instance()->StopSound(SOUND_EFFECT);
			CSoundManager::Get_Instance()->PlaySoundW(L"spring.wav", SOUND_EFFECT, 1.f);
			if (dynamic_cast<CPlayerBase*>(_pOther)->Get_CurrentAnimState()
				!= PLAYER_ANIM_FLY)
				dynamic_cast<CPlayerBase*>(_pOther)->UpdateAnimState(PLAYER_ANIM_FLY);
		}
		else if (m_eSpringDir == DIR_RD && (_eDir == DIR_DOWN || _eDir == DIR_RIGHT))
		{
			dynamic_cast<CPlayerBase*>(_pOther)->Set_yVelocity(-m_fAddingYPower);
			dynamic_cast<CPlayerBase*>(_pOther)->Set_xVelocity(m_fAddingXPower);
			dynamic_cast<CPlayerBase*>(_pOther)->Set_OnGround(false);
			CSoundManager::Get_Instance()->StopSound(SOUND_EFFECT);
			CSoundManager::Get_Instance()->PlaySoundW(L"spring.wav", SOUND_EFFECT, 1.f);
		}
		else if (m_eSpringDir == DIR_LD && (_eDir == DIR_DOWN || _eDir == DIR_LEFT))
		{
			dynamic_cast<CPlayerBase*>(_pOther)->Set_yVelocity(-m_fAddingYPower);
			dynamic_cast<CPlayerBase*>(_pOther)->Set_xVelocity(-m_fAddingXPower);
			dynamic_cast<CPlayerBase*>(_pOther)->Set_OnGround(false);
			CSoundManager::Get_Instance()->StopSound(SOUND_EFFECT);
			CSoundManager::Get_Instance()->PlaySoundW(L"spring.wav", SOUND_EFFECT, 1.f);
		}		
	}
}

void CDiagonalSpring::SpringInit(DIAGONAL_DIR _eDir)
{
	m_eSpringDir = _eDir;

	switch (_eDir)
	{
	case DIR_LU:
		m_iDefaultFrameX = m_iColorAdd;
		m_iDefaultFrameY = 0;
		m_iTriggeredFrameX = m_iColorAdd;
		break;

	case DIR_RU:
		m_iDefaultFrameX = m_iColorAdd;
		m_iDefaultFrameY = 1;
		m_iDefaultFrameX = m_iColorAdd;
		break;

	case DIR_LD:
		m_iDefaultFrameX = m_iColorAdd;
		m_iDefaultFrameY = 2;
		m_iDefaultFrameX = m_iColorAdd;
		break;


	case DIR_RD:
		m_iDefaultFrameX = m_iColorAdd;
		m_iDefaultFrameY = 3;
		m_iDefaultFrameX = m_iColorAdd;
		break;

	}
}
