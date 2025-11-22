#pragma once

enum  {
	QUEUE_STATUS_NOACTION,
	QUEUE_STATUS_REMOVE
};


class CMsPlayerQueueItem
{
	CMsPlayerQueueItem* m_pNext;
	CMsPlayerQueueItem* m_pPrev;
	CMsObject* m_pMsObject; // Pointer to the associated music studio object
public:
	CMsPlayerQueueItem();
	virtual ~CMsPlayerQueueItem();
	bool Create();
	CMsPlayerQueueItem* GetNext() { return m_pNext; }
	CMsPlayerQueueItem* GetPrev() { return m_pPrev; }
	void SetNext(CMsPlayerQueueItem* pO) {
		m_pNext = pO;
	}
	void SetPrev(CMsPlayerQueueItem* pO) {
		m_pPrev = pO;
	}
	CMsObject* GetMsObject() { return m_pMsObject; }
	void SetMsObject(CMsObject* pObj) { m_pMsObject = pObj; }
	UINT Process(CMsSong* pSong = 0);
	UINT Play(CMsSong* pSong = 0);
};

