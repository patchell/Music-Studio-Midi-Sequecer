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

void CMsEvent::Print(FILE *pO)
{
	CMsObject *pObj = GetEventObjectHead();
	fprintf(pO, "=========Event %d Begin ========\n", m_Index);
	while(pObj)
	{
		pObj->Print(pO);
		pObj = pObj->GetNext();
	}
	fprintf(pO,"------------ event end-------------\n");
}

void CMsEvent::PrintEvents(const char *pTitel)
{
	CMsEvent* pEV = this;
	printf("^^^^^^^^^^^^^^^ %s ^^^^^^^^^^^^^^^^^^^^^^\n", pTitel);
	while (pEV)
	{
		printf("*** Event %d  ID %d\n", pEV->GetIndex(), pEV->GetEventID());
		pEV = pEV->GetNext();
	}
	printf("^^^^^^^^^^^ End %s ^^^^^^^^^^^^^^^^^^^^^^\n", pTitel);
}

void CMsEvent::AddObjectAtEnd(CMsObject *pO)
{
	if(GetEventObjectHead() == 0)
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

void CMsEvent::AddObjectAtStart(CMsObject *pO)
{
	if(GetEventObjectHead() == 0)
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
			br.CreateSolidBrush(RGB(128, 132, 81));
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
			br.CreateSolidBrush(RGB(228, 48, 128));

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
