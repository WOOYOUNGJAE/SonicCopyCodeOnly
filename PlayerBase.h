#pragma once
#include "MovableObject.h"
#include "CurvedLine.h"
#include "CircleLine.h"
#include "Player.h"

//enum PLAYER_ANIM_STATE
//{
//	PLAYER_ANIM_IDLE,
//	PLAYER_ANIM_WALK,
//	PLAYER_ANIM_RUN,
//	PLAYER_ANIM_SPIN,
//	PLAYER_ANIM_STAY_SPIN,
//	PLAYER_ANIM_LOOK_UP,
//	PLAYER_ANIM_LOOK_DOWN,
//	PLAYER_ANIM_SUDDEN_STOP,
//	PLAYER_ANIM_STATE_END,
//};

class CPlayerBase : public CMovableObject
{
public:
	CPlayerBase();
	virtual ~CPlayerBase() override;


public:
	// getter
	CLineObjBase* Get_CurrentLine() { return m_pCurrentTargetLine; }
	bool Get_IsRight() { return m_bIsRight; }
	PLAYER_ANIM_STATE Get_CurrentAnimState() { return m_eCurrAnimState; }
	// setter
	void Set_OnCurve(bool _b) { m_bOnCurve = _b; }
	void Set_OnCircle(bool _b) { m_bOnCircle  = _b; }
	void Set_OnCircleRiding(bool _b) { m_bRidingCircle = _b; }
	void Set_OnConnectedRiding(bool _b) { m_bRidingConnected = _b; }
	void Set_CurrentCurve(CCurvedLine* _pCurveLine) { m_pCurrentCurve = _pCurveLine; }
	void Set_CurrentCircle(CCircleLine* _pCircleLine) { m_pCurrentCircle = _pCircleLine; }
	void Set_CurrentLine(CLineObjBase* _pLine) { m_pCurrentTargetLine = _pLine; }
	void Set_Angle(float _fAngle) {  m_fAngle = _fAngle; }

	virtual void Initialize() override;
	virtual bool Update() override;
	virtual void LateUpdate() override;
	virtual void Render(HDC _dc) override;
	virtual void Release() override;

	virtual void OnCollisionStay(CObj* _pOther, MY_DIR _eDir, float _fDiffCX, float _fDiffCY) override;
	virtual void OnCollisionEnter(CObj* _pOther, MY_DIR _eDir, float _fDiffCX, float _fDiffCY) override;
	virtual void OnCollisionExit(CObj* _pOther) override;

	virtual void UpdateMove() override;
	virtual void UpdateMoveX() override;
	virtual void UpdateMoveY() override;
	virtual void UpdateInAir() override;

	//void UpdateOn

	virtual void CheckMotionChanged() override;

	void LineOnConnected();
	void LineOnCurve();
	void LineOnCircle();

	/**
	 * \brief �� ���� ������ üũ
	 * \return ���̸� true
	 */
	virtual bool CheckUnderFeet(float _fPosX, float _fPosY, float& _refTargetY);

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
	bool m_bIsJumping;

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

	bool m_bRidingConnected;

	CLineObjBase* m_pCurrentTargetLine;

	// animation ����
	PLAYER_ANIM_STATE m_ePrevAnimState;
	PLAYER_ANIM_STATE m_eCurrAnimState;
	float m_fWalkMin; // xVelocity�� �� ��ġ �̻��� �Ǹ� �ȱ���
	float m_fRunMin; // walk�ϴ� �� ��ġ �̻��� �Ǹ� �ٱ� ���
	int m_iOffsetInterval;
	float m_fAngle;


	// �ǰ�
	float m_fCurrKnockBackDistance;
};