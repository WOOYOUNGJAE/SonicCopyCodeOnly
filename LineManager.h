#pragma once
#include "Include.h"
#include "LineObjBase.h"
#include "CurvedLine.h"
#include "CircleLine.h"
#include "ConnectedLine.h"
#include "QuarterCurveLine.h"

/**
 * \brief ��������, ����� ��� ����\n
 * ���: ���� �׸���, ���� ��Ҵ��� , 
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

	void LoadData_Horizontal(const TCHAR* _path); // linedata �޾Ƽ� ����Ʈ�� pushback
	void LoadData_Curve(const TCHAR* _path); // linedata �޾Ƽ� ����Ʈ�� pushback
	void LoadData_Circle(const TCHAR* _path);
	void LoadData_Connected(const TCHAR* _path);

	void AddStraightLine(float _x0, float _y0, float _x1, float _y1);
	void AddStraightLine(LinePoint _L, LinePoint _R);

	void AddCurvedLine(float _x0, float _y0, float _x1, float _y1, float _x2, float _y2);
	void AddCurvedLine(Vector2 _pt0, Vector2 _pt1, Vector2 _pt2);

	void AddCircleLine(float _fLineStartX, float _fLineStartY, float _fLineEndX, float _fLineEndY);

	void AddConnectedLine(CConnectedLine* _pConnectedLine); // �ܼ� ��ġ�� ������ �����
	void AddConnectedLine(CConnectedLine _ConnectedLine); // �ܼ� ��ġ�� ������ �����

	// �� ���·θ� �ִ� connected�� ���� ����
	void InitializeAllConnectedLIne();

	/**
	 * \brief Ŀ�갡 ������ �� ������ ������
	 */
	void FindHorizontalLine(float& _refX, float& _refY);

	/**
	 * \brief 
	 * \param _fX ���ο� ������ x��ǥ
	 * \param _refY ��ǲ x�� ���� ��ȭ��ų y
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