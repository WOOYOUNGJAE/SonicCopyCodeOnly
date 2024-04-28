#pragma once
#include "SceneBase.h"

class CTestScene : public CSceneBase
{
public:
	CTestScene();
	virtual ~CTestScene() override;
	
public:
	virtual void		Initialize() override;
	virtual void		Update() override;
	virtual void		LateUpdate() override;
	virtual void		Render(HDC _dc) override;
	virtual void		Release() override;
	
private:
	float t;

	Vector2 vBezPrev;
	Vector2 vBezNext;


	float m_fPlayer_xPos;

	vector<Vector2*> dots0;
	vector<Vector2*> dots1;
	vector<Vector2*> dots2;
	vector<Vector2*> dots3;
	
	float fIndex;


	const TCHAR* m_pFrameKey;
	CMyAnimation m_anim;

	bool m_bRemainTick;
	DWORD m_dwRemainStart;
	DWORD m_dwRemainAcc;
};

