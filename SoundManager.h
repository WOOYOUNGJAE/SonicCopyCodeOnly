#pragma once
#include "Include.h"

class CSoundManager
{
private:
	CSoundManager();
	~CSoundManager();

public:
	static CSoundManager* Get_Instance()
	{
		if (!m_pInstance)
			m_pInstance = new CSoundManager;

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

	void Initialize();
	void Release();
//
	void PlaySound(const TCHAR* pSoundKey, CHANNELID eID, float fVolume);
	void PlayBGM(const TCHAR* pSoundKey, float fVolume);
	void StopSound(CHANNELID eID);
	void StopAll();
	void SetChannelVolume(CHANNELID eID, float fVolume);

	void LoadSoundFile();
private:
	static CSoundManager* m_pInstance;


	// ���� ���ҽ� ������ ���� ��ü 
	map<TCHAR*, FMOD_SOUND*> m_mapSound;

	// FMOD_CHANNEL : ����ϰ� �ִ� ���带 ������ ��ü 
	FMOD_CHANNEL* m_pChannelArr[MAXCHANNEL];

	// ���� ,ä�� ��ü �� ��ġ�� �����ϴ� ��ü 
	FMOD_SYSTEM* m_pSystem;
};

