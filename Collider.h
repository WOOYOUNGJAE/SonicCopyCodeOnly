#pragma once
#include "Enum.h"
#include "MyMath.h"
/**
 * \brief 충돌이 필요한 오브젝트에 붙이는 컴포넌트
 */
class CObj;
class CCollider
{
public:
	CCollider(): m_pParent(nullptr) {}
	~CCollider() = default;

public:
	Vector2 Get_Pos();
	Vector2 Get_Scale();
	CObj* Get_Parent();

	void Set_Parent(CObj* _pParent) { m_pParent = _pParent; }

	void Initialize();
	void OnEnable(CObj* _pParent);

	void OnCollisionStay(CObj* _pOther, MY_DIR _eDir, float _fDiffCX, float _fDiffCY);

private:
	CObj* m_pParent; // 이 콜라이더를 갖고 있는 부모
	Vector2 m_vPos;
	Vector2 m_vScale; // 부모의 Scale과 동일
};

