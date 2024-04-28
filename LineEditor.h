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
 * \brief 어차피 EditorScene에서만 사용되고 저장후에는 쓸모 없어지니 바로 LineManager에 Push
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
	void AdjustConnected(LONG& _refX, LONG& _refY); // 현재 찍은 왼쪽 점이 가까운 오른쪽 점과 이어지도록
	
	void UpdateHorizontal();
	void UpdateVertical();
	void UpdateConnected();
	void UpdateCurve(); // 무조건 왼쪽이 먼저
	void UpdateCircle();

	bool m_bStart; // 처음 점 찍을 차례면 true
	bool m_bConnectOn; // true면 이어지도록
	bool m_bConnectedHorizontal;

	LinePoint m_tOnePoint;
	LinePoint m_tLinePoint[2]; // left, right
	Vector2 m_vCurvePoints[3];
	
	Vector2 m_vCirclePoints[2]; // left, right



	CConnectedLine m_connectedLine; //그릇 역할

	LINE_MODE m_eLineMode;
	int m_iScrollX;
	int m_iScrollY;
};

