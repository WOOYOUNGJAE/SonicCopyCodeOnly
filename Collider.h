#pragma once
#include "Enum.h"
#include "MyMath.h"
/**
 * \brief �浹�� �ʿ��� ������Ʈ�� ���̴� ������Ʈ
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
	CObj* m_pParent; // �� �ݶ��̴��� ���� �ִ� �θ�
	Vector2 m_vPos;
	Vector2 m_vScale; // �θ��� Scale�� ����
};

