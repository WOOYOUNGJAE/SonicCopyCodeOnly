#pragma once

struct LinePoint
{
	LinePoint() { ZeroMemory(this, sizeof(LinePoint)); }
	LinePoint(float _x, float _y): fX(_x), fY(_y) { }

	float fX, fY;
};

struct MyLine
{
	MyLine() { ZeroMemory(this, sizeof(MyLine)); }
	MyLine(LinePoint _L_Point, LinePoint _R_Point) :
	L_Point(_L_Point),
	R_Point(_R_Point) {}
	
	LinePoint L_Point;
	LinePoint R_Point;
};

enum LINE_ID
{
	LINE_ID_HORIZONTAL,
	LINE_ID_CONNECTED,
	LINE_ID_CIRCLE,
	LINE_ID_CURVE,
	LINE_ID_END,
};

/**
 * \brief 수평라인, 경사라인 등 직선라인\n
 * 곡선 라인은 이 클래스에 상속받지 않음\n
 * 디폴트는 직선수평라인
 */
class CLineObjBase
{
public:
	CLineObjBase(); // 기본
	CLineObjBase(const MyLine& _tLine); // 라인 인자로
	CLineObjBase(LinePoint _L, LinePoint _R); // 두 점 인자로, 라인 만들어줌
	virtual ~CLineObjBase();

public:
	// getter
	const MyLine& Get_MyLine() const { return m_tLine; }
	virtual float Get_LeftX() const { return m_tLine.L_Point.fX; }
	virtual float Get_LeftY() const { return m_tLine.L_Point.fY; }
	virtual float Get_RightX() const { return m_tLine.R_Point.fX; }
	virtual float Get_RightY() const { return m_tLine.R_Point.fY; }
	virtual LINE_ID Get_LineID() { return m_eLineID; }

	virtual float Get_CurrentAngle(float _fInputX);

	/**
	 * \brief x,y 가 주어졌을 때 선 위에 있는지 
	 */
	bool IfHasPoint(float _x, float _y);

	virtual void Render(const HDC _dc);

protected:
	MyLine m_tLine;

	LINE_ID m_eLineID;
};

 