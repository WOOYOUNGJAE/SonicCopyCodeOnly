#include "stdafx.h"
#include "ConnectedLine.h"

#include "ObjManager.h"
#include "ScrollManager.h"

CConnectedLine::CConnectedLine()
{
	m_eLineID = LINE_ID_CONNECTED;
}

CConnectedLine::CConnectedLine(CConnectedLine& rhs)
{
	vecLinePoint.reserve(rhs.vecLinePoint.capacity());
	for (auto iter : rhs.vecLinePoint)
	{
		vecLinePoint.push_back(LinePoint(iter.fX, iter.fY));
	}
	m_eLineID = LINE_ID_CONNECTED;
}

//CConnectedLine::CConnectedLine(CConnectedLine&& rhs) noexcept
//{
//	vecLinePoint.reserve(rhs.vecLinePoint.capacity());
//	for (auto iter : rhs.vecLinePoint)
//	{
//		vecLinePoint.push_back(LinePoint(iter.fX, iter.fY));
//	}
//}

CConnectedLine::~CConnectedLine()
{
}

float CConnectedLine::Get_CurrentAngle(float _fInputX)
{
	if (_fInputX < vecLinePoint.front().fX || _fInputX > vecLinePoint.back().fX)
		return 0.f; // 수평

	MyLine tCurrentLine = ReturnCurrentLine(_fInputX);

	float fResult = atan2f(tCurrentLine.L_Point.fY - tCurrentLine.R_Point.fY, tCurrentLine.R_Point.fX - tCurrentLine.L_Point.fX);
	fResult *= 180.f / PI;
	if (fResult < 0)
		fResult += 360;
	return fResult;
}

void CConnectedLine::Initialize()
{
	for (auto iter = vecLinePoint.begin(); iter != vecLinePoint.end() - 1;
		++iter)
	{
		vecLine.push_back(MyLine(*iter, *(iter + 1)));
	}
}

void CConnectedLine::Render(const HDC _dc)
{
	/*int iScrollX = CScrollManager::Get_Instance()->Get_ScrollX();
	int iScrollY = CScrollManager::Get_Instance()->Get_ScrollY();
	

	for (auto iter = vecLinePoint.begin(); iter != vecLinePoint.end() - 1;
		++iter)
	{
		MoveToEx(_dc, int(iter->fX + iScrollX), int(iter->fY) + iScrollY, nullptr);
		LineTo(_dc, (iter + 1)->fX + iScrollX, (iter + 1)->fY + iScrollY);
	}*/
}

void CConnectedLine::AddPoint(float _newPointX, float _newPointY)
{
	vecLinePoint.push_back(LinePoint(_newPointX, _newPointY));
}

float CConnectedLine::LineEquation(float _fInputX)
{
	if (_fInputX < vecLinePoint.front().fX || _fInputX > vecLinePoint.back().fX)
		return CObjManager::Get_Instance()->Get_Player()->Get_Pos().fY; // 위치그대로 반환

	MyLine tCurrentLine = ReturnCurrentLine(_fInputX);

	return CMyMath::LineEquation(tCurrentLine.L_Point.fX, tCurrentLine.L_Point.fY,
		tCurrentLine.R_Point.fX, tCurrentLine.R_Point.fY, _fInputX);
}

MyLine CConnectedLine::ReturnCurrentLine(float _fInputX)
{
	if (_fInputX < vecLinePoint.front().fX || _fInputX > vecLinePoint.back().fX)
		return MyLine(LinePoint(0,0), LinePoint(0,0));

	for (int i = 0; i < vecLinePoint.size() - 1; ++i)
	{
		if (_fInputX < vecLinePoint[i + 1].fX)
			return vecLine[i];
	}
}

Vector2 CConnectedLine::ReturnTangentVector(float _fInputX)
{
	MyLine tCurrentLine = ReturnCurrentLine(_fInputX);

	Vector2 vFirst = Vector2(tCurrentLine.L_Point.fX, tCurrentLine.L_Point.fY);
	Vector2 vSecond = Vector2(tCurrentLine.R_Point.fX, tCurrentLine.R_Point.fY);
	Vector2 vResult = (vSecond - vFirst);
	vResult.Normalize();
	return  vResult;
}
