#pragma once
#include "Obj.h"
/**
 * \brief 돌아가는
 */
class CSignObj : public CObj
{
public:
	CSignObj();
	virtual ~CSignObj() override;

public:
	// setter
	void Set_LandingPos(float _fX, float _fY) { m_vTargetLandingPos = { _fX, _fY }; }
	virtual void Set_Pos(float _x, float _y) override { m_vPos = Vector2(_x, _y); m_vStartPos = m_vPos; }

	virtual void Initialize() override;
	virtual bool Update() override;
	virtual void LateUpdate() override;
	virtual void Render(HDC _dc) override;
	virtual void Release() override;

	virtual void OnCollisionStay(CObj* _pOther, MY_DIR _eDir, float _fDiffCX, float _fDiffCY) override {}
	virtual void OnCollisionEnter(CObj* _pOther, MY_DIR _eDir, float _fDiffCX, float _fDiffCY) override {}
	virtual void OnCollisionExit(CObj* _pOther) override {}
private:
	Vector2 m_vStartPos;
	Vector2 m_vTargetLandingPos;
	float m_fLerpSpeed;
	float m_fLerpAcc;

	int m_iAnimCycle = 0;
};