#include "stdafx.h"
#include "TestScene.h"
#include "BmpManager.h"
#include "DiagonalSpring.h"
#include "Factory.h"
#include "GameStateManager.h"
#include "ItemObj.h"
#include "LineObjBase.h"
#include "PlayerBase.h"
#include "QuarterCurveLine.h"
#include "Spring.h"
#include "Wall.h"
#include "Ring.h"
#include "Spike.h"
#include "UIManager.h"

CTestScene::CTestScene() : t(0.f), m_fPlayer_xPos(0.f), fIndex(0.f), m_dwRemainStart(0),
m_dwRemainAcc(5000), m_bRemainTick(false)
{
}

CTestScene::~CTestScene()
{
}

void CTestScene::Initialize()
{
	CBmpManager::Get_Instance()->Insert_Bmp(L"Start1",
		L"./Resources/Images/UI/Start1.bmp");
	m_pFrameKey = L"Start1";
	m_anim.Initialize(m_pFrameKey);
	m_anim.Set_FrameSize(400, 300);
	m_anim.Get_FrameInfo().iFrameStart = 0;
	m_anim.Get_FrameInfo().iFrameEnd = 2;
	m_anim.Get_FrameInfo().iMotion = 0;
	m_anim.Get_FrameInfo().dwInterval = 100.f;

	CTileManager::Get_Instance()->LoadTile_Mushroom(L"./Data/Tile_Stage1_1.dat");
	CObjManager::Get_Instance()->Load_ObjData(ID_SPRING, L"./Data/Obj_SpringStage2_0.dat");
	CObjManager::Get_Instance()->Load_ObjData(ID_DIAGONALSPRING, L"./Data/Obj_DiagonalSpringStage2_0.dat");
	CObjManager::Get_Instance()->Load_ObjData(ID_SPIKE, L"./Data/Obj_SpikeStage2_0.dat");

	CObjManager::Get_Instance()->Load_ObjData(ID_WALL, L"./Data/Obj_WallStage2_2.dat");
	CObjManager::Get_Instance()->Load_ObjData(ID_RING, L"./Data/Obj_RingStage2_4.dat");
	CObjManager::Get_Instance()->Load_ObjData(ID_ITEM, L"./Data/Obj_ItemStage2_0.dat");

	CLineManager::Get_Instance()->LoadData_Circle(L"./Data/CircleStage2_2.dat");
 	CLineManager::Get_Instance()->LoadData_Connected(L"./Data/ConnectedStage2_2.dat");

 	//CLineManager::Get_Instance()->LoadData_Connected(L"./Data/ConnectedFinalTest.dat");



	CLineManager::Get_Instance()->InitializeAllConnectedLIne();

	

	//CObj* pTestWall =  CFactory<CWall>::Create(400,100);
	//CObjManager::Get_Instance()->AddObject(ID_WALL, pTestWall);
	//dynamic_cast<CWall*>( pTestWall)->WallInit(100, 100);

	CObj* pTmp = CFactory<CPlayerBase>::Create(181, 100);
	CObjManager::Get_Instance()->AddObject(ID_PLAYER, pTmp);

	// 238 124
	//pTmp = new CItemObj;
	pTmp = CFactory<CItemObj>::Create(240, 111);
	CObjManager::Get_Instance()->AddObject(ID_ITEM, pTmp);
	dynamic_cast<CItemObj*>(pTmp)->Set_ItemID(0);
	pTmp = CFactory<CItemObj>::Create(300, 111);
	CObjManager::Get_Instance()->AddObject(ID_ITEM, pTmp);
	dynamic_cast<CItemObj*>(pTmp)->Set_ItemID(1);
	pTmp = CFactory<CItemObj>::Create(360, 111);
	CObjManager::Get_Instance()->AddObject(ID_ITEM, pTmp);
	dynamic_cast<CItemObj*>(pTmp)->Set_ItemID(2);
	pTmp = CFactory<CItemObj>::Create(420, 111);
	CObjManager::Get_Instance()->AddObject(ID_ITEM, pTmp);
	dynamic_cast<CItemObj*>(pTmp)->Set_ItemID(3);
	pTmp = CFactory<CItemObj>::Create(480, 111);
	CObjManager::Get_Instance()->AddObject(ID_ITEM, pTmp);
	dynamic_cast<CItemObj*>(pTmp)->Set_ItemID(4);




	/*CObj* pTestSpring = CFactory<CSpring>::Create(500, 130);
	CObjManager::Get_Instance()->AddObject(ID_SPRING, pTestSpring);
	dynamic_cast<CSpring*>(pTestSpring)->SpringInit(DIR_LEFT);*/

	/*CObj* pTestSpring = CFactory<CDiagonalSpring>::Create(500, 120);
	CObjManager::Get_Instance()->AddObject(ID_DIAGONALSPRING, pTestSpring);
	dynamic_cast<CDiagonalSpring*>(pTestSpring)->SpringInit(DIR_LU);*/

	//CObj* pTestRing = CFactory<CRing>::Create(300, 130);
	//CObjManager::Get_Instance()->AddObject(ID_RING, pTestRing);


	// quarter line test
	//CLineObjBase* pTmpLine = new CQuarterCurveLine;	
	//dynamic_cast<CQuarterCurveLine*>(pTmp)->Initialize(300, 100, 50, DIR_RU);
}

void CTestScene::Update()
{
	CTileManager::Get_Instance()->Update();
	CObjManager::Get_Instance()->Update();
	CLineManager::Get_Instance()->Update();
}

void CTestScene::LateUpdate()
{
	CObjManager::Get_Instance()->LateUpdate();
	CLineManager::Get_Instance()->LateUpdate();

	FrameInfo& refFrameInfo = m_anim.Get_FrameInfo();

	if (refFrameInfo.dwTime + refFrameInfo.dwInterval <
		GetTickCount())
	{
		// 드디어 TickCount가 따라와줬으면
		++refFrameInfo.iFrameStart; // 가로 인덱스 틱
		refFrameInfo.dwTime = GetTickCount(); // dwTime업데이트

		// 마지막 애니메이션 프레임 진행했으면 인덱스 0으로

		if (refFrameInfo.iFrameStart > refFrameInfo.iFrameEnd)
		{
			if (refFrameInfo.iMotion != 4)
			{
				++refFrameInfo.iMotion;
				refFrameInfo.iFrameStart = 0;
			}
			else
			{
				if (m_bRemainTick == false)
				{
					m_bRemainTick = true;
					m_dwRemainStart = GetTickCount();
				}
				--refFrameInfo.iFrameStart;
				
			}

		}
	}
}

void CTestScene::Render(HDC _dc)
{
	/*Rectangle(_dc, 0, 0, 400, 400);

	fIndex += 1.f;
	if (fIndex >= 50)
		fIndex = 0.f;

	Ellipse(_dc, dots0[(int)(fIndex)]->fX, dots0[(int)(fIndex)]->fY,
		dots0[(int)(fIndex)]->fX + 10, dots0[(int)(fIndex)]->fY + 10);*/

	CTileManager::Get_Instance()->Render(_dc);
	CLineManager::Get_Instance()->Render(_dc);
	CObjManager::Get_Instance()->Render(_dc);
	CUIManager::Get_Instance()->Render(_dc);

	if (m_bRemainTick && m_dwRemainStart + m_dwRemainAcc < GetTickCount())
	{
		m_pFrameKey = L"NULL";
		m_anim.Set_FrameSize(400, 300);
		m_anim.Get_FrameInfo().iFrameStart = 0;
		m_anim.Get_FrameInfo().iFrameEnd = 0;
		m_anim.Get_FrameInfo().iMotion = 0;
		m_anim.Get_FrameInfo().dwInterval = 200.f;
	}

	if (m_pFrameKey != L"NULL")
		m_anim.Render(_dc);


	CGameStateManager::Get_Instance()->Render(_dc);
}

void CTestScene::Release()
{
}
