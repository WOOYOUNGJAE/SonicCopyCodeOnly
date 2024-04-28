#pragma once
class CDebugManager
{
public:
	CDebugManager();
	~CDebugManager();

public:
	// SingleTon
	static CDebugManager* Get_Instance()
	{
		if (m_pInstance == nullptr)
		{
			m_pInstance = new CDebugManager;
		}
		return m_pInstance;
	}
	static void Destroy_Instance()
	{
		if (m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}

public:
	void Initialize();
	void Update();
	void LateUpdate();
	void Render(HDC _dc);
	void Release();
	void AddDebugLog(wstring _key, wstring _strInput);
	void ChangeDebugLog(wstring _key, wstring _strInput);
	void DeleteDebugLog(wstring _key);

	//void 
	void PrintLog(HDC _dc, int _iOrder, map<wstring, wstring>::iterator _iter);

private:
	static CDebugManager* m_pInstance;

	bool m_bDebugActive;
	int m_iInterval;
	/**
	 * \brief ¹®ÀÚ¿­ º¤ÅÍ
	 */
	map<wstring, wstring> m_mapLog;
};
