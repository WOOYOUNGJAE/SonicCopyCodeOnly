#pragma once

#include "Obj.h"

/// <summary>
///
/// </summary>
/// <typeparam name="T">Derived Ŭ����,</typeparam>
template<typename T>
class CFactory
{
public:
	CFactory() = default;
	~CFactory() = default;

public:
	/**
	 * \brief �ν��Ͻ� ���� ����\n
	 * ���ο��� initialize ����
	 * \return T ������ �ν��Ͻ�
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