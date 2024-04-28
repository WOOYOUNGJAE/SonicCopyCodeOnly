#pragma once
#include "Obj.h"
class CRing : public CObj
{
public:
	CRing();
	virtual ~CRing() override;

public:
	// getter
	bool Get_IsSpawned() { return m_bSpawned; }
	// setter
	void Set_Spawned(bool _b) { m_bSpawned = _b; }
	void Set_Dir(Vector2 _vDir) { m_vDir = _vDir; }
	void Set_Angle(float _fAngle) { m_fMoveAngle = _fAngle; }

	void Set_DropCondition(float _fAngle, float _XSpeed = 3, float _YSpeed = 3, float _fGravity = 5)
	{
		m_fMoveAngle = _fAngle;
		m_fXSpeed = _XSpeed;
		m_fYSpeed = _YSpeed;
		m_fRingGravity = _fGravity;
		m_bSpawned = false;
	}

	void Initialize() override;
	bool Update() override;
	void LateUpdate() override;
	void Render(HDC _dc) override;
	void Release() override;
	void OnCollisionStay(CObj* _pOther, MY_DIR _eDir, float _fDiffCX, float _fDiffCY) override;
	virtual void OnCollisionEnter(CObj* _pOther, MY_DIR _eDir, float _fDiffCX, float _fDiffCY) override;
	virtual void OnCollisionExit(CObj* _pOther) override{};
	void TriggerCollected();
	void StartShineEffectAnim();
private:
	// 고정된 위치에 스폰되어 있었냐, false면 플레이어가 흩뿌린경우
	bool m_bSpawned;

	float m_fXSpeed; // 아마 수정 필요
	float m_fYSpeed; // 아마 수정 필요
	float m_fRingGravity;
	float m_fMoveAngle;
	float m_fAccTime;
	int m_iBounceCount;
	Vector2 m_vDir; // 소닉 피격시 흩뿌릴 때 방향
};