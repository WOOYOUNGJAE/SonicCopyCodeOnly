#pragma once

struct Vector2
{
	Vector2() : fX(0), fY(0) {}
	Vector2(float _x, float _y) : fX(_x), fY(_y) { magnitude = sqrtf(fX * fX + fY * fY); }
	Vector2(Vector2& _rhs) : fX(_rhs.fX), fY(_rhs.fY) { magnitude = sqrtf(fX * fX + fY * fY); }
	Vector2(Vector2&& _rhs) : fX(_rhs.fX), fY(_rhs.fY) { magnitude = sqrtf(fX * fX + fY * fY); }

	float fX;
	float fY;

	float magnitude;// = sqrtf(fX * fX + fY * fY);;

	void Normalize()
	{
		if (fX == 0)
		{
			fY /= abs(fY);
			return;
		}
		else if (fY == 0)
		{
			fX /= abs(fX);
			return;
		}

		fX /= magnitude;
		fY /= magnitude;
	}

	Vector2 operator =(Vector2&& _vec)
	{
		fX = _vec.fX;
		fY = _vec.fY;
		magnitude = sqrtf(fX * fX + fY * fY);
		return *this;
	}
	Vector2 operator =(Vector2& _vec)
	{
		fX = _vec.fX;
		fY = _vec.fY;
		magnitude = sqrtf(fX * fX + fY * fY);
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

class CMyMath
{
public:
	CMyMath();
	~CMyMath();

public:
	/**
	 * \brief 제곱근 처리가 되지 않은 거리
	 * \return
	 */
	static float GetDistanceSquare(float _x1, float _y1, float _x2, float _y2);
	static float GetDistanceSquare(Vector2 _v1, Vector2 _v2);

	static float GetDistance(float _x1, float _y1, float _x2, float _y2);
	static float GetDistance(Vector2 _v1, Vector2 _v2);

	static void fClamp(float& _refInput, float _fMin, float _fMax);

	static float Bigger(float _f0, float _f1);
	static float Smaller(float _f0, float _f1);

	/**
	 * \brief 
	 * \param _x1 좌측 x
	 * \param _y1 우측 y
	 * \param _x2 좌측 x
	 * \param _y2 우측 y
	 * \param _fInput 대입값
	 * \return 대입값에 따른 결과값
	 */
	static float LineEquation(float _x1, float _y1, float _x2, float _y2, float _fInput);
	static bool IfOnStraightLine(float _x1, float _y1, float _x2, float _y2, float _fInputX, float _fInputY);

	/**
	 * \brief 한 점의 y와 그 점을 직선의 x로 대입했을 때 y 사이의 차
	 */
	static float DifferenceBetweenPointAndLinePoint(float _x1, float _y1, float _x2, float _y2, float _fInputX, float _fInputY);

	static float Inner_Product(Vector2 _Dst, Vector2 _Src);

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
	
	void FillBezierPoints(vector<Vector2*>& _refVecVec2, Vector2& _pt0, Vector2& _pt1, Vector2& _pt2);



	// SingleTon
	static CMyMath* Get_Instance()
	{
		if (m_pInstance == nullptr)
		{
			m_pInstance = new CMyMath;
		}
		return m_pInstance;
	}
	static void Destroy_Instance()
	{
		if (m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}
private:
	static CMyMath* m_pInstance;
};

