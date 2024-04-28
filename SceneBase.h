#pragma once
#include "Include.h"
#include "ObjManager.h"
#include "ScrollManager.h"
#include "LineManager.h"
#include "TileManager.h"

class CSceneBase abstract
{
public:
	CSceneBase() = default;
	virtual ~CSceneBase() = default;

public:
	/**
	 * \brief getScroll
	 */
	virtual void		Initialize()
	{
		CScrollManager::Get_Instance()->ResetScroll();
	}
	virtual void		Update();
	virtual void		LateUpdate();
	virtual void		Render(HDC _dc);
	virtual void		Release() = 0;
};
