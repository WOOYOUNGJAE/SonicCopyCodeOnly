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

	// animation 관련
	PLAYER_ANIM_STATE m_ePrevAnimState;
	PLAYER_ANIM_STATE m_eCurrAnimState;
	float m_fWalkMin; // xVelocity가 이 수치 이상이 되면 걷기모션
	float m_fRunMin; // walk하다 이 수치 이상이 되면 뛰기 모션

	int m_iOffsetInterval;
};