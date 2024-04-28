#include "stdafx.h"
#include "CollisionManager.h"

#include <memory>

CCollisionManager* CCollisionManager::m_pInstance = nullptr;

CCollisionManager::CCollisionManager()
{
}

CCollisionManager::~CCollisionManager()
{
}

void CCollisionManager::RectCollisionStay(list<CObj*> _Dst, list<CObj*> _Src)
{
	float fDiffCX = 0;
	float fDiffCY = 0;

	for (auto& Dst : _Dst)
	{
		for (auto& Src : _Src)
		{
			if (CheckRect(Dst, Src, &fDiffCX, &fDiffCY))
			{
				// ���� �浹�� ��
				if (fDiffCX > fDiffCY)
				{
					// �� �浹
					if (Dst->Get_Pos().fY < Src->Get_Pos().fY)
					{
						Dst->OnCollisionStay(Src, DIR_DOWN, fDiffCX, fDiffCY);
						Src->OnCollisionStay(Dst, DIR_UP, fDiffCX, fDiffCY);
					}
					else
					{
						Dst->OnCollisionStay(Src, DIR_UP, fDiffCX, fDiffCY);
						Src->OnCollisionStay(Dst, DIR_DOWN, fDiffCX, fDiffCY);
					}
				}
				else
				{
					if (Dst->Get_Pos().fX < Src->Get_Pos().fX) // ���浹
					{
						Dst->OnCollisionStay(Src, DIR_RIGHT, fDiffCX, fDiffCY);
						Src->OnCollisionStay(Dst, DIR_LEFT, fDiffCX, fDiffCY);
					}
					else
					{
						Dst->OnCollisionStay(Src, DIR_LEFT, fDiffCX, fDiffCY);
						Src->OnCollisionStay(Dst, DIR_RIGHT, fDiffCX, fDiffCY);
					}
				}
			}
		}
	}
}

void CCollisionManager::RectCollisionEnter(list<CObj*> _Dst, list<CObj*> _Src)
{
	float fDiffCX = 0;
	float fDiffCY = 0;

	for (auto& Dst : _Dst)
	{
		for (auto& Src : _Src)
		{
			//list<set<CObj*, CObj*>>::iterator iter = 
			auto iter = find(m_CollisionEnterList.begin(), m_CollisionEnterList.end(),
				pair<CObj*, CObj*>(Dst, Src));

			// ã������, �̹� �浹������ ����
			if (iter != m_CollisionEnterList.end())
				return;

			if (CheckRect(Dst, Src, &fDiffCX, &fDiffCY))
			{
				// ���� �浹�� ��
				if (fDiffCX > fDiffCY)
				{
					// �� �浹
					if (Dst->Get_Pos().fY < Src->Get_Pos().fY)
					{
						Dst->OnCollisionEnter(Src, DIR_DOWN, fDiffCX, fDiffCY);
						Src->OnCollisionEnter(Dst, DIR_UP, fDiffCX, fDiffCY);
					}
					else
					{
						Dst->OnCollisionEnter(Src, DIR_UP, fDiffCX, fDiffCY);
						Src->OnCollisionEnter(Dst, DIR_DOWN, fDiffCX, fDiffCY);
					}
				}
				else
				{
					if (Dst->Get_Pos().fX < Src->Get_Pos().fX) // ���浹
					{
						Dst->OnCollisionEnter(Src, DIR_RIGHT, fDiffCX, fDiffCY);
						Src->OnCollisionEnter(Dst, DIR_LEFT, fDiffCX, fDiffCY);
					}
					else
					{
						Dst->OnCollisionEnter(Src, DIR_LEFT, fDiffCX, fDiffCY);
						Src->OnCollisionEnter(Dst, DIR_RIGHT, fDiffCX, fDiffCY);
					}
				}

				m_CollisionEnterList.emplace_back(pair<CObj*, CObj*>(Dst, Src));
			}
		}
	}
}

void CCollisionManager::RectCollisionExit(list<CObj*> _Dst, list<CObj*> _Src)
{
	float fDiffCX = 0;
	float fDiffCY = 0;

	for (auto& Dst : _Dst)
	{
		for (auto& Src : _Src)
		{
			auto iter = find(m_CollisionEnterList.begin(), m_CollisionEnterList.end(),
				pair<CObj*, CObj*>(Dst, Src));

			// ��ã������,  ����
			if (iter == m_CollisionEnterList.end())
				continue;;

			// �浹 ��������
			if (CheckRect(Dst, Src, &fDiffCX, &fDiffCY) == false)
			{
				Dst->OnCollisionExit(Src);
				Src->OnCollisionExit(Dst);

				m_CollisionEnterList.erase(iter);
			}
		}
	}
}

bool CCollisionManager::CheckRect(CObj* _pDst, CObj* _pSrc, float* pDiffCX, float* pDiffCY)
{
	float fWidth = fabs(_pDst->Get_Pos().fX - _pSrc->Get_Pos().fX);
	float fHeight = fabs(_pDst->Get_Pos().fY - _pSrc->Get_Pos().fY);

	float fRadiusX = (_pDst->Get_Scale().fX + _pSrc->Get_Scale().fX) * 0.5f;
	float fRadiusY = (_pDst->Get_Scale().fY + _pSrc->Get_Scale().fY) * 0.5f;

	if ((fRadiusX >= fWidth) && (fRadiusY >= fHeight))
	{
		*pDiffCX = fRadiusX - fWidth;
		*pDiffCY = fRadiusY - fHeight;

		return  true;
	}

	return false;
}
