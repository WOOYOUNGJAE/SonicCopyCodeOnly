#include "stdafx.h"
#include "ObjManager.h"
#include "CollisionManager.h"
#include "Factory.h"
#include "GameStateManager.h"
#include "KeyManager.h"
#include "Ring.h"
#include "Spike.h"
#include "Spring.h"
#include "Wall.h"
#include "DiagonalSpring.h"
#include "ItemObj.h"
#include "SoundManager.h"

CObjManager* CObjManager::m_pInstance = nullptr;

CObjManager::CObjManager() : m_dwShakeStartTime(0), m_dwShakeAccTime(2400)
{
}

CObjManager::~CObjManager()
{
}

void CObjManager::Initialize()
{
}

void CObjManager::Update()
{
	bool bResultAlive = true;
	for (int i = 0; i < OBJ_ID_END; ++i)
	{
		for (auto iter = m_ObjList[i].begin();
			iter != m_ObjList[i].end();)
		{
			bResultAlive = (*iter)->Update();

			if (bResultAlive == false)
			{
				Safe_Delete<CObj*>(*iter);
				iter = m_ObjList[i].erase(iter);
			}
			else
				++iter;
		}
	}

	/*if (CKeyManager::Get_Instance()->Key_Down(VK_F1))
		DropRings();*/
}

void CObjManager::LateUpdate()
{
	for (int i = 0; i < OBJ_ID_END; ++i)
	{
		for (auto iter = m_ObjList[i].begin();
			iter != m_ObjList[i].end(); ++iter)
		{
			(*iter)->LateUpdate();
		}
	}

	CCollisionManager::Get_Instance()->RectCollisionStay(m_ObjList[ID_PLAYER], m_ObjList[ID_WALL]);
	CCollisionManager::Get_Instance()->RectCollisionStay(m_ObjList[ID_PLAYER], m_ObjList[ID_SPRING]);
	CCollisionManager::Get_Instance()->RectCollisionStay(m_ObjList[ID_PLAYER], m_ObjList[ID_DIAGONALSPRING]);
	CCollisionManager::Get_Instance()->RectCollisionEnter(m_ObjList[ID_PLAYER], m_ObjList[ID_RING]);
	CCollisionManager::Get_Instance()->RectCollisionExit(m_ObjList[ID_PLAYER], m_ObjList[ID_RING]);
	CCollisionManager::Get_Instance()->RectCollisionStay(m_ObjList[ID_PLAYER], m_ObjList[ID_ITEM]);
	CCollisionManager::Get_Instance()->RectCollisionEnter(m_ObjList[ID_PLAYER], m_ObjList[ID_ITEM]);
	CCollisionManager::Get_Instance()->RectCollisionExit(m_ObjList[ID_PLAYER], m_ObjList[ID_ITEM]);
	CCollisionManager::Get_Instance()->RectCollisionStay(m_ObjList[ID_PLAYER], m_ObjList[ID_SPIKE]);
	CCollisionManager::Get_Instance()->RectCollisionEnter(m_ObjList[ID_PLAYER], m_ObjList[ID_SPIKE]);
	CCollisionManager::Get_Instance()->RectCollisionExit(m_ObjList[ID_PLAYER], m_ObjList[ID_SPIKE]);
	CCollisionManager::Get_Instance()->RectCollisionEnter(m_ObjList[ID_PLAYER], m_ObjList[ID_MECHASONIC]);
	CCollisionManager::Get_Instance()->RectCollisionExit(m_ObjList[ID_PLAYER], m_ObjList[ID_MECHASONIC]);

	CCollisionManager::Get_Instance()->RectCollisionStay(m_ObjList[ID_MECHASONIC], m_ObjList[ID_WALL]);
	CCollisionManager::Get_Instance()->RectCollisionEnter(m_ObjList[ID_MECHASONIC], m_ObjList[ID_WALL]);
	CCollisionManager::Get_Instance()->RectCollisionExit(m_ObjList[ID_MECHASONIC], m_ObjList[ID_WALL]);
	CCollisionManager::Get_Instance()->RectCollisionEnter(m_ObjList[ID_RING], m_ObjList[ID_WALL]);
	CCollisionManager::Get_Instance()->RectCollisionExit(m_ObjList[ID_RING], m_ObjList[ID_WALL]);
	
}	

void CObjManager::Render(HDC _dc)
{
	for (int i = 0; i < OBJ_ID_END; ++i)
	{
		for (auto iter = m_ObjList[i].begin();
			iter != m_ObjList[i].end(); ++iter)
		{
			/*if ((*iter)->Get_RenderGroup() == RENDER_OBJECT1)
			{
				m_ObjPostRenderList.push_back(*iter);
				continue;
			}*/
			(*iter)->Render(_dc);
		}
	}

	/*for (auto& iter : m_ObjPostRenderList)
		iter->Render(_dc);*/

	//m_ObjPostRenderList.clear();
}

void CObjManager::Release()
{
}

void CObjManager::AddObject(OBJ_ID _eID, CObj* _pObj)
{
	if (_eID >= OBJ_ID_END || _pObj == nullptr)
		return;

	m_ObjList[_eID].push_back(_pObj);
}

void CObjManager::Load_ObjData(OBJ_ID _eID, const TCHAR* _path)
{
	HANDLE	hFile = CreateFile(_path,	// 파일 경로와 이름을 넣어줌
		GENERIC_READ,		// 모드 설정(GENERIC_WRITE : 쓰기 GENERIC_READ : 읽기)
		NULL,				// 공유 방식 : 파일이 열려 있는 상태에서 다른 프로세스가 파일을 오픈할 때 허가할 것인가(NULL인 경우 공유하지 않는다)
		NULL,				// 보안 속성 : NULL인 경우 기본 보안 상태
		OPEN_EXISTING,		// 파일 생성 방식 (CREATE_ALWAYS : 파일이 없다면 생성하고, 있으면 덮어 쓰기해라, OPEN_EXISTING : 파일이 있을 경우에만 로드)
		FILE_ATTRIBUTE_NORMAL,	// 파일 속성(읽기 전용, 숨김 파일 등) (FILE_ATTRIBUTE_NORMAL : 아무런 속성 없는 일반 파일) 
		NULL);	// 생성할 파일의 속성을 제공할 템플릿 파일(안쓸거니깐 NULL)

	if (INVALID_HANDLE_VALUE == hFile)
	{
		MessageBox(g_hWnd, _T("Load File"), L"Error", MB_OK);
		return;
	}

	DWORD	dwByte = 0;	// 예외처리 용 변수

	switch (_eID)
	{
	case ID_SPRING:
	{

		Vector2 vTmpPos = {};
		Vector2 vTmpScale = {};
		MY_DIR eTmpDir = DIR_UP;

		while (true)
		{
			ReadFile(hFile, &vTmpPos, sizeof(Vector2), &dwByte, nullptr);
			ReadFile(hFile, &vTmpScale, sizeof(Vector2), &dwByte, nullptr);
			ReadFile(hFile, &eTmpDir, sizeof(MY_DIR), &dwByte, nullptr);

			if (0 == dwByte)
				break;

			CObj* pTmpObj = CFactory<CSpring>::Create(vTmpPos.fX, vTmpPos.fY);
			pTmpObj->Set_Scale(vTmpScale.fX, vTmpScale.fY);
			dynamic_cast<CSpring*>(pTmpObj)->SpringInit(eTmpDir);
			AddObject(_eID, pTmpObj);
		}
	}
	case ID_DIAGONALSPRING:
	{

		Vector2 vTmpPos = {};
		Vector2 vTmpScale = {};
		DIAGONAL_DIR eTmpDir = DIR_RU;

		while (true)
		{
			ReadFile(hFile, &vTmpPos, sizeof(Vector2), &dwByte, nullptr);
			ReadFile(hFile, &vTmpScale, sizeof(Vector2), &dwByte, nullptr);
			ReadFile(hFile, &eTmpDir, sizeof(DIAGONAL_DIR), &dwByte, nullptr);

			if (0 == dwByte)
				break;

			CObj* pTmpObj = CFactory<CDiagonalSpring>::Create(vTmpPos.fX, vTmpPos.fY);
			pTmpObj->Set_Scale(vTmpScale.fX, vTmpScale.fY);
			dynamic_cast<CDiagonalSpring*>(pTmpObj)->SpringInit(eTmpDir);
			AddObject(_eID, pTmpObj);
		}
	}
	case ID_WALL:
		{
		Vector2 vTmpPos = {};
		Vector2 vTmpScale = {};
		while (true)
		{
			ReadFile(hFile, &vTmpPos, sizeof(Vector2), &dwByte, nullptr);
			ReadFile(hFile, &vTmpScale, sizeof(Vector2), &dwByte, nullptr);

			if (0 == dwByte)
				break;

			CObj* pTmpObj = CFactory<CWall>::Create(vTmpPos.fX, vTmpPos.fY);
			pTmpObj->Set_Scale(vTmpScale.fX, vTmpScale.fY);
			AddObject(_eID, pTmpObj);
		}
		}
		break;
	case ID_RING:
		{
		Vector2 vTmpPos = {};
		while (true)
		{
			ReadFile(hFile, &vTmpPos, sizeof(Vector2), &dwByte, nullptr);

			if (0 == dwByte)
				break;

			CObj* pTmpObj = CFactory<CRing>::Create(vTmpPos.fX, vTmpPos.fY);
			pTmpObj->Set_Scale(16, 16);
			pTmpObj->Set_ID(_eID);
			AddObject(_eID, pTmpObj);
		}
		}
		break;
	case ID_SPIKE:
		{
		Vector2 vTmpPos = {};
		while (true)
		{
			ReadFile(hFile, &vTmpPos, sizeof(Vector2), &dwByte, nullptr);

			if (0 == dwByte)
				break;

			CObj* pTmpObj = CFactory<CSpike>::Create(vTmpPos.fX, vTmpPos.fY);
			pTmpObj->Set_Scale(31, 31);
			pTmpObj->Set_ID(_eID);
			AddObject(_eID, pTmpObj);
		}
		}
		break;
	case ID_ITEM:
		{
		Vector2 vTmpPos = {};
		while (true)
		{
			ReadFile(hFile, &vTmpPos, sizeof(Vector2), &dwByte, nullptr);

			if (0 == dwByte)
				break;

			CObj* pTmpObj = CFactory<CItemObj>::Create(vTmpPos.fX, vTmpPos.fY);
			pTmpObj->Set_Scale(28, 32);
			pTmpObj->Set_ID(_eID);
			AddObject(_eID, pTmpObj);
		}
		}
		break;
	}

	CloseHandle(hFile);
}

void CObjManager::ShakeWalls()
{
	if (m_dwShakeStartTime + m_dwShakeAccTime < GetTickCount())
	{
		
	}
}

void CObjManager::DropRings()
{
	int iRingCount = CGameStateManager::Get_Instance()->Get_RingCount();

	if (iRingCount >= 1)
	{

		//sound
		CSoundManager::Get_Instance()->StopSound(SOUND_EFFECT1);
		CSoundManager::Get_Instance()->PlaySoundW(L"RingDrop.wav", SOUND_EFFECT1, 1.f);
	}


	Vector2 vPlayerPos = m_ObjList[ID_PLAYER].front()->Get_Pos();

	// i 가 9가되면
	// i / 8
	for (int i = 1; i <= iRingCount; ++i)
	{
		// 8 이상이면 0 ~ 7 은 8로 나눈다. 그렇지 않다면 .. + 1로
		float fAngle = i * PI /(iRingCount + 1);

		float fXSpeed = 3;
		float fYSpeed = 3;
		float fGravity = 5;

		CObj* pTmpRing = CFactory<CRing>::Create(vPlayerPos.fX, vPlayerPos.fY);
		AddObject(ID_RING, pTmpRing);
		dynamic_cast<CRing*>(pTmpRing)->Set_DropCondition(fAngle, fXSpeed, fYSpeed, fGravity);
	}

	CGameStateManager::Get_Instance()->ResetRingCount();
}