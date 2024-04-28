#include "stdafx.h"
#include "QuarterCurveLine.h"
#include "ScrollManager.h"

void CQuarterCurveLine::Initialize(float _centerX, float _centerY, float _fRadius, DIAGONAL_DIR _eDir)
{
	switch (_eDir)
	{
	case DIR_RU:
	{
		int iCenterX = (int)_centerX;
		int iCenterY = (int)_centerY;
		int iNewDotX = 0;
		int iNewDotY = 0;
		int iDegree = 0;

		while (iDegree < 90)
		{
			iNewDotX = iCenterX + cos(iDegree) * m_fRadius;
			iNewDotY = iCenterY + sin(iDegree) * m_fRadius;

			m_vecDots.push_back(new Vector2(iNewDotX, iNewDotY));
			++iDegree;
		}
	}
	break;
	}
}

void CQuarterCurveLine::Render(const HDC _dc)
{
	int iScrollX = CScrollManager::Get_Instance()->Get_ScrollX();
	int iScrollY = CScrollManager::Get_Instance()->Get_ScrollY();
	

	for (int i = 0; i < m_vecDots.size() - 1; ++i)
	{
		MoveToEx(_dc,
			m_vecDots[i]->fX + iScrollX,
			m_vecDots[i]->fY + iScrollY, nullptr);

		LineTo(_dc, m_vecDots[i + 1]->fX + iScrollX,
			m_vecDots[i + 1]->fY + iScrollY);
	}
}
