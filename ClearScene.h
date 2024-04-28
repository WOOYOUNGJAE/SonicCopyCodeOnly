#pragma once
#include "SceneBase.h"
class CClearScene : public CSceneBase
{
public:
	CClearScene();
	virtual ~CClearScene() override;

public:
	// setter
	void Set_WaitToChange(bool _b) { m_bWaitToChange = _b; }

	virtual void		Initialize() override;
	virtual void		Update() override;
	virtual void		LateUpdate() override;
	virtual void		Render(HDC _dc) override;
	virtual void		Release() override;

private:
	bool m_bWaitToChange;
	DWORD m_dwWaitStartTime;
	DWORD m_dwWaitAccTime;
};

