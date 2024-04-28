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
	 * \brief 발 밑이 땅인지 체크
	 * \return 땅이면 true
	 */
	virtual bool CheckUnderFeet(float _fPosX, float _fPosY, float& _refTargetY);

	void ClampVelocity();

	void InputKey();

	/**
	 * \brief 속도 0이면 spinning 마치기
	 */
	void CheckSpinFinish();

	void LateUpdateOffset();

	void UpdateAnimState(PLAYER_ANIM_STATE _eAnimState);
private:
	/**
	 * \brief 진짜 spinning하기 전 동작.\n
	 * 애니메이션 상에서는 스피닝
	 */
	bool m_bSpinReady;
	bool m_bIsSpinning;
	bool m_bPressDown;
	bool m_bIsJumping;

	float m_fSpinningPower; // 제자리에서 스피닝하다가 폭발적으로 나갈때의 속력
	int m_i_xVelocity;

	// for Clamp
	float m_fRunMaxSpeed;
	float m_fRunMinSpeed;
	float m_fSpinMaxSpeed;
	float m_fSpinMinSpeed;

	float fTargetLineY;

	// curve 관련
	bool m_bOnCurve;
	bool m_bJumpOnCurve;
	bool m_bAccelTriggered;
	CCurvedLine* m_pCurrentCurve; // 현재 타고있는 커브라인

	// circle 관련
	bool m_bOnCircle;
	bool m_bRidingCircle; // 링에 몸을 맡긴 경우
	CCircleLine* m_pCurrentCircle;

	bool m_bRidingConnected;

	CLineObjBase* m_pCurrentTargetLine;

	// animation 관련
	PLAYER_ANIM_STATE m_ePrevAnimState;
	PLAYER_ANIM_STATE m_eCurrAnimState;
	float m_fWalkMin; // xVelocity가 이 수치 이상이 되면 걷기모션
	float m_fRunMin; // walk하다 이 수치 이상이 되면 뛰기 모션
	int m_iOffsetInterval;
	float m_fAngle;


	// 피격
	float m_fCurrKnockBackDistance;
};