#pragma once

class CMsPlayerQueue
{
	CMsSong* m_pSong;
	CMsPlayerQueueItem* m_pHead;
	CMsPlayerQueueItem* m_pTail;
	int m_nTotalObjects;
public:
	CMsPlayerQueue();
	virtual ~CMsPlayerQueue();
	bool Create(CMsSong* pSong);
	virtual UINT ProcessQueue(CMsEvent* pEvent);
	virtual UINT Play(CMsSong* pSong);
	virtual UINT WindDown();
	bool AddObjectToTail(CMsPlayerQueueItem* pObj);
	bool AddObjectToHead(CMsPlayerQueueItem* pObj);
	bool RemoveObject(CMsPlayerQueueItem* pObj);
	void RemoveAllObjects();
	int GetTotalObjects() const { return m_nTotalObjects; }
	CMsSong* GetSong() const { return m_pSong; }
	int PrintNotesInQueue(FILE* pO);
};

