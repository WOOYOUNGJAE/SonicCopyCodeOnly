#include "stdafx.h"
#include "SceneBase.h"

#include "UIManager.h"

void CSceneBase::Update()
{
	CTileManager::Get_Instance()->Update();
	CObjManager::Get_Instance()->Update();
	CLineManager::Get_Instance()->Update();
}

void CSceneBase::LateUpdate()
{
	CTileManager::Get_Instance()->Update();
	CObjManager::Get_Instance()->LateUpdate();
	CLineManager::Get_Instance()->LateUpdate();
}

void CSceneBase::Render(HDC _dc)
{
	CTileManager::Get_Instance()->Render(_dc);
	CLineManager::Get_Instance()->Render(_dc);
	CObjManager::Get_Instance()->Render(_dc);
	CUIManager::Get_Instance()->Render(_dc);
}
