#include "stdafx.h"
#include "LineManager.h"
#include "FileManager.h"
#include "ObjManager.h"
#include "Player.h"
#include "PlayerBase.h"


CLineManager* CLineManager::m_pInstance;

CLineManager::CLineManager()
{
}

CLineManager::~CLineManager()
{
}

void CLineManager::Initialize()
{
}

void CLineManager::Update()
{

}

void CLineManager::LateUpdate()
{

}

void CLineManager::Render(HDC _dc)
{
	if (m_StraightLineList.empty() && m_CurvedLineList.empty() && m_CircleLineList.empty() && m_ConnectedLineList.empty())
		return;

	for (auto& iter : m_StraightLineList)
		iter->Render(_dc);

	for (auto& iter : m_ConnectedLineList)
		iter->Render(_dc);

	for (auto& iter : m_CurvedLineList)
		iter->Render(_dc);

	for (auto& iter : m_CircleLineList)
		iter->Render(_dc);


	
}

void CLineManager::Release()
{
	for_each(m_StraightLineList.begin(), m_StraightLineList.end(), CDeleteObj());
	m_StraightLineList.clear();

	for_each(m_CurvedLineList.begin(), m_CurvedLineList.end(), CDeleteObj());
	m_CurvedLineList.clear();
}

void CLineManager::	LoadData_Horizontal(const TCHAR* _path)
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
	MyLine	tLine{};

	while (true)
	{
		ReadFile(hFile, &tLine, sizeof(MyLine), &dwByte, nullptr);

		if (0 == dwByte)
			break;

		m_StraightLineList.push_back(new CLineObjBase(tLine));
	}

	CloseHandle(hFile);

	//MessageBox(g_hWnd, _T("Horizontal Load 완료"), L"성공", MB_OK);
}

void CLineManager::LoadData_Curve(const TCHAR* _path)
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
	//Vector2* pTmpArr = nullptr; // size 3
	Vector2* pTmpArr = new Vector2[3]; // size 3

	while (true)
	{
		ReadFile(hFile, pTmpArr, sizeof(Vector2) * 3, &dwByte, nullptr);

		if (0 == dwByte)
			break;
		m_CurvedLineList.push_back(new CCurvedLine(pTmpArr[0], pTmpArr[1], pTmpArr[2]));
	}

	CloseHandle(hFile);

	//MessageBox(g_hWnd, _T("Curve Line Load 완료"), L"성공", MB_OK);
	delete[] pTmpArr;
}

void CLineManager::LoadData_Circle(const TCHAR* _path)
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
	//Vector2* pTmpArr = nullptr; // size 3
	//CCircleLine tmpCircleClass;
	Vector2 vTmpArr[2];

	while (true)
	{
		ReadFile(hFile, vTmpArr, sizeof(Vector2) * 2, &dwByte, nullptr);

		if (0 == dwByte)
			break;
		m_CircleLineList.push_back( 
			new CCircleLine(vTmpArr[0].fX,
				vTmpArr[0].fY,
				vTmpArr[1].fX,
				vTmpArr[1].fY )
		); // 복사 생성자?
		/*ReadFile(hFile, &tmpCircleClass, sizeof(CCircleLine), &dwByte, nullptr);

		if (0 == dwByte)
			break;
		m_CircleLineList.push_back( 
			new CCircleLine(tmpCircleClass.Get_LineStartPoint().fX,
			tmpCircleClass.Get_LineStartPoint().fY,
				tmpCircleClass.Get_LineEndPoint().fX,
				tmpCircleClass.Get_LineEndPoint().fY )
		); // 복사 생성자?*/
	}

	//pTmpCircleClass->
	CloseHandle(hFile);
	//delete[] vTmpArr;
	//vTmpArr = nullptr;
	//Safe_Delete(pTmpCircleClass);
	//MessageBox(g_hWnd, _T("Curve Line Load 완료"), L"성공", MB_OK);
}

void CLineManager::LoadData_Connected(const TCHAR* _path)
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

	int iPointCount = 0;
	

	while (true)
	{
		// 점 몇개 있는지 읽고
		ReadFile(hFile, &iPointCount, sizeof(int), &dwByte, nullptr);
		CConnectedLine _tmpConnectedLine;

		// 점 개수만큼 반복해서 저장한다
		for (int i = 0; i < iPointCount; ++i)
		{
			LinePoint tTmpPoint{};
			// 점 하나 읽고
			ReadFile(hFile, &tTmpPoint, sizeof(LinePoint), &dwByte, nullptr);
			// 임시 클래스에 점 추가
			_tmpConnectedLine.AddPoint(tTmpPoint.fX, tTmpPoint.fY);
		}
		if (0 == dwByte)
			break;
		
		m_ConnectedLineList.push_back(new CConnectedLine(_tmpConnectedLine));
		_tmpConnectedLine.Release();
	}

	CloseHandle(hFile);

	//MessageBox(g_hWnd, _T("Curve Line Load 완료"), L"성공", MB_OK);
}

void CLineManager::AddStraightLine(float _x0, float _y0, float _x1, float _y1)
{
	m_StraightLineList.push_back(new CLineObjBase(LinePoint(_x0, _y0), LinePoint(_x1, _y1)));
}

void CLineManager::AddStraightLine(LinePoint _L, LinePoint _R)
{
	m_StraightLineList.push_back(new CLineObjBase(_L,_R));
}

void CLineManager::AddCurvedLine(float _x0, float _y0, float _x1, float _y1, float _x2, float _y2)
{
	m_CurvedLineList.push_back(new CCurvedLine(_x0, _y0, _x1, _y1, _x2, _y2));
}

void CLineManager::AddCurvedLine(Vector2 _pt0, Vector2 _pt1, Vector2 _pt2)
{
	m_CurvedLineList.push_back(new CCurvedLine(_pt0, _pt1, _pt2));
}

void CLineManager::AddCircleLine(float _fLineStartX, float _fLineStartY, float _fLineEndX, float _fLineEndY)
{
	m_CircleLineList.push_back(new CCircleLine(_fLineStartX, _fLineStartY, _fLineEndX, _fLineEndY));
}

void CLineManager::AddConnectedLine(CConnectedLine _ConnectedLine)
{
	m_ConnectedLineList.push_back(new CConnectedLine(_ConnectedLine));
}

void CLineManager::InitializeAllConnectedLIne()
{
	for (auto& iter : m_ConnectedLineList)
	{
		iter->Initialize();
	}
}

void CLineManager::FindHorizontalLine(float& _refX, float& _refY)
{
	for (auto& iter : m_StraightLineList)
	{
		// refX, refY가 선 선 위에 있다면
		if (CMyMath::DifferenceBetweenPointAndLinePoint(iter->Get_LeftX(), iter->Get_LeftY(),
			iter->Get_RightX(), iter->Get_RightY(), _refX, _refY)
			<
			2.0f)
		{
			_refY = CMyMath::LineEquation(iter->Get_LeftX(), iter->Get_LeftY(),
				iter->Get_RightX(), iter->Get_RightY(), _refX);
		}
	}
}

bool CLineManager::OnLine(float _fX, float _fY, float& _refY)
{
	//if (m_StraightLineList.empty())
	//	return false;

	//CLineObjBase* pTargetLine = nullptr;
	//float fHeighest = FLT_MAX; // 가장 낮은 높이로 초기화

	//// 자기 발 밑 가장 높은 라인 찾기
	//for (auto& iter : m_StraightLineList)
	//{
	//	// iter의 라인 x범위 밖이면 넘어감
	//	if (_fX < iter->Get_LeftX() || _fX > iter->Get_RightX())
	//		continue;

	//	//  y위치
	//	float fCurrentLineY0 = CMyMath::Bigger(iter->Get_MyLine().L_Point.fY,
	//		iter->Get_MyLine().R_Point.fY);

	//	// 좌측 점
	//	float x0 = iter->Get_LeftX();
	//	float y0 = iter->Get_MyLine().L_Point.fY;
	//	//우측 점
	//	float x1 = iter->Get_RightX();
	//	float y1 = iter->Get_MyLine().R_Point.fY;

	//	//float fDelta = (y1 - y0) / (x1 - x0);

	//	// 직선의 방정식으로 구한 현 라인의 y 값
	//	float fCurrentLineY = 
	//		CMyMath::LineEquation(x0, y0, x1, y1, _fX);

	//	// 탈 라인이 자신보다 위에 있으면 넘어감
	//	if (fCurrentLineY < _refY)
	//		continue;

	//	if (fCurrentLineY < fHeighest)
	//		fHeighest = fCurrentLineY;
	//}

	//// 위의 과정을 거치고도 그대로라 밟을 땅이 없음을 의미
	//if (fHeighest >= FLT_MAX)
	//	return false;

	//// 적절한 땅을 찾았으면 그 y좌표로 인자값 업데이트
	//_refY = fHeighest;
	//return true;

	if (m_StraightLineList.empty() && m_CurvedLineList.empty())
		return false;
	
	float fHeighest = 1000; // 가장 낮은 높이로 초기화

	// 직선 라인 중에서 자기 발 밑 가장 높은 라인 찾기
	for (auto& iter : m_StraightLineList)
	{
		// iter의 라인 x범위 밖이면 넘어감
		if (_fX < iter->Get_LeftX() || _fX > iter->Get_RightX())
			continue;

		//  y위치
		float fCurrentLineY0 = CMyMath::Bigger(iter->Get_MyLine().L_Point.fY,
			iter->Get_MyLine().R_Point.fY);

		// 좌측 점
		float x0 = iter->Get_LeftX();
		float y0 = iter->Get_MyLine().L_Point.fY;
		//우측 점
		float x1 = iter->Get_RightX();
		float y1 = iter->Get_MyLine().R_Point.fY;

		//float fDelta = (y1 - y0) / (x1 - x0);

		// 직선의 방정식으로 구한 현 라인의 y 값
		float fCurrentLineY =
			CMyMath::LineEquation(x0, y0, x1, y1, _fX);

		// 현 플레이어 위치 비교
		if (fCurrentLineY < _fY) // 문제
			continue;

		if (fCurrentLineY < fHeighest)
			fHeighest = fCurrentLineY;
	}

	for (auto& iter : m_CurvedLineList)
	{
		if (_fX < iter->Get_LeftX() || _fX > iter->Get_RightX())
			continue;
		// 곡선 좌우 끝 사이에 있는 경우


		//  y위치
		float fCurrentLineY = iter->Get_CurrentY(_fX);

		// 탈 라인이 자신보다 위에 있으면 넘어감
		if (fCurrentLineY < _fY)
			continue;
		

		if (fCurrentLineY < fHeighest)
			fHeighest = fCurrentLineY;
	}

	// 위의 과정을 거치고도 그대로라 밟을 땅이 없음을 의미
	if (fHeighest >= 1000)
		return false;

	// 적절한 땅을 찾았으면 그 y좌표로 인자값 업데이트
	_refY = fHeighest;
	return true;
}

bool CLineManager::OnLine(float _fX, float _fY, float& _refY, bool& _bOnCurve)
{
	if (m_StraightLineList.empty() && m_CurvedLineList.empty())
		return false;

	float fHighest = 1000; // 가장 낮은 높이로 초기화

	// 직선 라인 중에서 자기 발 밑 가장 높은 라인 찾기
	for (auto& iter : m_StraightLineList)
	{
		// iter의 라인 x범위 밖이면 넘어감
		if (_fX < iter->Get_LeftX() || _fX > iter->Get_RightX())
			continue;

		//  y위치
		float fCurrentLineY0 = CMyMath::Bigger(iter->Get_MyLine().L_Point.fY,
			iter->Get_MyLine().R_Point.fY);

		// 좌측 점
		float x0 = iter->Get_LeftX();
		float y0 = iter->Get_MyLine().L_Point.fY;
		//우측 점
		float x1 = iter->Get_RightX();
		float y1 = iter->Get_MyLine().R_Point.fY;

		//float fDelta = (y1 - y0) / (x1 - x0);

		// 직선의 방정식으로 구한 현 라인의 y 값
		float fCurrentLineY =
			CMyMath::LineEquation(x0, y0, x1, y1, _fX);

		// 현 플레이어 위치 비교
		if (fCurrentLineY < _fY) // 문제
			continue;

		if (fCurrentLineY < fHighest)
			fHighest = fCurrentLineY;
	}

	for (auto& iter : m_CurvedLineList)
	{
		if (_fX < iter->Get_LeftX() || _fX > iter->Get_RightX())
			continue;
		// 곡선 좌우 끝 사이에 있는 경우


		//  y위치
		float fCurrentLineY = iter->Get_CurrentY(_fX);

		// 탈 라인이 자신보다 위에 있으면 넘어감
		if (fCurrentLineY < _fY)
			continue;


		if (fCurrentLineY < fHighest)
		{
			fHighest = fCurrentLineY;
			_bOnCurve = true;
		}
	}

	// 위의 과정을 거치고도 그대로라 밟을 땅이 없음을 의미
	if (fHighest >= 1000)
		return false;

	// 적절한 땅을 찾았으면 그 y좌표로 인자값 업데이트
	_refY = fHighest;
	return true;
}

bool CLineManager::OnLine(float _fX, float _fY, float& _refY, bool& _bOnCurve, CCurvedLine** _ppCurvedLineToTake)
{
	if (m_StraightLineList.empty() && m_CurvedLineList.empty())
		return false;

	float fHighest = 1000; // 가장 낮은 높이로 초기화

	// 직선 라인 중에서 자기 발 밑 가장 높은 라인 찾기
	for (auto& iter : m_StraightLineList)
	{
		// iter의 라인 x범위 밖이면 넘어감
		if (_fX < iter->Get_LeftX() || _fX > iter->Get_RightX())
			continue;
		
		// 좌측 점
		float x0 = iter->Get_LeftX();
		float y0 = iter->Get_MyLine().L_Point.fY;
		//우측 점
		float x1 = iter->Get_RightX();
		float y1 = iter->Get_MyLine().R_Point.fY;

		//float fDelta = (y1 - y0) / (x1 - x0);

		// 직선의 방정식으로 구한 현 라인의 y 값
		float fCurrentLineY =
			CMyMath::LineEquation(x0, y0, x1, y1, _fX);

		// 현 플레이어 위치 비교
		if (fCurrentLineY < _fY) // 문제
			continue;

		if (fCurrentLineY < fHighest)
			fHighest = fCurrentLineY;
	}

	//for (auto iter : m_CurvedLineList)
	for (auto iter = m_CurvedLineList.begin(); iter != m_CurvedLineList.end(); ++iter)
	{
		if (_fX < (*iter)->Get_LeftX() || _fX >(*iter)->Get_RightX())
			continue;
		// 곡선 좌우 끝 사이에 있는 경우


		//  y위치
		float fCurrentLineY = (*iter)->Get_CurrentY(_fX);

		// 탈 라인이 자신보다 위에 있으면 넘어감
		if (fCurrentLineY < _fY)
			continue;


		if (fCurrentLineY < fHighest)
		{
			fHighest = fCurrentLineY;
			_bOnCurve = true;
			*_ppCurvedLineToTake = (*iter);
		}
	}

	// 위의 과정을 거치고도 그대로라 밟을 땅이 없음을 의미
	if (fHighest >= 1000)
		return false;

	// 적절한 땅을 찾았으면 그 y좌표로 인자값 업데이트
	_refY = fHighest;
	return true;
}

bool CLineManager::OnLine(float _fX, float _fY, float& _refY, bool& _bOnCurve, CCurvedLine** _ppCurvedLineToTake,
	bool& _bOnCircle, CCircleLine** _ppCircleLineToTake)
{
	if (m_StraightLineList.empty() && m_CurvedLineList.empty() && m_CircleLineList.empty() && m_ConnectedLineList.empty())
		return false;

	float fHighest = 1000; // 가장 낮은 높이로 초기화

	// 직선 라인 중에서 자기 발 밑 가장 높은 라인 찾기
	for (auto& iter : m_StraightLineList)
	{
		if (m_StraightLineList.empty())
			break;

		// iter의 라인 x범위 밖이면 넘어감
		if (_fX < iter->Get_LeftX() || _fX > iter->Get_RightX())
			continue;

		// 좌측 점
		float x0 = iter->Get_LeftX();
		float y0 = iter->Get_MyLine().L_Point.fY;
		//우측 점
		float x1 = iter->Get_RightX();
		float y1 = iter->Get_MyLine().R_Point.fY;

		//float fDelta = (y1 - y0) / (x1 - x0);

		// 직선의 방정식으로 구한 현 라인의 y 값
		float fCurrentLineY =
			CMyMath::LineEquation(x0, y0, x1, y1, _fX);

		// 현 플레이어 위치 비교
		if (fCurrentLineY  < _fY) // 문제
			continue;

		if (fCurrentLineY < fHighest)
			fHighest = fCurrentLineY;
	}

	//for (auto iter : m_CurvedLineList)
	for (auto iter = m_CurvedLineList.begin(); iter != m_CurvedLineList.end(); ++iter)
	{
		if (_fX < (*iter)->Get_LeftX() || _fX >(*iter)->Get_RightX())
			continue;
		// 곡선 좌우 끝 사이에 있는 경우


		//  y위치
		float fCurrentLineY = (*iter)->Get_CurrentY(_fX);

		// 탈 라인이 자신보다 위에 있으면 넘어감
		if (fCurrentLineY < _fY)
			continue;


		if (fCurrentLineY < fHighest)
		{
			fHighest = fCurrentLineY;
			_bOnCurve = true;
			_bOnCircle = false;
			*_ppCurvedLineToTake = (*iter);
		}
	}

	for (auto iter = m_CircleLineList.begin(); iter != m_CircleLineList.end(); ++iter)
	{
		if (m_CircleLineList.empty())
			break;

		if (_fX < (*iter)->Get_LeftX() || _fX >(*iter)->Get_RightX())
			continue;

		// y위치
		float fCurrentLineY = (*iter)->Get_BottomY();

		// 탈 라인이 자신보다 위에 있으면 넘어감
		if (fCurrentLineY < _fY)
			continue;
		
		if (fCurrentLineY < fHighest)
		{
			fHighest = fCurrentLineY;
			_bOnCurve = false;
			_bOnCircle = true;
			*_ppCircleLineToTake = (*iter);
		}
	}

	// 위의 과정을 거치고도 그대로라 밟을 땅이 없음을 의미
	if (fHighest >= 1000)
		return false;

	// 적절한 땅을 찾았으면 그 y좌표로 인자값 업데이트
	_refY = fHighest;
	return true;
}

// player base
bool CLineManager::LineCheck(float _fX, float _fY, float& _refTargetY)
{
	if (m_StraightLineList.empty() && m_CurvedLineList.empty() && m_CircleLineList.empty() && m_ConnectedLineList.empty())
		return false;

	CObj* pPlayer = CObjManager::Get_Instance()->Get_Player();

	float fHighest = 10000; // 가장 낮은 높이로 초기화

	// 직선 라인 중에서 자기 발 밑 가장 높은 라인 찾기
	for (auto& iter : m_StraightLineList)
	{
		// iter의 라인 x범위 밖이면 넘어감
		if (_fX < iter->Get_LeftX() || _fX >= iter->Get_RightX())
			continue;

		// 좌측 점
		float x0 = iter->Get_LeftX();
		float y0 = iter->Get_MyLine().L_Point.fY;
		//우측 점
		float x1 = iter->Get_RightX();
		float y1 = iter->Get_MyLine().R_Point.fY;

		//float fDelta = (y1 - y0) / (x1 - x0);

		// 직선의 방정식으로 구한 현 라인의 y 값
		float fCurrentLineY =
			CMyMath::LineEquation(x0, y0, x1, y1, _fX);

		// 현 플레이어 위치 비교 , 여유분 주기
		if (fCurrentLineY < _fY) // 문제
			continue;

		if (fCurrentLineY < fHighest)
			fHighest = fCurrentLineY;
	}

	//for (auto iter : m_CurvedLineList)
	for (auto iter = m_CurvedLineList.begin(); iter != m_CurvedLineList.end(); ++iter)
	{
		if (_fX < (*iter)->Get_LeftX() || _fX >= (*iter)->Get_RightX())
			continue;
		// 곡선 좌우 끝 사이에 있는 경우


		//  y위치
		float fCurrentLineY = (*iter)->Get_CurrentY(_fX);

		// 탈 라인이 자신보다 위에 있으면 넘어감
		if (fCurrentLineY < _fY)
			continue;


		if (fCurrentLineY < fHighest)
		{
			fHighest = fCurrentLineY;
			dynamic_cast<CPlayerBase*>(pPlayer)->Set_OnCurve(true);
			//dynamic_cast<CPlayerBase*>(pPlayer)->Set_OnCircleRiding(false);
			dynamic_cast<CPlayerBase*>(pPlayer)->Set_OnCircle(false);
			dynamic_cast<CPlayerBase*>(pPlayer)->Set_CurrentCurve(*iter);
		}
	}

	for (auto iter = m_CircleLineList.begin(); iter != m_CircleLineList.end(); ++iter)
	{
		if (_fX < (*iter)->Get_LeftX() || _fX >(*iter)->Get_RightX())		continue;

		// y위치
		float fCurrentLineY = (*iter)->Get_BottomY();

		// 탈 라인이 자신보다 위에 있으면 넘어감
		if (fCurrentLineY + 14 < _fY)
			continue;

		if (fCurrentLineY < fHighest)
		{
			fHighest = fCurrentLineY;
			dynamic_cast<CPlayerBase*>(pPlayer)->Set_OnCurve(false);
			//dynamic_cast<CPlayerBase*>(pPlayer)->Set_OnCircleRiding(false);
			dynamic_cast<CPlayerBase*>(pPlayer)->Set_OnCircle(true);
			dynamic_cast<CPlayerBase*>(pPlayer)->Set_CurrentCircle(*iter);
			dynamic_cast<CPlayerBase*>(pPlayer)->Set_CurrentLine(*iter);
		}
	}


	for (auto iter = m_ConnectedLineList.begin(); iter != m_ConnectedLineList.end(); ++iter)
	{
		if (_fX < (*iter)->Get_LeftX() || _fX >= (*iter)->Get_RightX())
			continue;

		// y위치
		float fCurrentLineY = (*iter)->LineEquation(_fX); // 현 x위치의 높이

		// 탈 라인이 자신보다 위에 있으면 넘어감
		if (fCurrentLineY + 15 < _fY)
			continue;

		if (fCurrentLineY < fHighest)
		{
			fHighest = fCurrentLineY;
			dynamic_cast<CPlayerBase*>(pPlayer)->Set_OnCurve(false);
			dynamic_cast<CPlayerBase*>(pPlayer)->Set_OnCircle(false);
			dynamic_cast<CPlayerBase*>(pPlayer)->Set_CurrentLine(*iter);
		}
	}

	// 위의 과정을 거치고도 그대로라 밟을 땅이 없음을 의미
	if (fHighest >= 10000)
		return false; 

	// 적절 한 땅을 찾았으면 그 y좌표로 인자값 업데이트
	_refTargetY = fHighest;
	return true;
}



