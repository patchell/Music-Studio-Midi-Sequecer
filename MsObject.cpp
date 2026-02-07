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
	m_HighLight = false;
	m_ObjType = MsObjType(-1);
	m_pNext = 0;
	m_pPrev = 0;
	m_pSelectedObjectNext = 0;
	m_pSelectedObjectPrev = 0;
	m_ObjectID = GETAPP->GetUniqueID();
	m_pParentEvent = 0;
	m_pSong = 0;
	m_PlayState = PLAYSTATE_NOT_STARTED;
}

CMsObject::CMsObject(MsObjType type)
{
	m_Selected = 0;
	m_HighLight = false;
	m_ObjType = type;
	m_pNext = 0;
	m_pPrev = 0;
	m_pSelectedObjectNext = 0;
	m_pSelectedObjectPrev = 0;
	m_ObjectID = GETAPP->GetUniqueID();
	m_pParentEvent = 0;
	m_pSong = 0;
	m_PlayState = PLAYSTATE_NOT_STARTED;
}

CMsObject::~CMsObject()
{
}

bool CMsObject::Create(CMsSong* pSong, CMsEvent* pEvent)
{
	if(!pEvent)
		pSong->GetStaffChildView()->MessageBoxW(
			CString("CMsObject::Create: Parent Event is NULL"),
			CString("Error"),
			MB_OK | MB_ICONERROR
		);
	//if(!pSong)
	//	GetStaffChildView()->MessageBoxW(
	//		CString("CMsObject::Create: Parent Song is NULL"),
	//		CString("Error"),
	//		MB_OK | MB_ICONERROR
	//	);
	m_pSong = pSong;
	m_pParentEvent = pEvent;
	return true;
}

bool CMsObject::Select(bool Select, CPoint ObjectPoint)
{
	bool rV = false;

	if (MouseOverObject(ObjectPoint))
	{
	SetSelected(Select);
		rV = true;
	}
	return rV;
}

//void CMsObject::SetParentEvent(UINT ParrentEvent)
//{
//	CMsEvent* pEV = 0;
//
//	m_pParentEvent = GetSong()->GetEventObject(ParrentEvent);
//}
//
void CMsObject::DebugDump()
{
	/*
	if(LogFile()) fprintf(LogFile(),">>>>> Object Dump >>>>>>>>>>\n");
	if(LogFile()) fprintf(LogFile(),"Object ID = %d  %08x\n",m_ObjectID, m_ObjectID);
	if(LogFile()) fprintf(LogFile(),"Selecte Flag=%d\n",m_Selected);	//object is selected
	if(LogFile()) fprintf(LogFile(),"HighLite Flag=%d\n",m_HighLight);
	if(LogFile()) fprintf(LogFile(),"Next Pointer =%p\n",m_pNext);
	if(LogFile()) fprintf(LogFile(),"Prev Pointer =%p\n",m_pPrev);
	if(LogFile()) fprintf(LogFile(),"Object Type =%d\n", m_ObjType);
	if(LogFile()) fprintf(LogFile(),"<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n");
	*/
}

void CMsObject::Print(FILE* pO, int Indent)
{
	char* pIndentString = new char[256];

	theApp.IndentString(pIndentString, 256, Indent);
	//	fprintf(pO, "%sCMsObject:Selected=%d\n", pIndentString, m_Selected);
	//	fprintf(pO, "%sCMsObject:ID=%d\n", pIndentString, m_ObjectID);
	if (pIndentString) delete[] pIndentString;
}

void CMsObject::GetTypeString(CString& csType)
{
	csType.Format(_T("%s"), GetStringFromType(GetType()));
}


bool CMsObject::MouseOverObject(CPoint pt)
{
	bool MouseOverNote = false;
	CRect rectNote;
	UINT Event = 0;

	Event = GetParentEvent()->GetParentView()->XtoEventIndex(pt.x);
	ObjectRectangle(rectNote, Event);
	MouseOverNote = rectNote.PtInRect(pt);
	return MouseOverNote;
}

bool CMsObject::HighLight(bool HL, CPoint ObjectPoint)
{
	bool rV = false;

	if (MouseOverObject(ObjectPoint))
	{
		SetHighLight(HL);
		rV = true;
	}
	return rV;
}

CChildViewStaff* CMsObject::GetStaffView()
{
	return  GetSong()->GetStaffChildView();
}

void CMsObject::Draw(CDC *pDC)
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

CChildViewStaff* CMsObject::GetStaffChildView()
{
	return GetSong()->GetStaffChildView();
}

CMsObject::MsObjType CMsObject::GetTypeFromString(const char* pTypeString)
{
	MsObjType TYPE = MsObjType(-1);

	for (int i = 0; ObjTypeStringTable[i].m_pTypeString != nullptr; i++)
	{
		if (ObjTypeStringTable[i].Compare(pTypeString))
		{
			TYPE =  ObjTypeStringTable[i].m_type;
		}
	}
	return TYPE;
}

const char* CMsObject::GetStringFromType(MsObjType type)
{
	const char* pR = nullptr;
	int i;

	for(i=0; ObjTypeStringTable[i].m_pTypeString != nullptr; i++)
	{
		if (ObjTypeStringTable[i].Is(type))
		{
			pR = ObjTypeStringTable[i].m_pTypeString;
		}
	}
	return pR;
}

const char* CMsObject::GetStringFromType() const
{
	const char* pR = nullptr;
	int i;
	MsObjType Type = GetType();

	for (i = 0; ObjTypeStringTable[i].m_pTypeString != nullptr; i++)
	{
		if (ObjTypeStringTable[i].Is(Type))
		{
			pR = ObjTypeStringTable[i].m_pTypeString;
		}
	}
	return pR;
}

void CMsObject::Save(FILE *pO)
{

}
