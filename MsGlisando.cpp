#include "pch.h"

CMsGlissando::CMsGlissando():CMsObject()
{
    m_ObjType = CMsObject::MsObjType::GLISSANDO;
	m_pEvEnd = nullptr;
	m_EndPitch = 0;
	m_StartPitch = 0;
	m_TotalIntervals = 0;
	m_CurrentPitch = 0;
	m_TicksPerInterval = 0;
	m_GlisandoState = GlisandoState::END;
	m_GlisandoDirection = GlisandoDirection::UP;	
	m_Track = COMBO_Index_INST_7;
	m_bSelected = false;
	m_DrawState = DRAWSTATE::WAITFORMOUSE_DOWN;
	m_EndStemDown = false;
	m_StartStemDown = false;
	m_bSelected = false;
	m_Ticks = 0;
	m_TotalDurationTicks = 0;
}

CMsGlissando::~CMsGlissando()
{
}

bool CMsGlissando::Create(CMsSong* pSong, CMsEvent* pEvStart, CMsEvent* pEvEnd)
{
	CMsObject::Create(pSong, pEvStart);
	m_pEvEnd = pEvEnd;
	return true;
}

UINT CMsGlissando::Process()
{
	m_CurrentPitch = m_StartPitch;
	m_TotalIntervals = m_StartPitch - m_EndPitch;
	if (m_TotalIntervals < 0)
	{
		m_GlisandoDirection = GlisandoDirection::UP;
		m_TotalIntervals = -m_TotalIntervals;
	}
	m_TotalDurationTicks = CalculateTotalDuration();
	int TicksPerInterval = 0;
	int RemainderTicks = 0;
	if (m_TotalIntervals)
	{
		TicksPerInterval = m_TotalDurationTicks / m_TotalIntervals;
		RemainderTicks = m_TotalDurationTicks % m_TotalIntervals;
		if(RemainderTicks)
		{
			// Adjust ticks per interval to account for remainder
			m_TotalDurationTicks = m_TotalIntervals - RemainderTicks;
			TicksPerInterval = m_TotalDurationTicks / m_TotalIntervals;
		}
	}
	m_GlisandoState = GlisandoState::START;
	return 0;

}

UINT CMsGlissando::Play()
{
	UINT ret = 0;	
	switch(m_GlisandoState)
	{
		case GlisandoState::START:
			m_Ticks = m_TicksPerInterval;
			NoteOn(127);
			m_GlisandoState = GlisandoState::RUNNING;
			break;
		case GlisandoState::RUNNING:
			if (--m_Ticks <= 0)
			{
				NoteOff(0);
				if(m_TotalIntervals--)
				{
					if (m_GlisandoDirection == GlisandoDirection::UP)
						m_CurrentPitch++;
					else
						m_CurrentPitch--;
					NoteOn(127);
					m_Ticks = m_TicksPerInterval;
				}
				else
				{
					m_GlisandoState = GlisandoState::END;
					ret = PLAY_OBJECT_DONE;
				}
			}
			else
			{
				m_Ticks--;
			}
			break;
		case GlisandoState::END:
			break;
	}
	return ret;
}

DRAWSTATE CMsGlissando::MouseLButtonDown(DRAWSTATE DrawState, CPoint pointMouse)
{

	switch (DrawState)
	{
	case DRAWSTATE::WAITFORMOUSE_DOWN:
		if (LogFile()) fprintf(LogFile(), "Gliss:MouseDown:Wait Goto First Note\n");
		DrawState = DRAWSTATE::GLISSANDO_FIRST_NOTE;
		break;
	case DRAWSTATE::GLISSANDO_FIRST_NOTE:
		if (LogFile()) fprintf(LogFile(), "Gliss:MouseDown:Never Get Here\n");
		break;
	case DRAWSTATE::GLISSANDO_SECOND_NOTE:
		if (LogFile()) fprintf(LogFile(), "Gliss:MouseDown:SecondNote\n");
		break;
	default:
		break;
	}
	m_DrawState = DrawState;
	return DrawState;
}

DRAWSTATE CMsGlissando::MouseLButtonUp(DRAWSTATE DrawState, CPoint pointMouse)
{
	CDlgGlissandoProperties Dlg;
	int Id;
	CString csText;
	int EventIndex = 0;
	CMsGlissando* pGliss = nullptr;
	CMsEvent* pEV = nullptr;

	switch (DrawState)
	{
	case DRAWSTATE::SET_ATTRIBUTES:
		csText.Format(_T("Configure Note Prameters"));
		GetStaffChildView()->GetStatusBar()->SetText(csText);
		if ((Id = Dlg.DoModal()) == IDOK)
		{
			DrawState = DRAWSTATE::WAITFORMOUSE_DOWN;
			GetStaffChildView()->Invalidate();
		}
		break;
	case DRAWSTATE::WAITFORMOUSE_DOWN:
		if (LogFile()) fprintf(LogFile(), "Gliss:MouseUp:Wait MD :Nver Get Here\n");
		break;
	case DRAWSTATE::GLISSANDO_FIRST_NOTE:
		if (LogFile()) fprintf(LogFile(), "Gliss:MouseUp:FirstNote:Goto Second Note\n");
		EventIndex = GetStaffChildView()->GetDrawEvent();
		pEV = GetSong()->GetEventObject(EventIndex);
		pEV->AddObject(this);
		GetStaffChildView()->CheckAndDoScroll(pointMouse);
		csText.Format(_T("Place Second Note at Event %d"), GetStaffChildView()->GetDrawEvent());
		GetStaffChildView()->GetStatusBar()->SetText(csText);
		GetStaffChildView()->Invalidate();
		DrawState = DRAWSTATE::GLISSANDO_SECOND_NOTE;
		break;
	case DRAWSTATE::GLISSANDO_SECOND_NOTE:
		if (LogFile()) fprintf(LogFile(), "Gliss:MouseUp:SecondNote:Goto End\n");
		EventIndex = GetStaffChildView()->GetDrawEvent();
		pEV = GetSong()->GetEventObject(EventIndex);
		pEV->AddObject(this);
		GetStaffChildView()->CheckAndDoScroll(pointMouse);
		pGliss = new CMsGlissando;
		pGliss->Create(GetSong(), 0, 0);
		GetStaffChildView()->SetDrawObject(pGliss);
		csText.Format(_T("Place First Note at Event %d"), GetStaffChildView()->GetDrawEvent());
		GetStaffChildView()->GetStatusBar()->SetText(csText);
		DrawState = DRAWSTATE::WAITFORMOUSE_DOWN;
		break;
	default:
		break;
	}
	m_DrawState = DrawState;
	return DrawState;
}

DRAWSTATE CMsGlissando::MouseMove(DRAWSTATE DrawState, CPoint pointMouse, MouseRegions Region, MouseRegionTransitionState Transition)
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

int CMsGlissando::IsTimedObject()
{
	return COMBO_NOTE_QUARTER;
}

UINT CMsGlissando::ObjectToString(CString& csString, UINT mode)
{
	csString.Format(_T("Glissando from %d to %d"), m_StartPitch, m_EndPitch);
	return csString.GetLength();
}

StaffMouseStates CMsGlissando::StaffTransition(CPoint pointMouse, int NewNote, CMsEvent* pEvent)
{
	return StaffMouseStates::MOUSE_STAFF_STATE_NONE;
}

void CMsGlissando::NoteOn(UINT Velociry)
{
	int chan = GetSong()->GetTrackInfo(GetTrack())->GetChannel();
	int note = m_CurrentPitch + CMsNote::RangeLUT[GetSong()->GetTrackInfo(GetTrack())->GetPitchRange()];
	int DeviceID = GetSong()->GetTrackInfo(GetTrack())->GetMidiOutDeviceID();
	GETAPP->GetMidiOutTable()->GetDevice(DeviceID).NoteOn(chan, note, Velociry);
	GetSong()->IncNoteOnCount(note);
}

void CMsGlissando::NoteOff(UINT Velociry)
{
	int chan = GetSong()->GetTrackInfo(GetTrack())->GetChannel();
	int note = m_CurrentPitch + CMsNote::RangeLUT[GetSong()->GetTrackInfo(GetTrack())->GetPitchRange()];
	int DeviceID = GetSong()->GetTrackInfo(GetTrack())->GetMidiOutDeviceID();
	GETAPP->GetMidiOutTable()->GetDevice(DeviceID).NoteOff(chan, note, Velociry);
	GetSong()->IncNoteOffCount(note);
}

void CMsGlissando::Copy(CMsObject* pSource)
{
	m_GlisandoDirection = ((CMsGlissando*)pSource)->m_GlisandoDirection;
	m_Track = ((CMsGlissando*)pSource)->m_Track;
	m_StartPitch = ((CMsGlissando*)pSource)->m_StartPitch;
	m_StartStemDown = ((CMsGlissando*)pSource)->m_StartStemDown;
	m_EndPitch = ((CMsGlissando*)pSource)->m_EndPitch;
	m_EndStemDown = ((CMsGlissando*)pSource)->m_EndStemDown;
	CMsObject::Copy(pSource);
}

void CMsGlissando::Draw(CDC* pDC)
{
	switch (m_DrawState)
	{
	case DRAWSTATE::WAITFORMOUSE_DOWN:
		DrawNote(pDC, m_StartPitch, true, m_StartStemDown);
		break;
	case DRAWSTATE::GLISSANDO_FIRST_NOTE:
		DrawNote(pDC, m_StartPitch, true, m_StartStemDown);
		break;
	case DRAWSTATE::GLISSANDO_SECOND_NOTE:
		[[fallthrough]];
	case DRAWSTATE::GLISSANDO_END:
		DrawNote(pDC, m_StartPitch, true, m_StartStemDown);
		DrawNote(pDC, m_EndPitch, true, m_EndStemDown);
		break;
	default:
		break;
	}
}

void CMsGlissando::Save(FILE* pO)
{
}

void CMsGlissando::Print(FILE* pO, int Indent)
{
}

void CMsGlissando::DrawNote(CDC* pDC, int Pitch, bool bStartPos, bool bStemDown)
{
	COLORREF Color = RGB(0, 0, 0);
	CPen* pOldPen, penNote;
	CBrush* pOldBrush, brushNote;
	int NoteYPos = NotePitchToYPos(Pitch);
	int NoteXPos = 0;
	CRect rectNoteHead;
	CSize szNoteHead = CSize(NOTE_HEAD_WIDTH, NOTE_HEAD_HEIGHT);
	int LinesNeeded = NeedsLine(Pitch);

	if (IsSelected())
	{
		Color = RGB(255, 0, 0);
	}
	else
	{
		Color = GetSong()->GetTrackInfo(GetTrack())->GetColor();
	}
	penNote.CreatePen(PS_SOLID, 2, Color);
	brushNote.CreateSolidBrush(Color);
	pOldPen = pDC->SelectObject(&penNote);
	pOldBrush = pDC->SelectObject(&brushNote);
	if (bStartPos)
	{
		if (bStemDown)
		{
			NoteXPos = NOTE_STEM_OFFSET;
			rectNoteHead = CRect(CPoint(NoteXPos,NoteYPos), szNoteHead);
			pDC->Ellipse(&rectNoteHead);
		}
		else
		{
			NoteXPos = NOTE_STEM_OFFSET - NOTE_HEAD_WIDTH;
			rectNoteHead = CRect(CPoint(NoteXPos, NoteYPos), szNoteHead);
			pDC->Ellipse(&rectNoteHead);
		}
	}
	else
	{
		if (bStemDown)
		{
			NoteXPos = NOTE_STEM_OFFSET;
			rectNoteHead = CRect(CPoint(NoteXPos, NoteYPos), szNoteHead);
			pDC->Ellipse(&rectNoteHead);
		}
		else
		{
			NoteXPos = NOTE_STEM_OFFSET - NOTE_HEAD_WIDTH;
			rectNoteHead = CRect(CPoint(NoteXPos, NoteYPos), szNoteHead);
			pDC->Ellipse(&rectNoteHead);
		}
	}
	if(LinesNeeded)
	{
		for(int i =0; i < LinesNeeded; i++)
		{
			int yLinePos;
			if (Pitch < 0x2A)
			{
				yLinePos = NoteYPos + (i * 8);
			}
			else
			{
				yLinePos = NoteYPos - (i * 8);
			}
			pDC->MoveTo(NoteXPos - 2, yLinePos);
			pDC->LineTo(NoteXPos + NOTE_HEAD_WIDTH + 2, yLinePos);
		}
	}
	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldBrush);	
}

void CMsGlissando::DrawGlisandoSquiggleLine(CDC* pDC, int xStart, int yStart, int xEnd, int yEnd)
{
}

int CMsGlissando::CalculateTotalDuration()
{
	CMsEvent* pEvent = GetParentEvent();
	CMsEvent* pEvEnd = m_pEvEnd;
	bool Loop = true;
	int Duration = 0;
	int m_Ticks = 0;

	while (Loop)
	{
		Duration = pEvent->GetMinEventDuration();
		if(Duration)
			m_Ticks += CMsNote::GetDurationTable()[Duration].DurationTimeTicks();
		if (pEvent == pEvEnd)
			Loop = false;
		else
			pEvent = pEvent->GetNext();
	}
	return m_Ticks;
}

int CMsGlissando::NotePitchToYPos(int NotePitch)
{
//	return NotePos[NotePitch % 12] + 28 * (7 - (NotePitch / 12)) + (CLIENT_TO_STAVE_TOP_Y - 44);
	return 0;
}

int CMsGlissando::YPosToNotePitch(int MouseY)
{
	int NoteIndex;
	int Octave = 0;
	int Note;

	MouseY -= CLIENT_Y_TO_TOP_OF_EDIT_RECT;
	//------- Invert Y Coord --------------
	MouseY = EDIT_RECT_HEIGHT - MouseY;
	//----------------------------------
	// quantize the y coordiant so that
	// the note will only be drawn either
	// in the space, or on the line.  We do
	// not quantize the mouse pointer
	//-----------------------------------
	MouseY -= STAVE_LINE_SPACING / 2;	//center
	MouseY = QuantizeY(MouseY);
	//------------------------------------
	// Limit the range that y can take
	//------------------------------------
	if (MouseY < 0)
		MouseY = 0;
	NoteIndex = MouseY % 7;
	Note = NoteLut[NoteIndex];
	Octave += (MouseY / 7) * 12;	// calculate octave
	Note = Note + Octave + 0x24;
	if (Note > 0x60)
		Note = 0x60;	// limit max value of Note
	return Note;
}

int CMsGlissando::QuantizeY(int Y)
{
	return Y / 4;
}


int CMsGlissando::NeedsLine(int Pitch)
{
	//-------------------------
	// Middle C is 0x3C
	//-------------------------
	int rV = 0;

	if (Pitch == 0x3c) rV = 1;
	else if (Pitch <= 0x2A)
	{
		rV = (Pitch - 0x2A) / 2;
	}
	else if (Pitch > 0x4f)
	{
		int notev = NotePitchToYPos(Pitch);
//		rV = (CLIENT_TO_STAVE_TOP_Y - notev) / 8;
	}
	return rV;
}
