#include "stdafx.h"
#include "CurvedLine.h"
#include "ScrollManager.h"
#include "ObjManager.h"
#include "Player.h"

CCurvedLine::CCurvedLine() : fY_Accel(8), fWidth(0.f)
//CCurvedLine::CCurvedLine() : fY_Accel(0), fWidth(0.f)
{
	//CMyMath::Get_Instance()->FillBezierPoints(m_vecDots, m_vPoint0, m_vPoint1, m_vPoint2);
	CMyMath::Get_Instance()->FillBezierPoints(m_vecDots, m_vPointArr[0], m_vPointArr[1], m_vPointArr[2]);
	fWidth = m_vPointArr[1].fX - m_vPointArr[0].fX;
	m_fY_AccelStart = m_vPointArr[0].fX + fWidth * 0.5f;
	//m_fYAccelEnd = m_vPointArr[0].fX + fWidth * 0.8f;
	m_fYAccelEnd = m_vPointArr[1].fX;

	m_eLineID = LINE_ID_CURVE;
}

CCurvedLine::CCurvedLine(float _x0, float _y0, float _x1, float _y1, float _x2, float _y2) : fY_Accel(8), fWidth(0.f)
//CCurvedLine::CCurvedLine(float _x0, float _y0, float _x1, float _y1, float _x2, float _y2) : fY_Accel(0), fWidth(0.f)
{
	m_vPointArr[0].fX = _x0; m_vPointArr[0].fY = _y0;
	m_vPointArr[1].fX = _x1; m_vPointArr[1].fY = _y1;
	m_vPointArr[2].fX = _x2; m_vPointArr[2].fY = _y2;
	CMyMath::Get_Instance()->FillBezierPoints(m_vecDots, m_vPointArr[0], m_vPointArr[1], m_vPointArr[2]);


	fWidth = m_vPointArr[1].fX - m_vPointArr[0].fX;
	m_fY_AccelStart = m_vPointArr[0].fX + fWidth * 0.5f;
	m_fYAccelEnd = m_vPointArr[1].fX;
	m_eLineID = LINE_ID_CURVE;
}

CCurvedLine::CCurvedLine(Vector2 _pt0, Vector2 _pt1, Vector2 _pt2) : fY_Accel(8), fWidth(0.f)
//CCurvedLine::CCurvedLine(Vector2 _pt0, Vector2 _pt1, Vector2 _pt2) : fY_Accel(0), fWidth(0.f)
{
	m_vPointArr[0].fX = _pt0.fX; m_vPointArr[0].fY = _pt0.fY;
	m_vPointArr[1].fX = _pt1.fX; m_vPointArr[1].fY = _pt1.fY;
	m_vPointArr[2].fX = _pt2.fX; m_vPointArr[2].fY = _pt2.fY;
	//CMyMath::Get_Instance()->FillBezierPoints(m_vecDots, m_vPoint0, m_vPoint1, m_vPoint2);
	CMyMath::Get_Instance()->FillBezierPoints(m_vecDots, m_vPointArr[0], m_vPointArr[1], m_vPointArr[2]);


	fWidth = m_vPointArr[1].fX - m_vPointArr[0].fX;
	m_fY_AccelStart = m_vPointArr[0].fX + fWidth * 0.5f;
	m_fYAccelEnd = m_vPointArr[1].fX;
	m_eLineID = LINE_ID_CURVE;
}

CCurvedLine::~CCurvedLine()
{
}

float CCurvedLine::Get_CurrentY(float _x)
{
	/*if (_x < m_vPoint0.fX || _x > m_vPoint2.fX)
		return FLT_MAX;*/

	if (_x < m_vPointArr[0].fX || _x > m_vPointArr[2].fX)
		return FLT_MAX;

	if (_x < m_fY_AccelStart)
		dynamic_cast<CPlayer*>(CObjManager::Get_Instance()->Get_Player())
		->Set_m_xVelocityMinus(1.f);
	int iX = static_cast<int>(_x);

	/*if (m_fY_AccelStart <= iX && iX <= m_fYAccelEnd)
	{
		MakeHigher();
	}*/

	for (int i = 0; i < m_vecDots.size() - 1; ++i)
	{
		if (m_vecDots[i]->fX <= iX && iX <= m_vecDots[i + 1]->fX)
			return m_vecDots[i]->fY;
	}

	return FLT_MAX;// *0.5f;
}

float CCurvedLine::Get_CurrentY(float _x, float& _yVelocity)
{
	if (_x < m_vPointArr[0].fX || _x > m_vPointArr[2].fX)
		return FLT_MAX;

	if (_x < m_fY_AccelStart)
		dynamic_cast<CPlayer*>(CObjManager::Get_Instance()->Get_Player())
		->Set_m_xVelocityMinus(1.f);

	int iX = static_cast<int>(_x);

	if (m_fY_AccelStart <= iX && iX <= m_fYAccelEnd)
	{
		_yVelocity += fY_Accel * 0.2f;
	}

	for (int i = 0; i < m_vecDots.size() - 1; ++i)
	{
		if (m_vecDots[i]->fX <= iX && iX <= m_vecDots[i + 1]->fX)
			return m_vecDots[i]->fY;
	}

	return FLT_MAX;// *0.5f;
}

float CCurvedLine::Get_CurrentY(float _x, float& _yVelocity, bool& _bAccelTriggered)
{
	if (_x < m_vPointArr[0].fX || _x > m_vPointArr[2].fX)
		return FLT_MAX;

	if (_x < m_fY_AccelStart)
		dynamic_cast<CPlayer*>(CObjManager::Get_Instance()->Get_Player())
		->Set_m_xVelocityMinus(1.f);
	int iX = static_cast<int>(_x);

	if (m_fY_AccelStart <= iX && iX <= m_fYAccelEnd)
	{
		_yVelocity = fY_Accel;
		_bAccelTriggered = true;
	}

	for (int i = 0; i < m_vecDots.size() - 1; ++i)
	{
		if (m_vecDots[i]->fX <= iX && iX <= m_vecDots[i + 1]->fX)
			return m_vecDots[i]->fY;
	}

	return FLT_MAX;// *0.5f;
}

float CCurvedLine::Get_LowesetY()
{
	// 값이 큰 게 더 낮은거
	return CMyMath::Bigger(m_vPointArr[0].fY, m_vPointArr[2].fY);
}

void CCurvedLine::Render(const HDC _dc)
{
	int iScrollX = CScrollManager::Get_Instance()->Get_ScrollX();

	for(int i = 0; i < m_vecDots.size() - 1; ++i)
	{
		MoveToEx(_dc,
			m_vecDots[i]->fX + iScrollX,
			m_vecDots[i]->fY, nullptr);

		LineTo(_dc, m_vecDots[i + 1]->fX + iScrollX,
			m_vecDots[i + 1]->fY);
	}
}

bool CCurvedLine::IfOnY_AccelRange(float _fX)
{
	return m_fY_AccelStart <= _fX && _fX <= m_fYAccelEnd;
}

bool CCurvedLine::IfOnX_Range(float _fX)
{
	//if (m_vPointArr[0])
	return true;
}
