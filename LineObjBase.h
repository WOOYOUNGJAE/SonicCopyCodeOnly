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
 * \brief �������, ������ �� ��������\n
 * � ������ �� Ŭ������ ��ӹ��� ����\n
 * ����Ʈ�� �����������
 */
class CLineObjBase
{
public:
	CLineObjBase(); // �⺻
	CLineObjBase(const MyLine& _tLine); // ���� ���ڷ�
	CLineObjBase(LinePoint _L, LinePoint _R); // �� �� ���ڷ�, ���� �������
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
	 * \brief x,y �� �־����� �� �� ���� �ִ��� 
	 */
	bool IfHasPoint(float _x, float _y);

	virtual void Render(const HDC _dc);

protected:
	MyLine m_tLine;

	LINE_ID m_eLineID;
};

 