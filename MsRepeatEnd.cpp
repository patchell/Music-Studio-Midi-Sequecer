// MsRepeatEnd.cpp: implementation of the CMsRepeatEnd class.
//
//////////////////////////////////////////////////////////////////////

#include "pch.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMsRepeatEnd::CMsRepeatEnd():CMsObject()
{
	m_ObjType = CMsObject::MsObjType::REPEATEND;
}

CMsRepeatEnd::~CMsRepeatEnd()
{

}

void CMsRepeatEnd::Print(FILE *pO, int Indent)
{
	char* pIndentString = new char[256];

	theApp.IndentString(pIndentString, 256, Indent);
//	fprintf(pO,"%sRepeat End :|\n", pIndentString);
	delete[] pIndentString;
}

UINT CMsRepeatEnd::ObjectToString(CString& csString, UINT mode)
{
	return 0;
}

void CMsRepeatEnd::ObjectRectangle(CRect& rect, UINT Event)
{
}

void CMsRepeatEnd::Draw(CDC *pDC)
{
	CPen Light, Heavy, *pOld;
	LOGBRUSH Lb;
	if (IsSelected())
	{
		Light.CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
		Lb.lbColor = RGB(255, 0, 0);
	}
	else
	{
		Light.CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
		Lb.lbColor = RGB(0, 0, 0);
	}
	Lb.lbStyle = BS_SOLID;
	Lb.lbHatch = 0;

	Heavy.CreatePen(PS_GEOMETRIC | PS_ENDCAP_FLAT,6,&Lb);
	int x = MEASUREBAR_OFFSET;
	pOld = pDC->SelectObject(&Light);
	pDC->MoveTo(x-6,TREBLE_TOP_LINE);
	pDC->LineTo(x-6,BASS_TOP_LINE + SINGLE_STAVE_HEIGHT);
	pDC->SelectObject(&Heavy);
	pDC->MoveTo(x,TREBLE_TOP_LINE);
	pDC->LineTo(x,BASS_TOP_LINE + SINGLE_STAVE_HEIGHT);
	pDC->SetPixel(x - 8, TREBLE_TOP_LINE + 12, RGB(0, 0, 0));
	pDC->SetPixel(x - 8, TREBLE_TOP_LINE + 20, RGB(0, 0, 0));
	pDC->SetPixel(x - 8, TREBLE_TOP_LINE + 60, RGB(0, 0, 0));
	pDC->SetPixel(x - 8, TREBLE_TOP_LINE + 68, RGB(0, 0, 0));
	pDC->SelectObject(pOld);
}

StaffMouseStates CMsRepeatEnd::StaffTransition(CPoint pointMouse, int NewNote, CMsEvent* pEvent)
{
	return StaffMouseStates::MOUSE_STAFF_STATE_NONE;
}

void CMsRepeatEnd::Copy(CMsObject* pSource)
{
	CMsObject::Copy(pSource);
}

void CMsRepeatEnd::Save(FILE *pO)
{
	fputc(MSFF_TOKEN_REPEAT_STOP,pO);	//token
}

bool CMsRepeatEnd::Create(CMsSong* pSong, CMsEvent* pEvent)
{
	return CMsObject::Create(pSong, pEvent);
}

UINT CMsRepeatEnd::Play()
{
	return PLAY_OBJECT_DONE;
}

UINT CMsRepeatEnd::Process()
{
	CMsEvent* pRepeatStartEvent;
	CMsRepeatStart* pRPS;

	pRepeatStartEvent = (CMsEvent*)GetSong()->GetRepeatStack().PeakTop();
	pRPS = (CMsRepeatStart*)pRepeatStartEvent->ContainsObjectType(CMsObject::MsObjType::REPEATSTART);
//	if(LogFile()) fprintf(LogFile(),"Repeat end Countdown %d\n", pRPS->GetCountDown());
	if (pRPS->DecrementRepeatCount())
	{
		//------------------------------
		// Done With Repeat Loop
		//-------------------------------
		GetSong()->GetRepeatStack().PopLIFO();
	}
	else
	{
		//-----------------------------
		// Keep Looping
		//-----------------------------
		GetSong()->SetSongPosition(pRepeatStartEvent);
	}
	return 0;
}

DRAWSTATE CMsRepeatEnd::MouseLButtonDown(DRAWSTATE DrawState, CPoint pointMouse)
{
	switch (DrawState)
	{
	case DRAWSTATE::WAITFORMOUSE_DOWN:
		break;
	default:
		break;
	}
	return DrawState;
}

DRAWSTATE CMsRepeatEnd::MouseLButtonUp(DRAWSTATE DrawState, CPoint pointMouse)
{
	switch (DrawState)
	{
	case DRAWSTATE::PLACE:
		break;
	default:
		break;
	}
	return DrawState;
}

DRAWSTATE CMsRepeatEnd::MouseMove(DRAWSTATE DrawState, CPoint pointMouse, MouseRegions Region, MouseRegionTransitionState Transition)
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
