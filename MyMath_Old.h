#pragma once
struct Vector2
{
	Vector2() : fX(0), fY(0) {}
	Vector2(float _x, float _y) : fX(_x), fY(_y) {}
	Vector2(Vector2& _rhs) : fX(_rhs.fX), fY(_rhs.fY) {}
	Vector2(Vector2&& _rhs) : fX(_rhs.fX), fY(_rhs.fY) {}

	float fX;
	float fY;

	float magnitude = sqrtf(fX * fX + fY * fY);

	void Normalize()
	{
		if (fX == 0 || fY == 0)
			return;
		fX /= magnitude;
		fY /= magnitude;
	}

	Vector2 operator =(Vector2&& _vec)
	{
		fX = _vec.fX;
		fY = _vec.fY;
		return *this;
	}
	Vector2 operator =(Vector2& _vec)
	{
		fX = _vec.fX;
		fY = _vec.fY;
		return *this;
	}

	Vector2 operator +(Vector2 _vec)
	{
		/*fX += _vec.fX;
		fY += _vec.fY;
		return *this;*/
		return Vector2(fX + _vec.fX, fY + _vec.fY);
	}
	Vector2 operator -(Vector2 _vec)
	{
		/*fX -= _vec.fX;
		fY -= _vec.fY;
		return *this;*/
		return Vector2(fX - _vec.fX, fY - _vec.fY);
	}

	template <typename  T>
	Vector2 operator *(T _t)
	{
		/*fX *= _t;
		fY *= _t;
		return *this;*/
		return Vector2(fX * _t, fY * _t);
	}
};

namespace MyMath
{
	/**
	 * \brief 제곱근 처리가 되지 않은 거리
	 * \return
	 */

	static float Inner_Product(Vector2 _Dst, Vector2 _Src);

	static float GetDistanceSquare(float _x1, float _y1, float _x2, float _y2);
		
	static float GetDistanceSquare(Vector2 _v1, Vector2 _v2);

	static float GetDistance(float _x1, float _y1, float _x2, float _y2);
	static float GetDistance(Vector2 _v1, Vector2 _v2);

	static float Lerp(float _f1, float _f2, float _t);

	static Vector2 Lerp(Vector2 _Dst, Vector2 _Src, float _fRate);

	static Vector2 vecLerp(Vector2 _v1, Vector2 _v2, float _t);


	/**
	 * \brief
	 * \param _pVecArr 크기 3인 Vector2 배열
	 * \param _t 비율
	 * \return
	 */
	static Vector2 QuadraticBezier(Vector2 _vPoint0, Vector2 _vPoint1, Vector2 _vPoint2, float _t);


}