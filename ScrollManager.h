#pragma once
class CScrollManager
{
private:
	CScrollManager();
	~CScrollManager() = default;

public:
	// SingleTon
	static CScrollManager* Get_Instance()
	{
		if (m_pInstance == nullptr)
		{
			m_pInstance = new CScrollManager;
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

	// getter
	float Get_ScrollX() const { return m_fScrollX; }
	float Get_ScrollY() const { return m_fScrollY; }
	float Get_IfFixed() const { return m_bScrollFixed; }
	// setter
	void Set_ScrollXPlus(float _fInput)
	{
		m_fScrollX += _fInput;
	}
	void Set_ScrollYPlus(float _fInput)
	{
		m_fScrollY += _fInput;
	}

	void Set_Fixed(bool _b) { m_bScrollFixed = _b; }

	void Initiailize();
	void Update();


	void ResetScroll()
	{
		m_fScrollX = 0.f;
		m_fScrollY = 0.f;
	}

private:
	static CScrollManager* m_pInstance;
	bool m_bScrollFixed;
	float m_fScrollX;
	float m_fScrollY;
};

