#pragma once
#include "MovableObject.h"

enum BOSS_STATE
{
	BOSS_BORN, // µîÀå
	BOSS_IDLE, // 
	BOSS_TARGETING,
	BOSS_ATTACK0, // ½ºÇÉÀ¸·Î ³­µ¿ÇÇ¿ì±â
	BOSS_ATTACK1, // ¿Ã¶ú´Ù°¡ ¶¥ Âï±â

	BOSS_STATE_END,
};

enum BOSS_ANIM_STATE
{
	BOSS_ANIM_STANDING,
	BOSS_ANIM_SIT,
	BOSS_ANIM_SPINSTART,
	BOSS_ANIM_SPINNING,
	BOSS_ANIM_SPINEND,
	BOSS_ANIM_REVERSE,
	BOSS_ANIM_SLIDING,
	BOSS_ANIM_DYING,
	BOSS_ANIM_STATE_END,
};

class CMechaSonic : public CMovableObject
{
public:
	CMechaSonic();
	virtual ~CMechaSonic() override;

	// getter
	BOSS_ANIM_STATE Get_AnimState() { return m_eCurrAnimState; }
	BOSS_STATE Get_State() { return m_eState; }
	//setter
	void Set_AttackedStartTime(DWORD _currentTime) { m_dwAttackedStartTime = _currentTime; }

	// override
	virtual void Initialize() override;
	virtual bool Update() override;
	virtual void LateUpdate() override;
	virtual void Render(HDC _dc) override;
	virtual void Release() override;

	virtual void OnCollisionStay(CObj* _pOther, MY_DIR _eDir, float _fDiffCX, float _fDiffCY) override;
	virtual void OnCollisionEnter(CObj* _pOther, MY_DIR _eDir, float _fDiffCX, float _fDiffCY) override;
	virtual void OnCollisionExit(CObj* _pOther) override;
	virtual void CheckMotionChanged() override;

	virtual void CheckAlive() override;

	void UpdateAnimState(BOSS_ANIM_STATE _eAnimState);
	
	void UpdateBorn();
	void UpdateTargeting();

	void UpdateAttack0();
	void UpdateAttack1();

	bool IfLookingRight();

	void RenewTargetPos() {
		m_vTargetPos = m_pTarget->Get_Pos();
		m_vDir = m_vTargetPos - m_vPos;
		m_vDir.Normalize();
	}
private:
	BOSS_STATE m_eState;
	BOSS_ANIM_STATE m_ePrevAnimState;
	BOSS_ANIM_STATE m_eCurrAnimState;
	CObj* m_pTarget;

	Vector2 m_vBornPos;
	Vector2 m_vTargetPos;

	// Time Count;
	DWORD m_dwTargetStartTime;
	DWORD m_dwTargetAccTime;

	DWORD m_dwRushDownWaitStartTime;
	DWORD m_dwRushDownWaitAccTime;

	DWORD m_dwAttackedStartTime;
	DWORD m_dwAttackedAccTime0;
	DWORD m_dwAttackedAccTime1;
	DWORD m_dwAttackedAccTime2;

	int m_iWallCollisionCount;
	int m_iWallCollisionMax;

	bool m_bWaitStart;
	bool m_bAnimPause;
	bool m_bAnimCycled;
	bool m_bRushDownComplete;
	bool m_bClearTick;

	float m_fRushDownSpeed;

	CObj* m_pDownWall;

	bool attack0;
};

//Vector2 vCenter = { WINCX * 0.5f, WINCY * 0.5f };