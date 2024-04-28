#pragma once
#include "ObjManager.h"

enum OBJ_EDITOR_MODE
{
	EDIT_SPRING,
	EDIT_WALL,
	EDIT_RING,
	EDIT_SPIKE,
	EDIT_DIAGONALSPRING,
	EDIT_ITEM,
	OBJ_EDIT_END,
};

class CObjEditor
{

public:
	CObjEditor();
	~CObjEditor();

public:
	void Initialize();
	void Update();
	void LateUpdate();
	void Render(HDC _dc);
	void Release();

	void UpdateSpring();
	void UpdateDiagonalSpring();
	void UpdateWall();
	void UpdateRing();
	void UpdateSpike();
	void UpdateItem();

	void RenderSpring(HDC _dc);
	void RenderDiagonalSpring(HDC _dc);
	void RenderWall(HDC _dc);
	void RenderRing(HDC _dc);
	void RenderSpike(HDC _dc);
	void RenderItem(HDC _dc);

	void ResetPos()
	{
		m_vCreatePos = Vector2(0, 0);
		ZeroMemory(&m_ptWallLU, sizeof(POINT));
		ZeroMemory(&m_ptWallRD, sizeof(POINT));
	}

private:
	CObj* m_pCurrentObj;
	Vector2 m_vCreatePos;
	TCHAR m_pCurrentObjName[32];
	const TCHAR* m_strCurrentBmpKey;

	MY_DIR m_eSpringDir;
	DIAGONAL_DIR m_eDiagonalSpringDir;
	OBJ_EDITOR_MODE _eEditor;

	int m_iScrollX;
	int m_iScrollY;

	POINT m_ptWallLU;
	POINT m_ptWallRD;

};

