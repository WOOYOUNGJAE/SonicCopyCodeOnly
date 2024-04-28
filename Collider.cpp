#include "stdafx.h"
#include "Obj.h"
#include "Collider.h"

Vector2 CCollider::Get_Pos()
{
	if (m_pParent)
		return m_vPos;
}

Vector2 CCollider::Get_Scale()
{
	if (m_pParent)
		return m_vScale;
}

CObj* CCollider::Get_Parent()
{
	if (m_pParent)
		return m_pParent;
}

void CCollider::Initialize()
{
	if (m_pParent)
	{
		m_vPos = m_pParent->Get_Pos();
		m_vScale = m_pParent->Get_Scale();
	}

}

void CCollider::OnEnable(CObj* _pParent)
{
	m_pParent = _pParent;
	//m_vPos = m_pParent->Get_Pos();
	m_vScale = m_pParent->Get_Scale();
}

void CCollider::OnCollisionStay(CObj* _pOther, MY_DIR _eDir, float _fDiffCX, float _fDiffCY)
{
	if (m_pParent)
		m_pParent->OnCollisionStay(_pOther, _eDir, _fDiffCX, _fDiffCY);
}
