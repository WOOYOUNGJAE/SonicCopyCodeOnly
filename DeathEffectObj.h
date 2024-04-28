#pragma once
#include "Obj.h"
class CDeathEffectObj : public CObj
{
public:
	CDeathEffectObj();
	virtual ~CDeathEffectObj() override;

	virtual void Initialize() override;
	virtual bool Update() override;
	virtual void LateUpdate() override;
	virtual void Render(HDC _dc) override;
	virtual void Release() override;

	virtual void OnCollisionStay(CObj* _pOther, MY_DIR _eDir, float _fDiffCX, float _fDiffCY) override {}
	virtual void OnCollisionEnter(CObj* _pOther, MY_DIR _eDir, float _fDiffCX, float _fDiffCY) override {}
	virtual void OnCollisionExit(CObj* _pOther) override {}


private:

};

