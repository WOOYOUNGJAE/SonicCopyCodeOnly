#include "stdafx.h"
#include "MyMath.h"

CMyMath* CMyMath::m_pInstance = nullptr;

CMyMath::CMyMath()
{
}

CMyMath::~CMyMath()
{
}

float CMyMath::GetDistanceSquare(float _x1, float _y1, float _x2, float _y2)
{
	float fWidth = _x1 - _x2;
	float fHeight = _y1 - _y2;
	return fWidth * fWidth + fHeight * fHeight;
}

float CMyMath::GetDistanceSquare(Vector2 _v1, Vector2 _v2)
{
	float fWidth = _v1.fX - _v2.fX;
	float fHeight = _v1.fY - _v2.fY;
	return fWidth * fWidth + fHeight * fHeight;
}

float CMyMath::GetDistance(float _x1, float _y1, float _x2, float _y2)
{
	return sqrtf(GetDistanceSquare(_x1, _y1, _x2, _y2));
}

float CMyMath::GetDistance(Vector2 _v1, Vector2 _v2)
{
	return sqrtf(GetDistanceSquare(_v1, _v2));
}

void CMyMath::fClamp(float& _refInput, float _fMin, float _fMax)
{
	if (_refInput < _fMin)
	{
		_refInput = _fMin;
		return;
	}

	if (_refInput > _fMax)	
		_refInput = _fMax;
	

}

float CMyMath::Bigger(float _f0, float _f1)
{
	if (_f0 >= _f1)
		return _f0;
	else
		return _f1;
}

float CMyMath::Smaller(float _f0, float _f1)
{
	if (_f0 <= _f1)
		return _f0;
	else
		return _f1;
}

float CMyMath::LineEquation(float _x1, float _y1, float _x2, float _y2, float _fInput)
{
	float fDelta = (_y2 - _y1) / (_x2 - _x1);

	return fDelta * (_fInput - _x1) + _y1;
}

bool CMyMath::IfOnStraightLine(float _x1, float _y1, float _x2, float _y2, float _fInputX, float _fInputY)
{
	float fDelta = (_y2 - _y1) / (_x2 - _x1);	

	return  ((fDelta * (_fInputX - _x1) + _y1)) == _fInputY;
}

float CMyMath::DifferenceBetweenPointAndLinePoint(float _x1, float _y1, float _x2, float _y2, float _fInputX, float _fInputY)
{
	float fDelta = (_y2 - _y1) / (_x2 - _x1);
	float fLineY = fDelta * (_fInputX - _x1) + _y1;
	return abs(fLineY - _fInputY);
}

float CMyMath::Inner_Product(Vector2 _Dst, Vector2 _Src)
{
	return _Dst.fX * _Src.fX + _Dst.fX * _Src.fY;
}

float CMyMath::Lerp(float _f1, float _f2, float _t)
{
	return _f1 + (_f2 - _f1) * _t;
}

Vector2 CMyMath::Lerp(Vector2 _Dst, Vector2 _Src, float _fRate)
{
	return _Dst + (_Src - _Dst) * _fRate;
}

Vector2 CMyMath::vecLerp(Vector2 _v1, Vector2 _v2, float _t)
{
	Vector2 vecResult;
	vecResult = Vector2(_v1 + (_v2 - _v1) * _t);
	return 	vecResult;
}

Vector2 CMyMath::QuadraticBezier(Vector2 _vPoint0, Vector2 _vPoint1, Vector2 _vPoint2, float _t)
{
	Vector2 vMedPoint0 = vecLerp(_vPoint0, _vPoint1, _t);
	Vector2 vMedPoint1 = vecLerp(_vPoint1, _vPoint2, _t);

	return vecLerp(vMedPoint0, vMedPoint1, _t);
}

void CMyMath::FillBezierPoints(vector<Vector2*>& _refVecVec2, Vector2& _pt0,  Vector2& _pt1, Vector2& _pt2)
{
	float t = 0.f;
	Vector2 vBezPrev = _pt0;
	Vector2 vBezNext = _pt0;

	while (t <= 1)
	{
		vBezNext =
			QuadraticBezier(_pt0, _pt1, _pt2, t);
		vBezPrev = vBezNext;
		_refVecVec2.push_back(new Vector2(int(vBezPrev.fX), (int)vBezPrev.fY));
		t += 0.02f;
	}
}
