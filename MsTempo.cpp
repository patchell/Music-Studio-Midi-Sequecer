// MsTempo.cpp: implementation of the CMsTempo class.
//
//////////////////////////////////////////////////////////////////////

#include "pch.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMsTempo::CMsTempo():CMsObject()
{
	m_ObjType = CMsObject::MsObjType::TEMPO;
	m_Tempo = 1;
}


CMsTempo::~CMsTempo()
{

}

bool CMsTempo::Create(CMsSong* pSong, CMsEvent* pEvent, UINT Tempo)
{
	CMsObject::Create(pSong, pEvent);
	m_Tempo = Tempo;
	pSong->SetCurrentTempo(this);
	return true;
}

UINT CMsTempo::Process()
{
	return 1;
}

UINT CMsTempo::Play()
{
	GetSong()->SetCurrentTempo(this);
	GETAPP->PlayerThreadSetTempo(GetQNPM());
	return PLAY_OBJECT_DONE;
}

DRAWSTATE CMsTempo::MouseLButtonDown(DRAWSTATE DrawState, CPoint pointMouse, MouseRegions Region, MouseRegionTransitionState Transition)
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
	}
	return DrawState;
}

DRAWSTATE CMsTempo:: MouseLButtonUp(DRAWSTATE DrawState, CPoint pointMouse, MouseRegions Region, MouseRegionTransitionState Transition)
{
	CMsTempo* pMT = nullptr;
	CString csText;
	int EventIndex = 0;
	CMsEvent* pEV = nullptr;

	switch(DrawState)
	{
	case DRAWSTATE::SET_ATTRIBUTES:
		break;
	case DRAWSTATE::PLACE:
		break;
	default:
		EventIndex = GetStaffChildView()->GetDrawEvent();
		pEV = GetSong()->GetEventObject(EventIndex);
		pEV->AddObject(this);
		//-----------------------------
		pMT = new CMsTempo;
		pMT->Create(GetSong(), 0, 0);
		GetStaffChildView()->SetDrawObject(pMT);
		GetStaffChildView()->CheckAndDoScroll(pointMouse);
		csText.Format(_T("Place Loudness at Event %d"), GetStaffChildView()->GetDrawEvent());
		GetStaffChildView()->GetStatusBar()->SetText(csText);
		DrawState = DRAWSTATE::WAITFORMOUSE_DOWN;
		GetStaffChildView()->Invalidate();
		break;
	}
	return DrawState;
}

DRAWSTATE CMsTempo::MouseMove(DRAWSTATE DrawState, CPoint pointMouse, MouseRegions Region, MouseRegionTransitionState Transition)
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

void CMsTempo::Print(FILE *pO, int Indent)
{
	char* pIndentString = new char[256];

	theApp.IndentString(pIndentString, 256, Indent);
//	fprintf(pO,"%sTempo:%d\n", pIndentString, m_Tempo);
	delete[] pIndentString;
}

UINT CMsTempo::ObjectToString(CString& csString, UINT mode)
{
	return 0;
}

void CMsTempo::ObjectRectangle(CRect& rect, UINT Event)
{
}

void CMsTempo::Draw(CDC *pDC)
{
	int x,y;
	char *s = new char[16];
	CDC dc;
	CBitmap* oldBM;
	COLORREF OldTextColor;
	int red = 0;
	CMyBitmap Bm;
	CPen BlkPen;
	CBrush BlkBr;
	CRect rect;
	CBrush WhtBr, *oldBr;
	CFont* oldFont;

	WhtBr.CreateSolidBrush(RGB(0, 255, 200));
	rect = CRect(CPoint(0, 0), CSize(15, 15));
	BlkBr.CreateSolidBrush(RGB(0, 0, 0));
	BlkPen.CreatePen(PS_SOLID, 1, RGB(0, 0, 0));

	if (IsSelected()) red = 255;
	OldTextColor = pDC->SetTextColor(RGB(red, 0, 0));

	dc.CreateCompatibleDC(pDC);
	Bm.CreateBitmap(16, 16, 1, 32, NULL);
	oldBM = dc.SelectObject(&Bm);
	oldBr = dc.SelectObject(&BlkBr);
	dc.FillRect(&rect, &WhtBr);
	dc.Ellipse(CRect(CPoint(2, 10), CSize(6, 3)));
	dc.MoveTo(6, 11);
	dc.LineTo(6, 2);
	dc.MoveTo(8, 8);
	dc.LineTo(12, 8);
	dc.MoveTo(8, 11);
	dc.LineTo(12, 11);

	x = TEMPO_X;
	y = TREBLE_TOP_LINE - 16;

	oldFont = pDC->SelectObject(GETAPP->GetFont());
	pDC->BitBlt(x,y-16,16,16,&dc,0,0,SRCCOPY);
	int l;
	l = sprintf_s(s,16,"%d",m_Tempo);
	pDC->TextOut(x,y,CString(s),l);
	pDC->SetTextColor(OldTextColor);
	dc.SelectObject(oldBM);
	pDC->SelectObject(oldFont);
	delete[] s;
}

StaffMouseStates CMsTempo::StaffTransition(CPoint pointMouse, int NewNote, CMsEvent* pEvent)
{
	return StaffMouseStates::MOUSE_STAFF_STATE_NONE;
}

void CMsTempo::Copy(CMsObject* pSource)
{
	m_Tempo = ((CMsTempo*)pSource)->m_Tempo;
	CMsObject::Copy(pSource);
}

void CMsTempo::Save(FILE *pO)
{
	fputc(MSFF_TOKEN_TEMPO,pO);		//token	
	fputc(m_Tempo,pO);	//value
}

