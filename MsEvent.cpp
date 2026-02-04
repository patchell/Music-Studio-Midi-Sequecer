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
	m_pEventMsObjectListHead = 0;
	m_pEventMsObjectListTail = 0;
	m_Selected = 0;	// Selected If True
	m_Index = -1;	//WHERE in event chain this is located
	m_pView = 0;
	m_pParentSong = 0;
	m_EventID = GETAPP->GetUniqueID();
	m_NumberOfObjects = 0;
}

CMsEvent::~CMsEvent()
{
	if(m_pEventMsObjectListHead)
	{
		CMsObject *pObj,*pObjDel;

		pObj = m_pEventMsObjectListHead;
		while(pObj)
		{
			pObjDel = pObj;
			pObj = pObj->GetNext();
			if(pObjDel)delete pObjDel;
		}
	}
}

bool CMsEvent::Create(CMsSong*pParentSong, CChildViewStaff* pCV)
{
	bool Ret = true;

	m_pView = pCV;
	m_pParentSong = pParentSong;
	return Ret;
}

void CMsEvent::Print(FILE *pO, const char* s, int Indent)
{
	char* pIndentString = new char[256];
	CMsObject *pObj = GetEventMsObjectHead();

	theApp.IndentString(pIndentString, 256, Indent);
//	fprintf(pO, "%s========= %s Event %d Begin ========\n", pIndentString, s, m_Index);
	while(pObj)
	{
		pObj->Print(pO, Indent + 4);
		pObj = pObj->GetNext();
	}
//	fprintf(pO,"%s------------ event %d end-------------\n", pIndentString, m_Index);
	if(pIndentString) delete[] pIndentString;
}

void CMsEvent::PrintEvents(FILE *pO, const char *pTitel, int Indent)
{
	CMsEvent* pEV = this;
	char* pIndentString = new char[256];
	int Count = 0;

	theApp.IndentString(pIndentString, 256, Indent);
//	fprintf(pO, "^^^^^^^^^^^^^^^ %s ^^^^^^^^^^^^^^^^^^^^^^\n", pTitel);
	while (pEV)
	{
//		fprintf(pO,"*** Event %d  ID %d\n", pEV->GetIndex(), pEV->GetEventID());
		pEV->Print(pO, "Objects", Indent+4);
		pEV = pEV->GetNext();
		Count++;
		if(Count > 1000)
		{
//			fprintf(pO, "Error: Too many events, possible infinite loop\n");
			break;
		}
	}
//	fprintf(pO, "^^^^^^^^^^^ End %s ^^^^^^^^^^^^^^^^^^^^^^\n\n\n", pTitel);
}

int CMsEvent::GetPhysicalIndex()
{
	int physicalIndex = 0;
	physicalIndex = m_Index - GetSong()->GetStaffChildView()->GetSongScrollPosition();
	return physicalIndex;
}

void CMsEvent::AddObject(CMsObject* pO)
{
	CMsNote* pNote = 0;

	if (pO)
	{
		if (LogFile())
		{
			fprintf(GETAPP->LogFile(), "\t\tEvent %d: Add Object Type %s Object Event %d ID=%d\n",
				GetIndex(),
				CMsObject::GetStringFromType(pO->GetType()),
				GetIndex(),
				pO ? pO->GetObjectID() : -1
			);
		}
		pO->SetParentEvent(this);
		switch (pO->GetType())
		{
		case CMsObject::MsObjType::NOTE:
			pNote = (CMsNote*)pO;
			if (pNote->IsRest())
				AddObjectAtEnd(pO);
			else
				AddNoteInOrder(pO);
			break;
		case CMsObject::MsObjType::REPEATSTART:
		case CMsObject::MsObjType::REPEATEND:
		case CMsObject::MsObjType::BAR:
		case CMsObject::MsObjType::ENDBAR:
		case CMsObject::MsObjType::KEYSIG:
		case CMsObject::MsObjType::TIMESIG:
		case CMsObject::MsObjType::TEMPO:
		case CMsObject::MsObjType::LOUDNESS:
			AddObjectAtEnd(pO);
			break;
		case CMsObject::MsObjType::PORTAMENTO_START:
			break;
		case CMsObject::MsObjType::PORTAMENTO_STOP:
			break;
		case CMsObject::MsObjType::GLISSANDO:
			break;
		case CMsObject::MsObjType::GLISSANDO_END:
			break;
		case CMsObject::MsObjType::CHORD:
			break;
		case CMsObject::MsObjType::CHORD_NOTE:
			break;
		default:
			break;
		}
	}
	else
	{
		AfxMessageBox(_T("Error: Attempting to add a NULL object to an event"));
	}
}

void CMsEvent::AddObjectAtEnd(CMsObject *pO)
{
	if (pO)
	{
		if (GetEventMsObjectHead() == 0)
		{
			SetEventMsObjectHead(pO);
			SetEventMsObjectTail(pO);
		}
		else
		{
			pO->SetPrev(GetEventMsObjectTail());
			GetEventMsObjectTail()->SetNext(pO);
			SetEventMsObjectTail(pO);
		}
		m_NumberOfObjects++;
	}
}

void CMsEvent::AddObjectAtStart(CMsObject *pO)
{
	if(GetEventMsObjectHead() == 0)
	{
		SetEventMsObjectHead(pO);
		SetEventMsObjectTail(pO);
	}
	else
	{
		pO->SetNext(GetEventMsObjectHead());
		GetEventMsObjectHead()->SetPrev(pO);
		SetEventMsObjectHead(pO);
	}
	m_NumberOfObjects++;
}

void CMsEvent::AddObjectAtHead(CMsObject* pO)
{
	pO->SetNext(GetEventMsObjectHead());
	GetEventMsObjectHead()->SetPrev(pO);
	SetEventMsObjectHead(pO);
	m_NumberOfObjects++;
}

void CMsEvent::AddObjectAtTail(CMsObject* pO)
{
	pO->SetPrev(GetEventMsObjectTail());
	if(GetEventMsObjectTail())
		GetEventMsObjectTail()->SetNext(pO);
	SetEventMsObjectTail(pO);
	m_NumberOfObjects++;
}

void CMsEvent::InsertObjectAfter(CMsObject* pO, CMsObject* pAfterObj)
{
	if(pAfterObj == GetEventMsObjectTail())
	{
		AddObjectAtTail(pO);
	}
	else
	{
		CMsObject* pNextObj = pAfterObj->GetNext();
		pAfterObj->SetNext(pO);
		pO->SetPrev(pAfterObj);
		pO->SetNext(pNextObj);
		if(pNextObj)
			pNextObj->SetPrev(pO);
		m_NumberOfObjects++;
	}
}

void CMsEvent::InsertObjectBefore(CMsObject* pO, CMsObject* pBeforeObj)
{
	if(pBeforeObj == GetEventMsObjectHead())
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

int CMsEvent::AreThereAnyNotesInThisEvent()
{
	int Found = 0;
	CMsObject *pObj = GetEventMsObjectHead();
	CMsNote* pNote;

	while (pObj)
	{
		if (pObj->Is(CMsObject::MsObjType::NOTE))
		{
			pNote = (CMsNote*)pObj;
			if(pNote->IsNote())
			{
				Found++;
			}
		}
		pObj = pObj->GetNext();
	}
	return Found;
}

bool CMsEvent::IsThisObjectInThisEvent(CMsObject* pO)
{
	bool bFound = false;
	CMsObject* pObj = GetEventMsObjectHead();

	if(LogFile())
	{
		fprintf(GETAPP->LogFile(), "\t\tEvent %d: Is This Object %s of %d Events  Object::Event=%d ID=%d\n", 
			GetIndex(),
			CMsObject::GetStringFromType(pO->GetType()),
			m_NumberOfObjects,
			pO?pO->GetParentEvent()->GetIndex(): -1,
			pO ? pO->GetObjectID() : -1
		);
	}
	while (pObj && !bFound)
	{
		if (pObj == pO && 
			pO->GetObjectID() == pObj->GetObjectID() && 
			pO->GetParentEvent()->GetIndex() == GetIndex()
		)
		{
			bFound = true;
		}
		else
		{
			pObj = pObj->GetNext();
		}
	}
	return bFound;
}

bool CMsEvent::IsThereOnlyOneNoteInThisEvent()
{
	bool bOnlyOne = true;
	CMsObject* pObj = GetEventMsObjectHead();
	CMsNote* pNote;
	int noteCount = 0;

	while (pObj && bOnlyOne)
	{
		if (pObj->Is(CMsObject::MsObjType::NOTE))
		{
			pNote = (CMsNote*)pObj;
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

CMsObject* CMsEvent::FindFirstObjectOfType(CMsObject::MsObjType ObjectType)
{
	CMsObject* pObj = GetEventMsObjectHead();
	bool bNotFound = true;

	while (pObj && bNotFound)
	{
		if (pObj->Is(ObjectType))
		{
			bNotFound = false;
		}
		else
		{
			pObj = pObj->GetNext();
		}
	}
	return pObj;
}

CMsObject* CMsEvent::FindNextObjectOfType(CMsObject::MsObjType ObjectType, CMsObject* pPrevObj)
{
	CMsObject* pObj = pPrevObj->GetNext();
	bool bNotFound = true;

	while (pObj && bNotFound)
	{
		if (pObj->Is(ObjectType))
		{
			bNotFound = false;
		}
		else
		{
			pObj = pObj->GetNext();
		}
	}
	return pObj;
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
	CMsNote* pNewNote = (CMsNote*)pO;
	CMsNote* pNote = 0;
	CMsObject* pObj = GetEventMsObjectHead();
	int NumberOfNotes = 0;
	bool bLoop = true;

	NumberOfNotes = AreThereAnyNotesInThisEvent();
	if (NumberOfNotes)
	{
		fprintf(GETAPP->LogFile(), "\t\tEvent %d: There **ARE** %d Notes in this Event\n", GetIndex(), NumberOfNotes);
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
//			FlipNoteHeads();
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
						
						pObj = GetEventMsObjectTail();
						InsertObjectAfter(pO, pObj);
						bLoop = false;
					}
				}
			}
		//			FlipNoteHeads();
		}
	}
	else
	{
		//--------------------------------
		// No notes in this event
		//--------------------------------
		fprintf(GETAPP->LogFile(), "\t\tEvent %d:**NO Notes\n", GetIndex());
		if(m_NumberOfObjects == 0)
		{
			SetEventMsObjectHead(pO);
			SetEventMsObjectTail(pO);
			m_NumberOfObjects++;
		}
		else
		{
			pObj = GetEventMsObjectTail();
			AddObjectAtTail(pO);
		}
	}
}

CMsNote* CMsEvent::FindFirstNote()
{
	CMsNote* pNote = 0;
	CMsObject* pObj = GetEventMsObjectHead();
	bool bFound = false;

	while (pObj && !bFound)
	{
		if (pObj->Is(CMsObject::MsObjType::NOTE))
		{
			pNote = (CMsNote*)pObj;
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
		if (pObj->Is(CMsObject::MsObjType::NOTE))
		{
			pNote = (CMsNote*)pObj;
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

void CMsEvent::Draw(CDC *pDC)
{
	//------------------------------
	// Draw the event
	// pDC is the device context for
	// this event.  It includes the
	// two selection bars, the draw
	// for the spaces that will not
	// include notes, and the edit
	// area.
	// This method will create a
	// device context for the event
	// objects to be drawn into.
	// 
	// This method will draw the
	// two selection bars, fill in
	// the background for the edit/
	// draw areas, and draw the
	// music staff before drawing
	// the event objects.
	//------------------------------
	CMsObject *pObj = GetEventMsObjectHead();
	CDC memDC;		//memory DC
	CMyBitmap bm;
	CBrush brUpperSel, brLowerSel, br;
	CBrush brBlue;
	CPoint ptRect;
	CSize szRect;
	CRect rectRect;
	COLORREF colorUpperSelBar = GetSong()->GetColorPalette()->color_UpperSelBar;
	COLORREF colorLowerSelBar = GetSong()->GetColorPalette()->color_LowerSelBar;

	brUpperSel.CreateSolidBrush(colorUpperSelBar);
	brLowerSel.CreateSolidBrush(colorLowerSelBar);
	brBlue.CreateSolidBrush(RGB(128, 128, 255));
	//------------------------------
	// Draw the select bars
	// Upper Select Bar
	//------------------------------
	szRect = CSize(EVENT_WIDTH, UPPER_SELECTION_BAR_HEIGHT);
	ptRect = CPoint(0, 0);
	rectRect = CRect(ptRect, szRect);
	pDC->FillRect(&rectRect, &brUpperSel);
	//-------- Lower Selection Bar ---------
	ptRect = CPoint(0, EVENT_HEIGHT - LOWER_SELECTION_BAR_HEIGHT);
	rectRect = CRect(ptRect, szRect);
	pDC->FillRect(&rectRect, &brLowerSel);


	//------------------------------
	// Create the memory DC for the
	// edit/draw area
	//------------------------------
	ptRect = CPoint(0, UPPER_SELECTION_BAR_HEIGHT);
	szRect = CSize(
		EVENT_WIDTH, 
		EDIT_RECT_HEIGHT + UPPER_DRAW_RECT_HEIGHT + LOWER_DRAW_RECT_HEIGHT
	);
	rectRect = CRect(ptRect, szRect);
	bm.CreateBitmap(szRect.cx, szRect.cy, 1, 32, 0);
	memDC.CreateCompatibleDC(pDC);
	memDC.SelectObject(&bm);
	if (IsSelected())
	{
		//--------------------
		// Selected Event Color
		// Used for Indicating
		// event that is being 
		// played
		//--------------------
		br.CreateSolidBrush(GetSong()->GetColorPalette()->color_SelectionRect);
	}
	else   
	{
		//--------------------------------
		// Color Indicating Event that
		// is active
		//--------------------------------
		br.CreateSolidBrush(GetSong()->GetColorPalette()->color_DefinedEvent);

	}
	//------------------------------
	// Fill in the draw areas
	// Background.  This is for 
	// DEBUGGING purposes only.
	// TODO: Remove this later
	//------------------------------
	ptRect = CPoint(0, 0);
	szRect = CSize(EVENT_WIDTH, UPPER_DRAW_RECT_HEIGHT);
	rectRect = CRect(ptRect, szRect);
	memDC.FillRect(&rectRect, &brBlue);

	ptRect = CPoint(0, UPPER_DRAW_RECT_HEIGHT + EDIT_RECT_HEIGHT);
	szRect = CSize(EVENT_WIDTH, LOWER_DRAW_RECT_HEIGHT);
	rectRect = CRect(ptRect, szRect);
	memDC.FillRect(&rectRect, &brBlue);
	//------------------------------
	// Fill in the edit area
	// Background
	//------------------------------
	ptRect = CPoint(0, UPPER_DRAW_RECT_HEIGHT);
	szRect = CSize(EVENT_WIDTH, EDIT_RECT_HEIGHT);
	rectRect = CRect(ptRect, szRect);
	memDC.FillRect(&rectRect, &br);
	DrawStaffLines(&memDC);
	while (pObj)
	{
		pObj->Draw(&memDC);
		pObj = pObj->GetNext();
	}
	//------------------------------
	// Add the edit/draw area
	// to the event DC
	//------------------------------
	pDC->BitBlt(
		0, 
		UPPER_SELECTION_BAR_HEIGHT, 
		EVENT_WIDTH,
		EDIT_RECT_HEIGHT + UPPER_DRAW_RECT_HEIGHT + LOWER_DRAW_RECT_HEIGHT,
		&memDC, 
		0, 
		0, 
		SRCCOPY
	);
}

void CMsEvent::DrawStaffLines(CDC* pDC)
{
	CPen penLines, * pOldPen;
	int x, y, i;

	//if(LogFile())
	//{ 
	//	fprintf(LogFile(), "Event %d: Draw Staff Lines--------\n", GetIndex());
	//}
	penLines.CreatePen(PS_SOLID, 1, GetSong()->GetColorPalette()->color_StaffLines);
	pOldPen = pDC->SelectObject(&penLines);
	x = EVENT_WIDTH;
	y = TOP_LINE_OF_STAVE_FROM_TOP_OF_EDIT_RECT + UPPER_DRAW_RECT_HEIGHT;
	for (i = 0; i < 5; ++i)	//treble
	{
		//if(LogFile())
		//{
		//	fprintf(LogFile(), "\tDrawing Treble Line %d at y=%d\n", i+1, y);
		//}
		pDC->MoveTo(0, y);
		pDC->LineTo(x, y);
		y += STAVE_LINE_SPACING;
	}
	y += STAVE_LINE_SPACING;
	for (i = 0; i < 5; ++i)	//bass
	{
		//if (LogFile())
		//{
		//	fprintf(LogFile(), "\tDrawing Bass Line %d at y=%d\n", i + 1, y);
		//}
		pDC->MoveTo(0, y);
		pDC->LineTo(x, y);
		y += STAVE_LINE_SPACING;
	}
	pDC->SelectObject(pOldPen);
	penLines.DeleteObject();
}

CMsObject *CMsEvent::ObjectAlreadyHere(CMsObject *pObj)
{
	int loop = 1;
	ObjectTypes obj,sobj;
	CMsObject *rpObj=0;

	CMsObject *pH = GetEventMsObjectHead();
	while(pH && loop)
	{
		if(pH->GetType() == pObj->GetType())
		{
			switch(pH->GetType())
			{
			case CMsObject::MsObjType::KEYSIG:
				case CMsObject::MsObjType::BAR:
				case CMsObject::MsObjType::LOUDNESS:
				case CMsObject::MsObjType::TEMPO:
				case CMsObject::MsObjType::TIMESIG:
					rpObj = pH;
					loop = 0;
					break;
				case CMsObject::MsObjType::ENDBAR:
					break;
				case CMsObject::MsObjType::NOTE:
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
				case CMsObject::MsObjType::REPEATEND:
					break;
				case CMsObject::MsObjType::REPEATSTART:
					break;
			}
		}
		pH = pH->GetNext();
	}
	return rpObj;
}

int CMsEvent::GetMinEventDuration()
{
	CMsObject* pObj = GetEventMsObjectHead();
	int MinDuration = 10000000;
	int Dur;

	while (pObj)
	{
		Dur = pObj->IsTimedObject();
		if(Dur < MinDuration)
			MinDuration = Dur;
		pObj = pObj->GetNext();
	}
    return MinDuration;
}

int CMsEvent::RemoveObject(CMsObject *pObj)
{
	//--------------------------------
	// Remove object from event
	//--------------------------------

	//------------------------------
	// Is Object NULL?
	if (pObj == 0)
	{
		CString csError;
		csError.Format(_T("Error: CMsEvent::RemoveObject: At Event %d Object is NULL"), GetIndex());
		if (LogFile()) fprintf(LogFile(), "Error: CMsEvent::RemoveObject : At Event % d Object is NULL\n", GetIndex());
		GETAPP->CloseAllLogFiles();
		GETAPP->ExitInstance();
		return -1;
	}
	//------------------------------
	// Is Object in this event?
	//------------------------------
	if(IsThisObjectInThisEvent(pObj) == false)
	{
		CString csError;
		csError.Format(_T("Error: CMsEvent::RemoveObject: At Event %d Object not in this event"), GetIndex());
		AfxMessageBox(csError);
		if (LogFile()) fprintf(LogFile(), "Error: CMsEvent::RemoveObject : At Event % d Object ID=%d not in this event\n", 
			GetIndex(),
			pObj->GetObjectID()
		);
		GETAPP->CloseAllLogFiles();
		GETAPP->CloseAllDocuments(true);
		GETAPP->PostThreadMessageW(WM_CLOSE, 0, 0);;
		return -1;
	}
	pObj->Print(LogFile(), 5);
	if(pObj == GetEventMsObjectHead() )
	{
		fprintf(GETAPP->LogFile(), "\t\tEvent:%d:Remove Object %s From Head ID=%d\n", 
			GetIndex(),
			CMsObject::GetStringFromType(pObj->GetType()),
			pObj->GetObjectID()
		);
		SetEventMsObjectHead(pObj->GetNext());
		if(GetEventMsObjectHead())
			GetEventMsObjectHead()->SetPrev(0);
		else
			SetEventMsObjectTail(0);
	}
	else if(pObj == GetEventMsObjectTail())
	{
		fprintf(GETAPP->LogFile(), "\t\tEvent:%d:Remove Object %s From Tail ID=%d\n",
			GetIndex(),
			CMsObject::GetStringFromType(pObj->GetType()),
			pObj->GetObjectID()
		);
		SetEventMsObjectTail(pObj->GetPrev());
		if(GetEventMsObjectTail())
			GetEventMsObjectTail()->SetNext(0);
		else
			SetEventMsObjectHead(0);
	}
	else
	{
		fprintf(GETAPP->LogFile(), "\t\tEvent:%d:Remove Object %s From Middle ID=%d\n",
			GetIndex(),
			CMsObject::GetStringFromType(pObj->GetType()),
			pObj->GetObjectID()
		);
		pObj->GetNext()->SetPrev(pObj->GetPrev());
		pObj->GetPrev()->SetNext(pObj->GetNext());
	}
	m_NumberOfObjects--;
	pObj->SetNext(0);
	pObj->SetPrev(0);	
	return m_NumberOfObjects;
}


// Function returns Note of a given pitch
CMsNote * CMsEvent::FindNote(int Note, int Accidental)
{
	CMsObject *pObj = GetEventMsObjectHead();
	CMsNote *pN=0;
	int loop = 1;
	while (pObj && loop)
	{
		if (pObj->GetType() == CMsObject::MsObjType::NOTE)
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
CMsObject *CMsEvent::ContainsObjectType(CMsObject::MsObjType ObjectType)
{
	CMsObject *pObj = GetEventMsObjectHead();
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
	CMsObject *pObj = GetEventMsObjectHead();
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
	CMsObject *pObj = GetEventMsObjectHead();
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
	// The highest pitched note will
	// have its head in the "normal"
	// position, i.e. if the stem
	// is down, the head will be
	// flipped, and if the stem is
	// up, the head will not be
	// flipped.  The second note
	// will be then flipped in
	// the opposite direction of the
	// previous note if the previos
	// note has the stem poinging
	// in the same direction.  From
	// there, things get messy.
	//------------------------------
	bool rV = false;
	CMsObject* pObj = GetEventMsObjectHead();
	CMsObject* pPrevObj = 0;
	CMsNote* pNote = 0, *pLastNote = 0;
	int Interval = 0;
	int NoteOneFlipped = 0;
	int NoteTwoFlipped = 0;
	int NoteFlagCount = 0;
	int NoteTwoNeedsFlags = 50;

	pNote = FindFirstNote();
	while (pNote && NoteTwoNeedsFlags)
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
//				if(LogFile()) fprintf(LogFile(),"Previous Note Needs Flags %d\n", NoteFlagCount);
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
		--NoteTwoNeedsFlags;
	}
	if (!NoteTwoNeedsFlags)
	{
		AfxMessageBox(_T("Error: Linked Less Error"));
		if (LogFile()) fprintf(LogFile(), "Error: CMsEvent::FlipNoteHeads: Linked List Error\n");
		GETAPP->CloseAllLogFiles();
		GETAPP->ExitInstance();
	}
    return rV;
}


CMsObject * CMsEvent::ContainsRepeatObject()
{
	CMsObject *pObj = GetEventMsObjectHead();
	int loop=1;
	CMsObject::MsObjType type;

	while (loop && pObj)
	{
		type = pObj->GetType();
		if (CMsObject::MsObjType::REPEATEND == type || CMsObject::MsObjType::REPEATSTART == type)
			loop = 0;
		else
		{
			pObj = pObj->GetNext();
		}
	}
	return pObj;
}

/*
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
	CMsObject* pObj = GetEventMsObjectHead();
	CMsObject* pPrevObj = 0;
	CMsNote* pNote = 0, *pLastNote = 0;
	int Interval = 0;
	int NoteOneFlipped = 0;
	int NoteTwoFlipped = 0;
	int NoteFlagCount = 0;
	int NoteTwoNeedsFlags = 50;

	pNote = FindFirstNote();
	while (pNote && NoteTwoNeedsFlags)
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
//				if(LogFile()) fprintf(LogFile(),"Previous Note Needs Flags %d\n", NoteFlagCount);
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
		--NoteTwoNeedsFlags;
	}
	if (!NoteTwoNeedsFlags)
	{
		AfxMessageBox(_T("Error: Linked Less Error"));
		if (LogFile()) fprintf(LogFile(), "Error: CMsEvent::FlipNoteHeads: Linked List Error\n");
		GETAPP->CloseAllLogFiles();
		GETAPP->ExitInstance();
	}
	return rV;
}
*/
