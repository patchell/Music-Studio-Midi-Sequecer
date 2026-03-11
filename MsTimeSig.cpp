// MsTimeSig.cpp: implementation of the CMsTimeSig class.
//
//////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "MsTimeSig.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMsTimeSignature::CMsTimeSignature() :CMsObject(CMsObject::MsObjType::TIMESIG)
{
	m_TimeSig = TimeSigID::TS_6_8;
}

CMsTimeSignature::~CMsTimeSignature()
{

}

bool CMsTimeSignature::Create(CMsSong* pSong, CMsEvent* pEvent, TimeSigID TS)
{
	m_TimeSig = TS;
	pSong->SetCurrentTimeSignature(this);
	return CMsObject::Create(pSong, pEvent);
}

UINT CMsTimeSignature::Process()
{
	return 1;
}

UINT CMsTimeSignature::Play()
{
	GetSong()->SetCurrentTimeSignature(this);
	return PLAY_OBJECT_DONE;
}

DRAWSTATE CMsTimeSignature::MouseLButtonDown(DRAWSTATE DrawState, CPoint pointMouse, MouseRegions Region, MouseRegionTransitionState Transition)
{
	CMsEvent* pEV = nullptr;

	switch (DrawState)
	{
	case DRAWSTATE::SET_ATTRIBUTES:		//MouseLButtonDown
		break;
	case DRAWSTATE::WAITFORMOUSE_DOWN:		//MouseLButtonDown
		switch (Transition)
		{
		case MouseRegionTransitionState::MOUSE_TRANSITION_NONE:
			if (Region == MouseRegions::MOUSE_IN_EDITREG)
			{
				switch (StaffTransition(pointMouse, 0, GetParentEvent()))
				{
				case StaffMouseStates::MOUSE_STAFF_STATE_NONE:
				case StaffMouseStates::MOUSE_STAFF_STATE_NOTE_CHANGE:
					DrawState = DRAWSTATE::PLACE;
					break;
				case StaffMouseStates::MOUSE_STAFF_STATE_NOTE__EVENT_CHANGE:
					[[fallthrough]];
				case StaffMouseStates::MOUSE_STAFF_STATE_EVENT_CHANGE:
					pEV = GetParentEvent();
					if (pEV && pEV->IsThisObjectInThisEvent(this))
					{
						pEV->RemoveObject(this);
						SetParentEvent(nullptr);
					}
					pEV = GetSong()->GetEventObject(GetStaffView()->XtoEventIndex(pointMouse.x));
					if (pEV)
					{
						pEV->AddObject(this);
						SetParentEvent(pEV);
					}
					DrawState = DRAWSTATE::PLACE;
					break;
				}
			}
			break;
		case MouseRegionTransitionState::MOUSE_TRANSITION_EDIT_TO_UPPER_DRAW:		//MouseMove
		case MouseRegionTransitionState::MOUSE_TRANSITION_EDIT_TO_LOWER_DRAW:
		case MouseRegionTransitionState::MOUSE_TRANSITION_EDIT_TO_OUTSIDE:
			pEV = GetParentEvent();
			if (pEV && pEV->IsThisObjectInThisEvent(this))
			{
				pEV->RemoveObject(this);
				SetParentEvent(nullptr);
			}
			break;
		case MouseRegionTransitionState::MOUSE_TRANSITION_UPPER_DRAW_TO_EDIT:		//MouseMove
		case MouseRegionTransitionState::MOUSE_TRANSITION_LOWER_DRAW_TO_EDIT:
			[[fallthrough]];
		case MouseRegionTransitionState::MOUSE_TRANSITION_OUTSIDE_TO_EDIT:
			pEV = GetSong()->GetEventObject(GetStaffView()->XtoEventIndex(pointMouse.x));
			if (pEV)
			{
				pEV->AddObject(this);
				SetParentEvent(pEV);
			}
			break;
		case MouseRegionTransitionState::MOUSE_TRANSITION_LOWERSEL_TO_OUTSIDE:			//MouseMove
		case MouseRegionTransitionState::MOUSE_TRANSITION_UPPERSEL_TO_OUTSIDE:
			break;
		case MouseRegionTransitionState::MOUSE_TRANSITION_OUTSIDE_TO_LOWERSEL:		//MouseMove
		case MouseRegionTransitionState::MOUSE_TRANSITION_OUTSIDE_TO_UPPERSEL:
			break;
		case MouseRegionTransitionState::MOUSE_TRANSITION_ERROR:		//MouseMove
			break;
		}
		//		GetSong()->GetStaffChildView()->GetStatusBar()->SetText(csStatusString);
		break;
	}
	return DrawState;
}

DRAWSTATE CMsTimeSignature:: MouseLButtonUp(DRAWSTATE DrawState, CPoint pointMouse, MouseRegions Region, MouseRegionTransitionState Transition)
{
	CMsTimeSignature* pTS = nullptr;
	CMsEvent* pEV = nullptr;
	CString csText;

	switch (DrawState)
	{
	case DRAWSTATE::PLACE:
		switch (Transition)
		{
		case MouseRegionTransitionState::MOUSE_TRANSITION_NONE:
			if (Region == MouseRegions::MOUSE_IN_EDITREG)
			{
				switch (StaffTransition(pointMouse, 0, GetParentEvent()))
				{
				case StaffMouseStates::MOUSE_STAFF_STATE_NONE:
					[[fallthrough]];
				case StaffMouseStates::MOUSE_STAFF_STATE_NOTE_CHANGE:
					DrawState = Place(DrawState, pointMouse);
					break;
				case StaffMouseStates::MOUSE_STAFF_STATE_NOTE__EVENT_CHANGE:
					[[fallthrough]];
				case StaffMouseStates::MOUSE_STAFF_STATE_EVENT_CHANGE:
					DrawState = PlaceEventChanged(DrawState, pointMouse);
					break;
				}
			}
			break;
		case MouseRegionTransitionState::MOUSE_TRANSITION_EDIT_TO_UPPER_DRAW:		//MouseMove
			[[fallthrough]];
		case MouseRegionTransitionState::MOUSE_TRANSITION_EDIT_TO_LOWER_DRAW:
			[[fallthrough]];
		case MouseRegionTransitionState::MOUSE_TRANSITION_EDIT_TO_OUTSIDE:
			DrawState = RemoveThisFromEvent(DrawState);
			break;
		case MouseRegionTransitionState::MOUSE_TRANSITION_UPPER_DRAW_TO_EDIT:		//MouseMove
			[[fallthrough]];
		case MouseRegionTransitionState::MOUSE_TRANSITION_LOWER_DRAW_TO_EDIT:
			[[fallthrough]];
		case MouseRegionTransitionState::MOUSE_TRANSITION_OUTSIDE_TO_EDIT:
			DrawState = AddThisToEvent(DrawState, pointMouse);
			break;
		case MouseRegionTransitionState::MOUSE_TRANSITION_LOWERSEL_TO_OUTSIDE:			//MouseMove
			[[fallthrough]];
		case MouseRegionTransitionState::MOUSE_TRANSITION_UPPERSEL_TO_OUTSIDE:
			break;
		case MouseRegionTransitionState::MOUSE_TRANSITION_OUTSIDE_TO_LOWERSEL:		//MouseMove
			[[fallthrough]];
		case MouseRegionTransitionState::MOUSE_TRANSITION_OUTSIDE_TO_UPPERSEL:
			break;
		case MouseRegionTransitionState::MOUSE_TRANSITION_ERROR:		//MouseMove
			break;
		}
		//		GetSong()->GetStaffChildView()->GetStatusBar()->SetText(csStatusString);
		GetStaffView()->Invalidate();
		csText.Format(_T("Place Time Signature at Event %d"), GetStaffChildView()->GetDrawEvent());
		GetStaffChildView()->GetStatusBar()->SetText(csText);
		DrawState = DRAWSTATE::WAITFORMOUSE_DOWN;
		GetStaffChildView()->Invalidate();
		break;
	case DRAWSTATE::WAITFORMOUSE_DOWN:
		break;
	case DRAWSTATE::SET_ATTRIBUTES:
		break;
	default:
		break;
	}
	return DrawState;
}

DRAWSTATE CMsTimeSignature::MouseMove(DRAWSTATE DrawState, CPoint pointMouse, MouseRegions Region, MouseRegionTransitionState Transition)
{
	//-------------------------------------------------------
	// MouseMove
	//		This is the state machine for creating this
	//	object on the screen.  This function is for when
	//	the left mouse is moved.
	//
	//	parameters:
	//		pointMouse..Current Mouse Position
	//		DrawState...Current state of drawing process
	//		Region.....Current Regiion where mouse is
	//		Transition.Current mouse region transition state
	//
	//	Returns:
	//		Next Draw State
	//-------------------------------------------------------
	CString csStatusString, csTemp;
	CMsEvent* pEV = 0;
	
	//printf("TimeSig MouseMove DrawState:%d Region:%d Transition:%d\n",
	//	(int)DrawState,
	//	(int)Region,
	//	(int)Transition
	//);
	switch (DrawState)
	{
	case DRAWSTATE::SET_ATTRIBUTES:		//MouseMove
		break;
	case DRAWSTATE::WAITFORMOUSE_DOWN:		//MouseMove
		switch (Transition)
		{
		case MouseRegionTransitionState::MOUSE_TRANSITION_NONE:
			if (Region == MouseRegions::MOUSE_IN_EDITREG)
			{
				switch (StaffTransition(pointMouse, 0, GetParentEvent()))
				{
				case StaffMouseStates::MOUSE_STAFF_STATE_NONE:
					break;
				case StaffMouseStates::MOUSE_STAFF_STATE_NOTE_CHANGE:
					[[fallthrough]];
				case StaffMouseStates::MOUSE_STAFF_STATE_EVENT_CHANGE:
					[[fallthrough]];
				case StaffMouseStates::MOUSE_STAFF_STATE_NOTE__EVENT_CHANGE:
					pEV = GetParentEvent();
					if (pEV)
					{
						if (pEV->IsThisObjectInThisEvent(this))
						{
							pEV->RemoveObject(this);
							SetParentEvent(nullptr);
						}
					}
					pEV = GetSong()->GetEventObject(GetStaffView()->XtoEventIndex(pointMouse.x));
					if (pEV)
					{
						pEV->AddObject(this);
						SetParentEvent(pEV);
					}
					break;
				}
			}
			break;
		case MouseRegionTransitionState::MOUSE_TRANSITION_EDIT_TO_UPPER_DRAW:		//MouseMove
			[[fallthrough]];
		case MouseRegionTransitionState::MOUSE_TRANSITION_EDIT_TO_LOWER_DRAW:
			[[fallthrough]];
		case MouseRegionTransitionState::MOUSE_TRANSITION_EDIT_TO_OUTSIDE:
			DrawState = RemoveThisFromEvent(DrawState);
			break;
		case MouseRegionTransitionState::MOUSE_TRANSITION_UPPER_DRAW_TO_EDIT:		//MouseMove
			[[fallthrough]];
		case MouseRegionTransitionState::MOUSE_TRANSITION_LOWER_DRAW_TO_EDIT:
			[[fallthrough]];
		case MouseRegionTransitionState::MOUSE_TRANSITION_OUTSIDE_TO_EDIT:
			DrawState = AddThisToEvent(DrawState, pointMouse);
			break;
		case MouseRegionTransitionState::MOUSE_TRANSITION_LOWERSEL_TO_OUTSIDE:			//MouseMove
			[[fallthrough]];
		case MouseRegionTransitionState::MOUSE_TRANSITION_UPPERSEL_TO_OUTSIDE:
			break;
		case MouseRegionTransitionState::MOUSE_TRANSITION_OUTSIDE_TO_LOWERSEL:		//MouseMove
			[[fallthrough]];
		case MouseRegionTransitionState::MOUSE_TRANSITION_OUTSIDE_TO_UPPERSEL:
			break;
		case MouseRegionTransitionState::MOUSE_TRANSITION_ERROR:		//MouseMove
			break;
		}
		csStatusString.Format(
			_T("Place Time Signature: Event %d"), 
			GetStaffChildView()->GetDrawEvent()
		);
		GetSong()->GetStaffChildView()->GetStatusBar()->SetText(csStatusString);
		break;
	case DRAWSTATE::MOVE_OBJECT_AROUND:
		break;
	case DRAWSTATE::PLACE:
		break;
	}
	GetStaffView()->Invalidate();
	return DrawState;
}

DRAWSTATE CMsTimeSignature::Place(DRAWSTATE DrawState, CPoint pointMouse)
{
	CMsTimeSignature* pTS = nullptr;

	pTS = new CMsTimeSignature;
	pTS->Create(GetSong(), GetParentEvent(), GetTimeSignature());
	pTS->Copy(this);
	GetStaffChildView()->SetDrawObject(pTS);
	//-----------------------------
	GetParentEvent()->AddObject(pTS);
	GetStaffChildView()->CheckAndDoScroll(pointMouse);
	DrawState = DRAWSTATE::WAITFORMOUSE_DOWN;
	GetStaffChildView()->Invalidate();
	return DrawState;
}

DRAWSTATE CMsTimeSignature::PlaceEventChanged(DRAWSTATE DrawState, CPoint pointMouse)
{
	CMsTimeSignature* pTS = nullptr;
	CMsEvent* pEV = nullptr;

	pEV = GetParentEvent();
	if (pEV && pEV->IsThisObjectInThisEvent(this))
	{
		pEV->RemoveObject(this);
		SetParentEvent(nullptr);
	}
	pEV = GetSong()->GetEventObject(GetStaffView()->XtoEventIndex(pointMouse.x));
	if (pEV)
	{
		pEV->AddObject(this);
		SetParentEvent(pEV);
	}
	pTS = new CMsTimeSignature;
	pTS->Create(GetSong(), GetParentEvent(), GetTimeSignature());
	pTS->Copy(GetStaffChildView()->GetDrawObject());
	GetStaffChildView()->SetDrawObject(pTS);
	GetParentEvent()->AddObject(pTS);
	GetStaffChildView()->CheckAndDoScroll(pointMouse);
	GetStaffChildView()->Invalidate();
	DrawState = DRAWSTATE::WAITFORMOUSE_DOWN;
	return DrawState;
}

UINT CMsTimeSignature::ObjectToString(CString& csString, UINT mode)
{
	return 0;
}

void CMsTimeSignature::ObjectRectangle(CRect& rect, UINT Event)
{
}

void CMsTimeSignature::Print(FILE *pO, int Indent)
{
	char* pIndentString = new char[256];

	theApp.IndentString(pIndentString, 256, Indent);
//	fprintf(pO,"%sTime Signature:%s\n", pIndentString, TimeSigLut[m_TimeSig]);
	delete[] pIndentString;
}

void CMsTimeSignature::Draw(CDC *pDC)
{
	int x;
	CBitmap* oldBitmap;
	CDC dc;

	dc.CreateCompatibleDC(pDC);
	x = TS_OFFSET_X;
	//fprintf(LogFile(), "Drawing Time Signature %s<%d> at x=%d\n", 
	//	TimeSigLut[(int)m_TimeSig].m_pTimeSigString,
	//	(int)m_TimeSig,
	//	x
	//);
	oldBitmap = dc.SelectObject(GETAPP->bmGetTimeSig(int(m_TimeSig)));
	pDC->BitBlt(x,Y_CENTER_OF_TREBLE,16,33,&dc,0,0,SRCAND);
	pDC->BitBlt(x,Y_CENTER_OF_BASS,16,33,&dc,0,0,SRCAND);
	dc.SelectObject(oldBitmap);
}

StaffMouseStates CMsTimeSignature::StaffTransition(CPoint pointMouse, int NewNote, CMsEvent* pEvent)
{
	StaffMouseStates State = StaffMouseStates::MOUSE_STAFF_STATE_NONE;
	CMsEvent* pCurrentEvent = GetSong()->GetEventObject(GetStaffChildView()->XtoEventIndex(pointMouse.x));
	int CurrentEventIndex = pCurrentEvent ? pCurrentEvent->GetIndex() : -1;
	int thisEventIndex = pEvent ? pEvent->GetIndex() : -1;
	bool bEventChanged = (CurrentEventIndex != thisEventIndex);

	if (bEventChanged)
		State = StaffMouseStates::MOUSE_STAFF_STATE_EVENT_CHANGE;
	return State;
}

void CMsTimeSignature::Copy(CMsObject* pSource)
{
	m_TimeSig = ((CMsTimeSignature*)pSource)->m_TimeSig;
	CMsObject::Copy(pSource);
}

void CMsTimeSignature::Save(FILE *pO)
{
	fputc(MSFF_TOKEN_TIME_SIGNATURE,pO);	//token
	fputc((int)m_TimeSig,pO);			//value
}
