#pragma once
#include "Include.h"
#include "LineObjBase.h"

/**
 * \brief 왼쪽이 첫 점
 */
class CConnectedLine : public CLineObjBase
{
public:
	CConnectedLine();
	CConnectedLine(CConnectedLine& rhs);
	/*CConnectedLine(CConnectedLine&& rhs) noexcept;*/
	virtual ~CConnectedLine() override;


public:
	// getter
	virtual float Get_LeftX() const override { return vecLinePoint.front().fX; }
	virtual float Get_LeftY() const override { return vecLinePoint.front().fY; }
	virtual float Get_RightX() const override { return vecLinePoint.back().fX; }
	virtual float Get_RightY() const override { return vecLinePoint.back().fY; }

	vector<LinePoint>& Get_PointArr(){ return vecLinePoint; }
	vector<MyLine>& Get_LineArr() { return vecLine; }

	virtual float Get_CurrentAngle(float _fInputX) override;

	void Initialize();
	void Render(const HDC _dc)override;
	void Release()
	{
		vecLinePoint.clear();
		vecLinePoint.shrink_to_fit();
	}
	// 마지막 끝점 추가
	void AddPoint(float _newPointX, float _newPointY);

	void ClearPoints() { vecLinePoint.clear(); }
	void ClearLines() { vecLine.clear(); }


	// 선 활용 관련
	float LineEquation(float _fInputX);
	MyLine ReturnCurrentLine(float _fInputX);
	// 접선벡터, 방향으로 활용
	Vector2 ReturnTangentVector(float _fInputX);
	
private:
	vector<LinePoint> vecLinePoint;
	vector<MyLine>	vecLine; // 평소엔 포인트만 갖고 있다가 initialize 발동하면 선 생성


};

