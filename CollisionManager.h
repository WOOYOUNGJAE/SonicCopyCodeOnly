#pragma once
#include "Obj.h"
#include "Collider.h"

class CCollisionManager
{
private:
	CCollisionManager();
	~CCollisionManager();

public:
	static CCollisionManager* Get_Instance(void)
	{
		if (nullptr == m_pInstance)
			m_pInstance = new CCollisionManager;

		return m_pInstance;
	}
	void	Destroy_Instance(void)
	{
		if (nullptr != m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}

	void RectCollisionStay(list<CObj*> _Dst, list<CObj*> _Src);
	void RectCollisionEnter(list<CObj*> _Dst, list<CObj*> _Src);
	void RectCollisionExit(list<CObj*> _Dst, list<CObj*> _Src);

	bool CheckRect(CObj* _pDst, CObj* _pSrc, float* pDiffCX, float* pDiffCY);

	
private:
	static CCollisionManager* m_pInstance;
	list<pair<CObj*, CObj*>> m_CollisionEnterList;

};

