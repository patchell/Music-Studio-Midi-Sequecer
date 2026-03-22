#pragma once

class CMsListItem
{
	CMsListItem* m_pNext;
	CMsListItem* m_pPrev;
public:
	CMsListItem();
	virtual ~CMsListItem();
	bool Create();
	//-----------------------
	CMsListItem* GetNext() { return m_pNext; }
	CMsListItem* GetPrev() { return m_pPrev; }
	void SetNext(CMsListItem* pNext) { m_pNext = pNext; }
	void SetPrev(CMsListItem* pPrev) { m_pPrev = pPrev; }
};

