#pragma once

class CMsPlayerQueue
{
	CMsPlayerQueueItem* m_pHead;
	CMsPlayerQueueItem* m_pTail;
	int m_nTotalObjects;
public:
	CMsPlayerQueue();
	virtual ~CMsPlayerQueue();
	bool Create();
	virtual UINT ProcessQueue(CMsSong* pSong);
	virtual UINT Play(CMsSong* pSong);
	bool AddObject(CMsPlayerQueueItem* pObj);
	bool RemoveObject(CMsPlayerQueueItem* pObj);
	void RemoveAllObjects();
	int GetTotalObjects() const { return m_nTotalObjects; }
	int PrintNotesInQueue(FILE* pO);
};

