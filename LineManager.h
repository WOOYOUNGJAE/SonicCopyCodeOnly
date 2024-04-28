#pragma once
#include "Include.h"
#include "LineObjBase.h"
#include "CurvedLine.h"
#include "CircleLine.h"
#include "ConnectedLine.h"
#include "QuarterCurveLine.h"

/**
 * \brief 직선라인, 곡선라인 모두 관리\n
 * 기능: 라인 그리기, 선에 닿았는지 , 
 */
class CLineManager
{
private:
	CLineManager();
	~CLineManager();

public:
	// SingleTon
	static CLineManager* Get_Instance()
	{
		if (m_pInstance == nullptr)
		{
			m_pInstance = new CLineManager;
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

public:
	// getter
	list<CLineObjBase*>& Get_StraightLineList() { return m_StraightLineList; }
	list<CCurvedLine*>& Get_CurveLineList() { return m_CurvedLineList; }
	list<CCircleLine*>& Get_CircleLineList() { return m_CircleLineList; }
	list<CConnectedLine*>& Get_ConnectedLineList() { return m_ConnectedLineList; }



	void Initialize();
	void Update();
	void LateUpdate();
	void Render(HDC _dc);
	void Release();

	void LoadData_Horizontal(const TCHAR* _path); // linedata 받아서 리스트에 pushback
	void LoadData_Curve(const TCHAR* _path); // linedata 받아서 리스트에 pushback
	void LoadData_Circle(const TCHAR* _path);
	void LoadData_Connected(const TCHAR* _path);

	void AddStraightLine(float _x0, float _y0, float _x1, float _y1);
	void AddStraightLine(LinePoint _L, LinePoint _R);

	void AddCurvedLine(float _x0, float _y0, float _x1, float _y1, float _x2, float _y2);
	void AddCurvedLine(Vector2 _pt0, Vector2 _pt1, Vector2 _pt2);

	void AddCircleLine(float _fLineStartX, float _fLineStartY, float _fLineEndX, float _fLineEndY);

	void AddConnectedLine(CConnectedLine* _pConnectedLine); // 단순 수치로 만들어내기 어려움
	void AddConnectedLine(CConnectedLine _ConnectedLine); // 단순 수치로 만들어내기 어려움

	// 점 형태로만 있는 connected에 라인 생성
	void InitializeAllConnectedLIne();

	/**
	 * \brief 커브가 끝났을 때 직선에 얹히기
	 */
	void FindHorizontalLine(float& _refX, float& _refY);

	/**
	 * \brief 
	 * \param _fX 라인에 대입할 x좌표
	 * \param _refY 인풋 x에 따라 변화시킬 y
	 * \return 
	 */
	bool OnLine(float _fX, float _fY, float& _refY);
	bool OnLine(float _fX, float _fY, float& _refY, bool& _bOnCurve);
	bool OnLine(float _fX, float _fY, float& _refY, bool& _bOnCurve, CCurvedLine** _ppCurvedLineToTake);
	bool OnLine(float _fX, float _fY, float& _refY, bool& _bOnCurve, CCurvedLine** _ppCurvedLineToTake,
		bool& _bOnCircle, CCircleLine** _ppCircleLineToTake);

	//playerBase
	bool LineCheck(float _fX, float _fY, float& _refTargetY);
	
private:
	static CLineManager* m_pInstance;

	list<CLineObjBase*> m_StraightLineList;
	list<CCurvedLine*> m_CurvedLineList;

	list<CCircleLine*> m_CircleLineList;

	list <CConnectedLine*> m_ConnectedLineList;

	list <CQuarterCurveLine*> m_QuarterCurveLineList;
};