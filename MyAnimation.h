#pragma once
//#include "Obj.h"
#include "MyBmp.h"

/**
 * \brief Initialize �� ���� Setter�� ����
 */
struct AnimBitmapInfo
{
	AnimBitmapInfo(): iFrameCX(0), iFrameCY(0) {}
	AnimBitmapInfo(int _iCX, int _iCY): iFrameCX(_iCX), iFrameCY(_iCY) {}
	~AnimBitmapInfo() = default;

	int iFrameCX;
	int iFrameCY;
};


struct FrameInfo
{
	int		iFrameStart;		// �ִϸ��̼��� ������ ��ġ �ε��� (�ִϸ��̼� ������Ʈ�� ����)
	int		iFrameEnd;			// �� ����� �ִ� �ִϸ��̼� �ε���. ù �������� ���� 0
	int		iMotion;			// �� �� ���, (�ִϸ��̼� ������Ʈ�� ����)
	DWORD	dwInterval;			// �ִϸ��̼� ��� �ӵ�, ex) 200.f
	DWORD	dwTime;				// �ִϸ��̼� ��� �ð�
};

class CMyAnimation
{
public:
	CMyAnimation();
	virtual ~CMyAnimation();

public:
	// getter
	FrameInfo& Get_FrameInfo() { return m_tFrameInfo; }

	// setter
	/**
	 * \brief initailize ����
	 */
	void Set_FrameSize(int _iCX, int _iCY) { m_tAnimBitmapInfo = { _iCX,  _iCY }; }
	void Set_FrameSize(int _iCXY) { m_tAnimBitmapInfo = { _iCXY,  _iCXY }; }

	virtual void Initialize();
	virtual void Initialize(const TCHAR* _key);
	virtual void Update();
	virtual void Render(HDC _dc);
	virtual void AdjustRender(HDC _dc, const RECT& _rect, int _iAdjustCX, int _iAdjustCY);
	virtual void Render(HDC _dc, const RECT& _rect);
	virtual void Render(HDC _dc, const Vector2& _vPos, const Vector2& _vScale);
	virtual void Release();

	void ChangeAnimSheet(const TCHAR* _key);
protected:
	AnimBitmapInfo m_tAnimBitmapInfo;
	FrameInfo m_tFrameInfo;
	CMyBmp* m_pBmp;
};

