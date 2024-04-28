#pragma once
#include "Obj.h"
class CTileManager
{
private:
	CTileManager();
	~CTileManager();

public:
	static CTileManager* Get_Instance(void)
	{
		if (nullptr == m_pInstance)
			m_pInstance = new CTileManager;

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

	void Initialize();
	void Update();
	void LateUpdate();
	void Render(HDC _dc);
	void Release();

	void Pick(POINT _pt, int _iDrawID, int _iOption);
	void Pick(POINT _pt, int _iDrawIDX, int _iDrawIDY, int _iOption);
	void SaveTile_Mushroom(const TCHAR* _path);
	void LoadTile_Mushroom(const TCHAR* _path);

	void CreateDefaultMushroom();


private:
	static CTileManager* m_pInstance;
	vector<CObj*> m_vecMushroomTile;
};

