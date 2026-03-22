#pragma once

class CMsList
{
	CMsListItem* m_pHead;
	CMsListItem* m_pTail;
	int m_NumberOfItems;
	char* m_pName;
public:
	CMsList();
	virtual ~CMsList();
	bool Create();
	int GetNumberOfItems() { return m_NumberOfItems; }
	const char* GetName() { return m_pName; }
	void SetName(const char* pName);
	void AddItemToHead(CMsListItem* pItem);
	void AddItemToTail(CMsListItem* pItem);
	CMsListItem* GetHead() { return m_pHead; }
	CMsListItem* GetTail() { return m_pTail; }
	bool RemoveItem(CMsListItem* pItem);
	void SetHead(CMsListItem* pHead) { m_pHead = pHead; }
	void SetTail(CMsListItem* pTail) { m_pTail = pTail; }
};

