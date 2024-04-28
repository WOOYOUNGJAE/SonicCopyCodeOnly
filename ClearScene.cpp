#include "stdafx.h"
#include "ClearScene.h"

#include "Factory.h"
#include "SceneManager.h"
#include "SignObj.h"
#include "SoundManager.h"
#include "UIManager.h"

CClearScene::CClearScene() : m_bWaitToChange(false), m_dwWaitStartTime(0), m_dwWaitAccTime(8000)
{
}

CClearScene::~CClearScene()
{
}

void CClearScene::Initialize()
{
	// Load Data

	// Add Object
	CObj* pTestSign = CFactory<CSignObj>::Create(WINCX * 0.1f, -100);
	CObjManager::Get_Instance()->AddObject(ID_OTHER, pTestSign);


	CSoundManager::Get_Instance()->StopSound(SOUND_BGM);
	CSoundManager::Get_Instance()->PlaySoundW(L"ClearBgm.wav", SOUND_MONSTER, 1.f);
}

void CClearScene::Update()
{
	CSceneBase::Update();

	if (m_bWaitToChange)
	{
		if (m_dwWaitStartTime + m_dwWaitAccTime < GetTickCount())
		{
			CSceneManager::Get_Instance()->ChangeScene();
			m_bWaitToChange = false;
		}
	}
}

void CClearScene::LateUpdate()
{
	CSceneBase::LateUpdate();
}

void CClearScene::Render(HDC _dc)
{
	CSceneBase::Render(_dc);
}

void CClearScene::Release()
{
}