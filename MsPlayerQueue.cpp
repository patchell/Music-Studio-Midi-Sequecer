#include "pch.h"

CMsPlayerQueue::CMsPlayerQueue()
{
	m_pHead = nullptr;
	m_pTail = nullptr;
	m_nTotalObjects = 0;
	m_pSong = nullptr;
}

CMsPlayerQueue::~CMsPlayerQueue()
{
	RemoveAllObjects();
}

bool CMsPlayerQueue::Create(CMsSong* pSong)
{
	// Create the player queue
	m_pSong = pSong;
	return true;
}

UINT CMsPlayerQueue::ProcessQueue(CMsEvent* pEvent)
{
	//--------------------------------
	// Process the player queue
	// This function scans the Event
	// Looking for objects that need
	// to go into the Player Queue
	//--------------------------------
	UINT ProcessStatus = 0;
	CMsPlayerQueueItem* pQueueObj;
	CMsObject* pMusObj = nullptr;
	UINT AddedCount = 0;

	pMusObj = pEvent->GetEventMsObjectHead();
	while (pMusObj)
	{
		ProcessStatus = pMusObj->Process();
		if(ProcessStatus)
		{
			// Object needs to be added to the player queue
			pQueueObj = new CMsPlayerQueueItem();
			pQueueObj->Create();
			pMusObj->PlayStateStart();
			pQueueObj->SetMsObject(pMusObj);
			if(pMusObj->IsTimedObject())
				AddObjectToTail(pQueueObj);
			else
			{
				AddObjectToHead(pQueueObj);
			}
			AddedCount++;
		}
		pMusObj = pMusObj->GetNext();
	}
//	fprintf(GETAPP->LogFile(), "==== PROCESS->Added %d Objects\n", AddedCount);
	if (GetSong()->GetSongPosition())
	{
		GetSong()->GetStaffChildView()->PostMessageW(
			WM_STAFF_DISP_EVENT,
			GetSong()->GetSongPosition()->GetIndex(),
			STAFF_DISP_EVENT_NEXT
		);
	}
	return AddedCount;
}

UINT CMsPlayerQueue::Play(CMsSong* pSong)
{
	// Play the player queue
	CMsPlayerQueueItem* pObj = m_pHead;
	CMsPlayerQueueItem* pTempObj = 0;
	UINT DeleteObjectFlag = 0;
	UINT AnObjectHasEnded = 0;
	UINT AnObjectIsDone = 0;
	static UINT LastTickLog = 0;
	UINT ObjectsPlayed = 0;
	CMsNote* pNote = 0;

//	fprintf(GETAPP->LogFile(), "---- PLAY QUEUE: Total Objects:%d\n", GetTotalObjects());
	while(pObj)
	{
		DeleteObjectFlag = pObj->GetMsObject()->Play();
		ObjectsPlayed++;
		switch (DeleteObjectFlag)
		{
		case PLAY_NO_ACTION:
			pObj = pObj->GetNext();
			break;
		case PLAY_OBJECT_DONE:
			pTempObj = pObj->GetNext();
			RemoveObject(pObj);
			delete pObj;
			pObj = pTempObj;
			AnObjectIsDone++;
			break;
		case PLAY_OBJECT_TIMED_OUT:
			// Object has finished playing, remove it from the queue
			pTempObj = pObj->GetNext();
			if(pObj->GetMsObject()->Is(CMsObject::MsObjType::NOTE))
			{
				pNote = (CMsNote*)pObj->GetMsObject();
				//if (pNote)
				//{
				//	fprintf(GETAPP->LogFile(), "   ---- Note Timed Out: Pitch:%d Ticks:%d\n",
				//		pNote->GetPitch(),
				//		pNote->GetTick()
				//	);
				//}
			}
			RemoveObject(pObj);
			delete pObj;
			pObj = pTempObj;
			AnObjectHasEnded++;
			break;
		default:
			pObj = pObj->GetNext();
			break;
		}
	}
	//if (AnObjectIsDone || AnObjectHasEnded)
	//{
	//	fprintf(GETAPP->LogFile (), "Player Queue: Ticks:%d Delta:%d ->Played:%d Played Objects Done:%d  Timed Out:%d  Total Left:%d\n",
	//		pSong->GetTotalTicks(),
	//		pSong->GetTotalTicks() - LastTickLog,
	//		ObjectsPlayed,
	//		AnObjectIsDone,
	//		AnObjectHasEnded,
	//		GetTotalObjects()
	//	);
	//	LastTickLog = pSong->GetTotalTicks();
	//}
	if(m_nTotalObjects == 0)
		AnObjectHasEnded = 1;
	return AnObjectHasEnded;
}

UINT CMsPlayerQueue::WindDown()
{
	CMsPlayerQueueItem* pObj = m_pHead;
	CMsPlayerQueueItem* pTempObj = 0;
	UINT DeleteObjectFlag = 0;

	while (pObj)
	{
		DeleteObjectFlag = pObj->GetMsObject()->Play();
		switch (DeleteObjectFlag)
		{
		case PLAY_NO_ACTION:
			pObj = pObj->GetNext();
			break;
		case PLAY_OBJECT_DONE:
			[[fallthrough]];
		case PLAY_OBJECT_TIMED_OUT:
			// Object has finished playing, remove it from the queue
			pTempObj = pObj->GetNext();
			RemoveObject(pObj);
			delete pObj;
			pObj = pTempObj;
			break;
		default:
			pObj = pObj->GetNext();
			break;
		}
	}
	return GetTotalObjects();
}

bool CMsPlayerQueue::AddObjectToTail(CMsPlayerQueueItem* pObj)
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
	++m_nTotalObjects;
	return true;
}

bool CMsPlayerQueue::AddObjectToHead(CMsPlayerQueueItem* pObj)
{
	if (!m_pHead && !m_pTail)
	{
		// First object in the queue
		m_pHead = pObj;
		m_pTail = pObj;
		pObj->SetNext(nullptr);
		pObj->SetPrev(nullptr);
	}
	else
	{
		// Add to the front of the queue
		pObj->SetNext(m_pHead);
		m_pHead->SetPrev(pObj);
		m_pHead = pObj;
	}
	++m_nTotalObjects;
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
	--m_nTotalObjects;
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
//	fprintf(pO, "Total Objects in Queue:%d\n", GetTotalObjects());
	CMsPlayerQueueItem* pItem = m_pHead;
	CMsNote* pNote = nullptr;

	while (pItem)
	{
		if (pItem)
		{
			if(pItem->GetMsObject()->Is(CMsObject::MsObjType::NOTE))
			{
				pNote = (CMsNote*)pItem->GetMsObject();
				pNote->Print(pO, 4);
			}
		}
		pItem = pItem->GetNext();
	}
	return 0;
}
