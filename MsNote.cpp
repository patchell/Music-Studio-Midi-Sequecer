///////////////////////////////////////////////////////
// MsNote.cpp: implementation of the CMsNote class.
//
//////////////////////////////////////////////////////////////////////

#include "pch.h"

void PrintRecPtSz(const char* pTit, CRect rec)
{
//	printf("---------------rect---------------------\n");
//	printf("RECT : %s\n",pTit);
//	PrintPoint("Upper Left",rec.TopLeft());
//	PrintSize("Dimensions", rec.Size());
//	printf("---------------end----------------------\n");
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMsNote::CMsNote() :CMsObject()
{
	m_ObjType = MSOBJ_NOTE;
	m_BitmapFlag = false;
	m_Ticks = 0;
	m_NoteTieNext = 0; //pointer to note object is the next tied note
	m_NoteTiePrev = 0;	//pointer to previous note that is tied
	m_pRestBitmap = 0;
	m_NotePlayed = -1;
}


CMsNote::~CMsNote()
{
	
}

void CMsNote::LoadRestBitmap(int Selection)
{
	if (COMBO_REST_HALF < Selection)
	{
		if(GetRestBitmap()->GetSafeHandle())
			GetRestBitmap()->DeleteObject();
		GetRestBitmap()->LoadBitmapW(GETAPP->GetRestTypeID(Selection));
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
		printf("pSong is NULL in CMsNote::Create\n");
	return CMsObject::Create(pSong, pParentEvent);
}


CString NoteLUT[12] = {
	_T("C"),
	_T("C#"),
	_T("D"),
	_T("D#"),
	_T("E"),
	_T("F"),
	_T("F#"),
	_T("G"),
	_T("G#"),
	_T("A"),
	_T("A#"),
	_T("B")
};

int AccidentalLUT[4] = {
	' ',
	'N',
	'#',
	'b'
};

int NearestLineOdd[12] = {
	0,	//C  0
	-1,	//C# 1
	2,	//D	 2
	1,	//D# 3
	0,	//E  4
	-1,	//F  5
	1,	//F# 6
	0,	//G  7
	-1,	//G# 8
	-2,	//A  9
	1,	//A# 10
	0	//B  11
};

int NearestLineEven[12] = {
	-1,	//C		0
	1,	//C#	1
	0,	//D		2
	-1,	//D#	3
	1,	//E		4
	0,	//F		5
	-1,	//F#	6
	2,	//G		7
	1,	//G#	8
	0,	//A		9
	-1,	//A#	10
	1	//B		11
};

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
//	fprintf(pO, "%s***********************************\n", pIndentString);
	CMsObject::Print(pO, Indent+4);
	GetData().PrintData(pO, GetDataPointer(), Indent+4);
//	fprintf(pO, "%s***********************************\n", pIndentString);

	delete[] pIndentString;
}

UINT CMsNote::ObjectToString(CString& csString, UINT mode)
{
	int Note, Oct;

	Note = m_Data.GetPitch() % 12;
	Oct = m_Data.GetPitch() / 12;
	if (mode == 0)
		csString.Format(
			_T("NOTE:%4lS%d%c Track %d Sel %d"),
			NoteLUT[Note].GetString(),
			Oct,
			AccidentalLUT[GetAccidental()],
			GetTrack(),
			IsSelected()
		);
	else if (mode == 1)
	{
		csString.Format(
			_T("NOTE:%4lS%d%c Track:%2d Tie:%c:%c <%s> <%s>"),
			NoteLUT[Note].GetString(),
			Oct,
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


	NotePosition = NotePos[Note % 12];
	NotePosition += 28 * (7 - (Note / 12));
	NotePosition += (STAVE_OFFSET - 44);
	return NotePosition;
}

void CMsNote::Draw(CDC *pDC, int Event, int maxevent)
{
	CPoint End,Start;
	CPen pen,*oldpen;
	CBrush brush,*oldbrush;
	int NoteY;
	int Pitch;
	COLORREF Color;
	CRect rectRest;


	Color = GETMIDIINFO->GetTrack(GetTrack()).GetColor();
	if (IsSelected() || IsHighLighted())
		Color ^= 0x00ffffff;
	Pitch = GetPitch();
	NoteY = NoteToPosition(Pitch);

	if(IsRest())	///draw rest on staff
	{
		pen.CreatePen(PS_SOLID, 1, Color);
		oldpen = pDC->SelectObject(&pen);
		brush.CreateSolidBrush(Color);
		oldbrush = pDC->SelectObject(&brush);
		switch(GetDuration())
		{
		case MSFF_REST_QUARTER:
		case MSFF_REST_EIGTH:
		case MSFF_REST_SIXTEENTH:
		case MSFF_REST_THIRTYSECOND:
			DrawRestBitmap(pDC, Event, NoteY, Color);
			break;
		case MSFF_REST_HALF:
			NoteY += 4;
			rectRest.SetRect(
				EVENT_OFFSET+EVENT_WIDTH*Event,
				NoteY,
				EVENT_OFFSET+EVENT_WIDTH*Event+8,
				NoteY - 6);
			if (NeedsLine())
			{
				pDC->MoveTo(EVENT_OFFSET + EVENT_WIDTH * Event - 2, NoteY);
				pDC->LineTo(EVENT_OFFSET + EVENT_WIDTH * Event + 10, NoteY);
			}
			pDC->Rectangle(&rectRest);
			break;
		case MSFF_REST_WHOLE:
			//---------------------------------
			// The second parameter, the Y
			// Coordinate of t he CRect Upper
			// Left corner is the value of
			// notev + 2.  The rest needs to be
			// one pixel lower for the line,and
			// one pixel to make the rect butt
			// up against the line.
			//----------------------------------
			NoteY += 5;
			;
			rectRest.SetRect(
				EVENT_OFFSET + EVENT_WIDTH * Event,
				NoteY,
				EVENT_OFFSET + EVENT_WIDTH * Event + 8,
				NoteY + 6
			);
			rectRest.NormalizeRect();
			if (NeedsLine())
			{
				pDC->MoveTo(EVENT_OFFSET + EVENT_WIDTH * Event - 2, NoteY);
				pDC->LineTo(EVENT_OFFSET + EVENT_WIDTH * Event + 10, NoteY);
			}
			pDC->Rectangle(&rectRest);
			
			break;
		default:
//			printf("Opps Dur=%d\n", GetDuration());
			break;
		}
		pDC->SelectObject(oldpen);
		pDC->SelectObject(oldbrush);
	}
	else	//draw note on staff
	{
		DrawNote(pDC, Event, maxevent, NoteY, Color);
	}
}

void CMsNote::DrawNote(CDC* pDC, int Event, int MaxEvent, int NoteY, COLORREF Color)
{
	//		CMsNote* pSecInterval;
	DrawNoteHead(pDC, Event, NoteY, Color);
	DrawNoteLines(pDC, Event, NoteY, RGB(0,0,0));
	DrawNoteStem(pDC, Event, NoteY, Color);

	if (IsDotted())
	{
		DrawNoteDots(pDC, Event, NoteY, Color);
	}
	else if(IsTriplet())
	{
		DrawNoteTriplet(pDC, Event, NoteY, Color);
	}
	if (IsAccentted())
	{
		DrawNoteAccent(pDC, Event, NoteY, Color);
	}
	if (IsTieEnd())
	{
		DrawNoteTie(pDC, Event, NoteY, Color);
	}
	if(GetAccidental() != MSFF_ACCIDENTAL_INKEY)
	{
		DrawNoteAccidental(pDC, Event, NoteY, Color);
	}
}

void CMsNote::DrawNoteHead(
	CDC* pDC, 
	int Event, 
	int NoteY, 
	COLORREF Color
)
{
	CRect rectNoteHead;
	CPoint ptNoteHead;
	CSize szNoteHead;
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
	if (!IsHeadFlipped())
	{
		ptNoteHead = CPoint(
			EVENT_OFFSET + EVENT_WIDTH * Event + NOTE_LINE_OFFSET,
			NoteY
		);
		szNoteHead = CSize(
			-NOTE_HEAD_WIDTH, 
			NOTE_HEAD_HEIGHT
		);
		rectNoteHead = CRect(ptNoteHead, szNoteHead);
		pDC->Ellipse(&rectNoteHead);
	}
	else        //Note Head Flipped
	{
		//			printf("Normal Note Head\n");
		ptNoteHead = CPoint(
			EVENT_OFFSET + EVENT_WIDTH * Event + NOTE_LINE_OFFSET,
			NoteY
		);
		szNoteHead = CSize(
			NOTE_HEAD_WIDTH,
			NOTE_HEAD_HEIGHT
		);
		rectNoteHead = CRect(ptNoteHead, szNoteHead);
		pDC->Ellipse(&rectNoteHead);
	}
	pDC->SelectObject(oldPen);
	pDC->SelectObject(oldBrush);
}

void CMsNote::DrawNoteLines(CDC* pDC, int Event, int NoteY, COLORREF Color)
{
	int NeesALine = NeedsLine();
	int y = 0;
	int f = 0;
	int i;
	CPen penLines, *penOld = 0;


	NeesALine = NeedsLine();
	if (NeesALine)
	{
		penLines.CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
		penOld = pDC->SelectObject(&penLines);
		if (NeesALine < 0)
		{
			f = -1;
			NeesALine = -NeesALine;
		}
		else
			f = 1;
		for (i = 0; i < NeesALine; ++i)
		{
			int lineEnd = EVENT_WIDTH - (EVENT_WIDTH + 10) / 2;
			UINT x = EVENT_OFFSET + EVENT_WIDTH * Event + NOTE_LINE_OFFSET;
			if (IsOnLine())
				y = NoteY + i * 8 * f;
			else
				y = NoteY + 4 + i * 8 * f;
			pDC->MoveTo(x - lineEnd, y);
			pDC->LineTo(x + lineEnd, y);
		}
		pDC->SelectObject(penOld);
	}
}

void CMsNote::DrawNoteStem(CDC* pDC, int Event, int NoteY, COLORREF Color)
{
	UINT x = 0;
	CPen penStem, * oldPen;

	if (NeedsStem())
	{
		penStem.CreatePen(PS_SOLID, 1, Color);
		oldPen = pDC->SelectObject(&penStem);
		x = EVENT_OFFSET + EVENT_WIDTH * Event + NOTE_LINE_OFFSET;
		if (IsStemDown())
		{
			pDC->MoveTo(x, NoteY + 4);
			pDC->LineTo(x, NoteY + 28);
		}
		else
		{
			pDC->MoveTo(x, NoteY + 4);
			pDC->LineTo(x, NoteY - 20);
		}
		pDC->SelectObject(oldPen);
		DrawNoteFlags(pDC, Event, NoteY, Color);
	}
}

void CMsNote::DrawNoteFlags(CDC* pDC, int Event, int NoteY, COLORREF Color)
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
		x = EVENT_OFFSET + EVENT_WIDTH * Event + NOTE_LINE_OFFSET;
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

void CMsNote::DrawNoteAccidental(CDC* pDC, int Event, int NoteY, COLORREF Color)
{
	switch (GetAccidental())
	{
	case MSFF_ACCIDENTAL_NATURAL:
	{
		CMsNatural nat;
		nat.Draw(pDC, Color, EVENT_OFFSET + EVENT_WIDTH * Event - 8, NoteY);
	}
	break;
	case MSFF_ACCIDENTAL_SHARP:
	{
		CMsSharp sharp;
		sharp.Draw(pDC, Color, EVENT_OFFSET + EVENT_WIDTH * Event - 8, NoteY);
	}
	break;
	case MSFF_ACCIDENTAL_FLAT:
	{
		CMsFlat flat;
		flat.Draw(pDC, Color, EVENT_OFFSET + EVENT_WIDTH * Event - 8, NoteY);
	}
	break;
	}
}

void CMsNote::DrawNoteAccent(CDC* pDC, int Event, int NoteY, COLORREF Color)
{
	CPen penAccent, * oldPen;

	penAccent.CreatePen(PS_SOLID, 1, Color);
	oldPen = pDC->SelectObject(&penAccent);
	if (IsStemDown())
	{
		pDC->MoveTo(EVENT_OFFSET + EVENT_WIDTH * Event, NoteY - 4);
		pDC->LineTo(EVENT_OFFSET + EVENT_WIDTH * Event + 8, NoteY - 7);
		pDC->LineTo(EVENT_OFFSET + EVENT_WIDTH * Event, NoteY - 10);
	}
	else
	{
		pDC->MoveTo(EVENT_OFFSET + EVENT_WIDTH * Event, NoteY + 12);
		pDC->LineTo(EVENT_OFFSET + EVENT_WIDTH * Event + 8, NoteY + 15);
		pDC->LineTo(EVENT_OFFSET + EVENT_WIDTH * Event, NoteY + 18);
	}
	pDC->SelectObject(oldPen);
}

void CMsNote::DrawNoteDots(CDC* pDC, int Event, int NoteY, COLORREF Color)
{
	int y;

	if (IsOnLine())
		y = NoteY;
	else
		y = NoteY + 4;
	if (IsStemDown())
	{
		pDC->SetPixel(EVENT_OFFSET + EVENT_WIDTH * Event + 31, y + 3, Color);
		pDC->SetPixel(EVENT_OFFSET + EVENT_WIDTH * Event + 32, y + 3, Color);
		pDC->SetPixel(EVENT_OFFSET + EVENT_WIDTH * Event + 33, y + 3, Color);
		pDC->SetPixel(EVENT_OFFSET + EVENT_WIDTH * Event + 32, y + 4, Color);
		pDC->SetPixel(EVENT_OFFSET + EVENT_WIDTH * Event + 32, y + 2, Color);
	}
	else
	{
		pDC->SetPixel(EVENT_OFFSET + EVENT_WIDTH * Event + 23, y + 3, Color);
		pDC->SetPixel(EVENT_OFFSET + EVENT_WIDTH * Event + 24, y + 3, Color);
		pDC->SetPixel(EVENT_OFFSET + EVENT_WIDTH * Event + 22, y + 3, Color);
		pDC->SetPixel(EVENT_OFFSET + EVENT_WIDTH * Event + 23, y + 4, Color);
		pDC->SetPixel(EVENT_OFFSET + EVENT_WIDTH * Event + 23, y + 2, Color);
	}
}

void CMsNote::DrawNoteTriplet(CDC* pDC, int Event, int NoteY, COLORREF Color)
{
	int x;
	x = EVENT_OFFSET + EVENT_WIDTH * Event;

	pDC->SetPixel(x + 12, NoteY + 6, Color);
	pDC->SetPixel(x + 13, NoteY + 6, Color);
	pDC->SetPixel(x + 14, NoteY + 7, Color);
	pDC->SetPixel(x + 13, NoteY + 8, Color);
	pDC->SetPixel(x + 14, NoteY + 9, Color);
	pDC->SetPixel(x + 13, NoteY + 10, Color);
	pDC->SetPixel(x + 12, NoteY + 10, Color);
}

void CMsNote::DrawNoteTie(CDC* pDC, int Event, int NoteY, COLORREF Color)
{
	CMsNote* pN;
	CPoint Start, End;
	int widthEv;
	CRect rectTieCurve;
	int width;
	int x;

	pN = FindTieBegin();
	if (pN)
	{
		CMsEvent* pEv = pN->GetParentEvent();
		widthEv = GetParentEvent()->GetIndex() - pEv->GetIndex();
		width = EVENT_WIDTH * widthEv;
		x = 5 + EVENT_OFFSET + (Event - widthEv) * EVENT_WIDTH;
		rectTieCurve.SetRect(x, NoteY, x + width, NoteY + 16);
		if (IsStemDown())
		{
			End = CPoint(x + 1, NoteY - 8);
			Start = CPoint(x + width - 1, NoteY - 8);
		}
		else
		{
			Start = CPoint(x + 1, NoteY + 8);
			End = CPoint(x + width - 1, NoteY + 8);
		}
		pDC->Arc(rectTieCurve, Start, End);
	}
}

void CMsNote::DrawRestBitmap(CDC* pDC, int event, int notev, COLORREF color)
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
		x = EVENT_OFFSET + EVENT_WIDTH * event;
		y = notev;
		pDC->BitBlt(x, y, bmSize.cx, bmSize.cy, &dc, 0, 0, SRCAND);
		ChangeRestColor(&dc, color, bmSize.cx, bmSize.cy);
		pDC->BitBlt(x, y, bmSize.cx, bmSize.cy, &dc, 0, 0, SRCAND);
		dc.SelectObject(pOldBm);
	}

}

int CMsNote::NeedsLine()
{
	//-------------------------
	// Middle C is 0x3C
	//-------------------------
	int rV = 0;
	int pitch = GetPitch();

	if(pitch == 0x3c) rV = 1;
	else if(pitch <= 0x2A)
	{
		rV = (pitch - 0x2A)/2;
	}
	else if(pitch > 0x4f)
	{
		int notev = NoteToPosition(GetPitch());
		rV = (STAVE_OFFSET - notev)/8;
	}
	return rV;
}

bool CMsNote::IsTriplet()
{
	bool rV = false;
	if(DurTab[GetDuration()].Triplet) rV = true;
	return rV;
}

INT CMsNote::NeedsFlags(void)
{
	INT rV = 0;

	rV = DurTab[(int)GetDuration()].Flags;
	return rV;
}

CMsNote * CMsNote::FindTieBegin()
{
	CMsEvent *pEv = GetParentEvent();
	ObjectTypes pObj;
	CMsNote *pMsN = 0;

	int loop = 1;

	pEv = (CMsEvent *)pEv->GetPrev();
	while(pEv && loop)
	{
		pObj.pObj = pEv->GetEventObjectHead();
		while(pObj.pObj && loop)
		{
			switch(pObj.pObj->GetType())
			{
				case MSOBJ_NOTE:
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

	GetSong()->IncNoteOnCount();	// for diagnostics

	if(IsAccentted())
	{
		Loudness = 127;
	}
	else
		Loudness = Velocity;
	MidiChannel = GETMIDIINFO->GetChannel(GetTrack());
	NotePitch = GetPitch() + RangeLUT[GETMIDIINFO->GetRange(GetTrack())];
	//-------------------------------
	// Apply Key Signature Correction
	// or Accidental to correct pitch
	//-------------------------------
	NotePitch = GetSong()->GetCurrentKeySignature()->GetKeySigCorrection(NotePitch, GetAccidental());
	m_NotePlayed = NotePitch;
	DeviceID = GETMIDIINFO->GetMidiOutDeviceId(GetTrack());
	GETMIDIOUTTABLE.NoteOn(DeviceID, MidiChannel, NotePitch, Loudness);
}

void CMsNote::NoteOff(int Velociry)
{
	int MidiChannel;
	int DeviceID;

	GetSong()->IncNoteOffCount();	// for diagnostics

	MidiChannel = GETMIDIINFO->GetChannel(GetTrack());
	DeviceID = GETMIDIINFO->GetMidiOutDeviceId(GetTrack());
	GETMIDIOUTTABLE.NoteOff(DeviceID, MidiChannel, m_NotePlayed, Velociry);
//	printf("Notes ON %d  Notes Off %d\n", GetSong()->GetNoteOnCount(), GetSong()->GetNoteOffCount());
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
		printf("pSong is NULL in CMsNote::Copy\n");
	}
	SetRestBitmap(pN->GetRestBitmap());
	GetData().CopyData(pN->GetData());
	SetParentEvent(pN->GetParentEvent()->GetIndex());
	if (m_BitmapFlag)
		GetRestBitmap()->LoadBitmapW(CMidiSeqMSApp::GetRestBmIdsTypes()[pN->GetShape()]);
	CMsObject::Copy(pN);
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
//		m_P1 = m_P2 = pASV->m_SnapPos;
//		pASV->EnableAutoScroll(1);
//		DrawState = DRAWSTATE_PLACE;;
		GetStaffView()->Invalidate();
		break;
	case DRAWSTATE::PLACE:
		GetSong()->AddObjectToSong(GetStaffView()->GetDrawEvent(), this);
		{
			CMsNote* pN = new CMsNote;
			CMsNote* pNote = (CMsNote * )GetStaffView()->GetDrawObject();

			SetPitch(pNote->GetPitch());
			if (IsRest())
				pN->Create(CMidiSeqMSApp::GetRestBmIdsTypes()[pNote->GetShape()], GetSong(), GetSong()->GetEventObject(GetStaffView()->GetDrawEvent()));	// Create Rest
			else
				pN->Create(0, GetSong(), GetSong()->GetEventObject(GetStaffView()->GetDrawEvent()));	// Create Note
			//-----------------------------
			// Copy attributes
			//------------------------------
			pN->GetData().CopyData(GetStaffView()->GetNoteData());
			GetStaffView()->SetDrawObject(pN);
			GetStaffView()->CheckAndDoScroll(pointMouse);
		}
		DrawState = DRAWSTATE::WAITFORMOUSE_DOWN;
		GetStaffView()->Invalidate();
		break;
	}
	return DrawState;
}

DRAWSTATE CMsNote::MouseMove(DRAWSTATE DrawState, CPoint pointMouse)
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
	//
	//	Returns:
	//		Next Draw State
	//-------------------------------------------------------
	int Note = -1;
	int CorrectedNote;
	CString csStatus;

	CString csStatusString, csTemp;

	switch (DrawState)
	{
	case DRAWSTATE::SET_ATTRIBUTES:
		break;
	case DRAWSTATE::WAITFORMOUSE_DOWN:
		Note = GetStaffView()->YtoNote(pointMouse.y);
		if (Note != m_NotePlayed)
		{
			NoteOff(0);
			SetPitch(Note);
			NoteOn(GetSong()->GetCurrentLoudness()->GetLoudness());
		}
		csStatus.Format(_T("Note: %s%d  Track: %d  Event: %d"), 
			NoteLUT[GetPitch() % 12].GetString(),
			GetPitch() / 12,
			GetTrack(),
			GetSong()->GetStaffChildView()->GetDrawEvent()
		);
		GetSong()->GetStaffChildView()->GetStatusBar()->SetText(csStatus);
		break;
	case DRAWSTATE::MOVE_OBJECT_AROUND:
		break;
	case DRAWSTATE::PLACE:
		break;
	}
	GetStaffView()->Invalidate();
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
	return GETMIDIINFO->GetChannel(GetTrack());
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
			fprintf(GETAPP->LogFile(),"Tie Note Start\n");
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
	if(GetSong()->CheckChan(GetTrack(), GETMIDIINFO->GetChannel(GetTrack())))
	{
		GetSong()->ChangePatch(
			GETMIDIINFO->GetMidiOutDeviceId(GetTrack()),
			GETMIDIINFO->GetChannel(GetTrack()),
			GETMIDIINFO->GetPatch(GetTrack())
		);
	}
//	printf("MsNote added %d objects\n",AddedToQueue);
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

void CMsNote::ObjectRectangle(CRect& rect, UINT Event)
{
	UINT noteVerticalPos;
	noteVerticalPos = NoteToPosition(GetPitch());
	rect.SetRect(EVENT_OFFSET + EVENT_WIDTH * Event, noteVerticalPos, EVENT_OFFSET + EVENT_WIDTH * Event + 10, noteVerticalPos + 8);
}

UINT CMsNote::CorrectPitchWithKeySignature()
{
	return 0;
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
	
	if ((pCurrentSongObject = pCurrentEvent->ContainsObjectType(MSOBJ_NOTE)) != NULL)
	{
		while (pCurrentSongObject && Loop)
		{
			if (pCurrentSongObject->GetType() == MSOBJ_NOTE)
			{
				CMsNote* pN = (CMsNote*)pCurrentSongObject;
				pitchDiff = pN->GetPitch() - GetPitch();
				if ((pitchDiff <= 2) && (0 < pitchDiff))	//can these be a second?
				{
					pWholeStep = pN;
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
	//fprintf_s(pO, "%sPitch = %d\n", pIndentString, nd->m_Pitch);
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
	fprintf_s(pO, "%s==================Note Data==========\n", pIndentString);
	fprintf_s(pO, "%  s%s\n", pIndentString, pS);
	fprintf_s(pO, "%s=================END=================\n", pIndentString);
	delete[] pS;
	delete[] pIndentString;
}

int CMsNote::SetPitch(int NoteLocationOnStave)
{
	GetData().SetPitch(NoteLocationOnStave);
	m_NotePlayed = NoteLocationOnStave;
	m_NotePlayed += RangeLUT[GETMIDIINFO->GetRange(GetTrack())];
	m_NotePlayed = GetSong()->GetCurrentKeySignature()->GetKeySigCorrection(m_NotePlayed, GetAccidental());
	return m_NotePlayed;
}