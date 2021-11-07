// MsObject.cpp: implementation of the CMsObject class.
//
//////////////////////////////////////////////////////////////////////

#include "pch.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CString csObjectTypeString[] = {
	CString("Bar"),
	CString("EndBar"),
	CString("KeySig"),
	CString("Loudness"),
	CString("Note/Rest"),
	CString("Repeat End"),
	CString("Repeat Start"),
	CString("Tempo"),
	CString("TimeSig"),
	CString("Portamento Start"),
	CString("Portamento Stop"),
	CString("Glisando Start"),
	CString("Glisando End"),
	CString("Chord"),
	CString("Chord Note")
};

CMsObject::CMsObject()
{
	m_Selected = 0;
	m_HighLight = FALSE;
	m_ObjType = 0;
	m_pNext = 0;
	m_pPrev = 0;
	m_pNextQueueObj = 0;
	m_pPrevQueueObj = 0;
	m_pSelectedObjectNext = 0;
	m_pSelectedObjectPrev = 0;
	m_ObjectID = GETAPP->GetUniqueID();
	m_pParentEvent = 0;
	m_pSong = 0;
}

CMsObject::~CMsObject()
{
}

void CMsObject::Create(CMsSong* pSong, UINT ParentEvent)
{
	m_pSong = pSong;
	SetParentEvent(ParentEvent);
}

void CMsObject::Print(FILE *pO)
{
	fprintf(pO, "CMsObject:Selected=%d\n", m_Selected);
	fprintf(pO, "CMsObject:ID=%d\n", m_ObjectID);
}

UINT CMsObject::Play()
{
	//------------------------------
	// This Methode is meant to
	// be overriden by the Object
	// that derived from this class
	// This is the defalt.
	// Return Values:
	//	0......Keep on ticking MSOBJ_PLAY_KEEP_TICKING
	//	1......Done with Object MSOBJ_PLAY_DONE
	//  2......Done with Object, but tied MSOBJ_PLAY_DONE_BUT_DO_NOT_REMOVE
	//-------------------------------
	return MSOBJ_PLAY_DONE;
}

UINT CMsObject::Process() {
	return 0; 
}

BOOL CMsObject::MouseOverObject(CPoint pt)
{
	BOOL MouseOverNote = FALSE;
	CRect rectNote;
	UINT Event = 0;

	Event = GetParentEvent()->GetParentView()->XtoEventIndex(pt.x);
	ObjectRectangle(rectNote, Event);
	MouseOverNote = rectNote.PtInRect(pt);
	return MouseOverNote;
}

BOOL CMsObject::HighLight(BOOL HL, CPoint ObjectPoint)
{
	BOOL rV = FALSE;

	if (MouseOverObject(ObjectPoint))
	{
		SetHighLight(HL);
		rV = TRUE;
	}
	return rV;
}

BOOL CMsObject::Select(BOOL Select, CPoint ObjectPoint)
{
	BOOL rV = FALSE;

	if (MouseOverObject(ObjectPoint))
	{
		SetSelected(Select);
		rV = TRUE;
	}
	return rV;
}

void CMsObject::SetParentEvent(UINT ParrentEvent)
{
	CString csType;
	CMsEvent* pEV = 0, * pLastEvent = 0;
	UINT Event;

	GetTypeString(csType);
	printf("Object %lS Parenet Event = %d\n", csType.GetString(),ParrentEvent);
	m_pParentEvent = GetSong()->GetEventObject(ParrentEvent);
	if (m_pParentEvent == NULL)
	{
		//-----------------------------------------
		// OK, this event does not acruall
		// exist yet, so make one
		//-----------------------------------------
		pLastEvent = GetSong()->GetEventListTail();
		if (pLastEvent)
			Event = pLastEvent->GetIndex() + 1;
		else
			Event = 0;
		do {
			pEV = new CMsEvent;
			pEV->Create(
				GetSong(),
				GetSong()->GetStaffChildView(),
				Event++
			);
			GetSong()->AddEventAtEnd(pEV);
		} while (pEV->GetIndex() != ParrentEvent);
		m_pParentEvent = pEV;
	}
}

int CMsObject::MouseLButtonDown(int DrawState, CPoint pointMouse)
{
	return DrawState;
}

int CMsObject::MouseLButtonUp(int DrawState, CPoint pointMouse)
{
	return DrawState;
}

int CMsObject::MouseMove(int DrawState, CPoint pointMouse)
{
	return DrawState;
}

void CMsObject::DebugDump()
{
	/*
	printf(">>>>> Object Dump >>>>>>>>>>\n");
	printf("Object ID = %d  %08x\n",m_ObjectID, m_ObjectID);
	printf("Selecte Flag=%d\n",m_Selected);	//object is selected
	printf("HighLite Flag=%d\n",m_HighLight);
	printf("Next Pointer =%p\n",m_pNext);
	printf("Prev Pointer =%p\n",m_pPrev);
	printf("Object Type =%d\n", m_ObjType);
	printf("<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n");
	*/
}

void CMsObject::GetTypeString(CString& csType)
{
	csType = csObjectTypeString[GetType()];
}

void CMsObject::Draw(CDC *pDC, int event, int maxevent)
{

}

void CMsObject::Copy(CMsObject* pSource)
{
	m_HighLight = 0;	//don't copy this
	m_Selected = 0;		//or this
	m_ObjType = pSource->GetType();
	m_ObjectID = GETAPP->GetUniqueID();
	m_pParentEvent = pSource->GetParentEvent();
	m_pSong = pSource->GetSong();
}

void CMsObject::Save(FILE *pO)
{

}
