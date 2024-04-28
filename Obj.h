#pragma once
#include "Include.h"
#include "MyAnimation.h"
#include "Collider.h"

// ��� ���� ������Ʈ�� ���̽� Ŭ����
class CObj abstract
{
public:
	CObj();
	virtual ~CObj() = default;

	/*bool operator>(const CObj& _rhs) const { return m_bAlive >= _rhs.m_bAlive; }
	bool operator<(const CObj& _rhs) const { return m_bAlive < _rhs.m_bAlive; }*/

public:
	// getter
	Vector2& Get_Pos() { return m_vPos; }
	Vector2& Get_Scale() { return m_vScale; }
	OBJ_ID Get_ID() { return m_eID; }
	RENDER_GROUP Get_RenderGroup() { return m_eRenderGroup; }
	CCollider& Get_Collider() { return  m_collider; }
	wstring Get_Name() { return m_strName; }
	CMyAnimation& Get_Anim() { return m_anim; }

	// setter
	void Set_ID(OBJ_ID _eID) { m_eID = _eID; }
	virtual void Set_Pos(float _x, float _y) { m_vPos = Vector2(_x, _y); }
	void Set_Scale(float _x, float _y) { m_vScale = Vector2(_x, _y); }
	void Set_FrameKey(const TCHAR* _pFrameKey) { m_pFrameKey = _pFrameKey; }
	void Set_PosStatic(bool _b) { m_bIsStatic = _b; }
	void Set_Alive(bool _b) { m_bAlive = _b; }

	/**
	 * \brief ���� �׷쿡 �ֱ⸸
	 */
	virtual void Initialize();
	virtual bool Update() = 0; // return If Obj Active
	virtual void LateUpdate() = 0;
	virtual void Render(HDC _dc);
	virtual void Release() = 0;

	virtual void OnCollisionStay(CObj* _pOther, MY_DIR _eDir, float _fDiffCX, float _fDiffCY) = 0;
	virtual void OnCollisionEnter(CObj* _pOther, MY_DIR _eDir, float _fDiffCX, float _fDiffCY) = 0;
	virtual void OnCollisionExit(CObj* _pOther) = 0;

	void LateUpdateRECT();

	/**
	 * \brief ������ FrameKey, AnimState, �� �Ǵ��Ͽ� �ִϸ��̼� �����̱�
	 * \return �� ����Ŭ ������ true��ȯ
	 */
	bool LateUpdateAnimFrame(); // �����Ӵ��� �̵�
	bool AnimOneCycle(); // LateUpdateAnimFrame ����
protected:
	Vector2 m_vPos;
	Vector2 m_vScale; // �ݶ��̴� ��
	RECT m_RECT;

	bool m_bAlive;
	bool m_bIsRight; // ������ ����

	wstring m_strName;
	OBJ_ID m_eID;

	MY_DIR	m_eCollisionDIr;

	int iScrollX;
	int iScrollY;


	// Animation Control
	const TCHAR* m_pFrameKey;
	CMyAnimation m_anim;

	// Collider
	CCollider m_collider;
	bool m_bIsStatic; // ��ġ ������ ������Ʈ

	RENDER_GROUP m_eRenderGroup;
};

