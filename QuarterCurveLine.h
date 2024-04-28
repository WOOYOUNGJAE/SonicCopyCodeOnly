#pragma once
#include "LineObjBase.h"
#include "Include.h"


class CQuarterCurveLine :
    public CLineObjBase
{
public:
	CQuarterCurveLine();
	CQuarterCurveLine(CQuarterCurveLine& rhs);
	virtual ~CQuarterCurveLine() override;

public:
	// getter
	virtual float Get_LeftX() const override {}
	virtual float Get_LeftY() const override {}
	virtual float Get_RightX() const override {}
	virtual float Get_RightY() const override {}

	virtual float Get_CurrentAngle(float _fInputX) override;

	void Initialize(){};
	void Initialize(float _centerX, float _centerY, float _fRadius, DIAGONAL_DIR _eDir);
	void Render(const HDC _dc) override;

private:
    DIAGONAL_DIR m_eDiagonalDir;

	vector<LinePoint> vecLinePoint;
	vector<MyLine>	vecLine; // ��ҿ� ����Ʈ�� ���� �ִٰ� initialize �ߵ��ϸ� �� ����

	vector<Vector2*> m_vecDots;

	float m_fRadius;
};

