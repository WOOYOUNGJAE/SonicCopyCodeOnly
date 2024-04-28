#pragma once
#include "TileManager.h"

enum TILEMAPSHEET
{
	TILEMAP_MUSHROOM,
	TILEMAP_END,
};

class CTileEditor
{
public:
	CTileEditor();
	~CTileEditor();

public:
	// getter
	bool Get_TileSorting() { return m_bTileSorting; }

	// setter
	void Set_CurrentTileID(int _iID) { m_iCurrentTileIDX = _iID; }
	void Set_TileSorting(bool _b) { m_bTileSorting = _b; }


	void Initialize();
	void Update();
	void LateUpdate();
	void Render(HDC _dc);
	void Release();



private:
	TILEMAPSHEET m_eCurrentTileMap;
	int m_iCurrentPage;
	int m_iCurrentTileIDX;
	int m_iCurrentTileIDY;

	bool m_bTileSorting;
};

