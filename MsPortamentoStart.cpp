#include "pch.h"

CMsPortamentoStart::CMsPortamentoStart():CMsObject()
{
	m_ObjType = CMsObject::MsObjType::PORTAMENTO_START;
}

CMsPortamentoStart::~CMsPortamentoStart()
{
}

bool CMsPortamentoStart::Create(CMsSong* pSong, CMsEvent* pEvent)
{
	CMsObject::Create(pSong, pEvent);
	return true;
}

UINT CMsPortamentoStart::Process()
{
	return 0;
}

UINT CMsPortamentoStart::Play()
{
	return 0;
}

DRAWSTATE CMsPortamentoStart::MouseLButtonDown(DRAWSTATE DrawState, CPoint pointMouse)
{
	switch (DrawState)
	{
	case DRAWSTATE::WAITFORMOUSE_DOWN:
		break;
	case DRAWSTATE::SET_ATTRIBUTES:
		break;
	case DRAWSTATE::PLACE:
		break;
	default:
		break;
	}
	return DrawState;
}

DRAWSTATE CMsPortamentoStart::MouseLButtonUp(DRAWSTATE DrawState, CPoint pointMouse)
{
	switch (DrawState)
	{
	case DRAWSTATE::WAITFORMOUSE_DOWN:
		break;
	case DRAWSTATE::SET_ATTRIBUTES:
		break;
	case DRAWSTATE::PLACE:
		break;
	default:
		break;
	}
	return DrawState;
}	

DRAWSTATE CMsPortamentoStart::MouseMove(DRAWSTATE DrawState, CPoint pointMouse, MouseRegions Region, MouseRegionTransitionState Transition)
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
				switch (	StaffTransition(pointMouse, 0, GetParentEvent()))
				{
				case StaffMouseStates::MOUSE_STAFF_STATE_NONE:
					break;
				case StaffMouseStates::MOUSE_STAFF_STATE_NOTE_CHANGE:
					break;
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
				case StaffMouseStates::MOUSE_STAFF_STATE_NOTE__EVENT_CHANGE:
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
				}
			}
			break;
		case MouseRegionTransitionState::MOUSE_TRANSITION_EDIT_TO_UPPER_DRAW:		//MouseMove
		case MouseRegionTransitionState::MOUSE_TRANSITION_EDIT_TO_LOWER_DRAW:
		case MouseRegionTransitionState::MOUSE_TRANSITION_EDIT_TO_OUTSIDE:
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
			break;
		case MouseRegionTransitionState::MOUSE_TRANSITION_OUTSIDE_TO_LOWERSEL:		//MouseMove
		case MouseRegionTransitionState::MOUSE_TRANSITION_OUTSIDE_TO_UPPERSEL:
			break;
		case MouseRegionTransitionState::MOUSE_TRANSITION_ERROR:		//MouseMove
			break;
		}
		csStatusString.Format(_T("Measure Bar: %d Event: %d"),
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

void CMsPortamentoStart::Save(FILE* pO)
{
}

void CMsPortamentoStart::Copy(CMsObject* pSource)
{
	CMsObject::Copy(pSource);
}

void CMsPortamentoStart::Draw(CDC* pDC)
{
}

StaffMouseStates CMsPortamentoStart::StaffTransition(CPoint pointMouse, int NewNote, CMsEvent* pEvent)
{
	return StaffMouseStates::MOUSE_STAFF_STATE_NONE;
}

void CMsPortamentoStart::Print(FILE* pO, int Indent)
{
}

