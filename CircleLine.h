#pragma once
#include "Include.h"
#include "LineObjBase.h"

class CCircleLine : public CLineObjBase
{
public:
	CCircleLine();
	CCircleLine(float _fLineStartX, float _fLineStartY, 
		float _fLineEndX, float _fLineEndY);
	virtual ~CCircleLine() override;


public:
	// getter
	virtual float Get_LeftX() const override { return m_vLineStart.fX; }
	virtual float Get_LeftY() const override { return m_vLineStart.fY; }
	virtual float Get_RightX() const override { return m_vLineEnd.fX; }
	virtual float Get_RightY() const override { return m_vLineStart.fY; }
	virtual float Get_CurrentAngle(float _fInputX) override;
	float Get_BottomY() { return m_vLineStart.fY; }
	
	const Vector2& Get_LineStartPoint() { return m_vLineStart; }
	const Vector2& Get_LineEndPoint() { return m_vLineEnd; }
	const Vector2& Get_CircleEndPoint() { return m_vCircleEnd; }

	// x�� �ش��ϴ� ���� y
	float Get_CurrentY(float& _refX, float _xVelocity);
	float Get_CurrentY(float& _refX, float _xVelocity, bool& _bRidingCircle);
	float Get_CurrentY(float& _refX, float _posY , float _xVelocity, bool& _bRidingCircle);
	//float Get_CurrentY(float _x, float& _yVelocity);
	//float Get_CurrentY(float _x, float& _yVelocity, bool& _bAccelTriggered);
	//setter

	void Initialize();
	void Initialize(float _fLineStartX, float _fLineStartY,
		float _fLineEndX, float _fLineEndY);
	virtual void Render (const HDC _dc) override;

	void ResetReturningIndex() { m_iReturningIndex = 0; }

private:
	vector<Vector2*> m_vecDots;

	vector<Vector2*> m_vecDotsRD; // ������ �Ʒ� ��п�
	vector<Vector2*> m_vecDotsRU;
	vector<Vector2*> m_vecDotsLU;
	vector<Vector2*> m_vecDotsLD;



	Vector2 m_vLineStart;
	Vector2 m_vCircleStart;
	Vector2 m_vCircleEnd;
	Vector2 m_vTopPoint;
	Vector2 m_vLineEnd;

	float m_fRadius; // ������
	/**
	 * \brief true�� ��Ÿ��, flase�� ������ ����
	 * top�� ���� false��
	 */
	bool m_bOnCircleWay;
	int m_iReturningIndex; // ���� �����ִ� ��ġ�� index
	int m_iIndexSpeed;

	MyLine tLeftLine;
	MyLine tRightLine;
};

