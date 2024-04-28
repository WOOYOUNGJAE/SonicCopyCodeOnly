#include "stdafx.h"
#include "MyMath.h"

static float GetDistanceSquare(float _x1, float _y1, float _x2, float _y2)
{
	float fWidth = _x1 - _x2;
	float fHeight = _y1 - _y2;
	return fWidth * fWidth + fHeight * fHeight;
}


static float GetDistanceSquare(Vector2 _v1, Vector2 _v2)
{
	float fWidth = _v1.fX - _v2.fX;
	float fHeight = _v1.fY - _v2.fY;
	return fWidth * fWidth + fHeight * fHeight;
}


static float GetDistance(float _x1, float _y1, float _x2, float _y2)
{
	return sqrtf(GetDistanceSquare(_x1, _y1, _x2, _y2));
}

static float GetDistance(Vector2 _v1, Vector2 _v2)
{
	return sqrtf(GetDistanceSquare(_v1, _v2));
}

static float Lerp(float _f1, float _f2, float _t)
{
	return _f1 + (_f2 - _f1) * _t;
}

static Vector2 vecLerp(Vector2 _v1, Vector2 _v2, float _t)
{
	Vector2 vecResult;
	vecResult = Vector2(_v1 + (_v2 - _v1) * _t);
	return 	vecResult;
}
static Vector2 Lerp(Vector2 _Dst, Vector2 _Src, float _fRate)
{
	return _Dst + (_Src - _Dst) * _fRate;
}

static float Inner_Product(Vector2 _Dst, Vector2 _Src)
{
	return _Dst.fX * _Src.fX + _Dst.fX * _Src.fY;
}


Vector2 QuadraticBezier(Vector2 _vPoint0, Vector2 _vPoint1, Vector2 _vPoint2, float _t)
{
	Vector2 vMedPoint0 = vecLerp(_vPoint0, _vPoint1, _t);
	Vector2 vMedPoint1 = vecLerp(_vPoint1, _vPoint2, _t);

	return vecLerp(vMedPoint0, vMedPoint1, _t);
}