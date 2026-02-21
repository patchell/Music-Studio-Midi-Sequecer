// MsBar.cpp: implementation of the CMsBar class.
//
//////////////////////////////////////////////////////////////////////
#include "pch.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CMsBar::CMsBar():CMsObject(MsObjType::BAR)
{
	m_BarNumber = GenBarNumber();
}

CMsBar::~CMsBar()
{
	LastBarNumber--;

}

bool CMsBar::Create(CMsSong* pSong, CMsEvent* pEvent)
{
	return CMsObject::Create(pSong, pEvent);
}


UINT CMsBar::Process()
{
	return 0;
}

UINT CMsBar::Play()
{
	return 0;
}

DRAWSTATE CMsBar::MouseLButtonDown(DRAWSTATE DrawState, CPoint pointMouse, MouseRegions Region, MouseRegionTransitionState Transition)
{
	CMsEvent* pEV = 0;

	switch (DrawState)
	{
	case DRAWSTATE::WAITFORMOUSE_DOWN:
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
					DrawState = DRAWSTATE::PLACE;
					break;
				case StaffMouseStates::MOUSE_STAFF_STATE_NOTE__EVENT_CHANGE:
					[[fallthrough]];
				case StaffMouseStates::MOUSE_STAFF_STATE_EVENT_CHANGE:
					if (GetParentEvent())
					{
						GetParentEvent()->RemoveObject(this);
						SetParentEvent(nullptr);
					}
					pEV = GetSong()->GetEventObject(GetStaffView()->XtoEventIndex(pointMouse.x));
					if (pEV)
					{
						pEV->AddObject(this);
						SetParentEvent(pEV);
					}
					break;
					DrawState = DRAWSTATE::PLACE;
				}
			}
			break;
		case MouseRegionTransitionState::MOUSE_TRANSITION_EDIT_TO_UPPER_DRAW:		//MouseMove
		case MouseRegionTransitionState::MOUSE_TRANSITION_EDIT_TO_LOWER_DRAW:
		case MouseRegionTransitionState::MOUSE_TRANSITION_EDIT_TO_OUTSIDE:
			pEV = GetParentEvent();
			if (pEV)
			{
				pEV->RemoveObject(this);
				SetParentEvent(nullptr);
			}
			break;
		case MouseRegionTransitionState::MOUSE_TRANSITION_UPPER_DRAW_TO_EDIT:		//MouseMove
		case MouseRegionTransitionState::MOUSE_TRANSITION_LOWER_DRAW_TO_EDIT:
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
	case DRAWSTATE::MOVE_OBJECT_AROUND:
		break;
	case DRAWSTATE::PLACE:
		break;
	}
	GetStaffView()->Invalidate();
	return DrawState;
}

DRAWSTATE CMsBar:: MouseLButtonUp(DRAWSTATE DrawState, CPoint pointMouse, MouseRegions Region, MouseRegionTransitionState Transition)
{
	CMsBar* pBarNew;
	CMsEvent* pEV = 0;

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
					GetSong()->RenumberMeasureBars();
					pBarNew = new CMsBar;
					pBarNew->Create(GetSong(), GetParentEvent());
					pBarNew->Copy(GetStaffChildView()->GetDrawObject());
					GetStaffChildView()->SetDrawObject(pBarNew);
					GetParentEvent()->AddObject(pBarNew);
					GetStaffChildView()->CheckAndDoScroll(pointMouse);
					GetStaffChildView()->Invalidate();
					DrawState = DRAWSTATE::WAITFORMOUSE_DOWN;
					break;
				case StaffMouseStates::MOUSE_STAFF_STATE_NOTE__EVENT_CHANGE:
				case StaffMouseStates::MOUSE_STAFF_STATE_EVENT_CHANGE:
					if (GetParentEvent())
					{
						GetParentEvent()->RemoveObject(this);
						SetParentEvent(nullptr);
					}
					pEV = GetSong()->GetEventObject(GetStaffView()->XtoEventIndex(pointMouse.x));
					if (pEV)
					{
						pEV->AddObject(this);
						SetParentEvent(pEV);
					}
					GetSong()->RenumberMeasureBars();
					pBarNew = new CMsBar;
					pBarNew->Create(GetSong(), GetParentEvent());
					pBarNew->Copy(GetStaffChildView()->GetDrawObject());
					GetStaffChildView()->SetDrawObject(pBarNew);
					GetParentEvent()->AddObject(pBarNew);
					GetStaffChildView()->CheckAndDoScroll(pointMouse);
					GetStaffChildView()->Invalidate();
					DrawState = DRAWSTATE::WAITFORMOUSE_DOWN;
					break;
				}
			}
			break;
		case MouseRegionTransitionState::MOUSE_TRANSITION_EDIT_TO_UPPER_DRAW:		//MouseMove
		case MouseRegionTransitionState::MOUSE_TRANSITION_EDIT_TO_LOWER_DRAW:
		case MouseRegionTransitionState::MOUSE_TRANSITION_EDIT_TO_OUTSIDE:
			pEV = GetParentEvent();
			if (pEV)
			{
				pEV->RemoveObject(this);
				SetParentEvent(nullptr);
			}
			break;
		case MouseRegionTransitionState::MOUSE_TRANSITION_UPPER_DRAW_TO_EDIT:		//MouseMove
		case MouseRegionTransitionState::MOUSE_TRANSITION_LOWER_DRAW_TO_EDIT:
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
	GetStaffView()->Invalidate();
	return DrawState;
}

DRAWSTATE CMsBar::MouseMove(DRAWSTATE DrawState, CPoint pointMouse, MouseRegions Region, MouseRegionTransitionState Transition)
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
				case StaffMouseStates::MOUSE_STAFF_STATE_NOTE__EVENT_CHANGE:
					[[fallthrough]];
				case StaffMouseStates::MOUSE_STAFF_STATE_EVENT_CHANGE:
					if (GetParentEvent())
					{
						if (GetParentEvent()->IsThisObjectInThisEvent(this))
						{
							GetParentEvent()->RemoveObject(this);
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
		case MouseRegionTransitionState::MOUSE_TRANSITION_EDIT_TO_LOWER_DRAW:
		case MouseRegionTransitionState::MOUSE_TRANSITION_EDIT_TO_OUTSIDE:
			pEV = GetParentEvent();
			if (pEV)
			{
				pEV->RemoveObject(this);
				SetParentEvent(nullptr);
			}
			break;
		case MouseRegionTransitionState::MOUSE_TRANSITION_UPPER_DRAW_TO_EDIT:		//MouseMove
		case MouseRegionTransitionState::MOUSE_TRANSITION_LOWER_DRAW_TO_EDIT:
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
		csStatusString.Format(_T("Measure Bar: %d Event: %d"),
			m_BarNumber,
			GetParentEvent() ? GetParentEvent()->GetIndex() : -1
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

void CMsBar::Print(FILE *pO, int Indent)
{
	char* pIndentString = new char[256];

	theApp.IndentString(pIndentString, 256, Indent);
//	fprintf(pO,"%sMeasure Bar:%d\n", pIndentString,m_BarNumber);
	delete[] pIndentString;
}


void CMsBar::Draw(CDC *pDC)
{
	CPen penSelectHighlight;
	CPen penBlackLine;
	CRect rectHighlight;
	CPen* p_penOld;
	CBrush* pOldBrush = 0, brushNULL;
	CPoint pointUpperLeft;
	CSize szRectSize;
	CFont* pfontOld = 0;
	CFont fontMeasureNumber;
	CString csMeasureNumber;
	int nOldBkMode;

	fontMeasureNumber.CreateFontW(
		MEASUREBAR_NUMBER_HEIGHT,
		0,
		0,
		0,
		FW_DONTCARE,
		false,
		false,
		false,
		ANSI_CHARSET,
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DRAFT_QUALITY,
		DEFAULT_PITCH,
		_T("Arial")
	);

	penBlackLine.CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
	penSelectHighlight.CreatePen(2, PS_SOLID, RGB(255, 0, 0));
	brushNULL.CreateStockObject(NULL_BRUSH);
	int x = MEASUREBAR_OFFSET;
	if (IsSelected() || IsHighLighted())
	{
		//----------------------------------------
		// Bar
		//-----------------------------------------
	
		pointUpperLeft = CPoint(x - 2, TREBLE_TOP_LINE - 2);
		szRectSize = CSize(5, STAFF_HEIGHT + 2);
		rectHighlight = CRect(pointUpperLeft, szRectSize);
		p_penOld = pDC->SelectObject(&penSelectHighlight);
		pOldBrush = pDC->SelectObject(&brushNULL);
		pDC->Rectangle(&rectHighlight);
		pDC->SelectObject(pOldBrush);
		pDC->SelectObject(p_penOld);
	}
	p_penOld = pDC->SelectObject(&penBlackLine);
	pDC->MoveTo(x, TREBLE_TOP_LINE);
	pDC->LineTo(x, BASS_TOP_LINE + SINGLE_STAVE_HEIGHT);
	pDC->SelectObject(p_penOld);
	//----------------------------------------
	nOldBkMode = pDC->SetBkMode(TRANSPARENT);
	pfontOld = pDC->SelectObject(&fontMeasureNumber);
	csMeasureNumber.Format(_T("%d"), m_BarNumber);
	pDC->TextOutW(
		x - MEASUREBAR_NUMBER_OFFSET_X, 
		TREBLE_TOP_LINE - MEASUREBAR_NUMBER_OFFSET_Y,
		csMeasureNumber
	);
	pDC->SelectObject(pfontOld);
	pDC->SetBkMode(nOldBkMode);
}

StaffMouseStates CMsBar::StaffTransition(CPoint pointMouse, int NewNote, CMsEvent* pEvent)
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

UINT CMsBar::Process(CMsSong* pSong)
{
	return 0;
}

void CMsBar::ObjectRectangle(CRect& rect, UINT Event)
{
	rect.SetRect(
		(EVENT_WIDTH + 1) * Event -2,
		TREBLE_TOP_LINE, 
		(EVENT_WIDTH + 1) * Event + 2, 
		BASS_TOP_LINE + SINGLE_STAVE_HEIGHT
	);
}

UINT CMsBar::ObjectToString(CString& csString, UINT mode)
{
	csString.Format(_T("Measure Bar %d at %d"), m_BarNumber, GetParentEvent()->GetIndex());
	return csString.GetLength();
}

CMsObject* CMsBar::MakeANewObject()
{
//	if(LogFile()) fprintf(LogFile(),"Make a new Bar\n");
	CMsBar* pBar;
	pBar = new CMsBar;
	return pBar;
}

 void CMsBar::Copy(CMsObject* pSource)
{
	CMsObject::Copy(pSource);
//	m_BarNumber = GenBarNumber();
}

void CMsBar::Save(FILE *pO)
{
	fputc(MSFF_TOKEN_BAR,pO);
}

