#pragma once
#include "stdafx.h"

class CDebugLog
{
public:
	DebugLog() {};
	~DebugLog() {};

public:



	void Update()
	{
		
	}
	static void Print0(HDC _dc, const TCHAR* _strInput)
	{
		TextOut(_dc, 10, 10, _strInput, lstrlen(_strInput));
	}
	static void Print1(HDC _dc, const TCHAR* _strInput)
	{
		TextOut(_dc, 10, 30, _strInput, lstrlen(_strInput));
	}
	static void Print2(HDC _dc, const TCHAR* _strInput)
	{
		TextOut(_dc, 10, 50, _strInput, lstrlen(_strInput));
	}
		
	static const TCHAR* Combine_Str(const TCHAR* _szStr, float _f)
	{
		TCHAR strDst[32] {};
		TCHAR strSrc[32] {};
		lstrcpy(strDst, _szStr);
		swprintf_s(strSrc, L"%f", _f);
		return lstrcat(strDst, strSrc);
	}

private:
	static CDebugLog* m_pInstance;

	TCHAR szBuff[32];
};

