#include "pch.h"

CMsGlissando::CMsGlissando():CMsObject()
{
    m_ObjType = MSOBJ_GLISANDO;
	m_pEvEnd = nullptr;
	m_EndPitch = 0;
	m_StartPitch = 0;
	m_TotalIntervals = 0;
	m_CurrentPitch = 0;
	m_TicksPerInterval = 0;
	m_GlisandoState = GlisandoState::END;
	m_GlisandoDirection = GlisandoDirection::UP;	
	m_Track = COMBO_Index_INST_7;
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
	static int Count = 0;

	printf("Gliss:MouseDown:Count=%d\n", ++Count);
	switch (DrawState)
	{
	case DRAWSTATE::WAITFORMOUSE_DOWN:
		printf("Gliss:MouseDown:Wait\n");
		DrawState = DRAWSTATE::GLISSANDO_FIRST_NOTE;
		break;
	case DRAWSTATE::GLISSANDO_FIRST_NOTE:
		printf("Gliss:MouseDown:FirstNote\n");
		break;
	case DRAWSTATE::GLISSANDO_SECOND_NOTE:
		printf("Gliss:MouseDown:SecondNote\n");
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
	static int Count = 0;

	printf("Gliss:MouseUp:Count=%d\n", ++Count);
	switch (DrawState)
	{
	case DRAWSTATE::SET_ATTRIBUTES:
		csText.Format(_T("Configure Note Prameters"));
		GetStaffView()->GetStatusBar()->SetText(csText);
		if ((Id = Dlg.DoModal()) == IDOK)
		{
			DrawState = DRAWSTATE::WAITFORMOUSE_DOWN;
			GetStaffView()->Invalidate();
		}
		break;
	case DRAWSTATE::WAITFORMOUSE_DOWN:
		//		m_P1 = m_P2 = pASV->m_SnapPos;
		//		pASV->EnableAutoScroll(1);
		//		DrawState = DRAWSTATE_PLACE;
		printf("Gliss:MouseUp:Wait\n");
		GetStaffView()->Invalidate();
		break;
	case DRAWSTATE::GLISSANDO_FIRST_NOTE:
		printf("Gliss:MouseUp:FirstNote\n");
//		GetSong()->AddObjectToSong(GetStaffView()->GetDrawEvent(), this);
		GetStaffView()->CheckAndDoScroll(pointMouse);
		csText.Format(_T("Place Second Note at Event %d"), GetStaffView()->GetDrawEvent());
		GetStaffView()->GetStatusBar()->SetText(csText);
		GetStaffView()->Invalidate();
		DrawState = DRAWSTATE::GLISSANDO_SECOND_NOTE;
		break;
	case DRAWSTATE::GLISSANDO_SECOND_NOTE:
		printf("Gliss:MouseUp:SecondNote\n");
		GetSong()->AddObjectToSong(GetStaffView()->GetDrawEvent(), this);
		GetStaffView()->CheckAndDoScroll(pointMouse);
		csText.Format(_T("Place First Note at Event %d"), GetStaffView()->GetDrawEvent());
		GetStaffView()->GetStatusBar()->SetText(csText);
		DrawState = DRAWSTATE::GLISSANDO_END;
		break;
	default:
		break;
	}
	m_DrawState = DrawState;
	return DrawState;
}

DRAWSTATE CMsGlissando::MouseMove(DRAWSTATE DrawState, CPoint pointMouse)
{
	int Pitch = YPosToNotePitch(pointMouse.y);
	switch (DrawState)
	{
	case DRAWSTATE::WAITFORMOUSE_DOWN:
		printf("Mouse Move Pitch=%d WMD\n", Pitch);
		SetStartPitch(Pitch);
		break;
	case DRAWSTATE::GLISSANDO_FIRST_NOTE:
		printf("Mouse Move Pitch=%d FN\n", Pitch);
		SetStartPitch(Pitch);
		break;
	case DRAWSTATE::GLISSANDO_SECOND_NOTE:
		printf("Mouse Move Pitch=%d SD\n", Pitch);
		SetEndPitch(Pitch);
		break;
	default:
		printf("Mouse Move Pitch=%d DEFAULT\n", Pitch);
		break;
	}
	GetSong()-> GetStaffChildView()->Invalidate();
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

void CMsGlissando::NoteOn(UINT Velociry)
{
	GetSong()->IncNoteOnCount();
	int chan = GETMIDIINFO->GetChannel(GetTrack());
	int note = m_CurrentPitch + CMsNote::RangeLUT[GETMIDIINFO->GetRange(GetTrack())];
	int DeviceID = GETMIDIINFO->GetMidiOutDeviceId(GetTrack());
	GETMIDIOUTTABLE.NoteOn(DeviceID, chan, note, Velociry);
}

void CMsGlissando::NoteOff(UINT Velociry)
{
	GetSong()->IncNoteOffCount();
	int chan = GETMIDIINFO->GetChannel(GetTrack());
	int note = m_CurrentPitch + CMsNote::RangeLUT[GETMIDIINFO->GetRange(GetTrack())];
	int DeviceID = GETMIDIINFO->GetMidiOutDeviceId(GetTrack());
	GETMIDIOUTTABLE.NoteOff(DeviceID, chan, note, Velociry);
}

CMsObject* CMsGlissando::Copy(void)
{
    return nullptr;
}

void CMsGlissando::Draw(CDC* pDC, int event, int maxevent)
{
	switch (m_DrawState)
	{
	case DRAWSTATE::WAITFORMOUSE_DOWN:
		DrawNote(pDC, m_StartPitch, event, true, m_StartStemDown);
		break;
	case DRAWSTATE::GLISSANDO_FIRST_NOTE:
		DrawNote(pDC, m_StartPitch, event, true, m_StartStemDown);
		break;
	case DRAWSTATE::GLISSANDO_SECOND_NOTE:
		[[fallthrough]];
	case DRAWSTATE::GLISSANDO_END:
		DrawNote(pDC, m_StartPitch, event, true, m_StartStemDown);
		DrawNote(pDC, m_EndPitch, event, true, m_EndStemDown);
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

void CMsGlissando::DrawNote(CDC* pDC, int Pitch, int Event, bool bStartPos, bool bStemDown)
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
		Color = GETMIDIINFO->GetInstColor(GetTrack());
	}
	penNote.CreatePen(PS_SOLID, 2, Color);
	brushNote.CreateSolidBrush(Color);
	pOldPen = pDC->SelectObject(&penNote);
	pOldBrush = pDC->SelectObject(&brushNote);
	if (bStartPos)
	{
		if (bStemDown)
		{
			NoteXPos = EVENT_OFFSET + EVENT_WIDTH * Event + NOTE_LINE_OFFSET;
			rectNoteHead = CRect(CPoint(NoteXPos,NoteYPos), szNoteHead);
			pDC->Ellipse(&rectNoteHead);
		}
		else
		{
			NoteXPos = EVENT_OFFSET + EVENT_WIDTH * Event + NOTE_LINE_OFFSET - NOTE_HEAD_WIDTH;
			rectNoteHead = CRect(CPoint(NoteXPos, NoteYPos), szNoteHead);
			pDC->Ellipse(&rectNoteHead);
		}
	}
	else
	{
		if (bStemDown)
		{
			NoteXPos = EVENT_OFFSET + EVENT_WIDTH * Event + NOTE_LINE_OFFSET;
			rectNoteHead = CRect(CPoint(NoteXPos, NoteYPos), szNoteHead);
			pDC->Ellipse(&rectNoteHead);
		}
		else
		{
			NoteXPos = EVENT_OFFSET + EVENT_WIDTH * Event + NOTE_LINE_OFFSET - NOTE_HEAD_WIDTH;
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
	return NotePos[NotePitch % 12] + 28 * (7 - (NotePitch / 12)) + (STAVE_OFFSET - 44);
}

int CMsGlissando::YPosToNotePitch(int YPos)
{
	int NoteIndex;
	int Octave = 0;
	int Note;
	//----------------------------
	// Notes place on staff will
	// start at HIGHC_OFFSET and
	// go to HIGHC_OFFSET- 8, so
	// we need to offset this by
	// 4
	//---------------------------------
	// subract off the offset for
	// C8 from the mouse Y coordiant
	// so that we know where to draw
	//----------------------------------
	YPos -= HIGHC_OFFSET;
	//----------------------------------
	// quantize the y coordiant so that
	// the note will only be drawn either
	// in the space, or on the line.  We do
	// not quantize the mouse pointer
	//-----------------------------------
	YPos = GetStaffView()->QuantizeY(YPos);
	YPos = QUANTIZED_STAFF_HEIGHT - YPos;	//invert y
	YPos -= STAVE_LINE_SPACING / 2;	//center
	//------------------------------------
	// Limit the range that y can take
	//------------------------------------
	if (YPos < 0) YPos = 0;
	NoteIndex = YPos % 7;
	Note = NoteLut[NoteIndex];
	Octave += (YPos / 7) * 12;	/// calculate octave
	Note = Note + Octave + 0x24;
	if (Note > 0x60)
		Note = 0x60;	// limit max value of Note
	return Note;
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
		rV = (STAVE_OFFSET - notev) / 8;
	}
	return rV;
}
