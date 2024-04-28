#pragma once
#include "Obj.h"
class CMyTile : public CObj
{
public:
	CMyTile();
	virtual ~CMyTile() override;

public:
	// getter
	int Get_DrawIDX() { return m_iDrawIDX; }
	int Get_DrawIDY() { return m_iDrawIDY; }
	int Get_Option() { return m_iOption; }
	// setter
	void Set_DrawIDX(int _iDrawID) { m_iDrawIDX = _iDrawID; }
	void Set_DrawIDY(int _iDrawID) { m_iDrawIDY = _iDrawID; }
	void Set_Option(int _iOption) { m_iOption = _iOption; }

	// override
	virtual void Initialize() override;
	virtual bool Update() override;
	virtual void LateUpdate() override;
	virtual void Render(HDC _dc) override;
	virtual void Release() override;
	virtual void OnCollisionStay(CObj* _pOther, MY_DIR _eDir, float _fDiffCX, float _fDiffCY) override {}
	virtual void OnCollisionEnter(CObj* _pOther, MY_DIR _eDir, float _fDiffCX, float _fDiffCY) override{};
	virtual void OnCollisionExit(CObj* _pOther) override{};

private:
	int m_iDrawIDX;
	int m_iDrawIDY;
	int m_iOption;

};

