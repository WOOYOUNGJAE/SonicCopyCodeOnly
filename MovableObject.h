#pragma once
#include "Obj.h"

#define GRAVITY 9.81

class CMovableObject : public CObj
{
public:
	CMovableObject();
	virtual ~CMovableObject() override;

public:
	// getter
	int Get_MaxHP() const { return m_iMaxHP; }
	int Get_CurrentHP() const{ return m_iCurrentHP; }
	bool Get_Attacked() const { return m_bIsAttacked; }
	// setter
	void Set_Attacked(bool _b) { m_bIsAttacked = _b; }
	void Set_Dir(float _x, float _y) { m_vDir = Vector2(_x, _y); }
	void Set_xVelocity(float _xVelocity) { m_f_xVelocity = _xVelocity; }
	void Set_yVelocity(float _yVelocity) { m_f_yVelocity = _yVelocity; }
	void Set_OnGround(bool _b) { m_bOnGround = _b; }
	void Set_m_xVelocityMinus(float _xVelocity) { m_f_xVelocity -= _xVelocity; }
	void Set_HPMinus() { --m_iCurrentHP; }

	void Set_Scale_AdjustXY(float _fX, float _fY, float _adjustX, float _adjustY);
	// override
	virtual void Initialize() override;
	virtual bool Update() override;
	virtual void LateUpdate() override;
	virtual void Render(HDC _dc) override;
	virtual void Release() override;

	virtual void OnCollisionStay(CObj* _pOther, MY_DIR _eDir, float _fDiffCX, float _fDiffCY) override;

	virtual void UpdateMove();
	virtual void UpdateMoveX();
	virtual void UpdateMoveY();
	virtual void UpdateInAir();

	virtual void CheckMotionChanged() = 0;

	virtual void CheckAlive();
protected:
	Vector2 m_vDir;


	bool m_bOnGround;
	float m_fDefaultSpeed;
	float m_f_xVelocity;
	/**
	 * \brief 일반적인 좌표 방향으로 쓰되,\n
	 * 마지막에 움직일 때만 yPos -= yVelocity 식으로
	 */
	float m_f_yVelocity;
	float m_fJumpPower;
	
	float m_fAirAccTime;


	int m_iMaxHP;
	int m_iCurrentHP;

	int m_iAnimAdjustCX;
	int m_iAnimAdjustCY;
	bool m_bIsAttacked;
};

