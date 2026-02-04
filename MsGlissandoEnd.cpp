#include "pch.h"

CMsGlissandoEnd::CMsGlissandoEnd() :CMsObject()
{
	m_ObjType = CMsObject::MsObjType::GLISSANDO_END;
	m_bSelected = false;
	m_bStemDown = false;
	m_Pitch = 60;
	m_pParentGlissando = nullptr;
}

CMsGlissandoEnd::~CMsGlissandoEnd()
{
}

bool CMsGlissandoEnd::Create(CMsSong* pSong, CMsGlissando* pParent, CMsEvent* pEvEnd)
{
	m_pParentGlissando = pParent;
	CMsObject::Create(pSong, pEvEnd);
	return true;
}

UINT CMsGlissandoEnd::Process()
{
	return 0;
}

UINT CMsGlissandoEnd::Play()
{
	return 0;
}

DRAWSTATE CMsGlissandoEnd::MouseLButtonDown(DRAWSTATE DrawState, CPoint pointMouse)
{
	switch (DrawState)
	{
	case DRAWSTATE::SET_ATTRIBUTES:
		break;
	case DRAWSTATE::WAITFORMOUSE_DOWN:
		break;
	default:
		break;
	}
	return DrawState;
}

DRAWSTATE CMsGlissandoEnd::MouseLButtonUp(DRAWSTATE DrawState, CPoint pointMouse)
{
	switch (DrawState)
	{
	case DRAWSTATE::SET_ATTRIBUTES:
		break;
	case DRAWSTATE::WAITFORMOUSE_DOWN:
		break;
	default:
		break;
	}
	return DrawState;
}

DRAWSTATE CMsGlissandoEnd::MouseMove(DRAWSTATE DrawState, CPoint pointMouse, MouseRegions Region, MouseRegionTransitionState Transition)
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
				switch (StaffTransition(pointMouse, 0,GetParentEvent()))
				{
				case StaffMouseStates::MOUSE_STAFF_STATE_NONE:
					break;
				case StaffMouseStates::MOUSE_STAFF_STATE_NOTE_CHANGE:
				case StaffMouseStates::MOUSE_STAFF_STATE_NOTE__EVENT_CHANGE:
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

void CMsGlissandoEnd::Print(FILE* pO, int Indent)
{
}

void CMsGlissandoEnd::Save(FILE* pO)
{
}

void CMsGlissandoEnd::Copy(CMsObject* Source)
{
	CMsObject::Copy(Source);
}

void CMsGlissandoEnd::Draw(CDC* pDC)
{
}

StaffMouseStates CMsGlissandoEnd::StaffTransition(CPoint pointMouse, int NewNote, CMsEvent* pEvent)
{
	return StaffMouseStates::MOUSE_STAFF_STATE_NONE;
}

UINT CMsGlissandoEnd::ObjectToString(CString& csString, UINT mode)
{
	return 0;
}

void CMsGlissandoEnd::ObjectRectangle(CRect& rect, UINT Event)
{
}

