#pragma once
#include "SceneBase.h"
#include "LineEditor.h"
#include "ObjEditor.h"
#include "TileEditor.h"

enum EDIT_MODE
{
	EDIT_LINE,
	EDIT_TILE,
	EDIT_OBJ,
	EDIT_END,
};

class CEditorScene : public CSceneBase
{
public:
	CEditorScene();
	virtual ~CEditorScene() override;

public:
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void LateUpdate() override;
	virtual void Render(HDC _dc) override;
	virtual void Release() override;


	// I/O
	void SaveData_Horizontal();
	void SaveData_Curve();
	void SaveData_Connected();
	void SaveData_Circle();
	void SaveData_Tile_Mushroom();
	void SaveData_Spring();
	void SaveData_DiagonalSpring();
	void SaveData_Wall();
	void SaveData_Ring();
	void SaveData_Spike();
	void SaveData_Item();

	void RenderTileCrossLine(HDC _dc);

private:
	CLineEditor m_lineEditor;
	CTileEditor m_tileEditor;
	CObjEditor m_ObjEditor;

	EDIT_MODE m_eEditMode;
	
};

