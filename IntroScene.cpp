#include "stdafx.h"
#include "IntroScene.h"

#include "BmpManager.h"

CIntroScene::CIntroScene()
{
	m_pFrameKey = L"Logo";
}

CIntroScene::~CIntroScene()
{
}

void CIntroScene::Initialize()
{
	CBmpManager::Get_Instance()->Insert_Bmp(L"Logo",
		L"./Resources/Images/Logo, Main/Logo.bmp");
	CBmpManager::Get_Instance()->Insert_Bmp(L"MainMenu",
		L"./Resources/Images/Logo, Main/MainMenu.bmp");

	m_anim.Initialize(m_pFrameKey);
	m_anim.Set_FrameSize(400, 300);
	m_anim.Get_FrameInfo().iFrameStart = 0;
	m_anim.Get_FrameInfo().iFrameEnd = 2;
	m_anim.Get_FrameInfo().iMotion = 0;
	m_anim.Get_FrameInfo().dwInterval = 100.f;
}

void CIntroScene::Update()
{
	
}

void CIntroScene::LateUpdate()
{
	FrameInfo& refFrameInfo = m_anim.Get_FrameInfo();

	if (refFrameInfo.dwTime + refFrameInfo.dwInterval <
		GetTickCount())
	{
		// ���� TickCount�� �����������
		++refFrameInfo.iFrameStart; // ���� �ε��� ƽ
		refFrameInfo.dwTime = GetTickCount(); // dwTime������Ʈ

		// ������ �ִϸ��̼� ������ ���������� �ε��� 0����

		if (refFrameInfo.iFrameStart > refFrameInfo.iFrameEnd)
		{
			if (m_pFrameKey == L"Logo")
			{
				if (refFrameInfo.iMotion != 3)
				{
					++refFrameInfo.iMotion;
					refFrameInfo.iFrameStart = 0;
				}
				else
				{
					m_pFrameKey = L"MainMenu";
					m_anim.ChangeAnimSheet(m_pFrameKey);
					m_anim.Set_FrameSize(400, 300);
					m_anim.Get_FrameInfo().iFrameStart = 0;
					m_anim.Get_FrameInfo().iFrameEnd = 8;
					m_anim.Get_FrameInfo().iMotion = 0;
					m_anim.Get_FrameInfo().dwInterval = 200.f;
				}
			}
			else
				refFrameInfo.iFrameStart = 0;
		}
	}
}

void CIntroScene::Render(HDC _dc)
{
	m_anim.Render(_dc);
}

void CIntroScene::Release()
{
}
