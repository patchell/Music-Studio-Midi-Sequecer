// MsEvent.cpp: implementation of the CMsEvent class.
//
//////////////////////////////////////////////////////////////////////

#include "pch.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMsEvent::CMsEvent()
{
	m_pNext = 0;
	m_pPrev = 0;
	m_pEventObjectListHead = 0;
	m_pEventObjectListTail = 0;
	m_Selected = 0;	// Selected If True
	m_Index = 0;	//WHERE in event chat this is located
	m_pView = 0;
	m_pParentSong = 0;
	m_EventID = GETAPP->GetUniqueID();
	m_NumberOfObjects = 0;
}

CMsEvent::~CMsEvent()
{
	if(m_pEventObjectListHead)
	{
		CMsObject *pObj,*pObjDel;

		pObj = m_pEventObjectListHead;
		while(pObj)
		{
			pObjDel = pObj;
			pObj = pObj->GetNext();
			delete pObjDel;
		}
	}
}

void CMsEvent::Create(CMsSong*pParentSong,CChildViewStaff* pCV, UINT Event)
{
	m_pView = pCV;
	//--------------------------------------------------
	// A lot of confusion over the EventCount param
	//--------------------------------------------------
	m_Index = Event;
	m_pParentSong = pParentSong;
}

void CMsEvent::Print(FILE *pO, const char* s, int Indent)
{
	char* pIndentString = new char[256];
	CMsObject *pObj = GetEventObjectHead();

	theApp.IndentString(pIndentString, 256, Indent);
	fprintf(pO, "%s========= %s Event %d Begin ========\n", pIndentString, s, m_Index);
	while(pObj)
	{
		pObj->Print(pO, Indent + 4);
		pObj = pObj->GetNext();
	}
	fprintf(pO,"%s------------ event %d end-------------\n", pIndentString, m_Index);
	delete[] pIndentString;
}

void CMsEvent::PrintEvents(FILE *pO, const char *pTitel, int Indent)
{
	CMsEvent* pEV = this;
	char* pIndentString = new char[256];
	theApp.IndentString(pIndentString, 256, Indent);
	fprintf(pO, "^^^^^^^^^^^^^^^ %s ^^^^^^^^^^^^^^^^^^^^^^\n", pTitel);
	while (pEV)
	{
//		fprintf(pO,"*** Event %d  ID %d\n", pEV->GetIndex(), pEV->GetEventID());
		pEV->Print(pO, "Objects", Indent+4);
		pEV = pEV->GetNext();
	}
	fprintf(pO, "^^^^^^^^^^^ End %s ^^^^^^^^^^^^^^^^^^^^^^\n\n\n", pTitel);
}

void CMsEvent::AddObjectAtEnd(CMsObject *pO)
{
	if (pO->Is(MSOBJ_NOTE))
	{
		CMsNote* pNote = dynamic_cast<CMsNote*>(pO);
		if (pNote->IsNote())
		{
			AddNoteInOrder(pO);
		}
		else if (pNote->IsRest())
		{
			if (GetEventObjectHead() == 0)
			{
				SetEventObjectHead(pO);
				SetEventObjectTail(pO);
			}
			else
			{
				pO->SetPrev(GetEventObjectTail());
				GetEventObjectTail()->SetNext(pO);
				SetEventObjectTail(pO);
			}
		}
		else
		{
			printf("This is strange, it should not happen\n");
		}
	}
	else if(GetEventObjectHead() == 0)
	{
		SetEventObjectHead(pO);
		SetEventObjectTail(pO);
	}
	else
	{
		pO->SetPrev(GetEventObjectTail());
		GetEventObjectTail()->SetNext(pO);
		SetEventObjectTail(pO);
	}
	m_NumberOfObjects++;
}

void CMsEvent::AddObjectAtStart(CMsObject *pO)
{
	if (pO->Is(MSOBJ_NOTE))
	{
		CMsNote* pNote = dynamic_cast<CMsNote*>(pO);
		if(pNote->IsNote())
		{
			AddNoteInOrder(pO);
		}
	}
	else if(GetEventObjectHead() == 0)
	{
		SetEventObjectHead(pO);
		SetEventObjectTail(pO);
	}
	else
	{
		pO->SetNext(GetEventObjectHead());
		GetEventObjectHead()->SetPrev(pO);
		SetEventObjectHead(pO);
	}
	m_NumberOfObjects++;
}

void CMsEvent::AddObjectAtHead(CMsObject* pO)
{
	pO->SetNext(GetEventObjectHead());
	GetEventObjectHead()->SetPrev(pO);
	SetEventObjectHead(pO);
	m_NumberOfObjects++;
}

void CMsEvent::AddObjectAtTail(CMsObject* pO)
{
	pO->SetPrev(GetEventObjectTail());
	GetEventObjectTail()->SetNext(pO);
	SetEventObjectTail(pO);
	m_NumberOfObjects++;
}

void CMsEvent::InsertObjectAfter(CMsObject* pO, CMsObject* pAfterObj)
{
	if(pAfterObj == GetEventObjectTail())
	{
		AddObjectAtTail(pO);
	}
	else
	{
		CMsObject* pNextObj = pAfterObj->GetNext();
		pAfterObj->SetNext(pO);
		pO->SetPrev(pAfterObj);
		pO->SetNext(pNextObj);
		pNextObj->SetPrev(pO);
		m_NumberOfObjects++;
	}
}

void CMsEvent::InsertObjectBefore(CMsObject* pO, CMsObject* pBeforeObj)
{
	if(pBeforeObj == GetEventObjectHead())
	{
		AddObjectAtHead(pO);
	}
	else
	{
		CMsObject* pPrevObj = pBeforeObj->GetPrev();
		pPrevObj->SetNext(pO);
		pO->SetPrev(pPrevObj);
		pO->SetNext(pBeforeObj);
		pBeforeObj->SetPrev(pO);
		m_NumberOfObjects++;
	}
}

bool CMsEvent::AreThereAnyNotesInThisEvent()
{
	bool bFound = 0;
	CMsObject *pObj = GetEventObjectHead();
	CMsNote* pNote;

	while (pObj && !bFound)
	{
		if (pObj->Is(MSOBJ_NOTE))
		{
			pNote = dynamic_cast<CMsNote*>(pObj);
			if(pNote->IsNote())
			{
				bFound = 1;
			}
			else
			{
				pObj = pObj->GetNext();
			}
		}
		else
			pObj = pObj->GetNext();
	}
	return bFound;
}

bool CMsEvent::IsThereOnlyOneNoteInThisEvent()
{
	bool bOnlyOne = true;
	CMsObject* pObj = GetEventObjectHead();
	CMsNote* pNote;
	int noteCount = 0;

	while (pObj && bOnlyOne)
	{
		if (pObj->Is(MSOBJ_NOTE))
		{
			pNote = dynamic_cast<CMsNote*>(pObj);
			if (pNote->IsNote())
			{
				noteCount++;
				if (noteCount > 1)
				{
					bOnlyOne = false;
				}
				else
				{
					pObj = pObj->GetNext();
				}
			}
			else
			{
				pObj = pObj->GetNext();
			}
		}
		else
			pObj = pObj->GetNext();
	}
	return bOnlyOne;
}

void CMsEvent::AddNoteInOrder(CMsObject* pO)
{
	//--------------------------------
	// Add the note in order
	// Higher pitch notes go first
	// (near the Head) and the
	// lower pitch notes go last
	// (near the Tail)=-]ol
	//--------------------------------
	CMsNote* pNewNote = dynamic_cast<CMsNote*>(pO);
	CMsNote* pNote = 0;
	CMsObject* pObj = GetEventObjectHead();

	bool bLoop = true;

	if (AreThereAnyNotesInThisEvent())
	{
		if (IsThereOnlyOneNoteInThisEvent())
		{
			pNote = FindFirstNote();
			if(pNewNote->GetPitch() > pNote->GetPitch())
			{
				InsertObjectBefore(pO, pNote);
			}
			else
			{
				InsertObjectAfter(pO, pNote);
			}
			FlipNoteHeads();
		}
		else
		{
			//--------------------------------
			// More than one note in this event
			//--------------------------------
			pNote = FindFirstNote();
			while(pNote && bLoop)
			{
				if(pNewNote->GetPitch() > pNote->GetPitch())
				{
					InsertObjectBefore(pO, pNote);
					bLoop = false;
				}
				else
				{
					pNote = FindNextNote(pNote);
					if(pNote == 0)
					{
						//--------------------------------
						// Reached end of notes in this event
						// Add new note at end
						//--------------------------------
						
						pObj = GetEventObjectTail();
						InsertObjectAfter(pO, pObj);
						bLoop = false;
					}
				}
			}
			FlipNoteHeads();
		}
	}
	else
	{
		//--------------------------------
		// No notes in this event
		//--------------------------------
		if(m_NumberOfObjects == 0)
		{
			SetEventObjectHead(pO);
			SetEventObjectTail(pO);
		}
		else
		{
			pObj = GetEventObjectTail();
			AddObjectAtTail(pO);
		}
	}
}

CMsNote* CMsEvent::FindFirstNote()
{
	CMsNote* pNote = 0;
	CMsObject* pObj = GetEventObjectHead();
	bool bFound = false;

	while (pObj && !bFound)
	{
		if (pObj->Is(MSOBJ_NOTE))
		{
			pNote = dynamic_cast<CMsNote*>(pObj);
			if (pNote->IsNote())
			{
				bFound = true;
			}
			else
			{
				pObj = pObj->GetNext();
			}
		}
		else
			pObj = pObj->GetNext();
	}
	return pNote;
}

CMsNote* CMsEvent::FindNextNote(CMsNote* pPrevNote)
{
	CMsObject* pObj = pPrevNote->GetNext();
	bool bFound = false;
	CMsNote* pNote = 0;

	while (pObj && !bFound)
	{
		if (pObj->Is(MSOBJ_NOTE))
		{
			pNote = dynamic_cast<CMsNote*>(pObj);
			if (pNote->IsNote())
			{
				bFound = true;
			}
			else
			{
				pObj = pObj->GetNext();
			}
		}
		else
			pObj = pObj->GetNext();
	}
	return pNote;
}

void CMsEvent::Draw(CDC *pDC, int event, int maxevent)
{
	CMsObject *pObj = GetEventObjectHead();
	CDC dc;		//memory DC
	CMyBitmap bm;
	CPoint ptRectUpperLeft;
	CRect srect;
	int x = EVENT_OFFSET + EVENT_WIDTH * event;
	CSize rectSize = GetParentView()->GetSelectionRegionSize();
	CBrush br;
	int RawEvent = GetParentView()->GetRawEvent();
	
	ptRectUpperLeft = CPoint(x, m_pView->GetSelectionRegionTop());
	srect = CRect(ptRectUpperLeft, rectSize);
	bm.CreateBitmap(rectSize.cx, rectSize.cy, 1, 32, 0);
	dc.CreateCompatibleDC(pDC);
	dc.SelectObject(&bm);
	if (event != (GetParentView()->GetMaxEvents() ) )
	{
		if (IsSelected() && (event != RawEvent))
		{
			//--------------------
			// Selected Event Color
			// Used for Indicating
			// event that is being 
			// played
			//--------------------
			br.CreateSolidBrush(RGB(128, 0, 0));
		}
		else if (IsSelected() && (event == RawEvent))
		{
			br.CreateSolidBrush(RGB(220, 255, 200));
		}
		else if (event == RawEvent)
		{
			br.CreateSolidBrush(RGB(128, 200, 81));
		}
		else   
		{
			//--------------------------------
			// Color Indicating Event that
			// is active
			//--------------------------------
			br.CreateSolidBrush(RGB(255, 200, 230));

		}
		dc.FillRect(CRect(0, 0, rectSize.cx, rectSize.cy), &br);
		dc.BitBlt(
			0, 
			0, 
			rectSize.cx, 
			rectSize.cy, 
			pDC, 
			ptRectUpperLeft.x, 
			ptRectUpperLeft.y, 
			SRCAND
		);
		pDC->BitBlt(
			ptRectUpperLeft.x, 
			ptRectUpperLeft.y, 
			rectSize.cx, 
			rectSize.cy, 
			&dc, 
			0, 
			0, 
			SRCCOPY
		);
	}
//	printf("----------Start Draw Objects In Event %d --------\n", GetIndex());
	UINT itter = 0;
	while(pObj)
	{
		if (pObj->GetObjectID() >= 200)
		{
			printf("OOOPPPS %d\n", pObj->GetObjectID());
		}
//		printf("************* Draw Itter = %d  pObj=%p Type %lS  *****************\n", ++itter, pObj, csObjectTypeString[pObj->GetType()].GetString());
		pObj->DebugDump();
		pObj->Draw(pDC,event,maxevent);
		pObj->DebugDump();
		pObj = pObj->GetNext();
	}
//	printf("--------- End Draw Objects in Event %d-----------\n", GetIndex());
}

CMsObject *CMsEvent::ObjectAlreadyHere(CMsObject *pObj)
{
	int loop = 1;
	ObjectTypes obj,sobj;
	CMsObject *rpObj=0;

	CMsObject *pH = GetEventObjectHead();
	while(pH && loop)
	{
		if(pH->GetType() == pObj->GetType())
		{
			switch(pH->GetType())
			{
				case MSOBJ_KEYSIG:
				case MSOBJ_BAR:
				case MSOBJ_LOUDNESS:
				case MSOBJ_TEMPO:
				case MSOBJ_TIMESIG:
					rpObj = pH;
					loop = 0;
					break;
				case MSOBJ_ENDBAR:
					break;
				case MSOBJ_NOTE:
					obj.pObj = pObj;
					sobj.pObj = pH;
					if(obj.pNote->GetPitch() == sobj.pNote->GetPitch())	//found it
					{
						if(obj.pNote->GetTrack() == sobj.pNote->GetTrack())
						{
							rpObj=pH;
							loop = 0;
						}
					}
					break;
				case MSOBJ_REPEATEND:
					break;
				case MSOBJ_REPEATSTART:
					break;
			}
		}
		pH = pH->GetNext();
	}
	return rpObj;
}

int CMsEvent::RemoveObject(CMsObject *pObj)
{
	if(pObj == GetEventObjectHead() )
	{
		SetEventObjectHead(pObj->GetNext());
		if(GetEventObjectHead())
			GetEventObjectHead()->SetPrev(0);
		else
			SetEventObjectTail(0);
	}
	else if(pObj == GetEventObjectTail())
	{
		SetEventObjectTail(pObj->GetPrev());
		if(GetEventObjectTail())
			GetEventObjectTail()->SetNext(0);
		else
			SetEventObjectHead(0);
	}
	else
	{
		pObj->GetNext()->SetPrev(pObj->GetPrev());
		pObj->GetPrev()->SetNext(pObj->GetNext());
	}
	delete pObj;
	return 0;
}


// Function returns Note of a given pitch
CMsNote * CMsEvent::FindNote(int Note, int Accidental)
{
	CMsObject *pObj = GetEventObjectHead();
	CMsNote *pN=0;
	int loop = 1;
	while (pObj && loop)
	{
		if (pObj->GetType() == MSOBJ_NOTE)
		{
			pN = (CMsNote *)pObj;
			if (pN->GetPitch() == Note && pN->GetAccidental() == Accidental)
				loop = 0;
			else
			{
				pObj = pObj->GetNext();
				pN = 0;
			}
		}
		else
		{
			pObj = pObj->GetNext();
		}
	}
	return pN;
}


// Determines if the paricular object is in this event
CMsObject *CMsEvent::ContainsObjectType(int ObjectType)
{
	CMsObject *pObj = GetEventObjectHead();
	int loop = 1;

	while (loop && pObj)
	{
		if (pObj->GetType() == ObjectType)
		{
			loop = 0;
		}
		else
			pObj = pObj->GetNext();
	}
	return pObj;
}

int CMsEvent::AreObjectsSelected()
{
	int Count = 0;
	CMsObject *pObj = GetEventObjectHead();
	while (pObj)
	{
		if (pObj->IsSelected())
			++Count;
		pObj = pObj->GetNext();
	}
	return Count;
}

CMsObject * CMsEvent::GetFirstSelectedObject()
{
	CMsObject *pObj = GetEventObjectHead();
	int loop=1;
	while (pObj && loop)
	{
		if (pObj->IsSelected())
			loop = 0;
		else
			pObj = pObj->GetNext();
	}
	return pObj;
}

CMsObject * CMsEvent::GetNextSelectedObject(CMsObject * pLastSelectedObject)
{
	CMsObject *pObj = pLastSelectedObject->GetNext();
	int loop = 1;
	while (pObj && loop)
	{
		if (pObj->IsSelected())
			loop = 0;
		else
			pObj = pObj->GetNext();
	}
	return pObj;
}

bool CMsEvent::FlipNoteHeads()
{
	//------------------------------
	// Flip note heads so that they
	// do not overlap each other
	// Also, we need to take care
	// of the flags used on eighth
	// notes, sixteenth notes, and
	// thirty-second notes
	//------------------------------
	bool rV = false;
	CMsObject* pObj = GetEventObjectHead();
	CMsObject* pPrevObj = 0;
	CMsNote* pNote = 0, *pLastNote = 0;
	int Interval = 0;
	int NoteOneFlipped = 0;
	int NoteTwoFlipped = 0;
	int NoteFlagCount = 0;

	pNote = FindFirstNote();
	while (pNote)
	{
		if (pLastNote)
		{
			//------------------------------
			// If the prevous note has flags
			// on it's stem, and the stem
			// points in the same direction
			// as the current note, the
			// current note will need to
			// be set so that it's flags
			// are not displayed.
			// And if the previous note
			// has flags and the current
			// note has no flags, then
			// the stem should point
			// in the opposite direction
			//------------------------------
			NoteFlagCount = pLastNote->NeedsFlags();
			if (NoteFlagCount)
			{
				printf("Previous Note Needs Flags %d\n", NoteFlagCount);
				if (pNote->NeedsFlags())
				{
					if (pLastNote->IsStemDown() && pNote->IsStemDown())
					{
						pLastNote->SetFlagsOff(1);
					}
					if(!pLastNote->IsStemDown() && ! pNote->IsStemDown())
					{
						pNote->SetFlagsOff(1);
					}
					else
					{
					}
					
				}

			}
			//------------------------------
			// If the previous note is only
			// one step away from the
			// current note, flip the head
			// of the current note
			//-----------------------------
			Interval = pLastNote->GetPitch() - pNote->GetPitch();
			if (Interval <= 2)
			{
				NoteOneFlipped = pLastNote->GetHeadFlipped();
				if (NoteOneFlipped)
					pNote->SetHeadFlipped(0);
				else
					pNote->SetHeadFlipped(1);
			}
		}
		pLastNote = pNote;
		pNote = FindNextNote(pNote);
	}
    return rV;
}


CMsObject * CMsEvent::ContainsRepeatObject()
{
	CMsObject *pObj = GetEventObjectHead();
	int loop=1;
	int type;

	while (loop && pObj)
	{
		type = pObj->GetType();
		if (MSOBJ_REPEATEND == type || MSOBJ_REPEATSTART == type)
			loop = 0;
		else
		{
			pObj = pObj->GetNext();
		}
	}
	return pObj;
}
