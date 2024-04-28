#include "stdafx.h"
#include "LineObjBase.h"
#include "ScrollManager.h"

CLineObjBase::CLineObjBase() : m_eLineID(LINE_ID_HORIZONTAL)
{
}

CLineObjBase::CLineObjBase(const MyLine& _tLine) : m_tLine(_tLine), m_eLineID(LINE_ID_HORIZONTAL)
{
}

CLineObjBase::CLineObjBase(LinePoint _L, LinePoint _R) :
	m_tLine(_L, _R)
{
}

CLineObjBase::~CLineObjBase()
{
}

float CLineObjBase::Get_CurrentAngle(float _fInputX)
{
	return -1.f;
}

void CLineObjBase::Render(const HDC _dc)
{
	int iScrollX = CScrollManager::Get_Instance()->Get_ScrollX();
	int iScrollY = CScrollManager::Get_Instance()->Get_ScrollY();

	MoveToEx(_dc,
		static_cast<int>(m_tLine.L_Point.fX + iScrollX),
		static_cast<int>(m_tLine.L_Point.fY + iScrollY), 
		nullptr);

	LineTo(_dc, 
		static_cast<int>(m_tLine.R_Point.fX + iScrollX),
		static_cast<int>(m_tLine.R_Point.fY + iScrollY));
}
