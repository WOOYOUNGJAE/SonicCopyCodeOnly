#include "stdafx.h"
#include "SceneManager.h"
#include "ObjManager.h"
#include "BmpManager.h"
#include "BossScene.h"
#include "ClearScene.h"
#include "TestScene.h"
#include "EditorScene.h"
#include "Factory.h"
#include "GameStateManager.h"
#include "IntroScene.h"
#include "KeyManager.h"
#include "Player.h"
#include "PlayerBase.h"
#include "SoundManager.h"
#include "UIManager.h"

CSceneManager* CSceneManager::m_pInstance = nullptr;

CSceneManager::CSceneManager() : m_iCurrentScene(0),
m_iNextScene(1), m_iPrevScene(0)
{
	m_vecScene.reserve(STAGE_END);
}

CSceneManager::~CSceneManager()
{
	Release();
}

void CSceneManager::Initialize()
{
	// Push back scenes
	m_vecScene.push_back(new CTestScene);
	m_vecScene.push_back(new CBossScene);
	m_vecScene.push_back(new CIntroScene);
	m_vecScene.push_back(new CClearScene);
	m_vecScene.push_back(new CEditorScene);
	m_vecScene[m_iCurrentScene]->Initialize();

	// Background Image
	CBmpManager::Get_Instance()->Insert_Bmp(L"BackTemp0", 
L"./Resources/Images/Background/ForestBackTemp0.bmp"	);
	CBmpManager::Get_Instance()->Insert_Bmp(L"BossStage", 
L"./Resources/Images/Background/BossStage.bmp"	);
	CBmpManager::Get_Instance()->Insert_Bmp(L"BossBackground", 
L"./Resources/Images/Background/BossBackground.bmp"	);
	CBmpManager::Get_Instance()->Insert_Bmp(L"BossStageFloor", 
L"./Resources/Images/Background/BossStageFloor.bmp"	);

	CSoundManager::Get_Instance()->PlayBGM(L"Angel_Island.wav", SOUND_BGM);
	//CSoundManager::Get_Instance()->PlayBGM(L"BossBgm.wav", SOUND_BGM);
	//CSoundManager::Get_Instance()->PlayBGM(L"MainMenuTheme.wav", SOUND_BGM);

	CGameStateManager::Get_Instance()->Initialize(); // init inventory
}

void CSceneManager::Update()
{
	if (CKeyManager::Get_Instance()->Key_Pressing(VK_CONTROL))
	{
		if (CKeyManager::Get_Instance()->Key_Down(VK_HOME) && 
			m_iCurrentScene < m_vecScene.size() -1 )
		{
			++m_iCurrentScene;
		}
		else if (CKeyManager::Get_Instance()->Key_Down(VK_END) && 
			m_iCurrentScene > 0)
		{
			--m_iCurrentScene;
		}
	}
	m_vecScene[m_iCurrentScene]->Update();

	if (CKeyManager::Get_Instance()->Key_Down(VK_F4))
		CUIManager::Get_Instance()->Set_InventoryOnOff();
}

void CSceneManager::LateUpdate()
{
	m_vecScene[m_iCurrentScene]->LateUpdate();
}

void CSceneManager::Render(HDC _dc)
{
	 //Background
	HDC hMemDC = CBmpManager::Get_Instance()->Find_Img(L"BackTemp0");
	//HDC hMemDC = CBmpManager::Get_Instance()->Find_Img(L"BossBackground");

	BitBlt(_dc,	// ������� ���� ��� dc
		0, 0, WINCX, WINCY, // ��Ʈ���� ���� ������
		hMemDC, // �����  �̹��� dc
		0, // ��Ʈ���� ��� ���� x��ǥ
		0, // ��Ʈ���� ��� ���� y��ǥ
		SRCCOPY); // ��� ȿ�� ����, SRCCOPY : ���� ������� ���

	//hMemDC = CBmpManager::Get_Instance()->Find_Img(L"BossStageFloor");

	//BitBlt(_dc,	// ������� ���� ��� dc
	//	0, WINCY - 39, WINCX, 39, // ��Ʈ���� ���� ������
	//	hMemDC, // �����  �̹��� dc
	//	0, // ��Ʈ���� ��� ���� x��ǥ
	//	0, // ��Ʈ���� ��� ���� y��ǥ
	//	SRCCOPY); // ��� ȿ�� ����, SRCCOPY : ���� ������� ���
	m_vecScene[m_iCurrentScene]->Render(_dc);

	
}

void CSceneManager::Release()
{
}

void CSceneManager::ChangeScene()
{
	m_vecScene[m_iCurrentScene]->Release();
	++m_iCurrentScene;
}
