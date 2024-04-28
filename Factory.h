#pragma once

#include "Obj.h"

/// <summary>
///
/// </summary>
/// <typeparam name="T">Derived 클래스,</typeparam>
template<typename T>
class CFactory
{
public:
	CFactory() = default;
	~CFactory() = default;

public:
	/**
	 * \brief 인스턴스 동적 생성\n
	 * 내부에서 initialize 실행
	 * \return T 바탕의 인스턴스
	 */
	static CObj* Create()
	{
		CObj* pTmp = new T;
		pTmp->Initialize();

		return pTmp;
	}
	
	static CObj* Create(float _x, float _y)
	{
		CObj* pTmp = new T;
		pTmp->Initialize();
		pTmp->Set_Pos(_x, _y);

		return pTmp;
	}

};