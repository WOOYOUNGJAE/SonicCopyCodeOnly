#pragma once
#include "Include.h"

class CKeyManager
{
public:
	CKeyManager();
	~CKeyManager();

public:
	static CKeyManager* Get_Instance()
	{
		if (!m_pInstance)
			m_pInstance = new CKeyManager;

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

	bool Key_Pressing(int _iKey);
	bool Key_Down(int _iKey);
	bool Key_Up(int _iKey);

	bool InputNothing();

	void ResetAllHorizonAxis() {
		m_fHorizonAxis = 0.f;
		m_fHorizonAxis2 = 0.f;
		m_fHorizonAxis3 = 0.f;
	}
	void RestHorizontalAxis(int _iNum)
	{
		switch (_iNum)
		{
		case 0:
			m_fHorizonAxis = 0;
			return;
		case 1:
			m_fHorizonAxis2 = 0;
			return;

		case 2:
			m_fHorizonAxis3 = 0;
			return;
		}
	}
	/**
	 * \brief 누르고 있는 시간에 따라 증가, 최대 1, 최소 -1
	 * \param _iKey 
	 * \param _fDeltaSpeed : 0.0X, 매우 작은 수
	 * \return -1 to 1
	 */
	float Get_Axis_Horizontal(int _iKey, float _fDeltaSpeed);
	float Get_Axis_Horizontal2(int _iKey, float _fDeltaSpeed);
	float Get_Axis_Horizontal3(int _iKey, float _fDeltaSpeed);
private:
	static CKeyManager* m_pInstance;

	bool m_bKeyActiveArr[VK_MAX];
	float m_fHorizonAxis; // -1.f ~ 0.0f ~ 1.0f
	float m_fHorizonAxis2; // -1.f ~ 0.0f ~ 1.0f
	float m_fHorizonAxis3; // -1.f ~ 0.0f ~ 1.0f



	bool			m_bKeyState[VK_MAX];
};

