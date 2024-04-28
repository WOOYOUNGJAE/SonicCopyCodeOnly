#pragma once
#include "Include.h"
#include "LineObjBase.h"
/**
 * \brief 선이라기보단 점들의 집합\n
 * LineObjBase 에 상속받지 않음.\n
 * Point 0, 1, 2중 1이 중간 점. \n
 * 항상 point0의 x가 가장 작아야 함
 */
class CCurvedLine : public CLineObjBase
{
public:
	CCurvedLine();
	CCurvedLine(float _x0, float _y0, float _x1, float _y1, float _x2, float _y2);
	CCurvedLine(Vector2 _pt0, Vector2 _pt1, Vector2 _pt2);
	~CCurvedLine() override;

public:
	// getter
	vector<Vector2*>& Get_AllDots() { return m_vecDots; }
	Vector2* Get_ThreePoints() { return m_vPointArr; }
	//Vector2* Get_ThreeDots() {return }

	virtual  float Get_LeftX() const override { return m_vPointArr[0].fX; }
	virtual  float Get_RightX() const override { return m_vPointArr[2].fX; }
	
	virtual  float Get_LeftY() const override { return m_vPointArr[0].fY; }
	virtual  float Get_RightY() const override { return m_vPointArr[2].fY; }

	// x에 해당하는 점의 y
	float Get_CurrentY(float _x);
	float Get_CurrentY(float _x, float& _yVelocity);
	float Get_CurrentY(float _x, float& _yVelocity, bool& _bAccelTriggered);

	// 가장 낮은 y좌표
	float Get_LowesetY();

	virtual void Render (const HDC _dc) override;

	bool IfOnY_AccelRange(float _fX); // y속도 높여준는 범위 안에 있니
	bool IfOnX_Range(float _fX); // 현재 커브 좌우 사이에 있니


	float MakeHigher();
private:/*
	Vector2 m_vPoint0;
	Vector2 m_vPoint1;
	Vector2 m_vPoint2;*/

	Vector2 m_vPointArr[3];

	vector<Vector2*> m_vecDots; // int 값만 저장

	float fWidth; // point0 ~ point1 거리

	// 이 사이에 있을 때 MakeHigher호출
	float m_fY_AccelStart;
	float m_fYAccelEnd;

	float fY_Accel; // Makehigh에서 높여주는 정도
};

