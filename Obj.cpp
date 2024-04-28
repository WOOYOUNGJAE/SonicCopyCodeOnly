#include "stdafx.h"
#include "Obj.h"
#include "ScrollManager.h"

CObj::CObj() : m_eID(OBJ_ID_END), m_eCollisionDIr(DIR_END),
m_bAlive(true), m_bIsRight(true), iScrollX(0), iScrollY(0), m_bIsStatic(false),
m_eRenderGroup(RENDER_OBJECT0)
{
	ZeroMemory(&m_vPos, sizeof(Vector2));
	ZeroMemory(&m_vScale, sizeof(Vector2));
	ZeroMemory(&m_RECT, sizeof(RECT));
	ZeroMemory(&m_strName, sizeof(wstring));
	
}

void CObj::Initialize()
{
	
}

void CObj::Render(HDC _dc)
{
	/*iScrollX = CScrollManager::Get_Instance()->Get_ScrollX();
	iScrollY = CScrollManager::Get_Instance()->Get_ScrollY();

	Rectangle(_dc,
		static_cast<int>(m_vPos.fX - m_vScale.fX * 0.5f + iScrollX),
		static_cast<int>(m_vPos.fY - m_vScale.fY * 0.5f + iScrollY),
		static_cast<int>(m_vPos.fX + m_vScale.fX * 0.5f + iScrollX),
		static_cast<int>(m_vPos.fY + m_vScale.fY * 0.5f + iScrollY)
	);*/

	
	//m_anim.Render(_dc, m_vPos, m_vScale);
}

void CObj::LateUpdateRECT()
{

	m_RECT.left = static_cast<long>(m_vPos.fX - (m_vScale.fX * 0.5f));
	m_RECT.top = static_cast<long>(m_vPos.fY - (m_vScale.fY * 0.5f));
	m_RECT.right = static_cast<long>(m_vPos.fX + (m_vScale.fX * 0.5f));
	m_RECT.bottom = static_cast<long>(m_vPos.fY + (m_vScale.fY * 0.5f));
}

bool CObj::LateUpdateAnimFrame()
{
	FrameInfo& refFrameInfo = m_anim.Get_FrameInfo();

	// �� �����ӿ� ��ȯ�� ���� �ʵ��� TickCount�� ������ٶ����� ��ٸ�
	if (refFrameInfo.dwTime + refFrameInfo.dwInterval <
		GetTickCount())
	{
		// ���� TickCount�� �����������
		++refFrameInfo.iFrameStart; // ���� �ε��� ƽ
		refFrameInfo.dwTime = GetTickCount(); // dwTime������Ʈ

		// ������ �ִϸ��̼� ������ ���������� �ε��� 0����
		if (refFrameInfo.iFrameStart > refFrameInfo.iFrameEnd)
		{
			refFrameInfo.iFrameStart = 0;
			return true;
		}
	}

	return false;
	
}

bool CObj::AnimOneCycle()
{
	FrameInfo& refFrameInfo = m_anim.Get_FrameInfo();

	if (refFrameInfo.dwTime + refFrameInfo.dwInterval <
		GetTickCount())
	{
		return true;
	}

	return false;
}
