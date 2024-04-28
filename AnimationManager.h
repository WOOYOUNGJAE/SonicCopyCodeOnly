#pragma once
class CAnimationManager
{
private:
	CAnimationManager();
	~CAnimationManager();

public:
	static CAnimationManager* Get_Instance(void)
	{
		if (nullptr == m_pInstance)
			m_pInstance = new CAnimationManager;

		return m_pInstance;
	}
	void	Destroy_Instance(void)
	{
		if (nullptr != m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}

	void Initialize();
private:
	static CAnimationManager* m_pInstance;
};

