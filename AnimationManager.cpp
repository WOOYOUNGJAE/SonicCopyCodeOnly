#include "stdafx.h"
#include "AnimationManager.h"
#include "BmpManager.h"

CAnimationManager* CAnimationManager::m_pInstance = nullptr;

CAnimationManager::CAnimationManager()
{
}

CAnimationManager::~CAnimationManager()
{
}

void CAnimationManager::Initialize()
{
	// Insert Bmp
	// Sonic
	CBmpManager::Get_Instance()->Insert_Bmp(L"SonicR",
		L"./Resources/Images/Player/SonicR.bmp");
	CBmpManager::Get_Instance()->Insert_Bmp(L"SonicR0",
		L"./Resources/Images/Player/SonicR0.bmp");
	CBmpManager::Get_Instance()->Insert_Bmp(L"SonicR45",
		L"./Resources/Images/Player/SonicR45.bmp");
	CBmpManager::Get_Instance()->Insert_Bmp(L"SonicR90",
		L"./Resources/Images/Player/SonicR90.bmp");
	CBmpManager::Get_Instance()->Insert_Bmp(L"SonicR135",
		L"./Resources/Images/Player/SonicR135.bmp");
	CBmpManager::Get_Instance()->Insert_Bmp(L"SonicR180",
		L"./Resources/Images/Player/SonicR180.bmp");
	CBmpManager::Get_Instance()->Insert_Bmp(L"SonicR225",
		L"./Resources/Images/Player/SonicR225.bmp");
	CBmpManager::Get_Instance()->Insert_Bmp(L"SonicR270",
		L"./Resources/Images/Player/SonicR270.bmp");
	CBmpManager::Get_Instance()->Insert_Bmp(L"SonicR315",
		L"./Resources/Images/Player/SonicR315.bmp");

	CBmpManager::Get_Instance()->Insert_Bmp(L"SonicL",
		L"./Resources/Images/Player/SonicL.bmp");
	CBmpManager::Get_Instance()->Insert_Bmp(L"SonicL0",
		L"./Resources/Images/Player/SonicL0.bmp");
	CBmpManager::Get_Instance()->Insert_Bmp(L"SonicL45",
		L"./Resources/Images/Player/SonicL45.bmp");
	CBmpManager::Get_Instance()->Insert_Bmp(L"SonicL90",
		L"./Resources/Images/Player/SonicL90.bmp");
	CBmpManager::Get_Instance()->Insert_Bmp(L"SonicL135",
		L"./Resources/Images/Player/SonicL135.bmp");
	CBmpManager::Get_Instance()->Insert_Bmp(L"SonicL180",
		L"./Resources/Images/Player/SonicL180.bmp");
	CBmpManager::Get_Instance()->Insert_Bmp(L"SonicL225",
		L"./Resources/Images/Player/SonicL225.bmp");
	CBmpManager::Get_Instance()->Insert_Bmp(L"SonicL270",
		L"./Resources/Images/Player/SonicL270.bmp");
	CBmpManager::Get_Instance()->Insert_Bmp(L"SonicL315",
		L"./Resources/Images/Player/SonicL315.bmp");
	
	CBmpManager::Get_Instance()->Insert_Bmp(L"Shine",
		L"./Resources/Images/UnmovableObj/ShineEffect.bmp");
	CBmpManager::Get_Instance()->Insert_Bmp(L"Ring",
		L"./Resources/Images/UnmovableObj/Ring.bmp");
	
	CBmpManager::Get_Instance()->Insert_Bmp(L"Spring",
		L"./Resources/Images/UnmovableObj/Spring.bmp");
	CBmpManager::Get_Instance()->Insert_Bmp(L"DiagonalSpring",
		L"./Resources/Images/UnmovableObj/DiagonalSpring.bmp");

	CBmpManager::Get_Instance()->Insert_Bmp(L"Spike",
		L"./Resources/Images/UnmovableObj/Spike.bmp");

	CBmpManager::Get_Instance()->Insert_Bmp(L"Sign",
		L"./Resources/Images/UnmovableObj/Sign.bmp");

	CBmpManager::Get_Instance()->Insert_Bmp(L"Item",
		L"./Resources/Images/UnmovableObj/Item.bmp");



	CBmpManager::Get_Instance()->Insert_Bmp(L"MechaSonicR",
		L"./Resources/Images/MechaSonic/MechaSonicR.bmp");
	CBmpManager::Get_Instance()->Insert_Bmp(L"MechaSonicL",
		L"./Resources/Images/MechaSonic/MechaSonicL.bmp");
	CBmpManager::Get_Instance()->Insert_Bmp(L"MechaSonicWhiteR",
		L"./Resources/Images/MechaSonic/MechaSonicWhiteR.bmp");
	CBmpManager::Get_Instance()->Insert_Bmp(L"MechaSonicWhiteL",
		L"./Resources/Images/MechaSonic/MechaSonicWhiteL.bmp");

	//UI
	CBmpManager::Get_Instance()->Insert_Bmp(L"UI",
		L"./Resources/Images/UI/UI.bmp");
	CBmpManager::Get_Instance()->Insert_Bmp(L"Number",
		L"./Resources/Images/UI/Number.bmp");
	CBmpManager::Get_Instance()->Insert_Bmp(L"SmallNumber",
		L"./Resources/Images/UI/SmallNumber.bmp");

	/*CBmpManager::Get_Instance()->ChangeWhite(L"./Resources/Images/MechaSonic/MechaSonicR.bmp", L"./Resources/Images/MechaSonic/MechaSonicWhiteR.bmp");
	CBmpManager::Get_Instance()->ChangeWhite(L"./Resources/Images/MechaSonic/MechaSonicL.bmp", L"./Resources/Images/MechaSonic/MechaSonicWhiteL.bmp");*/
}
