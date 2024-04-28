#include "stdafx.h"
#include "LineEditor.h"
#include "KeyManager.h"
#include "DebugManager.h"
#include "ScrollManager.h"

/**
 * \brief update에서 단축키가 다른 에디터와 독립적이도록
 */
CLineEditor::CLineEditor() : m_eLineMode(LINE_MODE_END), m_bStart(true),
m_bConnectOn(true), m_iScrollX(0.f), m_iScrollY(0.f),
m_bConnectedHorizontal(false)
{
	ResetPoints();
	m_eLineMode = STRAIGHT_HORIZONTAL;
}

CLineEditor::~CLineEditor()
{
}

void CLineEditor::Initialize()
{
	CDebugManager::Get_Instance()->AddDebugLog(L"Connect", L"Connect Mode : ");
	CDebugManager::Get_Instance()->AddDebugLog(L"ConnectHorizontal", L"ConnectHorizontal : ");
}

void CLineEditor::Update()
{
	m_iScrollX = CScrollManager::Get_Instance()->Get_ScrollX();
	m_iScrollY = CScrollManager::Get_Instance()->Get_ScrollY();

	if (CKeyManager::Get_Instance()->Key_Pressing(VK_CONTROL))
	{
		if (CKeyManager::Get_Instance()->Key_Down('1'))
			m_eLineMode = STRAIGHT_HORIZONTAL;
		else if (CKeyManager::Get_Instance()->Key_Down('2'))
			m_eLineMode = STRAIGHT_VERTICAL;
		else if (CKeyManager::Get_Instance()->Key_Down('3'))
			m_eLineMode = LINE_CONNECTED;
		else if (CKeyManager::Get_Instance()->Key_Down('4'))
			m_eLineMode = LINE_CURVE;
		else if (CKeyManager::Get_Instance()->Key_Down('5'))
			m_eLineMode = LINE_CIRCLE;
		

		else if (CKeyManager::Get_Instance()->Key_Down('Z'))
			ResetPoints();
		else if (CKeyManager::Get_Instance()->Key_Down('C'))
			m_bConnectOn = !m_bConnectOn;
		else if (CKeyManager::Get_Instance()->Key_Down('H'))
			m_bConnectedHorizontal = !m_bConnectedHorizontal;
	}


	switch (m_eLineMode)
	{
	case STRAIGHT_HORIZONTAL:
		UpdateHorizontal();
		break;
	case STRAIGHT_VERTICAL: // 나중에 벽 만들 때 쓸지 의문
		break;
	case LINE_CONNECTED:
		UpdateConnected();
		break;
	case LINE_CURVE:
		UpdateCurve();
		break;
	case LINE_CIRCLE:
		UpdateCircle();
		break;
	}
	

	if (m_bConnectOn)
		CDebugManager::Get_Instance()->ChangeDebugLog(L"Connect", L"Connect Mode : true");
	else
		CDebugManager::Get_Instance()->ChangeDebugLog(L"Connect", L"Connect Mode : false");

	if (m_bConnectedHorizontal)
		CDebugManager::Get_Instance()->ChangeDebugLog(L"ConnectedHorizontal", L"ConnectedHorizontal : true");
	else
		CDebugManager::Get_Instance()->ChangeDebugLog(L"ConnectedHorizontal", L"ConnectedHorizontal : false");

}

void CLineEditor::LateUpdate()
{
}

void CLineEditor::Render(HDC _dc)
{
}

void CLineEditor::Release()
{
}

void CLineEditor::ResetPoints()
{
	ZeroMemory(m_tLinePoint, 2 * sizeof(LinePoint));
	ZeroMemory(m_vCurvePoints, 3 * sizeof(Vector2));
	m_bStart = true;
}

void CLineEditor::AdjustConnected(LONG& _refX, LONG& _refY)
{

	list<CLineObjBase*>& refStraightList =
		CLineManager::Get_Instance()->Get_StraightLineList();

	list<CCurvedLine*>& refCurveList =
		CLineManager::Get_Instance()->Get_CurveLineList();

	list<CCircleLine*>& refCircleList =
		CLineManager::Get_Instance()->Get_CircleLineList();

	list<CConnectedLine*>& refConnectedList =
		CLineManager::Get_Instance()->Get_ConnectedLineList();

	if (refStraightList.empty() && refCurveList.empty() && refCircleList.empty() && refConnectedList.empty())
	{
		// 아직 연결할 선조차 없는 경우 들어온 대로 내보냄
		m_bConnectOn = false;
		return;
	}

	float fNearestX = 0.f;
	float fNearestY = 0.f;
	float fNearestDistance = 0.f;

	if (!refStraightList.empty())
		fNearestDistance = CMyMath::GetDistanceSquare(_refX, _refY, refStraightList.front()->Get_LeftX(),
			refStraightList.front()->Get_LeftY());
	else if (!refConnectedList.empty())
		fNearestDistance = CMyMath::GetDistanceSquare(_refX, _refY, refConnectedList.front()->Get_LeftX(),
			refConnectedList.front()->Get_LeftY());
	else if (!refCurveList.empty())
		fNearestDistance = CMyMath::GetDistanceSquare(_refX, _refY, refCurveList.front()->Get_LeftX(),
			refCurveList.front()->Get_LeftY());
	else if (!refCircleList.empty())
		fNearestDistance = CMyMath::GetDistanceSquare(_refX, _refY, refCircleList.front()->Get_LeftX(),
			refCircleList.front()->Get_LeftY());
	else
		return;

	for (const auto& iter : refStraightList)
	{
		// 마우스포인트 - 수평선 왼쪽점 비교
		float fCurrentX = iter->Get_LeftX();
		float fCurrentY = iter->Get_LeftY();
		float fCurrentDistance = CMyMath::GetDistanceSquare(_refX, _refY, fCurrentX, fCurrentY);

		if (fCurrentDistance < fNearestDistance)
		{
			fNearestX = fCurrentX;
			fNearestY = fCurrentY;
			fNearestDistance = fCurrentDistance;
		}

		// 마우스 포인트 - 수평선 오른쪽 점 비교
		fCurrentX = iter->Get_RightX();
		fCurrentY = iter->Get_RightY();
		fCurrentDistance = CMyMath::GetDistanceSquare(_refX, _refY, fCurrentX, fCurrentY);

		if (fCurrentDistance < fNearestDistance)
		{
			fNearestX = fCurrentX;
			fNearestY = fCurrentY;
			fNearestDistance = fCurrentDistance;
		}
	}

	for (const auto& iter : refConnectedList)
	{
		// 마우스포인트 - 수평선 왼쪽점 비교
		float fCurrentX = iter->Get_LeftX();
		float fCurrentY = iter->Get_LeftY();
		float fCurrentDistance = CMyMath::GetDistanceSquare(_refX, _refY, fCurrentX, fCurrentY);

		if (fCurrentDistance < fNearestDistance)
		{
			fNearestX = fCurrentX;
			fNearestY = fCurrentY;
			fNearestDistance = fCurrentDistance;
		}

		// 마우스 포인트 - 수평선 오른쪽 점 비교
		fCurrentX = iter->Get_RightX();
		fCurrentY = iter->Get_RightY();
		fCurrentDistance = CMyMath::GetDistanceSquare(_refX, _refY, fCurrentX, fCurrentY);

		if (fCurrentDistance < fNearestDistance)
		{
			fNearestX = fCurrentX;
			fNearestY = fCurrentY;
			fNearestDistance = fCurrentDistance;
		}
	}


	for (const auto& iter : refCurveList)
	{
		// 마우스포인트 - 수평선 왼쪽점 비교
		float fCurrentX = iter->Get_LeftX();
		float fCurrentY = iter->Get_LeftY();
		float fCurrentDistance = CMyMath::GetDistanceSquare(_refX, _refY, fCurrentX, fCurrentY);

		if (fCurrentDistance < fNearestDistance)
		{
			fNearestX = fCurrentX;
			fNearestY = fCurrentY;
			fNearestDistance = fCurrentDistance;
		}

		// 마우스 포인트 - 수평선 오른쪽 점 비교
		fCurrentX = iter->Get_RightX();
		fCurrentY = iter->Get_RightY();
		fCurrentDistance = CMyMath::GetDistanceSquare(_refX, _refY, fCurrentX, fCurrentY);

		if (fCurrentDistance < fNearestDistance)
		{
			fNearestX = fCurrentX;
			fNearestY = fCurrentY;
			fNearestDistance = fCurrentDistance;
		}
	}

	for (const auto& iter : refCircleList)
	{
		// 마우스포인트 - 수평선 왼쪽점 비교
		float fCurrentX = iter->Get_LeftX();
		float fCurrentY = iter->Get_BottomY();
		float fCurrentDistance = CMyMath::GetDistanceSquare(_refX, _refY, fCurrentX, fCurrentY);

		if (fCurrentDistance < fNearestDistance)
		{
			fNearestX = fCurrentX;
			fNearestY = fCurrentY;
			fNearestDistance = fCurrentDistance;
		}

		// 마우스 포인트 - 수평선 오른쪽 점 비교
		fCurrentX = iter->Get_RightX();
		fCurrentDistance = CMyMath::GetDistanceSquare(_refX, _refY, fCurrentX, fCurrentY);

		if (fCurrentDistance < fNearestDistance)
		{
			fNearestX = fCurrentX;
			fNearestY = fCurrentY;
			fNearestDistance = fCurrentDistance;
		}
	}
	_refX = fNearestX;
	_refY = fNearestY;
}



//FSM
void CLineEditor::UpdateHorizontal()
{
	POINT pt{};

	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	pt.x -= m_iScrollX;
	pt.y -= m_iScrollY;

	// 클릭 했으면
	if (CKeyManager::Get_Instance()->Key_Down(VK_LBUTTON))
	{
		if (m_bStart)
		{
			if (m_bConnectOn)
			{
				 AdjustConnected(pt.x, pt.y);
			}

			m_tLinePoint[DIR_LEFT].fX = (float)pt.x;
			m_tLinePoint[DIR_LEFT].fY = (float)pt.y;
			m_bStart = false;
		}
		else
		{
			m_tLinePoint[DIR_RIGHT].fX = (float)pt.x;
			m_tLinePoint[DIR_RIGHT].fY = m_tLinePoint[DIR_LEFT].fY; // Left와 수평
		}
	}
	else if (CKeyManager::Get_Instance()->Key_Down(VK_RETURN)) // enter
	{
		// 저장
		CLineManager::Get_Instance()->AddStraightLine(m_tLinePoint[DIR_LEFT], m_tLinePoint[DIR_RIGHT]);
		ResetPoints();
	}
}

void CLineEditor::UpdateVertical()
{
	POINT pt{};

	pt.x -= m_iScrollX;
	pt.y -= m_iScrollY;
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);
}

void CLineEditor::UpdateConnected()
{
	POINT pt{};

	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	pt.x -= m_iScrollX;
	pt.y -= m_iScrollY;

	if (CKeyManager::Get_Instance()->Key_Down(VK_LBUTTON))
	{
		// 처음이면
		if (!m_tLinePoint[DIR_LEFT].fX && !m_tLinePoint[DIR_LEFT].fY)
		{

			if (m_bConnectOn)
			{
				AdjustConnected(pt.x, pt.y);
			}

			m_tLinePoint[DIR_LEFT].fX = (float)pt.x;
			m_tLinePoint[DIR_LEFT].fY = (float)pt.y;

			m_tOnePoint = m_tLinePoint[DIR_LEFT];
		}
		else
		{
			m_tLinePoint[DIR_RIGHT].fX = (float)pt.x;
			if (m_bConnectedHorizontal)
			{
				m_tLinePoint[DIR_RIGHT].fY = m_tLinePoint[DIR_LEFT].fY;
			}
			else
			{
				m_tLinePoint[DIR_RIGHT].fY = (float)pt.y;
			}
			m_tOnePoint = m_tLinePoint[DIR_RIGHT];				
		}
	}
	else if (CKeyManager::Get_Instance()->Key_Down('E')) // 현 그릇에 점 저장
	{
		m_connectedLine.AddPoint(m_tOnePoint.fX, m_tOnePoint.fY);
	}
	else if (CKeyManager::Get_Instance()->Key_Down(VK_RETURN)) // 최종 푸쉬백
	{
		// 점이 1개 이하라 선이 형성이 안되면 리턴
		if (m_connectedLine.Get_PointArr().size() <= 1)
			return;		

		m_connectedLine.Initialize();
		// 깊은 복사로 매니저에 전달한 다음 재사용 할 수 있도록 초기화
		CLineManager::Get_Instance()->AddConnectedLine(m_connectedLine);		
		m_connectedLine.ClearLines();
		m_connectedLine.ClearPoints();
		ResetPoints();
	}
}

void CLineEditor::UpdateCurve()
{
	POINT pt{};

	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	pt.x -= m_iScrollX;
	pt.y -= m_iScrollY;

	// 클릭 했으면
	if (CKeyManager::Get_Instance()->Key_Down(VK_LBUTTON))
	{
		if (m_bStart)
		{
			if (m_bConnectOn)
			{
				AdjustConnected(pt.x , pt.y);
			}
			//m_vCurveStart = Vector2((float)pt.x, (float)pt.y);
			m_vCurvePoints[0] = Vector2((float)pt.x, (float)pt.y);
			m_bStart = false;
		}
		else if (m_vCurvePoints[1].fX == 0.f && m_vCurvePoints[1].fY == 0.f)
		{
			//m_vCurveSecond = Vector2((float)pt.x, m_vCurveStart.fY);
			m_vCurvePoints[1] = Vector2((float)pt.x, m_vCurvePoints[0].fY);
			//m_bStart = false;
		}
		else
		{
			if (pt.y < m_vCurvePoints[1].fY) // 마지막으로 찍은 점이 중간 점보다 위에 있으면
			{
				m_vCurvePoints[2] = Vector2(m_vCurvePoints[1].fX, m_vCurvePoints[1].fY -
					abs(m_vCurvePoints[0].fX - m_vCurvePoints[1].fX));				
			}
			else
			{
				m_vCurvePoints[2] = Vector2(m_vCurvePoints[1].fX, m_vCurvePoints[1].fY +
					abs(m_vCurvePoints[0].fX - m_vCurvePoints[1].fX));
			}
			m_bStart = false;
		}
	}
	else if (CKeyManager::Get_Instance()->Key_Down(VK_RETURN)) // enter
	{
		if ((m_vCurvePoints[1].fX == 0.f && m_vCurvePoints[1].fY == 0.f) || (m_vCurvePoints[2].fX == 0.f && m_vCurvePoints[2].fY == 0.f))
			return;

		CLineManager::Get_Instance()->AddCurvedLine(m_vCurvePoints[0], m_vCurvePoints[1], m_vCurvePoints[2]);
		ResetPoints();
	}
}

void CLineEditor::UpdateCircle()
{

	POINT pt{};

	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	pt.x -= m_iScrollX;
	pt.y -= m_iScrollY;

	// 클릭 했으면
	if (CKeyManager::Get_Instance()->Key_Down(VK_LBUTTON))
	{
		if (m_bStart)
		{
			if (m_bConnectOn)
			{
				AdjustConnected(pt.x, pt.y);
			}
			//m_vCurveStart = Vector2((float)pt.x, (float)pt.y);
			m_vCirclePoints[0] = Vector2((float)pt.x, (float)pt.y);
			m_bStart = false;
		}
		else
		{
			m_vCirclePoints[1] = Vector2((float)pt.x, m_vCirclePoints[0].fY);
			m_bStart = false;
		}
	}
	else if (CKeyManager::Get_Instance()->Key_Down(VK_RETURN)) // enter
	{
		CLineManager::Get_Instance()->AddCircleLine(m_vCirclePoints[0].fX, m_vCirclePoints[0].fY,
			m_vCirclePoints[1].fX, m_vCirclePoints[1].fY);
		ResetPoints();
	}
}
