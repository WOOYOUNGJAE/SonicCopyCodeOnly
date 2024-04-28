#pragma once
#include "Include.h"
class CObj;
class CMAIN
{
public:
	CMAIN();
	~CMAIN();

public:
	void Initialize();
	void Update();
	void LateUpdate();
	void Render();
	void Release();

private:
	HDC m_dc;
	DWORD			m_dwTime;
	int				m_iFPS;
	TCHAR			m_szFPS[32];

	HDC m_subDC;
};

