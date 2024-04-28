#pragma once
#include "Obj.h"
class CSpring : public CObj
{
public:
	CSpring();
	virtual ~CSpring() override;

public:
	// getter
	MY_DIR Get_SpringDir() { return m_eSpringDir; }
	int Get_SpringDirInt() { return (int)m_eSpringDir; }
	// setter
	void Set_SpringDir(MY_DIR _eDir) { m_eSpringDir = _eDir; }
	void Set_AddingPower(float _fAddingPower) { m_fAddingYPower = _fAddingPower; }
	// override
	virtual void Initialize() override;
	virtual bool Update() override;
	virtual void LateUpdate() override;
	virtual void Render(HDC _dc) override;
	virtual void Release() override;

	virtual void OnCollisionStay(CObj* _pOther, MY_DIR _eDir, float _fDiffCX, float _fDiffCY) override;
	virtual void OnCollisionEnter(CObj* _pOther, MY_DIR _eDir, float _fDiffCX, float _fDiffCY) override{};
	virtual void OnCollisionExit(CObj* _pOther) override{};
	void SpringInit(MY_DIR _eDir);
private:
	MY_DIR m_eSpringDir;
	float m_fAddingXPower;
	float m_fAddingYPower;
	int m_iColorAdd; // ±‚∫ª 0 ª°∞≠¿Ã∏È ¿Œµ¶Ω∫ + 2

	int m_iFrameCX;
	int m_iFrameCY;

	int m_iDefaultFrameX; // ¿Œµ¶Ω∫
	int m_iDefaultFrameY;
	int m_iTriggeredFrameX;
};

