#include "stdafx.h"
#include "PlayerInventory.h"
#include "ItemObj.h"
#include "UIManager.h"

CPlayerInventory::CPlayerInventory()
{
}

CPlayerInventory::~CPlayerInventory()
{
}

void CPlayerInventory::Initialize()
{
	CUIManager::Get_Instance()->Set_Inventory(this);

	/*CItemObj* pTmp = new CItemObj;
	PushItem(pTmp);
	pTmp = new CItemObj;
	pTmp->Set_ItemID(1);
	PushItem(pTmp);*/
}

void CPlayerInventory::Update()
{
}

void CPlayerInventory::Release()
{
}

void CPlayerInventory::Render(HDC _dc)
{
	// if on
	CUIManager::Get_Instance()->RenderInventory(_dc);
}

void CPlayerInventory::PushItem(CItemObj* _pItem)
{
	m_ItemList.push_back(_pItem);
}

void CPlayerInventory::PopItem(CItemObj* _pItem, ITEM_ID _eItemID)
{
	ITEM_ID eItemID = _pItem->Get_ItemID();
	list<CItemObj*>::iterator itemIter = m_ItemList.begin();
	for (auto iter = m_ItemList.begin(); iter != m_ItemList.end(); ++iter)
	{
		if ((*iter)->Get_ItemID() == _eItemID)
			itemIter = iter;
	}
	m_ItemList.erase(itemIter);
}

void CPlayerInventory::SwapItem(ITEM_ID _Dst, ITEM_ID _Src)
{
	list<CItemObj*>::iterator itemDstIter = m_ItemList.begin();
	list<CItemObj*>::iterator itemSrcIter = m_ItemList.begin();
	for (auto iter = m_ItemList.begin(); iter != m_ItemList.end(); ++iter)
	{
		if ((*iter)->Get_ItemID() == _Dst)
			itemDstIter = iter;
		else if ((*iter)->Get_ItemID() == _Src)
			itemSrcIter = iter;
	}
	CItemObj* _pTmpPointer = nullptr;
	_pTmpPointer = *itemDstIter;
	*itemDstIter = *itemSrcIter;
	*itemSrcIter = _pTmpPointer;
}
