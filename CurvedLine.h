#pragma once
#include "Include.h"
#include "LineObjBase.h"
/**
 * \brief ���̶�⺸�� ������ ����\n
 * LineObjBase �� ��ӹ��� ����.\n
 * Point 0, 1, 2�� 1�� �߰� ��. \n
 * �׻� point0�� x�� ���� �۾ƾ� ��
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

	// x�� �ش��ϴ� ���� y
	float Get_CurrentY(float _x);
	float Get_CurrentY(float _x, float& _yVelocity);
	float Get_CurrentY(float _x, float& _yVelocity, bool& _bAccelTriggered);

	// ���� ���� y��ǥ
	float Get_LowesetY();

	virtual void Render (const HDC _dc) override;

	bool IfOnY_AccelRange(float _fX); // y�ӵ� �����ش� ���� �ȿ� �ִ�
	bool IfOnX_Range(float _fX); // ���� Ŀ�� �¿� ���̿� �ִ�


	float MakeHigher();
private:/*
	Vector2 m_vPoint0;
	Vector2 m_vPoint1;
	Vector2 m_vPoint2;*/

	Vector2 m_vPointArr[3];

	vector<Vector2*> m_vecDots; // int ���� ����

	float fWidth; // point0 ~ point1 �Ÿ�

	// �� ���̿� ���� �� MakeHigherȣ��
	float m_fY_AccelStart;
	float m_fYAccelEnd;

	float fY_Accel; // Makehigh���� �����ִ� ����
};

