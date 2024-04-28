#pragma once
#include "SceneBase.h"
class CBossScene : public CSceneBase
{
public:
	CBossScene();
	virtual ~CBossScene() override;

public:
	virtual void		Initialize() override;
	virtual void		Update() override;
	virtual void		LateUpdate() override;
	virtual void		Render(HDC _dc) override;
	virtual void		Release() override;

	
};

