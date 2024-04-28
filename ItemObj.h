#pragma once
#include "Obj.h"

enum ITEM_ID
{
	ITEM_RING,
	ITEM_LIFE,
	ITEM_FIRE,
	ITEM_WATER,
	ITEM_LIGHTENING,
	ITEM_ID_END,
};

class CItemObj : public CObj
{
public:
	CItemObj();
	virtual ~CItemObj() override;

public:
	// getter
	ITEM_ID Get_ItemID() { return m_eItemID; }

	void Set_ItemID(int i)
	{
		i %= 5;
		m_eItemID = (ITEM_ID)i;
		m_anim.Get_FrameInfo().iMotion = i;
	}

	void Initialize() override;
	bool Update() override;
	void LateUpdate() override;
	void Render(HDC _dc) override;
	void Release() override;
	void OnCollisionStay(CObj* _pOther, MY_DIR _eDir, float _fDiffCX, float _fDiffCY) override;
	virtual void OnCollisionEnter(CObj* _pOther, MY_DIR _eDir, float _fDiffCX, float _fDiffCY) override;
	virtual void OnCollisionExit(CObj* _pOther) override {};

	void ActiveItem();

	ITEM_ID m_eItemID;

	
};

