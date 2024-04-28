#pragma once
#include "Include.h"
#include "SceneBase.h"

class CSceneManager
{
public:
	CSceneManager();
	~CSceneManager();
	
public:
	// SingleTon
	static CSceneManager* Get_Instance()
	{
		if (m_pInstance == nullptr)
		{
			m_pInstance = new CSceneManager;
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

	void ChangeScene();

private:
	static CSceneManager* m_pInstance;

	vector<CSceneBase*> m_vecScene;
	int m_iCurrentScene;
	int m_iNextScene;
	int m_iPrevScene;
};

