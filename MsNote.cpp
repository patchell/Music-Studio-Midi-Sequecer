///////////////////////////////////////////////////////
// MsNote.cpp: implementation of the CMsNote class.
//
// Middle C (C4) is MIDI note 60
// Lowest note on Staff (C2) is MIDI note 36
// Highest note on Staff (C6) is MIDI note 84
// 
// Note C1 = 24
// Note C0 = 12
// Note C-1 = 0
//////////////////////////////////////////////////////////////////////

#include "pch.h"

void PrintRecPtSz(const char* pTit, CRect rec)
{
//	if(LogFile()) fprintf(LogFile(),"---------------rect---------------------\n");
//	if(LogFile()) fprintf(LogFile(),"RECT : %s\n",pTit);
//	PrintPoint("Upper Left",rec.TopLeft());
//	PrintSize("Dimensions", rec.Size());
//	if(LogFile()) fprintf(LogFile(),"---------------end----------------------\n");
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMsNote::CMsNote() :CMsObject()
{
	m_ObjType = CMsObject::MsObjType::NOTE;
	m_BitmapFlag = false;
	m_Ticks = 0;
	m_NoteTieNext = 0; //pointer to note object is the next tied note
	m_NoteTiePrev = 0;	//pointer to previous note that is tied
	m_pRestBitmap = 0;
	m_MouseStaffTransition = StaffMouseStates::MOUSE_STAFF_STATE_NONE;
	m_NotePlayed = 0;
}


CMsNote::~CMsNote()
{
	
}

char* CMsNote::NoteToString(char* pStr, int l)
{
	int Octave;
	int NoteTemp;
	int Note = GetPitch();
	
	Note = GetSong()->GetCurrentKeySignature()->CorrectNoteByKeySig(Note, GetAccidental());
	NoteTemp = Note % HALF_STEPS_PER_OCTAVE;
	Octave = Note / HALF_STEPS_PER_OCTAVE - 1;
	sprintf_s(
		pStr,
		l,
		"%s%d [%d]",
		GetNoteName(NoteTemp),
		Octave,
		Note
	);
	return pStr;
}

void CMsNote::LoadRestBitmap(int Selection)
{
	if (COMBO_REST_HALF < Selection)
	{
		if(GetRestBitmap()->GetSafeHandle())
			GetRestBitmap()->DeleteObject();
		GetRestBitmap()->LoadBitmapW(GETAPP->GetRestBmIdsTypes(Selection));
		m_BitmapFlag = true;
	}
	else
		m_BitmapFlag = false;
}

bool CMsNote::Create(
	int nBitmapID, 
	CMsSong* pSong, 
	CMsEvent* pParentEvent
)
{
	if (nBitmapID)
	{
		m_pRestBitmap = new CMyBitmap();
		m_pRestBitmap->LoadBitmapW(nBitmapID);
		m_BitmapFlag = true;
	}
	else
		m_BitmapFlag = false;
	if(pSong == NULL)
		if (LogFile()) fprintf(LogFile(), "pSong is NULL in CMsNote::Create\n");
	return CMsObject::Create(pSong, pParentEvent);
}


int CMsNote::NearestLine()
{
	int OctaveNumber;
	int Note;
	int NearestLineOffset;

	OctaveNumber = GetPitch() / 12;
	Note = GetPitch() % 12;
	if (OctaveNumber & 1)	//odd
		NearestLineOffset = NearestLineOdd[Note];
	else      //Even
		NearestLineOffset = NearestLineEven[Note];
	return NearestLineOffset;
}


void CMsNote::Print(FILE *pO, int Indent)
{
	char* pIndentString = new char[256];

	theApp.IndentString(pIndentString, 256, Indent);
	if(pO)fprintf(pO, "%s***********************************\n", pIndentString);
	CMsObject::Print(pO, Indent+4);
	GetData().PrintData(pO, GetDataPointer(), Indent+4);
	if(pO) fprintf(pO, "%s***********************************\n", pIndentString);

	if(pIndentString) delete[] pIndentString;
}

UINT CMsNote::ObjectToString(CString& csString, UINT mode)
{
	int Note, Octave;

	Note = m_Data.GetPitch() % HALF_STEPS_PER_OCTAVE;
	Octave = m_Data.GetPitch() / HALF_STEPS_PER_OCTAVE - 1;
	if (mode == 0)
		csString.Format(
			_T("NOTE:%4lS%d%c Track %d Sel %d"),
			NoteLUT[Note].GetString(),
			Octave,
			AccidentalLUT[GetAccidental()],
			GetTrack(),
			IsSelected()
		);
	else if (mode == 1)
	{
		csString.Format(
			_T("NOTE:%4lS%d%c Track:%2d Tie:%c:%c <%s> <%s>"),
			NoteLUT[Note].GetString(),
			Octave,
			AccidentalLUT[GetAccidental()],
			GetTrack(),
			IsTieBeg() ? 'B' : ' ',
			IsTieEnd() ? 'E' : ' ',
			IsRest() ? _T("REST") : _T(""),
			GetAccent() ? _T("ACCENT") : _T("")
		);
	}
	return csString.GetLength();
}

int CMsNote::NoteToPosition(int Note)
{
	int NotePosition;
	int Octave;

//	if(LogFile()) fprintf(LogFile(), "CMsNote:NoteToPos:Enter Note=%d\n", Note);
	Note -= NOTE_C2;  // Normalize Note value to be zero at C3
	Octave = Note / 12;
	Octave = 4 - Octave;	// Invert octave order
	Octave *= 7;		// Each octave is 7 staff positions
	Octave *= 4;		// Each staff position is 4 pixels
	NotePosition = NotePos[Note % 12];
	NotePosition += Octave + 4;
//	NotePosition = 60 - NotePosition; // Invert for screen coordinates
	//NotePosition = NotePos[Note % 12];
	//if(LogFile()) fprintf(LogFile(),"   CMsNote:NoteToPos:Position=%d\n", NotePosition);
	//NotePosition += SINGLE_STAVE_HEIGHT * (7 - (Note / 12));	// Calculates octave position
	//if(LogFile()) fprintf(LogFile(),"   CMsNote:NoteToPos:Position=%d\n", NotePosition);
	//NotePosition += 4;
	if (LogFile()) 
		fprintf(LogFile(), "   CMsNote:NoteToPos:Note=%d Pos=%d\n", 
			Note, 
			NotePosition
		);
	return NotePosition;
}

StaffMouseStates CMsNote::StaffTransition(CPoint pointMouse, int NewNote, CMsEvent* pNewEvent)
{
	//---------------------------------
	// Determine what kind of transition
	// that the mouse made on the staff
	// NewNote: Potential new note
	// pNewEvent: Potential new event
	//---------------------------------
	bool bNoteChanged, bEventChanged;
	int thisEventIndex, NewEventIndex;
	StaffMouseStates State;
	char* pStr1 = new char[256];
	char* pStr2 = new char[256];

	State = StaffMouseStates::MOUSE_STAFF_STATE_NONE;

	NewEventIndex = pNewEvent ? pNewEvent->GetIndex() : -1;
	thisEventIndex = GetParentEvent() ? GetParentEvent()->GetIndex() : -1;

	bNoteChanged = (NewNote != GetPitch());
	bEventChanged = (NewEventIndex != thisEventIndex);

	
	//fix
	//if(bNoteChanged)
	//	fprintf(LogFile(), 
	//		">>CMsNote::StaffTransition: Note Changed from %s to %s\n", 
	//		NoteToString(pStr1, 256,GetPitch()), 
	//		NoteToString(pStr2, 256, NewNote)
	//	);

	if (!bNoteChanged && !bEventChanged)
		State = StaffMouseStates::MOUSE_STAFF_STATE_NONE;
	else if (bNoteChanged && !bEventChanged)
		State = StaffMouseStates::MOUSE_STAFF_STATE_NOTE_CHANGE;
	else if (!bNoteChanged && bEventChanged)
		State = StaffMouseStates::MOUSE_STAFF_STATE_EVENT_CHANGE;
	else if (bNoteChanged && bEventChanged)
		State = StaffMouseStates::MOUSE_STAFF_STATE_NOTE__EVENT_CHANGE;
	if (pStr1) delete[] pStr1;
	if (pStr2) delete[] pStr2;
	return State;
}

void CMsNote::Draw(CDC *pDC)
{
	CPoint End,Start;
//	CPen pen,*oldpen;
	CBrush brush;
	int NoteY;
	int Pitch;
	COLORREF Color;
	CRect rectRest;
	CMsEvent* pEvent = GetParentEvent();
	char* pStr = new char[256];

	Color = GetSong()->GetTrackInfo(GetTrack())->GetColor();
	if (IsSelected() || IsHighLighted())
		Color ^= 0x00ffffff;
	Pitch = GetPitch();
	NoteY = NoteToPosition(Pitch);
	if (LogFile()) 
		fprintf(LogFile(), "CMsNote::Draw: NoteY=%d Pitch=%s\n", 
			NoteY, 
			NoteToString(pStr, 256)
		);
	if(IsRest())	///draw rest on staff
	{
		DrawRest(pDC, NoteY, Color);
	}
	else	//draw note on staff
	{
		DrawNote(pDC, NoteY, Color);
	}
}

void CMsNote::DrawNote(CDC* pDC, int NoteY, COLORREF Color)
{
	//		CMsNote* pSecInterval;
	DrawNoteHead(pDC, NoteY, Color);
	DrawNoteLines(pDC, NoteY, RGB(0,0,0));
	DrawNoteStem(pDC, NoteY, Color);

	if (IsDotted())
	{
		DrawNoteDots(pDC, NoteY, Color);
	}
	else if(IsTriplet())
	{
		DrawNoteTriplet(pDC, NoteY, Color);
	}
	if (IsAccentted())
	{
		DrawNoteAccent(pDC, NoteY, Color);
	}
	if (IsTieEnd())
	{
		DrawNoteTie(pDC, NoteY, Color);
	}
	if(GetAccidental() != MSFF_ACCIDENTAL_INKEY)
	{
		DrawNoteAccidental(pDC, NoteY, Color);
	}
	//CRect rect;
	//ObjectRectangle(rect, Event);
	//CPen penBlack, * oldPen;
	//penBlack.CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
	//oldPen = pDC->SelectObject(&penBlack);
	//pDC->Rectangle(&rect);
	//pDC->SelectObject(oldPen);
}

void CMsNote::DrawNoteHead(
	CDC* pDC, 
	int NoteY, 
	COLORREF Color
)
{
	CRect rectNoteHead;
	CPen penHead, * oldPen;
	CBrush brushHead, * oldBrush;
	bool bIsSolid = IsSolid();

	penHead.CreatePen(PS_SOLID, 1, Color);
	oldPen = pDC->SelectObject(&penHead);
	if (bIsSolid)
		brushHead.CreateSolidBrush(Color);
	else
		brushHead.CreateStockObject(HOLLOW_BRUSH);
	oldBrush = pDC->SelectObject(&brushHead);

	ObjectRectangle(rectNoteHead);

	pDC->Ellipse(&rectNoteHead);
	pDC->SelectObject(oldPen);
	pDC->SelectObject(oldBrush);
}

void CMsNote::DrawNoteLines(CDC* pDC, int NoteY, COLORREF Color)
{
	int NeedsALine = NeedsLine();
	int y = 0;
	CPen penLines, *penOld = 0;
	char* pStr = new char[256];
	if(LogFile())
		fprintf(LogFile(), "\t\tCMsNote::DrawNoteLines: Note=%s NeedsALine=%d\n", 
			NoteToString(pStr, 256),
			NeedsALine
		);
	if(NeedsALine > 0)
	{
		penLines.CreatePen(PS_SOLID, 1, Color);
		penOld = pDC->SelectObject(&penLines);
		int notev = NoteToPosition(GetPitch());
		for (int i = 0; i < NeedsALine; ++i)
		{
			int lineEnd = (EVENT_WIDTH + 10) / 2;
			int x = NOTE_STEM_OFFSET;
			if (IsOnLine())
				y = NoteY + i * 8;
			else
				y = NoteY + 4 + i * 8;
			pDC->MoveTo(x - lineEnd, y);
			pDC->LineTo(x + lineEnd, y);
		}
		pDC->SelectObject(penOld);
	}
	else if (NeedsALine == 0)
	{
		// no extra lines needed
	}
	else
	{
		// should not happen
	}
	if (pStr) delete[] pStr;
}

void CMsNote::DrawNoteStem(CDC* pDC, int NoteY, COLORREF Color)
{
	UINT x = 0;
	CPen penStem, * oldPen;

	if (NeedsStem())
	{
		penStem.CreatePen(PS_SOLID, 1, Color);
		oldPen = pDC->SelectObject(&penStem);
		x = NOTE_STEM_OFFSET;
		if (IsStemDown())
		{
			pDC->MoveTo(x, NoteY + 4);
			pDC->LineTo(x, NoteY + NOTE_STEM_LENGTH);
		}
		else
		{
			pDC->MoveTo(x, NoteY + 4);
			pDC->LineTo(x, NoteY - NOTE_STEM_LENGTH);
		}
		pDC->SelectObject(oldPen);
		DrawNoteFlags(pDC, NoteY, Color);
	}
}

void CMsNote::DrawNoteFlags(CDC* pDC, int NoteY, COLORREF Color)
{
	int n = 0;
	UINT x = 0;
	int i;
	CPen penFlags, * oldPen;
	//------------------------------
	// draw flags on stem for sub
	// quarter notes and smaller
	//------------------------------
	if ((n = NeedsFlags()) && !GetFlagsOff())
	{
		penFlags.CreatePen(PS_SOLID, 2, Color);
		oldPen = pDC->SelectObject(&penFlags);
		x = NOTE_STEM_OFFSET;
		for (i = 0; i < n; ++i)
		{
			if (IsStemDown())	//right side down
			{
				pDC->MoveTo(x, NoteY + 28 - (i * 5));
				pDC->LineTo(x + 4, NoteY + 24 - (i * 5));
			}
			else    // right side up
			{
				pDC->MoveTo(x, NoteY - 20 + i * 5);
				pDC->LineTo(x + 4, NoteY - 14 + i * 5);
			}
		}
		pDC->SelectObject(oldPen);
	}
}

void CMsNote::DrawNoteAccidental(CDC* pDC, int NoteY, COLORREF Color)
{
	CMsSharp sharp;
	CMsFlat flat;
	CMsNatural nat;
	int X;

	X = NOTE_ACCIDENTAL_OFFSET;
	switch (GetAccidental())
	{
	case MSFF_ACCIDENTAL_NATURAL:
		nat.Draw(pDC, Color, X, NoteY);
		break;
	case MSFF_ACCIDENTAL_SHARP:
		sharp.Draw(pDC, Color, X, NoteY);
		break;
	case MSFF_ACCIDENTAL_FLAT:
		flat.Draw(pDC, Color, X, NoteY);
		break;
	}
}

void CMsNote::DrawNoteAccent(CDC* pDC, int NoteY, COLORREF Color)
{
	CPen penAccent, * oldPen;

	penAccent.CreatePen(PS_SOLID, 1, Color);
	oldPen = pDC->SelectObject(&penAccent);
	if (IsStemDown())
	{
		pDC->MoveTo(0, NoteY - 4);
		pDC->LineTo(8, NoteY - 7);
		pDC->LineTo(0, NoteY - 10);
	}
	else
	{
		pDC->MoveTo(0, NoteY + 12);
		pDC->LineTo(8, NoteY + 15);
		pDC->LineTo(0, NoteY + 18);
	}
	pDC->SelectObject(oldPen);
}

void CMsNote::DrawNoteDots(CDC* pDC, int NoteY, COLORREF Color)
{
	int y;

	if (IsOnLine())
		y = NoteY;
	else
		y = NoteY + 4;
	if (IsStemDown())
	{
		pDC->SetPixel(31, y + 3, Color);
		pDC->SetPixel(32, y + 3, Color);
		pDC->SetPixel(33, y + 3, Color);
		pDC->SetPixel(32, y + 4, Color);
		pDC->SetPixel(32, y + 2, Color);
	}
	else
	{
		pDC->SetPixel(23, y + 3, Color);
		pDC->SetPixel(24, y + 3, Color);
		pDC->SetPixel(22, y + 3, Color);
		pDC->SetPixel(23, y + 4, Color);
		pDC->SetPixel(23, y + 2, Color);
	}
}

void CMsNote::DrawNoteTriplet(CDC* pDC, int NoteY, COLORREF Color)
{
	pDC->SetPixel(12, NoteY + 6, Color);
	pDC->SetPixel(13, NoteY + 6, Color);
	pDC->SetPixel(14, NoteY + 7, Color);
	pDC->SetPixel(13, NoteY + 8, Color);
	pDC->SetPixel(14, NoteY + 9, Color);
	pDC->SetPixel(13, NoteY + 10, Color);
	pDC->SetPixel(12, NoteY + 10, Color);
}

void CMsNote::DrawNoteTie(CDC* pDC, int NoteY, COLORREF Color)
{
	CMsNote* pN;
	CPoint Start, End;
//	int widthEv;
	CRect rectTieCurve;
	//int width;
	//int x;

	pN = FindTieBegin();
	if (pN)
	{
		//CMsEvent* pEv = pN->GetParentEvent();
		//widthEv = GetParentEvent()->GetIndex() - pEv->GetIndex();
		//width = EVENT_WIDTH * widthEv;
		//x = 5 + EVENT_OFFSET + (Event - widthEv) * EVENT_WIDTH;
		//rectTieCurve.SetRect(x, NoteY, x + width, NoteY + 16);
		//if (IsStemDown())
		//{
		//	End = CPoint(x + 1, NoteY - 8);
		//	Start = CPoint(x + width - 1, NoteY - 8);
		//}
		//else
		//{
		//	Start = CPoint(x + 1, NoteY + 8);
		//	End = CPoint(x + width - 1, NoteY + 8);
		//}
		//pDC->Arc(rectTieCurve, Start, End);
	}
}

void CMsNote::DrawRest(CDC* pDC, int NoteY, COLORREF Color)
{
	CPen pen, * oldpen;
	CBrush brush, * oldbrush;

	pen.CreatePen(PS_SOLID, 1, Color);
	oldpen = pDC->SelectObject(&pen);
	brush.CreateSolidBrush(Color);
	oldbrush = pDC->SelectObject(&brush);
	switch (GetDuration())
	{
	case MSFF_REST_QUARTER:
	case MSFF_REST_EIGTH:
	case MSFF_REST_SIXTEENTH:
	case MSFF_REST_THIRTYSECOND:
		DrawRestBitmap(pDC, NoteY, Color);
		break;
	case MSFF_REST_HALF:
		DrawHalfRest(pDC, NoteY, Color);
		break;
	case MSFF_REST_WHOLE:
		DrawWholeRest(pDC, NoteY, Color);

		break;
	default:
		//			if(LogFile()) fprintf(LogFile(),"Opps Dur=%d\n", GetDuration());
		break;
	}
	pDC->SelectObject(oldpen);
	pDC->SelectObject(oldbrush);
}

void CMsNote::DrawRestBitmap(CDC* pDC, int notev, COLORREF color)
{
	CMyBitmap* pOldBm;
	int x, y;
	CDC dc;
	CSize bmSize;

	if (m_pRestBitmap)
	{
		dc.CreateCompatibleDC(pDC);
		bmSize = m_pRestBitmap->GetBmDim();
		pOldBm = (CMyBitmap*)dc.SelectObject(m_pRestBitmap);
		x = 0;
		y = notev;
		pDC->BitBlt(x, y, bmSize.cx, bmSize.cy, &dc, 0, 0, SRCAND);
		ChangeRestColor(&dc, color, bmSize.cx, bmSize.cy);
		pDC->BitBlt(x, y, bmSize.cx, bmSize.cy, &dc, 0, 0, SRCAND);
		dc.SelectObject(pOldBm);
	}

}

void CMsNote::DrawHalfRest(CDC* pDC, int NoteY, COLORREF Color)
{
	NoteY += 4;
	CRect rectRest;

	rectRest.SetRect(
		0,
		NoteY,
		HALF_REST_WIDTH,
		NoteY - HALF_REST_HEIGHT
	);
	if (NeedsLine())
	{
		pDC->MoveTo(EVENT_WIDTH  - 2, NoteY);
		pDC->LineTo(EVENT_WIDTH + 10, NoteY);
	}
	pDC->Rectangle(&rectRest);
}

void CMsNote::DrawWholeRest(CDC* pDC, int NoteY, COLORREF Color)
{
	//---------------------------------
	// The second parameter, the Y
	// Coordinate of the CRect Upper
	// Left corner is the value of
	// notev + 2.  The rest needs to be
	// one pixel lower for the line,and
	// one pixel to make the rect butt
	// up against the line.
	//----------------------------------
	CRect rectRest;
	NoteY += 5;
	;
	rectRest.SetRect(
		0,
		NoteY,
		8,
		NoteY + 6
	);
	rectRest.NormalizeRect();
	if (NeedsLine())
	{
		pDC->MoveTo(EVENT_WIDTH - 2, NoteY);
		pDC->LineTo(EVENT_WIDTH  + 10, NoteY);
	}
	pDC->Rectangle(&rectRest);
}

//-----------------------------------------------------

int CMsNote::NeedsLine()
{
	//-------------------------
	// Middle C is 0x3C
	//-------------------------
	int LinesNeeded = 0;
	int pitch = GetPitch();
	int NotePosition = NoteToPosition(pitch);

	if (NotePosition >= 0 && NotePosition <= 49)
	{
		LinesNeeded = LinesNeededLUT[NotePosition].m_Lines;
	}
	else
	{
		LinesNeeded = -1;	// error 
	}
	return LinesNeeded;
}

bool CMsNote::IsTriplet()
{
	bool rV = false;
	if(DurTab[GetDuration()].Triplet) rV = true;
	return rV;
}

bool CMsNote::IsDuplicate()
{
	bool rV = false;
	int Pitch = GetPitch();
	CMsEvent* pEv = GetParentEvent();
	CMsNote* pNote = 0;

	pNote = pEv->FindFirstNote();
	while (pNote && !rV)
	{
		if (pNote == this)
		{
			pNote = pEv->FindNextNote(pNote);
		}
		else if (Pitch == pNote->GetPitch())
		{
			pEv->RemoveObject(pNote);
			delete pNote;
			rV = true;
		}
		else
			pNote = pEv->FindNextNote(pNote);

	}
	return rV;
}

int CMsNote::NeedsFlags(void)
{
	int rV = 0;

	rV = (int)DurTab[(int)GetDuration()].Flags;
	return rV;
}

CMsNote * CMsNote::FindTieBegin()
{
	CMsEvent *pEv = GetParentEvent();
	ObjectTypes pObj;
	CMsNote *pMsN = 0;

	int loop = 1;
	pObj.pObj = 0;

	pEv = (CMsEvent *)pEv->GetPrev();
	while(pEv && loop)
	{
		pObj.pObj = pEv->GetEventMsObjectHead();
		while(pObj.pObj && loop)
		{
			switch(pObj.pObj->GetType())
			{
				case CMsObject::MsObjType::NOTE:
					if((GetPitch() == pObj.pNote->GetPitch()) && pObj.pNote->IsTieBeg())
					{
						pMsN = pObj.pNote;
						loop = 0;
					}
					else
						pObj.pObj = pObj.pObj->GetNext();
					break;
				default:
					pObj.pObj = pObj.pObj->GetNext();
					break;
			}
		}
		pEv = pEv->GetPrev();
	}
	return pMsN;
}

UINT wholeLUT[12] = {
	1,	//C
	1,	//C#
	0,	//D
	0,	//D#
	1,	//E,
	0,	//F
	0,	//F#
	1,	//G
	1,	//G#
	0,	//A
	0,	//A#
	1	//B
};

int CMsNote::WholeRestOffset(int pitch)
{
	int n = pitch % 12;	//pitch
	int o = pitch / 12;	//octave
	int rV;
	if(!(o & 1))	//even octave
	{
		if(wholeLUT[n])
			rV = 6;	//OK
		else
			rV = -6; //Not OK
	}
	else     // Odd Octave
	{
		if(wholeLUT[n])
			rV = -6;	//Not OK
		else
			rV = 6;		//OK
	}
	return rV;
}

int CMsNote::HalfRestOffset()
{
	int n = GetPitch() % 12;
	int o = GetPitch() / 12;
	int rV;
	if(o & 1)
	{
		if(wholeLUT[n])
			rV = 5;
		else
			rV = -5;
	}
	else
	{
		if(wholeLUT[n])
			rV = -5;
		else
			rV = 5;
	}
	return rV;
}

void CMsNote::ChangeRestColor(CDC *pDC,COLORREF newcolor,int w,int h)
{
	int i,j;
	COLORREF c;

	for(i=0;i<w;++i)
	{
		for(j=0;j<h;++j)
		{
			((c = pDC->GetPixel(i, j)) == RGB(0,0,0))
				? pDC->SetPixel(i, j, newcolor)
				: pDC->SetPixel(i, j, c);
		}
	}
}


void CMsNote::NoteOn(int Velocity)
{
	int MidiChannel;
	int NotePitch;
	int DeviceID;
	int Loudness;
	char* pStr = new char[256];

	GetSong()->IncNoteOnCount();	// for diagnostics

	if(IsAccentted())
	{
		Loudness = 127;
	}
	else
		Loudness = Velocity;
	MidiChannel = GetSong()->GetTrackInfo(GetTrack())->GetChannel();
	NotePitch = GetPitch();
	//-------------------------------
	// Apply Key Signature Correction
	// or Accidental to correct pitch
	//-------------------------------
	NotePitch = GetSong()->GetCurrentKeySignature()->CorrectNoteByKeySig(NotePitch, GetAccidental());
	m_NotePlayed = NotePitch;
	DeviceID = GetSong()->GetTrackInfo(GetTrack())->GetMidiOutDeviceID();
	GETAPP->GetMidiOutTable()->GetDevice(DeviceID).NoteOn(MidiChannel, NotePitch, Loudness);
	if(LogFile())
		fprintf(LogFile(), "NoteOn: Ch=%d Note=%s Vel=%d\n", 
			MidiChannel, 
			NoteToString(pStr, 256),
			Loudness
		);
	if (pStr) delete[] pStr;
}

void CMsNote::NoteOff(int Velocity)
{
	int MidiChannel;
	int DeviceID;
	int NotePitch;
	GetSong()->IncNoteOffCount();	// for diagnostics
	char* pStr = new char[256];

	MidiChannel = GetSong()->GetTrackInfo(GetTrack())->GetChannel();
	NotePitch = m_NotePlayed;
	DeviceID = GetSong()->GetTrackInfo(GetTrack())->GetMidiOutDeviceID();
	GETAPP->GetMidiOutTable()->GetDevice(DeviceID).NoteOff(MidiChannel, NotePitch, Velocity);
	if(LogFile())
		fprintf(LogFile(), "NoteOff: Ch=%d Note=%s Vel=%d\n",
			MidiChannel, 
			NoteToString(pStr,256),
			Velocity
		);
	if (pStr) delete[] pStr;
}

void CMsNote::IncrNote()
{
	int Pitch = GetPitch();
	int i = Pitch % 12;
	Pitch += NoteInc[i];
	SetPitch(Pitch);
}

void CMsNote::DecrNote()
{
	int Pitch = GetPitch();
	int i = Pitch % 12;
	Pitch -= NoteDec[i];
	SetPitch(Pitch);
}

void CMsNote::Copy(CMsNote* pN)
{
	CMsSong* pSong = GetSong();

	if(pSong == NULL)
	{
		if (LogFile()) 
			fprintf(LogFile(), "pSong is NULL in CMsNote::Copy\n");
	}
	SetRestBitmap(pN->GetRestBitmap());
	GetData().CopyData(pN->GetData());
	SetParentEvent(pN->GetParentEvent());
	if (m_BitmapFlag)
		GetRestBitmap()->LoadBitmapW(CMidiSeqMSApp::GetRestBmIdsTypes()[pN->GetShape()]);
	CMsObject::Copy(pN);
}

int CMsNote::MouseYToNote(int MouseY)
{
	int NoteIndex;
	int Octave = 0;
	int Note;
	//----------------------------
	// Notes place on staff will
	// start at HIGH_C and
	// go to LOW_C
	//---------------------------------
	//----------- Normalize Y Coord To Edit Region -------------
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
	else if (MouseY > TOTAL_WHITE_KEYS)
		MouseY = TOTAL_WHITE_KEYS;
	//------ Note Position to Real Note ------
	NoteIndex = MouseY % WHITE_KEYS_PER_OCTAVE;
	Note = NoteLut[NoteIndex];
	Octave += (MouseY / WHITE_KEYS_PER_OCTAVE) * HALF_STEPS_PER_OCTAVE;	// calculate octave
	//------------------------------------
	// Set Note Pitch Value
	// This is the sum of the note, the
	// octave, plus an offset so that
	// C2 is the lowest note.  This also
	// makes middle C (C4) = 60
	//------------------------------------
	//char* psTemp = new char[256];
	//fprintf(LogFile(), "\tCMsNote::YtoNote: MouseY=%d Note=%d:%lS\n", 
	//	MouseY, 
	//	Note + Octave + 24,
	//	NoteToString(psTemp, 256, Note + Octave + 24)
	//);
	//if (psTemp) delete[] psTemp;
	return Note + Octave + NOTE_C2;
}

DRAWSTATE CMsNote::MouseLButtonDown(DRAWSTATE DrawState, CPoint pointMouse)
{
	//-------------------------------------------------------
	//	MouseDown
	//		This is the state machine for creating this
	//	object on the screen.  This function is for when
	//	the left mouse button goes down.
	//
	//	parameters:
	//		DrawState.Current state of drawing process
	//
	//	Returns:
	//		Next Draw State
	//-------------------------------------------------------

	switch (DrawState)
	{
	case DRAWSTATE::SET_ATTRIBUTES:
		break;
	case DRAWSTATE::WAITFORMOUSE_DOWN:
		DrawState = DRAWSTATE::PLACE;
		break;
	case DRAWSTATE::PLACE:
		break;
	}
	GetStaffView()->Invalidate();
	return DrawState;
}

DRAWSTATE CMsNote::MouseLButtonUp(DRAWSTATE DrawState, CPoint pointMouse)
{
	//-------------------------------------------------------
	// MouseUp
	//		This is the state machine for creating this
	//	object on the screen.  This function is for when
	//	the left mouse button goes up.
	//
	//	parameters:
	//		DrawState.Current state of drawing process
	//
	//	Returns:
	//		Next Draw State
	//-------------------------------------------------------
	CNotePropertiesDlg Dlg;
	int Id;
	CString csText;
	int EventIndex = 0;
	CMsEvent* pEV = 0;
	CMsNote* pN = 0;
	CMsNote* pNote = 0;

	switch (DrawState)
	{
	case DRAWSTATE::SET_ATTRIBUTES:
		csText.Format(_T("Configure Note Prameters"));
		GetStaffView()->GetStatusBar()->SetText(csText);
		Dlg.SetNoteToEdit(this);
		if ((Id = Dlg.DoModal()) == IDOK)
		{
			DrawState = DRAWSTATE::WAITFORMOUSE_DOWN;
			GetStaffView()->Invalidate();
		}
		break;
	case DRAWSTATE::WAITFORMOUSE_DOWN:
		csText.Format(_T("Click on the staff to place the note"));
		GetStaffView()->GetStatusBar()->SetText(csText);
		GetStaffView()->Invalidate();
		break;
	case DRAWSTATE::PLACE:
		GetStaffView()->SetLastSongPosition(0);
		//-----------------------------
		// Create new Note Object to be
		// placed on the staff view
		//------------------------------
		if (!IsDuplicate())
		{
			pN = new CMsNote;
//			pNote = (CMsNote*)GetStaffView()->GetDrawObject();
			pEV = GetSong()->GetEventObject(GetStaffView()->XtoEventIndex(pointMouse.x));
//			SetPitch(pNote->GetPitch());
			if (IsRest())
				pN->Create(CMidiSeqMSApp::GetRestBmIdsTypes()[pNote->GetShape()], GetSong(), GetSong()->GetEventObject(GetStaffView()->GetDrawEvent()));	// Create Rest
			else
				pN->Create(0, GetSong(), GetSong()->GetEventObject(GetStaffView()->GetDrawEvent()));	// Create Note
			//-----------------------------
			// Copy attributes
			//------------------------------vb 
			pN->GetData().CopyData(GetStaffView()->GetNoteData());
			pN->SetParentEvent(pEV);
			pEV->AddObject(pN);
			GetStaffView()->SetDrawObject(pN);
		}
		GetStaffView()->CheckAndDoScroll(pointMouse);
		DrawState = DRAWSTATE::WAITFORMOUSE_DOWN;
		break;
	}
	return DrawState;
}

DRAWSTATE CMsNote::MouseMove(DRAWSTATE DrawState, CPoint pointMouse, MouseRegions Region, MouseRegionTransitionState Transition)
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
	int Note = -1;
	CString csStatusString, csTemp;
	CMsEvent* pEV = 0, *pNewEvent = 0;
	char* pStr = new char[256];

	//if(LogFile() && Region == MouseRegions::MOUSE_IN_EDITREG)
	//	fprintf(LogFile(),"CMsNote::MouseMove: DrawState=%s ** Region=%s ** Transition=%s\n",
	//		CChildViewStaff::GetDrawStateName(DrawState),
	//		CChildViewStaff::GetMouseRegionName(Region)	,
	//		CChildViewStaff::GetMouseRegionTransitionName(Transition)
	//	);
	switch (DrawState)
	{
	case DRAWSTATE::SET_ATTRIBUTES:		//MouseMove
		break;
	case DRAWSTATE::WAITFORMOUSE_DOWN:		//MouseMove
		Note = MouseYToNote(pointMouse.y);	// Potential new note
		pNewEvent = GetSong()->GetEventObject(GetStaffChildView()->XtoEventIndex(pointMouse.x));
		switch (Transition)
		{
		case MouseRegionTransitionState::MOUSE_TRANSITION_NONE:
			if (Region == MouseRegions::MOUSE_IN_EDITREG)
			{
				//-----------------------------
				// How is the mouse moving 
				// between regions
				//-----------------------------
				switch (StaffTransition(pointMouse, Note, pNewEvent))
				{
				case StaffMouseStates::MOUSE_STAFF_STATE_NONE:
					break;
				case StaffMouseStates::MOUSE_STAFF_STATE_NOTE_CHANGE:
//					fprintf(LogFile(), "--CMsNote::MouseMove: Note Change\n");
					NoteOff(0);
					SetPitch(Note);
					NoteOn(GetVelocity());
//					fprintf(LogFile(), "++CMsNote::MouseMove: Note Change\n");
					break;
				case StaffMouseStates::MOUSE_STAFF_STATE_EVENT_CHANGE:
					if (GetParentEvent())
					{
						GetParentEvent()->RemoveObject(this);
						SetParentEvent(nullptr);
						NoteOff(0);
					}
					pEV = GetSong()->GetEventObject(GetStaffView()->XtoEventIndex(pointMouse.x));
					if (pEV)
					{
						pEV->AddObject(this);
						SetParentEvent(pEV);
						NoteOn(GetVelocity());
					}
					break;
				case StaffMouseStates::MOUSE_STAFF_STATE_NOTE__EVENT_CHANGE:
					if (GetParentEvent())
					{
						GetParentEvent()->RemoveObject(this);
						SetParentEvent(nullptr);
						NoteOff(0);
					}
					pEV = GetSong()->GetEventObject(GetStaffView()->XtoEventIndex(pointMouse.x));
					if (pEV)
					{
						pEV->AddObject(this);
						SetParentEvent(pEV);
						SetPitch(Note);
						NoteOn(GetVelocity());
					}
					break;
				}
			}
			break;
		case MouseRegionTransitionState::MOUSE_TRANSITION_EDIT_TO_UPPER_DRAW:		//MouseMove
		case MouseRegionTransitionState::MOUSE_TRANSITION_EDIT_TO_LOWER_DRAW:
		case MouseRegionTransitionState::MOUSE_TRANSITION_EDIT_TO_OUTSIDE:
			if (GetParentEvent())
			{
				if (GetParentEvent()->IsThisObjectInThisEvent(this))
				{
					GetParentEvent()->RemoveObject(this);
					SetParentEvent(nullptr);
					NoteOff(0);
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
//				Note = MouseYToNote(pointMouse.y);
				SetPitch(Note);
				NoteOn(GetVelocity());
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
		//Note = GetStaffView()->YtoNote(pointMouse.y);
		//if (Note != m_NotePlayed)
		//{
		//	NoteOff(0);
		//	SetPitch(Note);
		//	NoteOn(GetSong()->GetCurrentLoudness()->GetLoudness());
		//}
		NoteToString(pStr, 256);
		csStatusString.Format(_T("Note: %S  Track: %d  Event: %d"),
			pStr,
			GetTrack(),
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
	if (pStr) delete[] pStr;
	return DrawState;
}

void CMsNote::Save(FILE *pO)
{
	int Byte1=0,Byte2=0,Byte3=0;
	//-------------------------
	// First Byte
	//-------------------------
	if(IsTieBeg()) Byte1 |= MSFF_BEGTIE_FLAG;
	if(IsTieEnd()) Byte1 |= MSFF_ENDTIE_FLAG;
	if(IsRest()) Byte1 |= MSFF_REST_FLAG;
	Byte1 |= GetTrack();
	//-------------------------
	// Second Byte
	//-------------------------
	if(GetAccent()) Byte2 |= MSFF_ACCENT_FLAG;
	Byte2 |= GetDuration();
	Byte2 |= GetAccidental() << MSFF_KEY_SHIFT;
	//--------------------------
	// third Byte
	//--------------------------
	Byte3 = GetPitch();
	if(IsStemDown()) Byte3 |= MSFF_NOTE_UPSIDE_DOWN;
	fputc(Byte1,pO);
	fputc(Byte2,pO);
	fputc(Byte3,pO);
}

void CMsNote::SetTick(int Duration, int tempo)
{
	//----------------------------
	// Set the number of ticks for
	// the ticker.  This is the
	// product of the duration
	// and the tempo
	//----------------------------
	m_Ticks = DurTab[Duration].DurationTimeTicks() * tempo;
}

int CMsNote::GetChannel()
{
	//------------------------------------------
	//	GetChannel
	//		Returns the Midi Channel number for
	//	the note that is going to play.
	//------------------------------------------
	return GetSong()->GetTrackInfo(GetTrack())->GetChannel();
}

//------- These Methods Handle the playing of the song ---

UINT CMsNote::Play()
{
	//---------------------------------------
	// This method over rides the the Play()
	// method in the CMsObject base class.
	// returns 1 if object needs to be
	// deleted
	//---------------------------------------
	UINT DeleteObject = PLAY_NO_ACTION;

	switch (GetPlayState())
	{
	case PLAYSTATE_START:
		if (IsTieBeg())
		{
//			fprintf(GETAPP->LogFile(),"Tie Note Start\n");
		}
		SetTick(GetDuration(), 1);
		if(IsNote() && !IsTieEnd())
			NoteOn(GetSong()->GetCurrentLoudness()->GetLoudness());
		SetPlayState(PLAYSTATE_RUNNING);
		DecTick();
		break;
	case PLAYSTATE_RUNNING:
		DecTick();
		if (GetTick() == GetNoteOffTick())
		{
			if (IsNote())
			{
				if (GetTieNoteNext() == NULL)
					NoteOff(
						GetSong()->GetCurrentLoudness()->GetLoudness()
					);
			}
			SetPlayState(PLAYSTATE_NOTE_OFF);
		}
		break;
	case PLAYSTATE_NOTE_OFF:
		DecTick();
		if (0 == GetTick())
		{
			if (!GetTieNoteNext())
				DeleteObject = PLAY_OBJECT_TIMED_OUT;
			SetPlayState(PLAYSTATE_DONE);
		}
		break;
	case PLAYSTATE_DONE:
		DeleteObject = PLAY_OBJECT_TIMED_OUT;
		break;
	}
	return DeleteObject;
}

int CMsNote::IsTimedObject()
{
	return GetDuration();
}

UINT CMsNote::Process()
{
	//----------------------------------------------
	// This method over rides the defalt method in
	// the base class CMsObject.
	//----------------------------------------------
	UINT AddToQueue = 0;
	int flag = 0, Velocity;
	bool loop;
	CMsNote* pNote = 0;
	//-------------------------------------------------
	// This gets complicated.  If the note has a TieEnd
	// flag set, we do NOT want to create a new EventMidi
	// object, rather, we just want to keep using
	// the one that is already there
	//--------------------------------------------------
	if (GetAccent())
		Velocity = 127;
	else
		Velocity = GetSong()->GetCurrentLoudness()->GetLoudness();
	SetVelocity(Velocity);
	//------------------------------------------
	// We need to deal with tied notes
	// First, find the first note in the
	// tie chain.
	//------------------------------------------
	pNote = GetTieNotePrev();
	if (pNote)
	{
		loop = true;
		while (pNote && loop)
		{
			if (pNote->GetTieNotePrev())
				pNote = pNote->GetTieNotePrev();
			else
				//---------------------------
				// we have found the start
				// of the Note Tie chain
				//---------------------------
				loop = false;
		}
		//-------------------------------------
		// pNote is now the note at the front
		// of the chain.  Add this note's 
		// duration to the front note
		//-------------------------------------
		pNote->SetTick(GetDuration(), 1);
	}
	else
	{
		//---------------------------------
		// Not a Tie Note, so add note
		// to the play queue
		//---------------------------------
		AddToQueue = 1;
		SetTick(0,1);
	}
	//-------------------------------------
	// Do we need to change the patch?
	//-------------------------------------
	if(GetSong()->CheckChan(GetTrack(), GetSong()->GetTrackInfo(GetTrack())->GetChannel()))
	{
		int Track = GetTrack();
		CMsTrack* pTrack = GetSong()->GetTrackInfo(Track);

		GetSong()->ChangePatch(
			Track,
			pTrack->GetChannel(),
			pTrack->GetPatch()
		);
	}
//	if(LogFile()) fprintf(LogFile(),"MsNote added %d objects\n",AddedToQueue);
	return AddToQueue;
}

bool CMsNote::RemoveFromQueue()
{
	bool rB = false;

	if ((GetTick() == 0) && !IsTieBeg())
		rB = true;
	else if ((GetTick() == 0) && IsTieBeg())
	{
		rB = false;
	}
	return rB;
}

void CMsNote::ObjectRectangle(CRect& rect)
{
	int noteVerticalPos;
	CPoint ptNoteHead;
	CSize szNoteHead;

	noteVerticalPos = NoteToPosition(GetPitch());
	szNoteHead = CSize(NOTE_HEAD_WIDTH, NOTE_HEAD_HEIGHT);
	if (IsHeadFlipped())
	{
		ptNoteHead = CPoint(
			NOTE_STEM_OFFSET,
			noteVerticalPos
		);
	}
	else
	{
		ptNoteHead = CPoint(
			NOTE_STEM_OFFSET - NOTE_HEAD_WIDTH,
			noteVerticalPos
		);
	}
	rect = CRect(ptNoteHead, szNoteHead);
}

UINT CMsNote::CorrectPitchWithKeySignature()
{
	return 0;
}

const char* CMsNote::GetNoteName(int note)
{
	static const char* NotDef = "UDF";
	const char* NoteName = 0;

	if(note < 0 || note > 127)
		return NotDef;
	if(GetSong()->GetCurrentKeySignature()->IsFlatKeySig())
		NoteName = NoteAnsiLUTFlat[note % 12];
	else
		NoteName = NoteAnsiLUTSharp[note % 12];
	return NoteName;
}


CMsNote* CMsNote::IsSecondInterval()
{
	// <summary>
	// This function is used to figure out if
	// the note that is about to be drawn is
	// a half step or a whole step away from
	// another note in the same event so that
	// one of the notge heads can be flipped
	// </summary>
	// <returns>A pointer to a note that is
	// One half or whole step from this note</returns>
	CMsNote* pWholeStep = 0;
	CMsEvent* pCurrentEvent = GetParentEvent();
	CMsObject* pCurrentSongObject;
	INT pitchDiff;
	UINT Loop = 1;
	
	if ((pCurrentSongObject = pCurrentEvent->ContainsObjectType(CMsObject::MsObjType::NOTE)) != NULL)
	{
		while (pCurrentSongObject && Loop)
		{
			if (pCurrentSongObject->GetType() == CMsObject::MsObjType::NOTE)
			{
				CMsNote* pN = (CMsNote*)pCurrentSongObject;
				pitchDiff = pN->GetPitch() - GetPitch();
				if ((pitchDiff <= 2) && (0 < pitchDiff))	//can these be a second?
				{
					pWholeStep = pN;
					Loop = 0;
				}
				else if ((pitchDiff >= -2) && (0 > pitchDiff))	//can these be a second?
				{
					pWholeStep = this;
					Loop = 0;
				}
				else
					pCurrentSongObject = pCurrentSongObject->GetNext();
			}
			else
			{
				pCurrentSongObject = pCurrentSongObject->GetNext();
			}
		}
	}
	return pWholeStep;
}

void NoteData::PrintData(FILE* pO, NoteData* nd, int Indent)
{
	CMsNote::DUR * pDur = CMsNote::GetDurationTable();
	char* pIndentString = new char[256];
	int l = 1024, ls = 0, Size;
	char* pS = new char[l];

	theApp.IndentString(pIndentString, 256, Indent);
	//fprintf_s(pO, "%s==================Note Data================\n", pIndentString);
	//fprintf_s(pO, "%sRest = %d\n", pIndentString, nd->m_Rest);
	//fprintf_s(pO, "%sTie End = %d\n", pIndentString, nd->m_TieEnd);
	//fprintf_s(pO, "%sAccent = %d\n", pIndentString, nd->m_Accent);
	//fprintf_s(pO, "%sStacato = %d\n", pIndentString, nd->m_Stacato);
	//fprintf_s(pO, "%sLegato = %d\n", pIndentString, nd->m_Legato);
	//fprintf_s(pO, "%sVelocty = %d\n", pIndentString, nd->m_Velocity);
	//fprintf_s(pO, "%sNote Off Tick Mark = %d\n", pIndentString, nd->m_NoteOffTick);
	//fprintf_s(pO, "%sStem Down = %d\n", pIndentString, nd->m_StemDown);
	//fprintf_s(pO, "%sHead Flipped = %d\n", pIndentString, nd->m_HeadFlipped);
	//fprintf_s(pO, "%sAccidental = %d\n", pIndentString, nd->m_Accidental);
	//fprintf_s(pO, "%sTrack = %d\n", pIndentString, nd->m_Track);
	//fprintf_s(pO, "%sDuration = %d\n", pIndentString, nd->m_Duration);
	//fprintf_s(pO, "%sDuration: %s\n", pIndentString, pDur[(int)nd->m_Duration].pName);
	if (pO) fprintf_s(pO, "%sPitch = %d\n", pIndentString, nd->m_Pitch);
	//fprintf_s(pO, "%sDotted = %d\n", pIndentString, nd->m_Dotted);
	//fprintf_s(pO, "%sTriplet = %d\n", pIndentString, nd->m_Triplet);
	//fprintf_s(pO, "%sMidi DeviceID = %d\n", pIndentString, nd->m_MidiOutID);
	//fprintf_s(pO, "%s=================END=================\n", pIndentString);

	Size = l - ls;
	if (m_Rest)
		ls += sprintf_s(&pS[ls], Size, "REST:%d", m_Pitch);
	else
		ls += sprintf_s(&pS[ls], Size, "NOTE:%d", m_Pitch);
	if ((m_Accent))
	{
		Size = l - ls;
		ls += sprintf_s(&pS[ls], Size, " Accent");
	}
	Size = l - ls;
	ls += sprintf_s(&pS[ls], Size, " Duration:%d", m_Duration);
	Size = l - ls;
	ls += sprintf_s(&pS[ls], Size, " Accidental:%d", m_Accidental);
	if (m_TieBeg)
	{
		Size = l - ls;
		ls += sprintf_s(&pS[ls], Size, " Tie Start");
	}
	if (m_TieEnd)
	{
		Size = l - ls;
		ls += sprintf_s(&pS[ls], Size, " Tie End");
	}
	Size = l - ls;
	ls += sprintf_s(&pS[ls], Size, " Track:%d", m_Track);
	if (m_StemDown)
	{
		Size = l - ls;
		ls += sprintf_s(&pS[ls], Size, " Stem Down");
	}
//	if(pO) fprintf_s(pO, "%s==================Note Data==========\n", pIndentString);
//	if(pO) fprintf_s(pO, "%  s%s\n", pIndentString, pS);
//	if(pO) fprintf_s(pO, "%s=================END=================\n", pIndentString);
	if(pS) delete[] pS;
	if(pIndentString) delete[] pIndentString;
}

int CMsNote::SetPitch(int Pitch)
{
	//----------------------------------------
	// SetPitch
	//		Sets the pitch of the note based
	//	on its physical location on the staff
	//----------------------------------------

//	fprintf(LogFile(), "\t\t\t\tCMsNote::SetPitch: Pitch=%d\n", Pitch);
	if(Pitch == 2)
	{
		printf("Huh?");	//DebugBreak();
	}
	GetData().SetPitch(Pitch);
	return Pitch;
}

int CMsNote::GetPitch()
{
	int Pitch = GetData().GetPitch();
	return Pitch;
}