#pragma once
#include "Obj.h"

/**
 * \brief ���� �ʿ� ���� ��
 */
class CClearObject : public CObj
{
public:
	CClearObject();
	virtual ~CClearObject() override;

public:
	virtual void OnCollisionEnter(CObj* _pOther, MY_DIR _eDir, float _fDiffCX, float _fDiffCY) override;
};
