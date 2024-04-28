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
		// 드디어 TickCount가 따라와줬으면
		++refFrameInfo.iFrameStart; // 가로 인덱스 틱
		refFrameInfo.dwTime = GetTickCount(); // dwTime업데이트

		// 마지막 애니메이션 프레임 진행했으면 인덱스 0으로

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
