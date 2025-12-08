#include "pch.h"

CMsPlayerQueue::CMsPlayerQueue()
{
	m_pHead = nullptr;
	m_pTail = nullptr;
	m_nTotalObjects = 0;
}

CMsPlayerQueue::~CMsPlayerQueue()
{
	RemoveAllObjects();
}

bool CMsPlayerQueue::Create()
{
	// Create the player queue
	return true;
}

UINT CMsPlayerQueue::ProcessQueue(CMsSong* pSong)
{
	//WTF? Why is this function here? and is it even used?
	// Process the player queue
	UINT ProcessStatus = 0;
	CMsPlayerQueueItem* pNextObj = nullptr;
	CMsPlayerQueueItem* pObj = m_pHead;


	while (pObj)
	{
		ProcessStatus = pObj->Process();
		switch (ProcessStatus)
		{
		case QUEUE_STATUS_NOACTION:
			// No action needed
			pObj = pObj->GetNext();
			break;
		case QUEUE_STATUS_REMOVE:
			// Remove the object from the queue
			pNextObj = pObj->GetNext();
			RemoveObject(pObj);
			pObj = pNextObj;
			break;
		}
	}

	return ProcessStatus;
}

UINT CMsPlayerQueue::Play(CMsSong* pSong)
{
	// Play the player queue
	CMsPlayerQueueItem* pObj = m_pHead;
	CMsPlayerQueueItem* pTempObj = 0;
	UINT DeleteObjectFlag = 0;
	UINT AnObjectHasEnded = 0;

	while(pObj)
	{
		DeleteObjectFlag = pObj->GetMsObject()->Play();
		if(DeleteObjectFlag == 1)
		{
			// Object has finished playing, remove it from the queue
			pTempObj = pObj->GetNext();
			RemoveObject(pObj);
			delete pObj;
			pObj = pTempObj;
			AnObjectHasEnded++;
		}
		else
		{
			pObj = pObj->GetNext();

		}
	}
	return AnObjectHasEnded;
}

bool CMsPlayerQueue::AddObject(CMsPlayerQueueItem* pObj)
{
	// Add an object to the player queue
	if(!m_pHead && !m_pTail)
	{
		// First object in the queue
		m_pHead = pObj;
		m_pTail = pObj;
		pObj->SetNext(nullptr);
		pObj->SetPrev(nullptr);
	}
	else
	{
		// Add to the end of the queue
		m_pTail->SetNext(pObj);
		pObj->SetPrev(m_pTail);
		pObj->SetNext(nullptr);
		m_pTail = pObj;
	}
	return true;
}

bool CMsPlayerQueue::RemoveObject(CMsPlayerQueueItem* pObj)
{
	// Remove an object from the player queue
	bool rV = true;

	if(pObj == m_pHead)
	{
		// Remove from head
		m_pHead = pObj->GetNext();
		if(m_pHead)
			m_pHead->SetPrev(nullptr);
		else
			m_pTail = nullptr;
	}
	else if(pObj == m_pTail)
	{
		// Remove from tail
		m_pTail = pObj->GetPrev();
		if(m_pTail)
			m_pTail->SetNext(nullptr);
		else
			m_pHead = nullptr;
	}
	else
	{
		// Remove from middle
		if (pObj->GetPrev() && pObj->GetNext())
		{ 
			pObj->GetPrev()->SetNext(pObj->GetNext());
			pObj->GetNext()->SetPrev(pObj->GetPrev());
		}
		else
		{
			// Invalid object
			rV = false;
		}
	}
	return rV;
}

void CMsPlayerQueue::RemoveAllObjects()
{
	// Remove all objects from the player queue
	CMsPlayerQueueItem* pObj = m_pHead;
	CMsPlayerQueueItem* pNextObj;

	while(pObj)
	{
		pNextObj = pObj->GetNext();
		RemoveObject(pObj);
		delete pObj;
		pObj = pNextObj;
	}
	m_pHead = nullptr;
	m_pTail = nullptr;
}

int CMsPlayerQueue::PrintNotesInQueue(FILE* pO)
{
	fprintf(pO, "Total Objects in Queue:%d\n", GetTotalObjects());
	CMsPlayerQueueItem* pItem = m_pHead;
	CMsNote* pNote = nullptr;

	while (pItem)
	{
		if (pItem)
		{
			if(pItem->GetMsObject()->Is(MSOBJ_NOTE))
			{
				pNote = dynamic_cast<CMsNote*>(pItem->GetMsObject());
				pNote->Print(pO, 4);
			}
		}
		pItem = pItem->GetNext();
	}
	return 0;
}
