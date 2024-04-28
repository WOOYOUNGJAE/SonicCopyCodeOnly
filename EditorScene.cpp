#include "stdafx.h"
#include "EditorScene.h"
#include "KeyManager.h"
#include "LineManager.h"
#include "FileManager.h"
#include "TileManager.h"
#include "DebugManager.h"
#include "Spring.h"
#include "UIManager.h"
#include "DiagonalSpring.h"
#include "Factory.h"
#include "MechaSonic.h"
#include "Wall.h"

CEditorScene::CEditorScene() : m_eEditMode(EDIT_END)
{
}

CEditorScene::~CEditorScene()
{
}

void CEditorScene::Initialize()
{
	CSceneBase::Initialize(); // resetScroll
	CDebugManager::Get_Instance()->AddDebugLog(L"EditMode", L"Edit Mode : ");

	m_eEditMode = EDIT_LINE;

	//CTileManager::Get_Instance()->CreateDefaultMushroom();


	CTileManager::Get_Instance()->LoadTile_Mushroom(L"./Data/Tile_Stage1_1.dat");
	CLineManager::Get_Instance()->LoadData_Connected(L"./Data/ConnectedStage2_2.dat");
	CLineManager::Get_Instance()->LoadData_Circle(L"./Data/CircleStage2_2.dat");
	CObjManager::Get_Instance()->Load_ObjData(ID_SPIKE, L"./Data/Obj_SpikeStage2_0.dat");
	//CObjManager::Get_Instance()->Load_ObjData(ID_RING, L"./Data/Obj_RingStage2_3.dat");




	//CObjManager::Get_Instance()->Load_ObjData(ID_WALL, L"./Data/Obj_WallStage2_0.dat");
	//CObjManager::Get_Instance()->Load_ObjData(ID_SPRING, L"./Data/Obj_SpringStage2_0.dat");


	

	//CObj* pTestWall = CFactory<CWall>::Create(0, WINCY * 0.5f);
	//CObjManager::Get_Instance()->AddObject(ID_WALL, pTestWall);
	//dynamic_cast<CWall*>(pTestWall)->WallInit(WINCX * 0.1f, WINCY);

	//pTestWall = CFactory<CWall>::Create(WINCX * 0.95f, WINCY * 0.5f);
	//CObjManager::Get_Instance()->AddObject(ID_WALL, pTestWall);
	//dynamic_cast<CWall*>(pTestWall)->WallInit(WINCX * 0.1f, WINCY);

	//pTestWall = CFactory<CWall>::Create(WINCX * 0.5f, 0);
	//CObjManager::Get_Instance()->AddObject(ID_WALL, pTestWall);
	//dynamic_cast<CWall*>(pTestWall)->WallInit(WINCX, WINCY * 0.1f);

	//pTestWall = CFactory<CWall>::Create(WINCX * 0.5f, WINCY);
	//CObjManager::Get_Instance()->AddObject(ID_WALL, pTestWall);
	//dynamic_cast<CWall*>(pTestWall)->WallInit(WINCX, WINCY * 0.1f);
}

void CEditorScene::Update()
{
	int iScrollX = CScrollManager::Get_Instance()->Get_ScrollX();

	if (CKeyManager::Get_Instance()->Key_Pressing(VK_LEFT))
	{
		CScrollManager::Get_Instance()->Set_ScrollXPlus(10.f);
	}
	else if(CKeyManager::Get_Instance()->Key_Pressing(VK_RIGHT))
	{
		CScrollManager::Get_Instance()->Set_ScrollXPlus(-10.f);

	}
	else if(CKeyManager::Get_Instance()->Key_Pressing(VK_UP))
	{
		CScrollManager::Get_Instance()->Set_ScrollYPlus(10.f);
	}
	else if(CKeyManager::Get_Instance()->Key_Pressing(VK_DOWN))
	{
		CScrollManager::Get_Instance()->Set_ScrollYPlus(-10.f);
	}
	else if (CKeyManager::Get_Instance()->Key_Pressing(VK_F1))
	{
		m_eEditMode = EDIT_LINE;
	}
	else if (CKeyManager::Get_Instance()->Key_Pressing(VK_F2))
	{
		m_eEditMode = EDIT_TILE;
	}
	else if (CKeyManager::Get_Instance()->Key_Pressing(VK_F3))
	{
		m_eEditMode = EDIT_OBJ;
	}
	else if (CKeyManager::Get_Instance()->Key_Pressing(VK_CONTROL))
	{		

		if (CKeyManager::Get_Instance()->Key_Down('S'))
		{
			switch (m_eEditMode)
			{
			case EDIT_LINE:
				//SaveData_Horizontal();
				SaveData_Connected();
				//SaveData_Curve();
				SaveData_Circle();
				break;
			case EDIT_TILE:
				SaveData_Tile_Mushroom();
				break;
			case EDIT_OBJ:
				SaveData_Spring();
				SaveData_DiagonalSpring();
				SaveData_Wall();
				SaveData_Ring();
				SaveData_Spike();
				SaveData_Item();
				break;
			}

		}
	}

	switch (m_eEditMode)
	{
	case EDIT_LINE:
		m_lineEditor.Update();
		break;
	case EDIT_TILE:
		m_tileEditor.Update();
		break;
	case EDIT_OBJ:
		m_ObjEditor.Update();
		break;
	}

	CTileManager::Get_Instance()->Update();
#pragma region Debug
	CObjManager::Get_Instance()->Update();
	CLineManager::Get_Instance()->Update();

	switch (m_eEditMode)
	{
	case EDIT_LINE:
		CDebugManager::Get_Instance()->ChangeDebugLog(L"EditMode", L"Mode : Line Editor");
		break;
	case EDIT_TILE:
		CDebugManager::Get_Instance()->ChangeDebugLog(L"EditMode", L"Mode : Tile Editor");
		break;
	}
#pragma endregion
}

void CEditorScene::LateUpdate()
{
	switch (m_eEditMode)
	{
	case EDIT_LINE:
		m_lineEditor.LateUpdate();
		break;
	case EDIT_TILE:
		m_tileEditor.LateUpdate();
		break;
	case EDIT_OBJ:
		m_ObjEditor.LateUpdate();
		break;
	}
	CObjManager::Get_Instance()->LateUpdate();
	CLineManager::Get_Instance()->LateUpdate();
	CTileManager::Get_Instance()->LateUpdate();
}

void CEditorScene::Render(HDC _dc)
{
	switch (m_eEditMode)
	{
	case EDIT_LINE:
		CTileManager::Get_Instance()->Render(_dc);
		break;
	case EDIT_TILE:
	{
		if (m_tileEditor.Get_TileSorting() == false)
		{
			//CTileManager::Get_Instance()->Render(_dc);
			CTileManager::Get_Instance()->Render(_dc);

			// 격자
			RenderTileCrossLine(_dc);
		}
		else // sorting
		{
			CUIManager::Get_Instance()->Render_SelectEditMode_Mushroom(_dc);
		}
		break;
	}
	case EDIT_OBJ:
		CTileManager::Get_Instance()->Render(_dc);
		m_ObjEditor.Render(_dc);
		break;
	}

	CLineManager::Get_Instance()->Render(_dc);
	CObjManager::Get_Instance()->Render(_dc);

}

void CEditorScene::Release()
{
}

void CEditorScene::SaveData_Horizontal()
{
	HANDLE	hFile = CreateFile(L"./Data/HorizontalStage1.dat",	// 파일 경로와 이름을 넣어줌
		GENERIC_WRITE,		// 모드 설정(GENERIC_WRITE : 쓰기 GENERIC_READ : 읽기)
		NULL,				// 공유 방식 : 파일이 열려 있는 상태에서 다른 프로세스가 파일을 오픈할 때 허가할 것인가(NULL인 경우 공유하지 않는다)
		NULL,				// 보안 속성 : NULL인 경우 기본 보안 상태
		CREATE_NEW,		// 파일 생성 방식 (CREATE_ALWAYS : 파일이 없다면 생성하고, 있으면 덮어 쓰기해라, OPEN_EXISTING : 파일이 있을 경우에만 로드)
		FILE_ATTRIBUTE_NORMAL,	// 파일 속성(읽기 전용, 숨김 파일 등) (FILE_ATTRIBUTE_NORMAL : 아무런 속성 없는 일반 파일) 
		NULL);	// 생성할 파일의 속성을 제공할 템플릿 파일(안쓸거니깐 NULL)

	if (hFile == INVALID_HANDLE_VALUE)
	{
		MessageBox(g_hWnd, _T("save File"), L"Error", MB_OK);
		return;
	}

	DWORD	dwByte = 0;	// 예외처리 용 변수

	for (auto& iter : CLineManager::Get_Instance()->Get_StraightLineList())
	{
		WriteFile(hFile, &(iter->Get_MyLine()), sizeof(MyLine), &dwByte, nullptr);
	}

	CloseHandle(hFile);

	MessageBox(g_hWnd, _T("Save 완료"), L"성공", MB_OK);

	//CFileManager::Get_Instance()->PushDataPath(L"Horizontal0", L"./Data/Horizontal0.dat");
}

void CEditorScene::SaveData_Curve()
{
	HANDLE	hFile = CreateFile(L"./Data/CurveStage1.dat",	// 파일 경로와 이름을 넣어줌
		GENERIC_WRITE,		// 모드 설정(GENERIC_WRITE : 쓰기 GENERIC_READ : 읽기)
		NULL,				// 공유 방식 : 파일이 열려 있는 상태에서 다른 프로세스가 파일을 오픈할 때 허가할 것인가(NULL인 경우 공유하지 않는다)
		NULL,				// 보안 속성 : NULL인 경우 기본 보안 상태
		CREATE_NEW,		// 파일 생성 방식 (CREATE_ALWAYS : 파일이 없다면 생성하고, 있으면 덮어 쓰기해라, OPEN_EXISTING : 파일이 있을 경우에만 로드)
		FILE_ATTRIBUTE_NORMAL,	// 파일 속성(읽기 전용, 숨김 파일 등) (FILE_ATTRIBUTE_NORMAL : 아무런 속성 없는 일반 파일) 
		NULL);	// 생성할 파일의 속성을 제공할 템플릿 파일(안쓸거니깐 NULL)

	if (hFile == INVALID_HANDLE_VALUE)
	{
		MessageBox(g_hWnd, _T("save File"), L"Error", MB_OK);
		return;
	}

	DWORD	dwByte = 0;	// 예외처리 용 변수
	

	for (auto& iter: CLineManager::Get_Instance()->Get_CurveLineList())
	{
		WriteFile(hFile, (iter->Get_ThreePoints()), sizeof(Vector2) * 3, &dwByte, nullptr);
	}

	CloseHandle(hFile);

	MessageBox(g_hWnd, _T("Save 완료"), L"성공", MB_OK);

	//CFileManager::Get_Instance()->PushDataPath(L"Curve0", L"./Data/Curve0.dat");
}

void CEditorScene::SaveData_Connected()
{
	HANDLE	hFile = CreateFile(L"./Data/ConnectedStage2_2.dat",	// 파일 경로와 이름을 넣어줌
		GENERIC_WRITE,		// 모드 설정(GENERIC_WRITE : 쓰기 GENERIC_READ : 읽기)
		NULL,				// 공유 방식 : 파일이 열려 있는 상태에서 다른 프로세스가 파일을 오픈할 때 허가할 것인가(NULL인 경우 공유하지 않는다)
		NULL,				// 보안 속성 : NULL인 경우 기본 보안 상태
		CREATE_NEW,		// 파일 생성 방식 (CREATE_ALWAYS : 파일이 없다면 생성하고, 있으면 덮어 쓰기해라, OPEN_EXISTING : 파일이 있을 경우에만 로드)
		FILE_ATTRIBUTE_NORMAL,	// 파일 속성(읽기 전용, 숨김 파일 등) (FILE_ATTRIBUTE_NORMAL : 아무런 속성 없는 일반 파일) 
		NULL);	// 생성할 파일의 속성을 제공할 템플릿 파일(안쓸거니깐 NULL)

	if (hFile == INVALID_HANDLE_VALUE)
	{
		MessageBox(g_hWnd, _T("Connected Line save Failed"), L"Error", MB_OK);
		return;
	}

	DWORD	dwByte = 0;	// 예외처리 용 변수


	for (auto& iter : CLineManager::Get_Instance()->Get_ConnectedLineList())
	{
		// 점 개수
		int iPointCount = iter->Get_PointArr().size();
		vector<LinePoint>& refPointArr = iter->Get_PointArr();
		// 점개수 넘겨주기
		WriteFile(hFile, &iPointCount, sizeof(int), &dwByte, nullptr);
		// 점들 넘겨주기
		for (int i = 0; i < iPointCount; ++i)
		{
			WriteFile(hFile, &(refPointArr[i]), sizeof(LinePoint), &dwByte, nullptr);
			
		}
	}

	CloseHandle(hFile);

	MessageBox(g_hWnd, _T("Connected Line Save 완료"), L"성공", MB_OK);
}

void CEditorScene::SaveData_Circle()
{
	HANDLE	hFile = CreateFile(L"./Data/CircleStage2_2.dat",	// 파일 경로와 이름을 넣어줌
		GENERIC_WRITE,		// 모드 설정(GENERIC_WRITE : 쓰기 GENERIC_READ : 읽기)
		NULL,				// 공유 방식 : 파일이 열려 있는 상태에서 다른 프로세스가 파일을 오픈할 때 허가할 것인가(NULL인 경우 공유하지 않는다)
		NULL,				// 보안 속성 : NULL인 경우 기본 보안 상태
		CREATE_NEW,		// 파일 생성 방식 (CREATE_ALWAYS : 파일이 없다면 생성하고, 있으면 덮어 쓰기해라, OPEN_EXISTING : 파일이 있을 경우에만 로드)
		FILE_ATTRIBUTE_NORMAL,	// 파일 속성(읽기 전용, 숨김 파일 등) (FILE_ATTRIBUTE_NORMAL : 아무런 속성 없는 일반 파일) 
		NULL);	// 생성할 파일의 속성을 제공할 템플릿 파일(안쓸거니깐 NULL)

	if (hFile == INVALID_HANDLE_VALUE)
	{
		MessageBox(g_hWnd, _T("Circle Line save Failed"), L"Error", MB_OK);
		return;
	}

	DWORD	dwByte = 0;	// 예외처리 용 변수
	
	for (auto& iter : CLineManager::Get_Instance()->Get_CircleLineList())
	{
		WriteFile(hFile, &(iter->Get_LineStartPoint()), sizeof(Vector2), &dwByte, nullptr);
		WriteFile(hFile, &(iter->Get_CircleEndPoint()), sizeof(Vector2), &dwByte, nullptr);
	}

	CloseHandle(hFile);

	MessageBox(g_hWnd, _T("Circle Line Save 완료"), L"성공", MB_OK);
}

void CEditorScene::SaveData_Tile_Mushroom()
{
	CTileManager::Get_Instance()->SaveTile_Mushroom(L"./Data/Tile_Stage1_1.dat");
}

void CEditorScene::SaveData_Spring()
{
	HANDLE	hFile = CreateFile(L"./Data/Obj_SpringStage2_0.dat",	// 파일 경로와 이름을 넣어줌
		GENERIC_WRITE,		// 모드 설정(GENERIC_WRITE : 쓰기 GENERIC_READ : 읽기)
		NULL,				// 공유 방식 : 파일이 열려 있는 상태에서 다른 프로세스가 파일을 오픈할 때 허가할 것인가(NULL인 경우 공유하지 않는다)
		NULL,				// 보안 속성 : NULL인 경우 기본 보안 상태
		CREATE_NEW,		// 파일 생성 방식 (CREATE_ALWAYS : 파일이 없다면 생성하고, 있으면 덮어 쓰기해라, OPEN_EXISTING : 파일이 있을 경우에만 로드)
		FILE_ATTRIBUTE_NORMAL,	// 파일 속성(읽기 전용, 숨김 파일 등) (FILE_ATTRIBUTE_NORMAL : 아무런 속성 없는 일반 파일) 
		NULL);	// 생성할 파일의 속성을 제공할 템플릿 파일(안쓸거니깐 NULL)

	if (hFile == INVALID_HANDLE_VALUE)
	{
		MessageBox(g_hWnd, _T("Spring save Failed"), L"Error", MB_OK);
		return;
	}

	DWORD	dwByte = 0;	// 예외처리 용 변수

	for (auto& iter : CObjManager::Get_Instance()->Get_ObjList()[ID_SPRING])
	{
		//int iDirInt = dynamic_cast<CSpring*>(iter)->Get_SpringDirInt();
		MY_DIR eSpringDir = dynamic_cast<CSpring*>(iter)->Get_SpringDir();
		// scale, pos, dir
		WriteFile(hFile, &(iter->Get_Pos()), sizeof(Vector2), &dwByte, nullptr);
		WriteFile(hFile, &(iter->Get_Scale()), sizeof(Vector2), &dwByte, nullptr);
		//WriteFile(hFile, &iDirInt, sizeof(int), &dwByte, nullptr);
		WriteFile(hFile, &eSpringDir, sizeof(MY_DIR), &dwByte, nullptr);
	}
	
	CloseHandle(hFile);

	MessageBox(g_hWnd, _T("Spring Save 완료"), L"성공", MB_OK);
}

void CEditorScene::SaveData_DiagonalSpring()
{
	HANDLE	hFile = CreateFile(L"./Data/Obj_DiagonalSpringStage2_0.dat",	// 파일 경로와 이름을 넣어줌
		GENERIC_WRITE,		// 모드 설정(GENERIC_WRITE : 쓰기 GENERIC_READ : 읽기)
		NULL,				// 공유 방식 : 파일이 열려 있는 상태에서 다른 프로세스가 파일을 오픈할 때 허가할 것인가(NULL인 경우 공유하지 않는다)
		NULL,				// 보안 속성 : NULL인 경우 기본 보안 상태
		CREATE_NEW,		// 파일 생성 방식 (CREATE_ALWAYS : 파일이 없다면 생성하고, 있으면 덮어 쓰기해라, OPEN_EXISTING : 파일이 있을 경우에만 로드)
		FILE_ATTRIBUTE_NORMAL,	// 파일 속성(읽기 전용, 숨김 파일 등) (FILE_ATTRIBUTE_NORMAL : 아무런 속성 없는 일반 파일) 
		NULL);	// 생성할 파일의 속성을 제공할 템플릿 파일(안쓸거니깐 NULL)

	if (hFile == INVALID_HANDLE_VALUE)
	{
		MessageBox(g_hWnd, _T("Diagonal Spring save Failed"), L"Error", MB_OK);
		return;
	}

	DWORD	dwByte = 0;	// 예외처리 용 변수

	for (auto& iter : CObjManager::Get_Instance()->Get_ObjList()[ID_DIAGONALSPRING])
	{
		//int iDirInt = dynamic_cast<CSpring*>(iter)->Get_SpringDirInt();
		DIAGONAL_DIR eSpringDir = dynamic_cast<CDiagonalSpring*>(iter)->Get_SpringDir();
		// scale, pos, dir
		WriteFile(hFile, &(iter->Get_Pos()), sizeof(Vector2), &dwByte, nullptr);
		WriteFile(hFile, &(iter->Get_Scale()), sizeof(Vector2), &dwByte, nullptr);
		//WriteFile(hFile, &iDirInt, sizeof(int), &dwByte, nullptr);
		WriteFile(hFile, &eSpringDir, sizeof(DIAGONAL_DIR), &dwByte, nullptr);
	}

	CloseHandle(hFile);

	MessageBox(g_hWnd, _T("Diagonal Spring Save 완료"), L"성공", MB_OK);
}

void CEditorScene::SaveData_Wall()
{
	//HANDLE	hFile = CreateFile(L"./Data/Obj_WallStage1_1.dat",	// 파일 경로와 이름을 넣어줌
	HANDLE	hFile = CreateFile(L"./Data/Obj_WallStage2_2.dat",	// 파일 경로와 이름을 넣어줌
		GENERIC_WRITE,		// 모드 설정(GENERIC_WRITE : 쓰기 GENERIC_READ : 읽기)
		NULL,				// 공유 방식 : 파일이 열려 있는 상태에서 다른 프로세스가 파일을 오픈할 때 허가할 것인가(NULL인 경우 공유하지 않는다)
		NULL,				// 보안 속성 : NULL인 경우 기본 보안 상태
		CREATE_NEW,		// 파일 생성 방식 (CREATE_ALWAYS : 파일이 없다면 생성하고, 있으면 덮어 쓰기해라, OPEN_EXISTING : 파일이 있을 경우에만 로드)
		FILE_ATTRIBUTE_NORMAL,	// 파일 속성(읽기 전용, 숨김 파일 등) (FILE_ATTRIBUTE_NORMAL : 아무런 속성 없는 일반 파일) 
		NULL);	// 생성할 파일의 속성을 제공할 템플릿 파일(안쓸거니깐 NULL)

	if (hFile == INVALID_HANDLE_VALUE)
	{
		MessageBox(g_hWnd, _T("Wall save Failed"), L"Error", MB_OK);
		return;
	}

	DWORD	dwByte = 0;	// 예외처리 용 변수

	for (auto& iter : CObjManager::Get_Instance()->Get_ObjList()[ID_WALL])
	{
		// scale, pos, dir
		WriteFile(hFile, &(iter->Get_Pos()), sizeof(Vector2), &dwByte, nullptr);
		WriteFile(hFile, &(iter->Get_Scale()), sizeof(Vector2), &dwByte, nullptr);
	}

	CloseHandle(hFile);

	MessageBox(g_hWnd, _T("Wall Save 완료"), L"성공", MB_OK);
}

void CEditorScene::SaveData_Ring()
{
	HANDLE	hFile = CreateFile(L"./Data/Obj_RingStage2_4.dat",	// 파일 경로와 이름을 넣어줌
		GENERIC_WRITE,		// 모드 설정(GENERIC_WRITE : 쓰기 GENERIC_READ : 읽기)
		NULL,				// 공유 방식 : 파일이 열려 있는 상태에서 다른 프로세스가 파일을 오픈할 때 허가할 것인가(NULL인 경우 공유하지 않는다)
		NULL,				// 보안 속성 : NULL인 경우 기본 보안 상태
		CREATE_NEW,		// 파일 생성 방식 (CREATE_ALWAYS : 파일이 없다면 생성하고, 있으면 덮어 쓰기해라, OPEN_EXISTING : 파일이 있을 경우에만 로드)
		FILE_ATTRIBUTE_NORMAL,	// 파일 속성(읽기 전용, 숨김 파일 등) (FILE_ATTRIBUTE_NORMAL : 아무런 속성 없는 일반 파일) 
		NULL);	// 생성할 파일의 속성을 제공할 템플릿 파일(안쓸거니깐 NULL)

	if (hFile == INVALID_HANDLE_VALUE)
	{
		MessageBox(g_hWnd, _T("Ring save Failed"), L"Error", MB_OK);
		return;
	}

	DWORD	dwByte = 0;	// 예외처리 용 변수

	for (auto& iter : CObjManager::Get_Instance()->Get_ObjList()[ID_RING])
	{
		// scale, pos, dir
		WriteFile(hFile, &(iter->Get_Pos()), sizeof(Vector2), &dwByte, nullptr);
	}

	CloseHandle(hFile);

	MessageBox(g_hWnd, _T("RIng Save 완료"), L"성공", MB_OK);
}

void CEditorScene::SaveData_Spike()
{
	HANDLE	hFile = CreateFile(L"./Data/Obj_SpikeStage2_0.dat",	// 파일 경로와 이름을 넣어줌
		GENERIC_WRITE,		// 모드 설정(GENERIC_WRITE : 쓰기 GENERIC_READ : 읽기)
		NULL,				// 공유 방식 : 파일이 열려 있는 상태에서 다른 프로세스가 파일을 오픈할 때 허가할 것인가(NULL인 경우 공유하지 않는다)
		NULL,				// 보안 속성 : NULL인 경우 기본 보안 상태
		CREATE_NEW,		// 파일 생성 방식 (CREATE_ALWAYS : 파일이 없다면 생성하고, 있으면 덮어 쓰기해라, OPEN_EXISTING : 파일이 있을 경우에만 로드)
		FILE_ATTRIBUTE_NORMAL,	// 파일 속성(읽기 전용, 숨김 파일 등) (FILE_ATTRIBUTE_NORMAL : 아무런 속성 없는 일반 파일) 
		NULL);	// 생성할 파일의 속성을 제공할 템플릿 파일(안쓸거니깐 NULL)

	if (hFile == INVALID_HANDLE_VALUE)
	{
		MessageBox(g_hWnd, _T("Spike save Failed"), L"Error", MB_OK);
		return;
	}

	DWORD	dwByte = 0;	// 예외처리 용 변수

	for (auto& iter : CObjManager::Get_Instance()->Get_ObjList()[ID_SPIKE])
	{
		// scale, pos, dir
		WriteFile(hFile, &(iter->Get_Pos()), sizeof(Vector2), &dwByte, nullptr);
	}

	CloseHandle(hFile);

	MessageBox(g_hWnd, _T("Spike Save 완료"), L"성공", MB_OK);
}

void CEditorScene::SaveData_Item()
{
	HANDLE	hFile = CreateFile(L"./Data/Obj_ItemStage2_0.dat",	// 파일 경로와 이름을 넣어줌
		GENERIC_WRITE,		// 모드 설정(GENERIC_WRITE : 쓰기 GENERIC_READ : 읽기)
		NULL,				// 공유 방식 : 파일이 열려 있는 상태에서 다른 프로세스가 파일을 오픈할 때 허가할 것인가(NULL인 경우 공유하지 않는다)
		NULL,				// 보안 속성 : NULL인 경우 기본 보안 상태
		CREATE_NEW,		// 파일 생성 방식 (CREATE_ALWAYS : 파일이 없다면 생성하고, 있으면 덮어 쓰기해라, OPEN_EXISTING : 파일이 있을 경우에만 로드)
		FILE_ATTRIBUTE_NORMAL,	// 파일 속성(읽기 전용, 숨김 파일 등) (FILE_ATTRIBUTE_NORMAL : 아무런 속성 없는 일반 파일) 
		NULL);	// 생성할 파일의 속성을 제공할 템플릿 파일(안쓸거니깐 NULL)

	if (hFile == INVALID_HANDLE_VALUE)
	{
		MessageBox(g_hWnd, _T("Item save Failed"), L"Error", MB_OK);
		return;
	}

	DWORD	dwByte = 0;	// 예외처리 용 변수

	for (auto& iter : CObjManager::Get_Instance()->Get_ObjList()[ID_ITEM])
	{
		// scale, pos, dir
		WriteFile(hFile, &(iter->Get_Pos()), sizeof(Vector2), &dwByte, nullptr);
	}

	CloseHandle(hFile);

	MessageBox(g_hWnd, _T("Item Save 완료"), L"성공", MB_OK);
}

void CEditorScene::RenderTileCrossLine(HDC _dc)
{
	for (int i = 0; i < WINCX; i += 128)
	{
		MoveToEx(_dc, i, 0, nullptr);
		LineTo(_dc, i, WINCY);
	}

	for (int j = 0; j < WINCY; j+= 128)
	{
		MoveToEx(_dc, 0, j, nullptr);
		LineTo(_dc, WINCX, j);
	}

}
