#pragma once
class CMyString
{
public:
	CMyString();
	~CMyString();

public:
	static const TCHAR* FloatToString3(const TCHAR* _szStr, float _f)
	{
		TCHAR strDst[32]{};
		TCHAR strSrc[32]{};
		lstrcpy(strDst, _szStr);
		swprintf_s(strSrc, L"%f", _f);
		//return lstrcat(strDst, strSrc);
		lstrcat(strDst, strSrc);
		return strDst;
	}

	/**
	 * \brief 
	 * \param _Dst 저장할 그릇
	 * \param _szStr 인풋
	 * \param _f 합할 수
	 * \return 
	 */
	static void FloatToString2(TCHAR** _Dst, const TCHAR* _szStr, float _f)
	{
		TCHAR strDst[32]{};
		TCHAR strSrc[32]{};
		lstrcpy(strDst, _szStr);
		swprintf_s(strSrc, L"%f", _f);
		//return lstrcat(strDst, strSrc);
		lstrcat(strDst, strSrc);
		lstrcpy(*_Dst, strDst);
	}
	static wstring FloatToString(wstring _str, float _f)
	{/*
		TCHAR strSrc[32]{};
		swprintf_s(strSrc, L"%f", _f);*/
		wstring strResult = _str + std::to_wstring(_f);
		return strResult;
	}
	

	static const TCHAR* FloatToString(float _f)
	{
		TCHAR strTmp[32]{};
		swprintf_s(strTmp, L"%f", _f);
		return strTmp;
	}
};

