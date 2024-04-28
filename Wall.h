#pragma once
#include "Obj.h"

class CWall : public CObj
{
public:
	CWall();
	virtual ~CWall() override;

public:
	virtual 
	virtual void Set_Pos(float _x, float _y) override;
	//setter
	void Set_ShakeTrigger(bool _b) { m_bShakeTriggered = _b; }
	void Set_BossGround(bool _b) { m_bIsBossGround = _b; }

	virtual void Initialize() override;
	virtual bool Update() override;
	virtual void LateUpdate() override;
	virtual void Render(HDC _dc) override;
	virtual void Release() override;

	void WallInit(Vector2 _vScale);
	void WallInit(float _fCX, float _fCY);

	virtual void OnCollisionStay(CObj* _pOther, MY_DIR _eDir, float _fDiffCX, float _fDiffCY) override;
	virtual void OnCollisionEnter(CObj* _pOther, MY_DIR _eDir, float _fDiffCX, float _fDiffCY) override{};
	virtual void OnCollisionExit(CObj* _pOther) override{};

	void ShakeSelf();

private:
	bool m_bShakeTriggered;
	float m_fAmplitude; // ÁøÆø
	int m_iShakeCount;
	Vector2 m_vStaticPos;
	float m_fStaticPosY;
	float m_fTmp = 0.f;
	float m_fLerpT;

	bool m_bIsBossGround;
};

