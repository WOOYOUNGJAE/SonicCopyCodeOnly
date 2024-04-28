#include "stdafx.h"
#include "BossScene.h"

#include "Factory.h"
#include "ItemObj.h"
#include "MechaSonic.h"
#include "PlayerBase.h"
#include "Ring.h"
#include "UIManager.h"
#include "Wall.h"


CBossScene::CBossScene()
{
}

CBossScene::~CBossScene()
{
}

void CBossScene::Initialize()
{
	CScrollManager::Get_Instance()->Set_Fixed(true);

	CObj* pTmp = CFactory<CPlayerBase>::Create(WINCX * 0.5f, 100);
	CObjManager::Get_Instance()->AddObject(ID_PLAYER, pTmp);
	CLineManager::Get_Instance()->LoadData_Connected(L"./Data/ConnectedBoss2_1.dat");
	CLineManager::Get_Instance()->InitializeAllConnectedLIne();

	CObj* pTestBoss = CFactory<CMechaSonic>::Create(WINCX - 50, 0);
	CObjManager::Get_Instance()->AddObject(ID_MECHASONIC, pTestBoss);

	// left
	CObj* pTestWall = CFactory<CWall>::Create(-WINCX * 0.05f, WINCY * 0.5f);
	CObjManager::Get_Instance()->AddObject(ID_WALL, pTestWall);
	dynamic_cast<CWall*>(pTestWall)->WallInit(WINCX * 0.1f, WINCY);
	//right
	pTestWall = CFactory<CWall>::Create(WINCX * 1.05f, WINCY * 0.5f);
	CObjManager::Get_Instance()->AddObject(ID_WALL, pTestWall);
	dynamic_cast<CWall*>(pTestWall)->WallInit(WINCX * 0.1f, WINCY);
	//up
	pTestWall = CFactory<CWall>::Create(WINCX * 0.5f, -WINCY * 0.05f);
	CObjManager::Get_Instance()->AddObject(ID_WALL, pTestWall);
	dynamic_cast<CWall*>(pTestWall)->WallInit(WINCX, WINCY * 0.1f);
	//down
	

}

void CBossScene::Update()
{
	CTileManager::Get_Instance()->Update();
	CObjManager::Get_Instance()->Update();
	CLineManager::Get_Instance()->Update();
}

void CBossScene::LateUpdate()
{
	CObjManager::Get_Instance()->LateUpdate();
}

void CBossScene::Render(HDC _dc)
{
	CTileManager::Get_Instance()->Render(_dc);
	CLineManager::Get_Instance()->Render(_dc);
	CObjManager::Get_Instance()->Render(_dc);
	CUIManager::Get_Instance()->Render(_dc);
}

void CBossScene::Release()
{
}
