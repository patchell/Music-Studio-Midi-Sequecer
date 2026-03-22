#pragma once

class CMsEventChain
{
	CMsEvent* m_pHead;
	CMsEvent* m_pTail;
	int m_TotalEvents;
	bool bOriginalChain;
public:
	CMsEventChain() {
		m_pHead = 0;
		m_pTail = 0;
		m_TotalEvents = 0;
		bOriginalChain = false;
	
	}
	~CMsEventChain() {}
	void CreateChain(UINT nEvents, CMsSong* pSong, CChildViewStaff* pStaffView, UINT StartIndex);
	void AddEventAtEnd(CMsEvent* pEV);
	CMsEvent* GetHead() { return m_pHead; }
	void SetHead(CMsEvent* pEV) { m_pHead = pEV; }
	CMsEvent* GetTail() { return m_pTail; }
	void SetTail(CMsEvent* pEV) { m_pTail = pEV; }
	void UnHookChain();
	void SetSelected(int SelFlag);
	UINT GetTotalEvents(){return m_TotalEvents;}
	bool Cut(CMsEvent* pFirst, CMsEvent* pLast);
	bool Copy(CMsEvent* pFirst, CMsEvent* pLast);
	bool Paste(CMsEvent* pInsertionPoint);
};

