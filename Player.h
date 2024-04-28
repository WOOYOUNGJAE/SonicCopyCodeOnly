#pragma once
#include "MovableObject.h"
#include "CurvedLine.h"
#include "CircleLine.h"

enum PLAYER_ANIM_STATE
{
	PLAYER_ANIM_IDLE,
	PLAYER_ANIM_WALK,
	PLAYER_ANIM_RUN,
	PLAYER_ANIM_SPIN,
	PLAYER_ANIM_STAY_SPIN,
	PLAYER_ANIM_LOOK_UP,
	PLAYER_ANIM_LOOK_DOWN,
	PLAYER_ANIM_SUDDEN_STOP,
	PLAYER_ANIM_ATTACKED,
	PLAYER_ANIM_FLY,
	PLAYER_ANIM_STATE_END,
};


class CPlayer : public CMovableObject
{
public:
	CPlayer();
	virtual ~CPlayer() override;


public:
	// getter
	// setter

	virtual void Initialize() override;
	virtual bool Update() override;
	virtual void LateUpdate() override;
	virtual void Render(HDC _dc) override;
	virtual void Release() override;

	virtual void OnCollisionStay(CObj* _pOther, MY_DIR _eDir, float _fDiffCX, float _fDiffCY) override;
	virtual void OnCollisionEnter(CObj* _pOther, MY_DIR _eDir, float _fDiffCX, float _fDiffCY) override{};
	virtual void OnCollisionExit(CObj* _pOther) override{};
	virtual void UpdateMove() override;
	virtual void UpdateMoveX() override;
	virtual void UpdateMoveY() override;
	virtual void UpdateInAir() override;

	virtual void CheckMotionChanged() override;

	void ClampVelocity();

	void InputKey();

	/**
	 * \brief �ӵ� 0�̸� spinning ��ġ��
	 */
	void CheckSpinFinish();

	void LateUpdateOffset();

	void UpdateAnimState(PLAYER_ANIM_STATE _eAnimState);
private:
	/**
	 * \brief ��¥ spinning�ϱ� �� ����.\n
	 * �ִϸ��̼� �󿡼��� ���Ǵ�
	 */
	bool m_bSpinReady;
	bool m_bIsSpinning;
	bool m_bPressDown;

	float m_fSpinningPower; // ���ڸ����� ���Ǵ��ϴٰ� ���������� �������� �ӷ�
	int m_i_xVelocity;

	// for Clamp
	float m_fRunMaxSpeed;
	float m_fRunMinSpeed;
	float m_fSpinMaxSpeed;
	float m_fSpinMinSpeed;

	float fTargetLineY;

	// curve ����
	bool m_bOnCurve;
	bool m_bJumpOnCurve;
	bool m_bAccelTriggered;
	CCurvedLine* m_pCurrentCurve; // ���� Ÿ���ִ� Ŀ�����

	// circle ����
	bool m_bOnCircle;
	bool m_bRidingCircle; // ���� ���� �ñ� ���
	CCircleLine* m_pCurrentCircle;

	// animation ����
	PLAYER_ANIM_STATE m_ePrevAnimState;
	PLAYER_ANIM_STATE m_eCurrAnimState;
	float m_fWalkMin; // xVelocity�� �� ��ġ �̻��� �Ǹ� �ȱ���
	float m_fRunMin; // walk�ϴ� �� ��ġ �̻��� �Ǹ� �ٱ� ���

	int m_iOffsetInterval;
};