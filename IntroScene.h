#pragma once
#include "SceneBase.h"
class CIntroScene : public CSceneBase
{
public:
	CIntroScene();
	virtual ~CIntroScene() override;

public:
	virtual void		Initialize() override;
	virtual void		Update() override;
	virtual void		LateUpdate() override;
	virtual void		Render(HDC _dc) override;
	virtual void		Release() override;


private:
	const TCHAR* m_pFrameKey;
	CMyAnimation m_anim;
};

