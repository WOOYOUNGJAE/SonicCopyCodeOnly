#pragma once
//#include "Obj.h"
#include "MyBmp.h"

/**
 * \brief Initialize 한 직후 Setter로 설정
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
	int		iFrameStart;		// 애니메이션을 시작할 위치 인덱스 (애니메이션 스파이트의 가로)
	int		iFrameEnd;			// 한 모션의 최대 애니메이션 인덱스. 첫 프레임의 끝이 0
	int		iMotion;			// 몇 번 모션, (애니메이션 스파이트의 세로)
	DWORD	dwInterval;			// 애니메이션 재생 속도, ex) 200.f
	DWORD	dwTime;				// 애니메이션 재생 시간
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
	 * \brief initailize 직후
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

