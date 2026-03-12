// MsLoudness.cpp: implementation of the CMsLoudness class.
//
//////////////////////////////////////////////////////////////////////

#include "pch.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMsLoudness::CMsLoudness():CMsObject()
{
	m_ObjType = CMsObject::MsObjType::LOUDNESS;
	m_Loudness = 100;
}

CMsLoudness::~CMsLoudness()
{

}

bool CMsLoudness::Create(CMsSong* pSong, CMsEvent* pEvent, int Loudness)
{
	SetLoudness(Loudness);
	pSong->SetCurrentLoudness(this);
	return CMsObject::Create(pSong, pEvent);
}

UINT CMsLoudness::Process()
{
	return 1;
}

UINT CMsLoudness::Play()
{
	GetSong()->SetCurrentLoudness(this);
	return PLAY_OBJECT_DONE;
}

DRAWSTATE CMsLoudness::MouseLButtonDown(DRAWSTATE DrawState, CPoint pointMouse, MouseRegions Region, MouseRegionTransitionState Transition)
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
					[[fallthrough]];
				case StaffMouseStates::MOUSE_STAFF_STATE_NOTE_CHANGE:
					[[fallthrough]];
				case StaffMouseStates::MOUSE_STAFF_STATE_NOTE__EVENT_CHANGE:
					DrawState = DRAWSTATE::PLACE;
					break;
				case StaffMouseStates::MOUSE_STAFF_STATE_EVENT_CHANGE:
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
					DrawState = DRAWSTATE::PLACE;
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
				if (pEV->IsThisObjectInThisEvent(this))
				{
					pEV->RemoveObject(this);
					SetParentEvent(nullptr);
				}
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
			DrawState = DRAWSTATE::PLACE;
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

DRAWSTATE CMsLoudness:: MouseLButtonUp(DRAWSTATE DrawState, CPoint pointMouse, MouseRegions Region, MouseRegionTransitionState Transition)
{
	CMsLoudness* pNewLoudness = nullptr;
	CString csText;
	int EventIndex = 0;
	CMsEvent* pEV = nullptr;

	switch (DrawState)
	{
	case DRAWSTATE::WAITFORMOUSE_DOWN:
		break;
	case DRAWSTATE::SET_ATTRIBUTES:
		break;
	case DRAWSTATE::PLACE:
		switch (Transition)
		{
		case MouseRegionTransitionState::MOUSE_TRANSITION_NONE:
			if (Region == MouseRegions::MOUSE_IN_EDITREG)
			{
				StaffMouseStates StaffTrans = StaffTransition(pointMouse, 0, GetParentEvent());

				switch (StaffTransition(pointMouse, 0, GetParentEvent()))
				{
				case StaffMouseStates::MOUSE_STAFF_STATE_NONE:
					[[fallthrough]];
				case StaffMouseStates::MOUSE_STAFF_STATE_NOTE_CHANGE:
					pNewLoudness = new CMsLoudness;
					pNewLoudness->Create(GetSong(), GetParentEvent(), GetLoudness());
					pNewLoudness->Copy(GetStaffChildView()->GetDrawObject());
					if (pNewLoudness->EditLoudness() == IDOK)
					{
						GetStaffChildView()->SetDrawObject(pNewLoudness);
						//-----------------------
						GetParentEvent()->AddObject(pNewLoudness);
						GetStaffChildView()->CheckAndDoScroll(pointMouse);
						GetStaffChildView()->Invalidate();
						DrawState = DRAWSTATE::WAITFORMOUSE_DOWN;
						csText.Format(_T("Place Loudness at Event %d"), GetStaffChildView()->GetDrawEvent());
					}
					else
					{
						delete pNewLoudness;
						GetStaffChildView()->SetDrawObject(nullptr);
						GetStaffChildView()->SetDrawMode(CChildViewStaff::DrawMode::NOP);
						DrawState = DRAWSTATE::NA;
						csText.Format(_T(""));
					}
					GetStaffChildView()->GetStatusBar()->SetText(csText);
					break;
				case StaffMouseStates::MOUSE_STAFF_STATE_NOTE__EVENT_CHANGE:
					[[fallthrough]];
				case StaffMouseStates::MOUSE_STAFF_STATE_EVENT_CHANGE:
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
					pNewLoudness = new CMsLoudness;
					pNewLoudness->Create(GetSong(), GetParentEvent(), GetLoudness());
					pNewLoudness->Copy(GetStaffChildView()->GetDrawObject());
					GetStaffChildView()->SetDrawObject(pNewLoudness);
					//-----------------------
					GetParentEvent()->AddObject(pNewLoudness);
					GetStaffChildView()->CheckAndDoScroll(pointMouse);
					GetStaffChildView()->Invalidate();
					DrawState = DRAWSTATE::WAITFORMOUSE_DOWN;
					csText.Format(_T("Place Loudness at Event %d"), GetStaffChildView()->GetDrawEvent());
					GetStaffChildView()->GetStatusBar()->SetText(csText);
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
				if (pEV->IsThisObjectInThisEvent(this))
				{
					pEV->RemoveObject(this);
					SetParentEvent(nullptr);
				}
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
		break;
	default:
		break;
	}
	return DrawState;
}

DRAWSTATE CMsLoudness::MouseMove(DRAWSTATE DrawState, CPoint pointMouse, MouseRegions Region, MouseRegionTransitionState Transition)
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
					break;
				case StaffMouseStates::MOUSE_STAFF_STATE_EVENT_CHANGE:
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
		case MouseRegionTransitionState::MOUSE_TRANSITION_EDIT_TO_LOWER_DRAW:
		case MouseRegionTransitionState::MOUSE_TRANSITION_EDIT_TO_OUTSIDE:
			pEV = GetParentEvent();
			if (pEV)
			{
				if (pEV->IsThisObjectInThisEvent(this))
				{
					pEV->RemoveObject(this);
					SetParentEvent(nullptr);
				}
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

void CMsLoudness::Print(FILE *pO, int Indent)
{
	char* pIndentString = new char[256];

	theApp.IndentString(pIndentString, 256, Indent);
//	fprintf(pO,"%sLoudness:%d\n", pIndentString, m_Loudness);
	delete[] pIndentString;
}

UINT CMsLoudness::ObjectToString(CString& csString, UINT mode)
{
	return 0;
}

void CMsLoudness::ObjectRectangle(CRect& rect, UINT Event)
{
}

int CMsLoudness::EditLoudness()
{
	int Id = 0;
	CParamDlg Dlg;

	Dlg.SetCaption(CString("Set Loudness (Velocity)"));
	Dlg.SetMin(0);
	Dlg.SetMax(127);
	Dlg.SetValue(100);
	if (IDOK == Dlg.DoModal())
	{
		m_Loudness = Dlg.GetValue();
	}
	return Id;
}

void CMsLoudness::Draw(CDC *pDC)
{
	int x,y;
	CFont* oldFont;
	CString csText;

	oldFont = pDC->SelectObject(GETAPP->GetFont());
//	x = EVENT_OFFSET+EVENT_WIDTH*event - X_CENTER_OF_EVENT;
	x = X_CENTER_OF_EVENT;
	y = TREBLE_TOP_LINE -16;
	csText.Format(_T("V="));
	pDC->TextOut(x,y-12, csText);
	csText.Format(_T("%d"), m_Loudness);
	pDC->TextOut(x,y, csText);
	pDC->SelectObject(oldFont);
}

StaffMouseStates CMsLoudness::StaffTransition(CPoint pointMouse, int NewNote, CMsEvent* pEvent)
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

void CMsLoudness::Copy(CMsObject* pSource)
{
	m_Loudness = ((CMsLoudness*)pSource)->m_Loudness;
	CMsObject::Copy(pSource);
}

void CMsLoudness::Save(FILE *pO)
{
	fputc(MSFF_TOKEN_LOUDNESS,pO);	//token
	fputc(0x01,pO);	//who knows what this is for
	fputc(m_Loudness,pO);	//value
}
