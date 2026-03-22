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
	CMsObject* pMsObj = nullptr;
	UINT AddedCount = 0;

	fprintf(LogFile(), "---- PROCESSING EVENT: %d\n", pEvent->GetIndex());
	pMsObj = pEvent->GetEventMsObjectHead();
	while (pMsObj)
	{
		ProcessStatus = pMsObj->Process();
		if(ProcessStatus)
		{
			// Object needs to be added to the player queue
			pQueueObj = new CMsPlayerQueueItem();
			pQueueObj->Create();
			pMsObj->PlayStateStart();
			pQueueObj->SetMsObject(pMsObj);
			if(pMsObj->IsTimedObject())
				AddObjectToTail(pQueueObj);
			else
			{
				AddObjectToHead(pQueueObj);
			}
			AddedCount++;
		}
		pMsObj = pMsObj->GetNext();
	}
//	fprintf(GETAPP->LogFile(), "==== PROCESS->Added %d Objects\n", AddedCount);
	return AddedCount;
}

UINT CMsPlayerQueue::Play(CMsSong* pSong)
{
	// Play the player queue
	CMsPlayerQueueItem* pQuObj = m_pHead;
	CMsPlayerQueueItem* pTempObj = 0;
	UINT DeleteObjectFlag = 0;
	UINT AnObjectHasEnded = 0;
	UINT AnObjectIsDone = 0;
	static UINT LastTickLog = 0;
	UINT ObjectsPlayed = 0;
	CMsNote* pNote = 0;

//	fprintf(GETAPP->LogFile(), "\t---- PLAY QUEUE: Total Objects:%d\n", GetTotalObjects());
	while(pQuObj)
	{
		DeleteObjectFlag = pQuObj->GetMsObject()->Play();
		ObjectsPlayed++;
		switch (DeleteObjectFlag)
		{
		case PLAY_NO_ACTION:
//			fprintf(GETAPP->LogFile(), "\t---- PLAY QUEUE: NO_ACTOION Total Objects:%d\n", GetTotalObjects());
			pQuObj = pQuObj->GetNext();
			break;
		case PLAY_OBJECT_DONE:
			fprintf(LogFile(), "\t---- PLAY QUEUE: OBJECT_DONE %s Event:%d Total Objects:%d\n", 
				pQuObj->GetMsObject()->GetStringFromType(),
				pQuObj->GetMsObject()->GetParentEvent()->GetIndex(),
				GetTotalObjects()
			);
			pTempObj = pQuObj->GetNext();
			RemoveObject(pQuObj);
			if(pQuObj) delete pQuObj;
			pQuObj = pTempObj;
			AnObjectIsDone++;
			break;
		case PLAY_OBJECT_TIMED_OUT:
			// Object has finished playing, remove it from the queue
			fprintf(GETAPP->LogFile(), "\t---- PLAY QUEUE: TIMED_OUT Total Objects:%d\n", GetTotalObjects());
			if(pQuObj->GetMsObject()->Is(CMsObject::MsObjType::NOTE))
			{
				pNote = (CMsNote*)pQuObj->GetMsObject();
				//if (pNote)
				//{
				//	fprintf(GETAPP->LogFile(), "   ---- Note Timed Out: Pitch:%d Ticks:%d\n",
				//		pNote->GetPitch(),
				//		pNote->GetTick()
				//	);
				//}
			}
			pTempObj = pQuObj->GetNext();
			RemoveObject(pQuObj);
			if(pQuObj) delete pQuObj;
			pQuObj = pTempObj;
			AnObjectHasEnded++;
			break;
		default:
			pQuObj = pQuObj->GetNext();
			break;
		}
	}	// End of while loop going through the player queue

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
			if(pObj) delete pObj;
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
	CMsObject* pMsObj = 0;

	if (pObj)
	{
		pMsObj = pObj->GetMsObject();
//		fprintf(LogFile(), "\t\t---- Adding %s to Tail of Queue\n", pMsObj->GetStringFromType());
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
			// Add to the end of the queue
			m_pTail->SetNext(pObj);
			pObj->SetPrev(m_pTail);
			pObj->SetNext(nullptr);
			m_pTail = pObj;
		}
		++m_nTotalObjects;
	}
	return true;
}

bool CMsPlayerQueue::AddObjectToHead(CMsPlayerQueueItem* pObj)
{
	CMsObject* pMsObj = 0;
	
	if(pObj)
	{
		pMsObj = pObj->GetMsObject();
//		fprintf(LogFile(), "\t\t---- Adding %s to Head of Queue\n", pMsObj->GetStringFromType());
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
	}
	return true;
}

bool CMsPlayerQueue::RemoveObject(CMsPlayerQueueItem* pObj)
{
	// Remove an object from the player queue
	bool rV = true;
	CMsNote* pNote = nullptr;
	CMsObject* pMsObj = nullptr;

	if (pObj)
	{
		pMsObj = pObj->GetMsObject();
		if (pMsObj->Is(CMsObject::MsObjType::NOTE))
		{
			char* pS = new char[256];
			pNote = (CMsNote*)pMsObj;
/*			fprintf(LogFile(), "\t\t---- Removing Note from Queue: Pitch:%s Event:%d Ticks:%d\n",
				pNote->NoteToString(pS, 256),
				pNote->GetParentEvent()->GetIndex(),
				pNote->GetTick()
			);*/
			delete[] pS;
		}
		else
		{
			pMsObj = pObj->GetMsObject();
			pNote = (CMsNote*)pMsObj;
/*			fprintf(LogFile(), "\t\t---- Removing Object from Queue: %s Event:%d Ticks:%d\n",
				pMsObj->GetStringFromType(),
				pMsObj->GetParentEvent()->GetIndex(),
				pNote->GetTick()
			);*/

		}
		if (pObj == m_pHead)
		{
			// Remove from head
			m_pHead = pObj->GetNext();
			if (m_pHead)
				m_pHead->SetPrev(nullptr);
			else
				m_pTail = nullptr;
		}
		else if (pObj == m_pTail)
		{
			// Remove from tail
			m_pTail = pObj->GetPrev();
			if (m_pTail)
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
				fprintf(LogFile(), "Error: Attempting to remove an object with bad links.\n");
			}
		}
		--m_nTotalObjects;
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
		if(pObj) delete pObj;
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
