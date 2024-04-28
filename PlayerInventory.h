#pragma once

class CItemObj;
enum ITEM_ID;
class CPlayerInventory
{
public:
	CPlayerInventory();
	~CPlayerInventory();

public:
	list<CItemObj*>& Get_ItemList() { return m_ItemList; }

	void Initialize();
	void Update();
	void Release();
	void Render(HDC _dc);
	void PushItem(CItemObj* _pItem);
	void PopItem(CItemObj* _pItem, ITEM_ID _eItemID);
	void SwapItem(ITEM_ID _Dst, ITEM_ID _Src);


private:
	list<CItemObj*> m_ItemList;

	int m_iInventoryIndex;
};

