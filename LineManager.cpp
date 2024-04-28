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
	HANDLE	hFile = CreateFile(_path,	// ���� ��ο� �̸��� �־���
		GENERIC_READ,		// ��� ����(GENERIC_WRITE : ���� GENERIC_READ : �б�)
		NULL,				// ���� ��� : ������ ���� �ִ� ���¿��� �ٸ� ���μ����� ������ ������ �� �㰡�� ���ΰ�(NULL�� ��� �������� �ʴ´�)
		NULL,				// ���� �Ӽ� : NULL�� ��� �⺻ ���� ����
		OPEN_EXISTING,		// ���� ���� ��� (CREATE_ALWAYS : ������ ���ٸ� �����ϰ�, ������ ���� �����ض�, OPEN_EXISTING : ������ ���� ��쿡�� �ε�)
		FILE_ATTRIBUTE_NORMAL,	// ���� �Ӽ�(�б� ����, ���� ���� ��) (FILE_ATTRIBUTE_NORMAL : �ƹ��� �Ӽ� ���� �Ϲ� ����) 
		NULL);	// ������ ������ �Ӽ��� ������ ���ø� ����(�Ⱦ��Ŵϱ� NULL)

	if (INVALID_HANDLE_VALUE == hFile)
	{
		MessageBox(g_hWnd, _T("Load File"), L"Error", MB_OK);
		return;
	}

	DWORD	dwByte = 0;	// ����ó�� �� ����
	MyLine	tLine{};

	while (true)
	{
		ReadFile(hFile, &tLine, sizeof(MyLine), &dwByte, nullptr);

		if (0 == dwByte)
			break;

		m_StraightLineList.push_back(new CLineObjBase(tLine));
	}

	CloseHandle(hFile);

	//MessageBox(g_hWnd, _T("Horizontal Load �Ϸ�"), L"����", MB_OK);
}

void CLineManager::LoadData_Curve(const TCHAR* _path)
{
	HANDLE	hFile = CreateFile(_path,	// ���� ��ο� �̸��� �־���
		GENERIC_READ,		// ��� ����(GENERIC_WRITE : ���� GENERIC_READ : �б�)
		NULL,				// ���� ��� : ������ ���� �ִ� ���¿��� �ٸ� ���μ����� ������ ������ �� �㰡�� ���ΰ�(NULL�� ��� �������� �ʴ´�)
		NULL,				// ���� �Ӽ� : NULL�� ��� �⺻ ���� ����
		OPEN_EXISTING,		// ���� ���� ��� (CREATE_ALWAYS : ������ ���ٸ� �����ϰ�, ������ ���� �����ض�, OPEN_EXISTING : ������ ���� ��쿡�� �ε�)
		FILE_ATTRIBUTE_NORMAL,	// ���� �Ӽ�(�б� ����, ���� ���� ��) (FILE_ATTRIBUTE_NORMAL : �ƹ��� �Ӽ� ���� �Ϲ� ����) 
		NULL);	// ������ ������ �Ӽ��� ������ ���ø� ����(�Ⱦ��Ŵϱ� NULL)

	if (INVALID_HANDLE_VALUE == hFile)
	{
		MessageBox(g_hWnd, _T("Load File"), L"Error", MB_OK);
		return;
	}

	DWORD	dwByte = 0;	// ����ó�� �� ����
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

	//MessageBox(g_hWnd, _T("Curve Line Load �Ϸ�"), L"����", MB_OK);
	delete[] pTmpArr;
}

void CLineManager::LoadData_Circle(const TCHAR* _path)
{
	HANDLE	hFile = CreateFile(_path,	// ���� ��ο� �̸��� �־���
		GENERIC_READ,		// ��� ����(GENERIC_WRITE : ���� GENERIC_READ : �б�)
		NULL,				// ���� ��� : ������ ���� �ִ� ���¿��� �ٸ� ���μ����� ������ ������ �� �㰡�� ���ΰ�(NULL�� ��� �������� �ʴ´�)
		NULL,				// ���� �Ӽ� : NULL�� ��� �⺻ ���� ����
		OPEN_EXISTING,		// ���� ���� ��� (CREATE_ALWAYS : ������ ���ٸ� �����ϰ�, ������ ���� �����ض�, OPEN_EXISTING : ������ ���� ��쿡�� �ε�)
		FILE_ATTRIBUTE_NORMAL,	// ���� �Ӽ�(�б� ����, ���� ���� ��) (FILE_ATTRIBUTE_NORMAL : �ƹ��� �Ӽ� ���� �Ϲ� ����) 
		NULL);	// ������ ������ �Ӽ��� ������ ���ø� ����(�Ⱦ��Ŵϱ� NULL)

	if (INVALID_HANDLE_VALUE == hFile)
	{
		MessageBox(g_hWnd, _T("Load File"), L"Error", MB_OK);
		return;
	}

	DWORD	dwByte = 0;	// ����ó�� �� ����
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
		); // ���� ������?
		/*ReadFile(hFile, &tmpCircleClass, sizeof(CCircleLine), &dwByte, nullptr);

		if (0 == dwByte)
			break;
		m_CircleLineList.push_back( 
			new CCircleLine(tmpCircleClass.Get_LineStartPoint().fX,
			tmpCircleClass.Get_LineStartPoint().fY,
				tmpCircleClass.Get_LineEndPoint().fX,
				tmpCircleClass.Get_LineEndPoint().fY )
		); // ���� ������?*/
	}

	//pTmpCircleClass->
	CloseHandle(hFile);
	//delete[] vTmpArr;
	//vTmpArr = nullptr;
	//Safe_Delete(pTmpCircleClass);
	//MessageBox(g_hWnd, _T("Curve Line Load �Ϸ�"), L"����", MB_OK);
}

void CLineManager::LoadData_Connected(const TCHAR* _path)
{
	HANDLE	hFile = CreateFile(_path,	// ���� ��ο� �̸��� �־���
		GENERIC_READ,		// ��� ����(GENERIC_WRITE : ���� GENERIC_READ : �б�)
		NULL,				// ���� ��� : ������ ���� �ִ� ���¿��� �ٸ� ���μ����� ������ ������ �� �㰡�� ���ΰ�(NULL�� ��� �������� �ʴ´�)
		NULL,				// ���� �Ӽ� : NULL�� ��� �⺻ ���� ����
		OPEN_EXISTING,		// ���� ���� ��� (CREATE_ALWAYS : ������ ���ٸ� �����ϰ�, ������ ���� �����ض�, OPEN_EXISTING : ������ ���� ��쿡�� �ε�)
		FILE_ATTRIBUTE_NORMAL,	// ���� �Ӽ�(�б� ����, ���� ���� ��) (FILE_ATTRIBUTE_NORMAL : �ƹ��� �Ӽ� ���� �Ϲ� ����) 
		NULL);	// ������ ������ �Ӽ��� ������ ���ø� ����(�Ⱦ��Ŵϱ� NULL)

	if (INVALID_HANDLE_VALUE == hFile)
	{
		MessageBox(g_hWnd, _T("Load File"), L"Error", MB_OK);
		return;
	}

	DWORD	dwByte = 0;	// ����ó�� �� ����

	int iPointCount = 0;
	

	while (true)
	{
		// �� � �ִ��� �а�
		ReadFile(hFile, &iPointCount, sizeof(int), &dwByte, nullptr);
		CConnectedLine _tmpConnectedLine;

		// �� ������ŭ �ݺ��ؼ� �����Ѵ�
		for (int i = 0; i < iPointCount; ++i)
		{
			LinePoint tTmpPoint{};
			// �� �ϳ� �а�
			ReadFile(hFile, &tTmpPoint, sizeof(LinePoint), &dwByte, nullptr);
			// �ӽ� Ŭ������ �� �߰�
			_tmpConnectedLine.AddPoint(tTmpPoint.fX, tTmpPoint.fY);
		}
		if (0 == dwByte)
			break;
		
		m_ConnectedLineList.push_back(new CConnectedLine(_tmpConnectedLine));
		_tmpConnectedLine.Release();
	}

	CloseHandle(hFile);

	//MessageBox(g_hWnd, _T("Curve Line Load �Ϸ�"), L"����", MB_OK);
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
		// refX, refY�� �� �� ���� �ִٸ�
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
	//float fHeighest = FLT_MAX; // ���� ���� ���̷� �ʱ�ȭ

	//// �ڱ� �� �� ���� ���� ���� ã��
	//for (auto& iter : m_StraightLineList)
	//{
	//	// iter�� ���� x���� ���̸� �Ѿ
	//	if (_fX < iter->Get_LeftX() || _fX > iter->Get_RightX())
	//		continue;

	//	//  y��ġ
	//	float fCurrentLineY0 = CMyMath::Bigger(iter->Get_MyLine().L_Point.fY,
	//		iter->Get_MyLine().R_Point.fY);

	//	// ���� ��
	//	float x0 = iter->Get_LeftX();
	//	float y0 = iter->Get_MyLine().L_Point.fY;
	//	//���� ��
	//	float x1 = iter->Get_RightX();
	//	float y1 = iter->Get_MyLine().R_Point.fY;

	//	//float fDelta = (y1 - y0) / (x1 - x0);

	//	// ������ ���������� ���� �� ������ y ��
	//	float fCurrentLineY = 
	//		CMyMath::LineEquation(x0, y0, x1, y1, _fX);

	//	// Ż ������ �ڽź��� ���� ������ �Ѿ
	//	if (fCurrentLineY < _refY)
	//		continue;

	//	if (fCurrentLineY < fHeighest)
	//		fHeighest = fCurrentLineY;
	//}

	//// ���� ������ ��ġ�� �״�ζ� ���� ���� ������ �ǹ�
	//if (fHeighest >= FLT_MAX)
	//	return false;

	//// ������ ���� ã������ �� y��ǥ�� ���ڰ� ������Ʈ
	//_refY = fHeighest;
	//return true;

	if (m_StraightLineList.empty() && m_CurvedLineList.empty())
		return false;
	
	float fHeighest = 1000; // ���� ���� ���̷� �ʱ�ȭ

	// ���� ���� �߿��� �ڱ� �� �� ���� ���� ���� ã��
	for (auto& iter : m_StraightLineList)
	{
		// iter�� ���� x���� ���̸� �Ѿ
		if (_fX < iter->Get_LeftX() || _fX > iter->Get_RightX())
			continue;

		//  y��ġ
		float fCurrentLineY0 = CMyMath::Bigger(iter->Get_MyLine().L_Point.fY,
			iter->Get_MyLine().R_Point.fY);

		// ���� ��
		float x0 = iter->Get_LeftX();
		float y0 = iter->Get_MyLine().L_Point.fY;
		//���� ��
		float x1 = iter->Get_RightX();
		float y1 = iter->Get_MyLine().R_Point.fY;

		//float fDelta = (y1 - y0) / (x1 - x0);

		// ������ ���������� ���� �� ������ y ��
		float fCurrentLineY =
			CMyMath::LineEquation(x0, y0, x1, y1, _fX);

		// �� �÷��̾� ��ġ ��
		if (fCurrentLineY < _fY) // ����
			continue;

		if (fCurrentLineY < fHeighest)
			fHeighest = fCurrentLineY;
	}

	for (auto& iter : m_CurvedLineList)
	{
		if (_fX < iter->Get_LeftX() || _fX > iter->Get_RightX())
			continue;
		// � �¿� �� ���̿� �ִ� ���


		//  y��ġ
		float fCurrentLineY = iter->Get_CurrentY(_fX);

		// Ż ������ �ڽź��� ���� ������ �Ѿ
		if (fCurrentLineY < _fY)
			continue;
		

		if (fCurrentLineY < fHeighest)
			fHeighest = fCurrentLineY;
	}

	// ���� ������ ��ġ�� �״�ζ� ���� ���� ������ �ǹ�
	if (fHeighest >= 1000)
		return false;

	// ������ ���� ã������ �� y��ǥ�� ���ڰ� ������Ʈ
	_refY = fHeighest;
	return true;
}

bool CLineManager::OnLine(float _fX, float _fY, float& _refY, bool& _bOnCurve)
{
	if (m_StraightLineList.empty() && m_CurvedLineList.empty())
		return false;

	float fHighest = 1000; // ���� ���� ���̷� �ʱ�ȭ

	// ���� ���� �߿��� �ڱ� �� �� ���� ���� ���� ã��
	for (auto& iter : m_StraightLineList)
	{
		// iter�� ���� x���� ���̸� �Ѿ
		if (_fX < iter->Get_LeftX() || _fX > iter->Get_RightX())
			continue;

		//  y��ġ
		float fCurrentLineY0 = CMyMath::Bigger(iter->Get_MyLine().L_Point.fY,
			iter->Get_MyLine().R_Point.fY);

		// ���� ��
		float x0 = iter->Get_LeftX();
		float y0 = iter->Get_MyLine().L_Point.fY;
		//���� ��
		float x1 = iter->Get_RightX();
		float y1 = iter->Get_MyLine().R_Point.fY;

		//float fDelta = (y1 - y0) / (x1 - x0);

		// ������ ���������� ���� �� ������ y ��
		float fCurrentLineY =
			CMyMath::LineEquation(x0, y0, x1, y1, _fX);

		// �� �÷��̾� ��ġ ��
		if (fCurrentLineY < _fY) // ����
			continue;

		if (fCurrentLineY < fHighest)
			fHighest = fCurrentLineY;
	}

	for (auto& iter : m_CurvedLineList)
	{
		if (_fX < iter->Get_LeftX() || _fX > iter->Get_RightX())
			continue;
		// � �¿� �� ���̿� �ִ� ���


		//  y��ġ
		float fCurrentLineY = iter->Get_CurrentY(_fX);

		// Ż ������ �ڽź��� ���� ������ �Ѿ
		if (fCurrentLineY < _fY)
			continue;


		if (fCurrentLineY < fHighest)
		{
			fHighest = fCurrentLineY;
			_bOnCurve = true;
		}
	}

	// ���� ������ ��ġ�� �״�ζ� ���� ���� ������ �ǹ�
	if (fHighest >= 1000)
		return false;

	// ������ ���� ã������ �� y��ǥ�� ���ڰ� ������Ʈ
	_refY = fHighest;
	return true;
}

bool CLineManager::OnLine(float _fX, float _fY, float& _refY, bool& _bOnCurve, CCurvedLine** _ppCurvedLineToTake)
{
	if (m_StraightLineList.empty() && m_CurvedLineList.empty())
		return false;

	float fHighest = 1000; // ���� ���� ���̷� �ʱ�ȭ

	// ���� ���� �߿��� �ڱ� �� �� ���� ���� ���� ã��
	for (auto& iter : m_StraightLineList)
	{
		// iter�� ���� x���� ���̸� �Ѿ
		if (_fX < iter->Get_LeftX() || _fX > iter->Get_RightX())
			continue;
		
		// ���� ��
		float x0 = iter->Get_LeftX();
		float y0 = iter->Get_MyLine().L_Point.fY;
		//���� ��
		float x1 = iter->Get_RightX();
		float y1 = iter->Get_MyLine().R_Point.fY;

		//float fDelta = (y1 - y0) / (x1 - x0);

		// ������ ���������� ���� �� ������ y ��
		float fCurrentLineY =
			CMyMath::LineEquation(x0, y0, x1, y1, _fX);

		// �� �÷��̾� ��ġ ��
		if (fCurrentLineY < _fY) // ����
			continue;

		if (fCurrentLineY < fHighest)
			fHighest = fCurrentLineY;
	}

	//for (auto iter : m_CurvedLineList)
	for (auto iter = m_CurvedLineList.begin(); iter != m_CurvedLineList.end(); ++iter)
	{
		if (_fX < (*iter)->Get_LeftX() || _fX >(*iter)->Get_RightX())
			continue;
		// � �¿� �� ���̿� �ִ� ���


		//  y��ġ
		float fCurrentLineY = (*iter)->Get_CurrentY(_fX);

		// Ż ������ �ڽź��� ���� ������ �Ѿ
		if (fCurrentLineY < _fY)
			continue;


		if (fCurrentLineY < fHighest)
		{
			fHighest = fCurrentLineY;
			_bOnCurve = true;
			*_ppCurvedLineToTake = (*iter);
		}
	}

	// ���� ������ ��ġ�� �״�ζ� ���� ���� ������ �ǹ�
	if (fHighest >= 1000)
		return false;

	// ������ ���� ã������ �� y��ǥ�� ���ڰ� ������Ʈ
	_refY = fHighest;
	return true;
}

bool CLineManager::OnLine(float _fX, float _fY, float& _refY, bool& _bOnCurve, CCurvedLine** _ppCurvedLineToTake,
	bool& _bOnCircle, CCircleLine** _ppCircleLineToTake)
{
	if (m_StraightLineList.empty() && m_CurvedLineList.empty() && m_CircleLineList.empty() && m_ConnectedLineList.empty())
		return false;

	float fHighest = 1000; // ���� ���� ���̷� �ʱ�ȭ

	// ���� ���� �߿��� �ڱ� �� �� ���� ���� ���� ã��
	for (auto& iter : m_StraightLineList)
	{
		if (m_StraightLineList.empty())
			break;

		// iter�� ���� x���� ���̸� �Ѿ
		if (_fX < iter->Get_LeftX() || _fX > iter->Get_RightX())
			continue;

		// ���� ��
		float x0 = iter->Get_LeftX();
		float y0 = iter->Get_MyLine().L_Point.fY;
		//���� ��
		float x1 = iter->Get_RightX();
		float y1 = iter->Get_MyLine().R_Point.fY;

		//float fDelta = (y1 - y0) / (x1 - x0);

		// ������ ���������� ���� �� ������ y ��
		float fCurrentLineY =
			CMyMath::LineEquation(x0, y0, x1, y1, _fX);

		// �� �÷��̾� ��ġ ��
		if (fCurrentLineY  < _fY) // ����
			continue;

		if (fCurrentLineY < fHighest)
			fHighest = fCurrentLineY;
	}

	//for (auto iter : m_CurvedLineList)
	for (auto iter = m_CurvedLineList.begin(); iter != m_CurvedLineList.end(); ++iter)
	{
		if (_fX < (*iter)->Get_LeftX() || _fX >(*iter)->Get_RightX())
			continue;
		// � �¿� �� ���̿� �ִ� ���


		//  y��ġ
		float fCurrentLineY = (*iter)->Get_CurrentY(_fX);

		// Ż ������ �ڽź��� ���� ������ �Ѿ
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

		// y��ġ
		float fCurrentLineY = (*iter)->Get_BottomY();

		// Ż ������ �ڽź��� ���� ������ �Ѿ
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

	// ���� ������ ��ġ�� �״�ζ� ���� ���� ������ �ǹ�
	if (fHighest >= 1000)
		return false;

	// ������ ���� ã������ �� y��ǥ�� ���ڰ� ������Ʈ
	_refY = fHighest;
	return true;
}

// player base
bool CLineManager::LineCheck(float _fX, float _fY, float& _refTargetY)
{
	if (m_StraightLineList.empty() && m_CurvedLineList.empty() && m_CircleLineList.empty() && m_ConnectedLineList.empty())
		return false;

	CObj* pPlayer = CObjManager::Get_Instance()->Get_Player();

	float fHighest = 10000; // ���� ���� ���̷� �ʱ�ȭ

	// ���� ���� �߿��� �ڱ� �� �� ���� ���� ���� ã��
	for (auto& iter : m_StraightLineList)
	{
		// iter�� ���� x���� ���̸� �Ѿ
		if (_fX < iter->Get_LeftX() || _fX >= iter->Get_RightX())
			continue;

		// ���� ��
		float x0 = iter->Get_LeftX();
		float y0 = iter->Get_MyLine().L_Point.fY;
		//���� ��
		float x1 = iter->Get_RightX();
		float y1 = iter->Get_MyLine().R_Point.fY;

		//float fDelta = (y1 - y0) / (x1 - x0);

		// ������ ���������� ���� �� ������ y ��
		float fCurrentLineY =
			CMyMath::LineEquation(x0, y0, x1, y1, _fX);

		// �� �÷��̾� ��ġ �� , ������ �ֱ�
		if (fCurrentLineY < _fY) // ����
			continue;

		if (fCurrentLineY < fHighest)
			fHighest = fCurrentLineY;
	}

	//for (auto iter : m_CurvedLineList)
	for (auto iter = m_CurvedLineList.begin(); iter != m_CurvedLineList.end(); ++iter)
	{
		if (_fX < (*iter)->Get_LeftX() || _fX >= (*iter)->Get_RightX())
			continue;
		// � �¿� �� ���̿� �ִ� ���


		//  y��ġ
		float fCurrentLineY = (*iter)->Get_CurrentY(_fX);

		// Ż ������ �ڽź��� ���� ������ �Ѿ
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

		// y��ġ
		float fCurrentLineY = (*iter)->Get_BottomY();

		// Ż ������ �ڽź��� ���� ������ �Ѿ
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

		// y��ġ
		float fCurrentLineY = (*iter)->LineEquation(_fX); // �� x��ġ�� ����

		// Ż ������ �ڽź��� ���� ������ �Ѿ
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

	// ���� ������ ��ġ�� �״�ζ� ���� ���� ������ �ǹ�
	if (fHighest >= 10000)
		return false; 

	// ���� �� ���� ã������ �� y��ǥ�� ���ڰ� ������Ʈ
	_refTargetY = fHighest;
	return true;
}



