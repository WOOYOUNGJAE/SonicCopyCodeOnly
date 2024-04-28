#include "stdafx.h"
#include "KeyManager.h"

CKeyManager* CKeyManager::m_pInstance = nullptr;

CKeyManager::CKeyManager() : m_fHorizonAxis(0.f), m_fHorizonAxis2(0.f), m_fHorizonAxis3(0.f)
{
	//ZeroMemory(m_bKeyActiveArr, VK_MAX);
	memset(m_bKeyState, 0, sizeof(m_bKeyState));
	memset(m_bKeyActiveArr, 0, sizeof(m_bKeyActiveArr));
}

CKeyManager::~CKeyManager()
{
}

bool CKeyManager::Key_Pressing(int _iKey)
{
	if ((GetAsyncKeyState(_iKey) & 0x8000))
		return true;

	return false;
}

bool CKeyManager::Key_Down(int _iKey)
{
	// 배열에는 안눌려있는 상태였는데 실제로 눌렀으면
	//if (!m_bKeyActiveArr[_iKey] && (GetAsyncKeyState(_iKey & 0x8000)))
	//{
	//	m_bKeyActiveArr[_iKey] = !m_bKeyActiveArr[_iKey];
	//	//m_bKeyActiveArr[_iKey] = true;
	//	return true;
	//}

	//for (int i = 0; i < VK_MAX; ++i)
	//{
	//	if (m_bKeyActiveArr[i] && !(GetAsyncKeyState(i) & 0x8000))
	//		m_bKeyActiveArr[i] = !m_bKeyActiveArr[i];
	//		//m_bKeyActiveArr[i] = false;
	//}

	//return false;

	if (!m_bKeyState[_iKey] && (GetAsyncKeyState(_iKey) & 0x8000))
	{
		m_bKeyState[_iKey] = !m_bKeyState[_iKey];
		return true;
	}

	for (int i = 0; i < VK_MAX; ++i)
	{
		if (m_bKeyState[i] && !(GetAsyncKeyState(i) & 0x8000))
			m_bKeyState[i] = !m_bKeyState[i];
	}

	return false;
}

bool CKeyManager::Key_Up(int _iKey)
{
	/*if (m_bKeyActiveArr[_iKey] && !(GetAsyncKeyState((_iKey) & 0x8000)))
	{
		m_bKeyActiveArr[_iKey] = false;
		return true;
	}

	for (int i = 0; i < VK_MAX; ++i)
	{
		if (!m_bKeyActiveArr[i] && ((GetAsyncKeyState(i) & 0x8000)))
			m_bKeyActiveArr[i] = true;
	}

	return false;*/

	if (m_bKeyState[_iKey] && !(GetAsyncKeyState(_iKey) & 0x8000))
	{
		m_bKeyState[_iKey] = !m_bKeyState[_iKey];
		return true;
	}

	for (int i = 0; i < VK_MAX; ++i)
	{
		if (!m_bKeyState[i] && (GetAsyncKeyState(i) & 0x8000))
			m_bKeyState[i] = !m_bKeyState[i];
	}

	return false;
}

bool CKeyManager::InputNothing()
{
	for (int i = 0; i < VK_MAX; ++i)
	{
		if (GetAsyncKeyState(i) & 0x8000)
		{
			return false;
		}
	}
	return true;
}

float CKeyManager::Get_Axis_Horizontal(int _iKey, float _fDeltaSpeed)
{
	if (_iKey == VK_NOINPUT)
	{
		if (m_fHorizonAxis > 0.02f)
		{
			m_fHorizonAxis -= _fDeltaSpeed * 0.5f;
		}
		else if (m_fHorizonAxis < -0.02f)
		{
			m_fHorizonAxis += _fDeltaSpeed * 0.5f;
		}

		//m_fHorizonAxis = 0.f;
		return m_fHorizonAxis;
	}

	if (Key_Pressing(_iKey))
	{
		if (_iKey == VK_RIGHT)
		{
			if (m_fHorizonAxis >= 0) // 이미 같은방향이면
				m_fHorizonAxis += _fDeltaSpeed;
			else // 방향 꺾었으면 좀 더 빨리 회복하도록 보정
				m_fHorizonAxis += _fDeltaSpeed * 1000;

			//m_fHorizonAxis += _fDeltaSpeed;
		}
		else if (_iKey == VK_LEFT)
		{
			if (m_fHorizonAxis <= 0) // 이미 같은방향이면
				m_fHorizonAxis -= _fDeltaSpeed;
			else // 방향 꺾었으면
				m_fHorizonAxis -= _fDeltaSpeed * 1000;
			
			//m_fHorizonAxis -= _fDeltaSpeed;
		}
		CMyMath::fClamp(m_fHorizonAxis, -1.f, 1.f);
	}
	return m_fHorizonAxis;
}

float CKeyManager::Get_Axis_Horizontal2(int _iKey, float _fDeltaSpeed)
{
	if (_iKey == VK_NOINPUT)
	{
		if (m_fHorizonAxis2 > 0.02f)
		{
			m_fHorizonAxis2 -= _fDeltaSpeed * 0.5f;
		}
		else if (m_fHorizonAxis2 < -0.02f)
		{
			m_fHorizonAxis2 += _fDeltaSpeed * 0.5f;
		}

		//m_fHorizonAxis = 0.f;
		return m_fHorizonAxis2;
	}

	if (Key_Pressing(_iKey))
	{
		if (_iKey == VK_RIGHT)
		{
			if (m_fHorizonAxis2 >= 0) // 이미 같은방향이면
				m_fHorizonAxis2 += _fDeltaSpeed;
			else // 방향 꺾었으면 좀 더 빨리 회복하도록 보정
				m_fHorizonAxis2 += _fDeltaSpeed * 100;

			//m_fHorizonAxis += _fDeltaSpeed;
		}
		else if (_iKey == VK_LEFT)
		{
			if (m_fHorizonAxis2 <= 0) // 이미 같은방향이면
				m_fHorizonAxis2 -= _fDeltaSpeed;
			else // 방향 꺾었으면
				m_fHorizonAxis2 -= _fDeltaSpeed * 100;

			//m_fHorizonAxis -= _fDeltaSpeed;
		}
		CMyMath::fClamp(m_fHorizonAxis2, -1.f, 1.f);
	}
	return m_fHorizonAxis2;
}

float CKeyManager::Get_Axis_Horizontal3(int _iKey, float _fDeltaSpeed)
{
	if (_iKey == VK_NOINPUT)
	{
		if (m_fHorizonAxis3 > 0.02f)
		{
			m_fHorizonAxis3 -= _fDeltaSpeed * 0.5f;
		}
		else if (m_fHorizonAxis3 < -0.02f)
		{
			m_fHorizonAxis3 += _fDeltaSpeed * 0.5f;
		}

		//m_fHorizonAxis = 0.f;
		return m_fHorizonAxis3;
	}

	if (Key_Pressing(_iKey))
	{
		if (_iKey == VK_RIGHT)
		{
			if (m_fHorizonAxis3 >= 0) // 이미 같은방향이면
				m_fHorizonAxis3 += _fDeltaSpeed;
			else // 방향 꺾었으면 좀 더 빨리 회복하도록 보정
				m_fHorizonAxis3 += _fDeltaSpeed * 100;

			//m_fHorizonAxis += _fDeltaSpeed;
		}
		else if (_iKey == VK_LEFT)
		{
			if (m_fHorizonAxis3 <= 0) // 이미 같은방향이면
				m_fHorizonAxis3 -= _fDeltaSpeed;
			else // 방향 꺾었으면
				m_fHorizonAxis3 -= _fDeltaSpeed * 100;

			//m_fHorizonAxis -= _fDeltaSpeed;
		}
		CMyMath::fClamp(m_fHorizonAxis3, -1.f, 1.f);
	}
	return m_fHorizonAxis3;
}
