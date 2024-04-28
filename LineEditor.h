#pragma once
#include "LineManager.h"

enum LINE_MODE
{
	STRAIGHT_HORIZONTAL,
	STRAIGHT_VERTICAL,
	LINE_CONNECTED,
	LINE_CURVE,
	LINE_CIRCLE,
	LINE_MODE_END,
};

/**
 * \brief ������ EditorScene������ ���ǰ� �����Ŀ��� ���� �������� �ٷ� LineManager�� Push
 */
class CLineEditor
{
public:
	CLineEditor();
	~CLineEditor();

public:
	void Initialize();
	void Update();
	void LateUpdate();
	void Render(HDC _dc);
	void Release();

	void ResetPoints();
	void AdjustConnected(LONG& _refX, LONG& _refY); // ���� ���� ���� ���� ����� ������ ���� �̾�������
	
	void UpdateHorizontal();
	void UpdateVertical();
	void UpdateConnected();
	void UpdateCurve(); // ������ ������ ����
	void UpdateCircle();

	bool m_bStart; // ó�� �� ���� ���ʸ� true
	bool m_bConnectOn; // true�� �̾�������
	bool m_bConnectedHorizontal;

	LinePoint m_tOnePoint;
	LinePoint m_tLinePoint[2]; // left, right
	Vector2 m_vCurvePoints[3];
	
	Vector2 m_vCirclePoints[2]; // left, right



	CConnectedLine m_connectedLine; //�׸� ����

	LINE_MODE m_eLineMode;
	int m_iScrollX;
	int m_iScrollY;
};

