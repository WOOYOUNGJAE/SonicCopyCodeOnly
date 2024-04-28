#include "stdafx.h"
#include "CircleLine.h"
#include "ObjManager.h"
#include "Player.h"
#include "PlayerBase.h"
#include "ScrollManager.h"

CCircleLine::CCircleLine() : m_fRadius(0.f), m_bOnCircleWay(true), m_iReturningIndex(0.f),
 m_iIndexSpeed(2)
{
	ZeroMemory(&tLeftLine, sizeof(MyLine));
	ZeroMemory(&tRightLine, sizeof(MyLine));
	m_eLineID = LINE_ID_CIRCLE;
}


CCircleLine::CCircleLine(float _fLineStartX, float _fLineStartY, float _fLineEndX, float _fLineEndY): m_fRadius(0.f),
m_bOnCircleWay(true), m_iReturningIndex(0.f), m_iIndexSpeed(5/*4*/)
{
	ZeroMemory(&tLeftLine, sizeof(MyLine));
	ZeroMemory(&tRightLine, sizeof(MyLine));
	Initialize(_fLineStartX, _fLineStartY, _fLineEndX, _fLineEndY);
	m_eLineID = LINE_ID_CIRCLE;
}

CCircleLine::~CCircleLine()
{
}

float CCircleLine::Get_CurrentAngle(float _fInputX)
{

	if (_fInputX < m_vLineStart.fX || _fInputX > m_vLineEnd.fX)
		return 0.f;
		//return m_vLineEnd.fY - m_fRadius;

	int iX = static_cast<int>(_fInputX);

	if (m_bOnCircleWay) // 원 타는 길
	{
		
	}
	else
		return 0.f;
}

float CCircleLine::Get_CurrentY(float& _refX, float _xVelocity)
{
	// 범위에 없을 때 예외처리
	if (_refX < m_vLineStart.fX || _refX > m_vLineEnd.fX + 1)
		return FLT_MAX;

	int iX = static_cast<int>(_refX);

	if (m_bOnCircleWay) // 원 타는 길
	{
		if (_refX < m_vTopPoint.fX) // 직선 위치라면
			return m_vLineStart.fY; // 고정된 위치 리턴
		else // 원을 돌아야 한다면
		{
			for (int i = 0; i < m_vecDots.size() - 1; ++i)
			{
				//if (abs(_xVelocity)>= 150.f)
				{
					_refX = m_vecDots[m_iReturningIndex]->fX;
					return m_vecDots[m_iReturningIndex++]->fY;
				}
			}
		}
	}

	/*for (int i = 0; i < m_vecDots.size() - 1; ++i)
	{
		if (m_vecDots[i]->fX <= iX && iX <= m_vecDots[i + 1]->fX)
			return m_vecDots[i]->fY;
	}*/

	return FLT_MAX;// *0.5f;
}

//using
float CCircleLine::Get_CurrentY(float& _refX, float _xVelocity, bool& _bRidingCircle)
{
	// 범위에 없을 때 예외처리
	if (_refX < m_vLineStart.fX || _refX > m_vLineEnd.fX)
		return CObjManager::Get_Instance()->Get_Player()->Get_Pos().fY + CObjManager::Get_Instance()->Get_Player()->Get_Scale().fY * 0.5f; // 위치그대로 반환
		//return m_vLineEnd.fY - m_fRadius;

	int iX = static_cast<int>(_refX);

	if (m_bOnCircleWay) // 원 타는 길
	{
		if (_refX < m_vTopPoint.fX && _bRidingCircle == false) // 직선 위치라면
		{
			ResetReturningIndex();
			return m_vLineStart.fY; // 고정된 위치 리턴
		}

		CObj* pPlayer = CObjManager::Get_Instance()->Get_Player();

		 // 원을 돌아야 한다면
		{
		/*	if (m_iReturningIndex >= m_vecDots.size() - 2)
				m_iReturningIndex = 0;*/

			// 반바퀴 돌았다면
			if (_refX < m_vTopPoint.fX)
			{
				float fRate = (float)m_iReturningIndex / (float)(m_vecDots.size());
				for (int i = 0; i < m_vecDots.size() - 1; ++i)
				{
					// 한바퀴 완벽히 돌았으면
					if (m_iReturningIndex >= m_vecDots.size() - 3)
					{
						_refX = m_vTopPoint.fX + 1;
						m_iReturningIndex = 0;
						m_bOnCircleWay = false;
						_bRidingCircle = false;
					}
					if (fRate < 0.0625)
						dynamic_cast<CPlayerBase*>(pPlayer)->Set_Angle(0);
					else if (fRate < 0.1875)
						dynamic_cast<CPlayerBase*>(pPlayer)->Set_Angle(45);
					else if (fRate < 0.3125)
						dynamic_cast<CPlayerBase*>(pPlayer)->Set_Angle(90);
					else if (fRate < 0.4375)
						dynamic_cast<CPlayerBase*>(pPlayer)->Set_Angle(135);
					else if (fRate < 0.5625)
						dynamic_cast<CPlayerBase*>(pPlayer)->Set_Angle(180);
					else if (fRate < 0.6875)
						dynamic_cast<CPlayerBase*>(pPlayer)->Set_Angle(225);
					else if (fRate < 0.8125)
						dynamic_cast<CPlayerBase*>(pPlayer)->Set_Angle(270);
					else if (fRate < 0.9375)
						dynamic_cast<CPlayerBase*>(pPlayer)->Set_Angle(315);
					else
						dynamic_cast<CPlayerBase*>(pPlayer)->Set_Angle(0);
					m_iReturningIndex += m_iIndexSpeed;
					_refX = m_vecDots[m_iReturningIndex- (m_iIndexSpeed - 1)]->fX;
					return m_vecDots[m_iReturningIndex- (m_iIndexSpeed - 1)]->fY;
				}
			}
			else // 아직 오르는 중이라면
			{
				if (_xVelocity >= 150.f) // 충분히 빠르면 탈 자격 유지
				{
					_bRidingCircle = true;
					m_iReturningIndex += m_iIndexSpeed;
					_refX = m_vecDots[m_iReturningIndex- (m_iIndexSpeed - 1)]->fX;

					float fRate = (float)m_iReturningIndex / (float)(m_vecDots.size());

					
					if (fRate < 0.0625)
						dynamic_cast<CPlayerBase*>(pPlayer)->Set_Angle(0);
					else if (fRate < 0.1875)
						dynamic_cast<CPlayerBase*>(pPlayer)->Set_Angle(45);
					else if (fRate < 0.3125)
						dynamic_cast<CPlayerBase*>(pPlayer)->Set_Angle(90);
					else if (fRate < 0.4375)
						dynamic_cast<CPlayerBase*>(pPlayer)->Set_Angle(135);
					else if (fRate < 0.5625)
						dynamic_cast<CPlayerBase*>(pPlayer)->Set_Angle(180);
					else if (fRate < 0.6875)
						dynamic_cast<CPlayerBase*>(pPlayer)->Set_Angle(225);
					else if (fRate < 0.8125)
						dynamic_cast<CPlayerBase*>(pPlayer)->Set_Angle(270);
					else if (fRate < 0.9375)
						dynamic_cast<CPlayerBase*>(pPlayer)->Set_Angle(315);
					else 
						dynamic_cast<CPlayerBase*>(pPlayer)->Set_Angle(0);

					
					return m_vecDots[m_iReturningIndex- (m_iIndexSpeed - 1)]->fY;
				}
				else // 느리면 
				{
					--m_iReturningIndex;
					if (m_iReturningIndex < 0)
						m_iReturningIndex = 0;
					//m_vecDotsRD
					_bRidingCircle = false; // 좌우 키 먹힘
					// 반대방향으로 x속도 감속
					dynamic_cast<CPlayerBase*>(pPlayer)
						->Set_m_xVelocityMinus(1.f);

					//RD 원일때
					if (pPlayer->Get_Pos().fY <= m_vecDots.back()->fY)
					{
						for (size_t i = 0; i < m_vecDotsRD.size() - 1; ++i)
						{
							if (m_vecDotsRD[i]->fX <= iX && iX <= m_vecDotsRD[i + 1]->fX)
								return m_vecDotsRD[i]->fY;
						}
					}
					else //RU 일 때
					{
						return pPlayer->Get_Pos().fY;
					}
					_refX = m_vecDotsRD.back()->fX;
					return  m_vecDotsRD.back()->fY;
				}
			}
			
		}
	}
	else
		return m_vLineStart.fY;

	/*for (int i = 0; i < m_vecDots.size() - 1; ++i)
	{
		if (m_vecDots[i]->fX <= iX && iX <= m_vecDots[i + 1]->fX)
			return m_vecDots[i]->fY;
	}*/

	return FLT_MAX;// *0.5f;
}

float CCircleLine::Get_CurrentY(float& _refX, float _posY, float _xVelocity, bool& _bRidingCircle)
{
	// 범위에 없을 때 예외처리
	if (_refX < m_vLineStart.fX || _refX > m_vLineEnd.fX)
		return m_vLineEnd.fY - m_fRadius;

	int iX = static_cast<int>(_refX);

	if (m_bOnCircleWay) // 원 타는 길
	{
		if (_refX < m_vTopPoint.fX && _bRidingCircle == false) // 직선 위치라면
			return m_vLineStart.fY; // 고정된 위치 리턴
		else // 원을 돌아야 한다면
		{
			if (m_iReturningIndex >= m_vecDots.size() - 2)
				m_iReturningIndex = 0;

			if ((_xVelocity) >= 150.f)
			{
				_bRidingCircle = true; // 문제				
			}

			for (int i = 0; i < m_vecDots.size() - 1; ++i)
			{
				if ((_xVelocity) >= 150.f) // 속도 충분하면 계속 타도록
				{
					_refX = m_vecDots[m_iReturningIndex]->fX;
					return m_vecDots[m_iReturningIndex++]->fY;
				}
				else // 느리면 
				{
					//m_vecDotsRD
					_bRidingCircle = false;

					if (_posY >= m_vecDotsRD.back()->fY)
					{
						for (int i = 0; i < m_vecDotsRD.size() - 1; ++i)
						{
							if (m_vecDotsRD[i]->fX <= iX && iX <= m_vecDotsRD[i + 1]->fX)
								return m_vecDotsRD[i]->fY;
						}						
					}


					_refX = m_vecDots[m_iReturningIndex]->fX;
					return m_vecDots[m_iReturningIndex++]->fY;
				}

			}
		}
	}

	/*for (int i = 0; i < m_vecDots.size() - 1; ++i)
	{
		if (m_vecDots[i]->fX <= iX && iX <= m_vecDots[i + 1]->fX)
			return m_vecDots[i]->fY;
	}*/

	return FLT_MAX;// *0.5f;
}

void CCircleLine::Initialize(float _fLineStartX, float _fLineStartY, float _fLineEndX, float _fLineEndY)
{
	m_fRadius = (_fLineEndX - _fLineStartX) * 0.5f;
	m_vLineStart = Vector2(_fLineStartX, _fLineStartY);
	m_vLineEnd = Vector2(_fLineEndX, _fLineEndY);
	m_vCircleStart = Vector2(_fLineStartX + m_fRadius, _fLineStartY);
	m_vCircleEnd = Vector2(_fLineEndX, _fLineEndY);
	m_vTopPoint = Vector2(_fLineStartX + m_fRadius, _fLineStartY - m_fRadius * 2.f);

	Vector2 vTmp0 = Vector2(m_vLineEnd.fX, m_vLineEnd.fY - m_fRadius); // right middle
	CMyMath::Get_Instance()->FillBezierPoints(m_vecDotsRD, m_vCircleStart, 
		m_vLineEnd, vTmp0);


	Vector2 vTmp1 = Vector2(m_vLineEnd.fX, m_vLineEnd.fY - m_fRadius * 2.f); // right top
	CMyMath::Get_Instance()->FillBezierPoints(m_vecDotsRU, vTmp0,
		vTmp1, m_vTopPoint);

	Vector2 vTmp2 = Vector2(m_vLineStart.fX, m_vLineStart.fY - m_fRadius * 2.f); // Left top
	Vector2 vTmp3 = Vector2(m_vLineStart.fX, m_vLineStart.fY - m_fRadius); // Left middle

	CMyMath::Get_Instance()->FillBezierPoints(m_vecDotsLU, m_vTopPoint,
		vTmp2, vTmp3);

	CMyMath::Get_Instance()->FillBezierPoints(m_vecDotsLD, vTmp3,
		m_vLineStart, m_vCircleStart);

	// 최종 원 점들 채우기
	for (auto& iter : m_vecDotsRD)
	{
		m_vecDots.push_back(new Vector2(iter->fX, iter->fY));
	}
	for (auto& iter : m_vecDotsRU)
	{
		m_vecDots.push_back(new Vector2(iter->fX, iter->fY));
	}
	for (auto& iter : m_vecDotsLU)
	{
		m_vecDots.push_back(new Vector2(iter->fX, iter->fY));
	}
	for (auto& iter : m_vecDotsLD)
	{
		m_vecDots.push_back(new Vector2(iter->fX, iter->fY));
	}

	// 좌우 끝 좀만 더 늘리기
	m_vLineStart.fX = _fLineStartX;// -70.f;
	m_vLineEnd.fX = _fLineEndX +70.f;

	// 시작 - 가운데 라인
	tLeftLine.L_Point = {m_vLineStart.fX, m_vLineStart.fY};
	tLeftLine.R_Point = {m_vLineStart.fX + m_fRadius, m_vLineEnd.fY};
	// 가운데 - 끝 라인
	tRightLine.L_Point = { m_vLineStart.fX + m_fRadius, m_vLineStart.fY};
	tRightLine.R_Point = { m_vLineEnd.fX, m_vLineEnd.fY};
}

void CCircleLine::Render(const HDC _dc)
{
	//int iScrollX = CScrollManager::Get_Instance()->Get_ScrollX();
	//int iScrollY = CScrollManager::Get_Instance()->Get_ScrollY();

	//MoveToEx(_dc, m_vLineStart.fX + iScrollX, m_vLineStart.fY + iScrollY, nullptr);
	//LineTo(_dc, m_vLineEnd.fX + iScrollX, m_vLineEnd.fY + iScrollY);

	////Ellipse(_dc, m_vLineStart.fX, m_vLineStart.fY - m_fRadius * 2.f,
	////	m_vLineEnd.fX, m_vLineEnd.fY);


	//for (int i = 0; i < m_vecDots.size() - 1; ++i)
	//{
	//	MoveToEx(_dc,
	//		m_vecDots[i]->fX + iScrollX,
	//		m_vecDots[i]->fY + iScrollY, nullptr);

	//	LineTo(_dc, m_vecDots[i + 1]->fX + iScrollX,
	//		m_vecDots[i + 1]->fY + iScrollY);
	//}
}
