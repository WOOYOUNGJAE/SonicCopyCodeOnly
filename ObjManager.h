#pragma once
#include "Obj.h"

class CObjManager
{
public:
	CObjManager();
	~CObjManager();

public:
	static CObjManager* Get_Instance()
	{
		if (!m_pInstance)
			m_pInstance = new CObjManager;

		return m_pInstance;
	}

	static void		Destroy_Instance(void)
	{
		if (m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}

public:
	// getter
	CObj* Get_Player() { return m_ObjList[ID_PLAYER].front(); }
	list<CObj*>* Get_ObjList() { return m_ObjList; }

	// setter
	void Set_ShakeStart() { m_dwShakeStartTime = GetTickCount(); }

	void Initialize();
	void Update();
	void LateUpdate();
	void Render(HDC _dc);
	void Release();

	void AddObject(OBJ_ID _eID, CObj* _pObj);

	void Load_ObjData(OBJ_ID _eID, const TCHAR* _path);

	void ShakeWalls();
	void DropRings();
private:
	static  CObjManager* m_pInstance;

	list<CObj*> m_ObjList[OBJ_ID_END];
	list<CObj*> m_ObjPostRenderList;

	DWORD m_dwShakeStartTime;
	DWORD m_dwShakeAccTime;
};

