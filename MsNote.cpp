///////////////////////////////////////////////////////
// MsNote.cpp: implementation of the CMsNote class.
//
//////////////////////////////////////////////////////////////////////

#include "pch.h"

void PrintRecPtSz(const char* pTit, CRect rec)
{
	printf("---------------rect---------------------\n");
	printf("RECT : %s\n",pTit);
	PrintPoint("Upper Left",rec.TopLeft());
	PrintSize("Dimensions", rec.Size());
	printf("---------------end----------------------\n");
}


INT CMsNote::AccedentalsLUT[APP_NUM_ACCIDENTALTYPES] = {
	MSFF_ACCIDENTAL_INKEY,
	MSFF_ACCIDENTAL_SHARP,
	MSFF_ACCIDENTAL_FLAT,
	MSFF_ACCIDENTAL_NATURAL
};

int CMsNote::NoteDurLut[APP_NUM_NOTETYPES] = {
	MSFF_WHOLE_NOTE,
	MSFF_HALF_NOTE,
	MSFF_QUARTER_NOTE,
	MSFF_EIGTH_NOTE,
	MSFF_SIXTEENTH_NOTE,
	MSFF_THIRTYSEC_NOTE
};

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMsNote::CMsNote() :CMsObject()
{
	m_ObjType = MSOBJ_NOTE;
	m_BitmapFlag = FALSE;
	m_Ticks = 0;
	m_NoteTieNext = 0; //pointer to note object is the next tied note
	m_NoteTiePrev = 0;	//pointer to previous note that is tied
}


CMsNote::~CMsNote()
{
	
}

void CMsNote::LoadRestBitmap(int Selection)
{
	if (COMBO_REST_HALF < Selection)
	{
		if(GetBitmap()->GetSafeHandle())
			GetBitmap()->DeleteObject();
		GetBitmap()->LoadBitmapW(GETAPP->GetRestTypeID(Selection));
		m_BitmapFlag = TRUE;
	}
	else
		m_BitmapFlag = FALSE;
}

void CMsNote::Create(int BitmapID, CMsSong* pSong, UINT ParentEvent)
{
	if (BitmapID)
	{
		m_RestBitmap.LoadBitmapW(BitmapID);
		m_BitmapFlag = TRUE;
	}
	else
		m_BitmapFlag = FALSE;
	CMsObject::Create(pSong, ParentEvent);
}

//----------------------------------
// Note Duration Data
// shape,dot,triplet,solid,flags,tail,durationtime
// Shape	=	0	->Whole Note
//			=	1	->Half Note
//			=	2	->Quarter Note
//			=	3	->Eigth Note
//			=	4	->Sixteenth Note
//			=	5	->Thirty Second Note
//			=	-1	->Does not apply
//
// Dot		=	True/False
// Triplet	=	True/False
// Solid	=	True/False Inside of note is solid
// flags	=	Number of flags on the note tail
// tail		=	True/False Needs a tail or stem
// DurationTime	= Clock ticks
//------------------------------------------
DUR DurTab[21] = {
	-1,0,0,1,0,0,0,"NA",				//0
	5,0,1,1,3,1,NOTE_TICKS_32nd_TRIPLET,"32nd Trip",			//1
	-1,0,0,1,0,0,0,"NA",				//2
	5,0,0,1,3,1,NOTE_TICKS_32nd,"32nd",		//3 thirty second
	4,0,1,1,2,1,NOTE_TICKS_16th_TRIPLET,"16th Trip",			//4
	5,1,0,1,3,1,NOTE_TICKS_32nd_DOTTED,"Dotted 32nd",			//5 Dotted Thirty Second
	4,0,0,1,2,1,NOTE_TICKS_16th,"16th",			//6 sixteenth
	3,0,1,1,1,1,NOTE_TICKS_8th_TRIPLET,"8th Trip",			//7
	4,1,0,1,2,1,NOTE_TICKS_16th_DOTTED,"Dotted 16th",	//8	Dotted Sixteenth
	3,0,0,1,1,1,NOTE_TICKS_8th,"Eighth",			//9 eighth
	2,0,1,1,0,1,NOTE_TICKS_QUARTER_TRIPLET,"1/4 Trip",			//10
	3,1,0,1,1,1,NOTE_TICKS_8th_DOTTED,"Dotted Eighth",		//11 Dotted Eighth
	2,0,0,1,0,1,NOTE_TICKS_QUARTER,"Quarter",			//12 quarter
	1,0,1,0,0,1,NOTE_TICKS_HALF_TRIPLET,"1/2 Trip",			//13
	2,1,0,1,0,1,NOTE_TICKS_QUARTER_DOTTED,"Dotted Quarter",	//14 Dotted Quarter Note
	1,0,0,0,0,1,NOTE_TICKS_HALF,"Half",				//15 half
	0,0,1,0,0,0,NOTE_TICKS_WHOLE_TRIPLET,"Whole Trip",		//16
	1,1,0,0,0,1,NOTE_TICKS_HALF_DOTTED,"Dptted Half",		//17 Dotted Halfe
	0,0,0,0,0,0,NOTE_TICKS_WHOLE,"Whole",			//18 whole
	-1,0,0,0,0,0,0,"NA",				//19
	0,1,0,0,0,0,NOTE_TICKS_WHOLE_DOTTED,"Dotted Whole"		//20 dotted whole
};

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


void CMsNote::Print(FILE *pO)
{
	fprintf(pO, "***********************************\n");
	CMsObject::Print(pO);
	GetData().PrintData(pO, GetDataPointer());
	fprintf(pO, "***********************************\n");

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

int NotePos[12] = {
	24,	//C
	24,	//C#
	20,	//D
	20,	//D#
	16,	//E
	12,	//F
	12,	//F#
	8,	//G
	8,	//G#
	4,	//A
	4,	//A#
	0	//B
};

int CMsNote::NoteToPosition()
{
	int Note = GetPitch();
	return NotePos[Note % 12] + 28 * (7 - (Note / 12)) + (STAVE_OFFSET - 44);
}

void CMsNote::Draw(CDC *pDC, int event, int maxevent)
{
	CPoint End,Start;
	CPen pen,*oldpen,redpen;
	CBrush brush,*oldbrush;
	COLORREF color;
	int i,f;
	int notev;
	int pitch;

	color = GETMIDIINFO->GetInstColor(GetTrack());
	if (IsSelected() || IsHighLighted())
		color ^= 0x00ffffff;
	redpen.CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
	pen.CreatePen(PS_SOLID,1,color);
	oldpen = pDC->SelectObject(&pen);

	if(IsSolid() || IsRest())
		brush.CreateSolidBrush(color);
	else
		brush.CreateSolidBrush(RGB(255,0,255));
	oldbrush = pDC->SelectObject(&brush);

	pitch = GetPitch();
	//--------------------------
	// the value of notev will
	// range from 24 to 60
	//------------------------
	CRect r;

	if(IsRest())	///draw rest on staff
	{
		SetPitch(pitch + NearestLine());
		notev = NoteToPosition();
		switch(GetDuration())
		{
		case MSFF_REST_QUARTER:
		case MSFF_REST_EIGTH:
		case MSFF_REST_SIXTEENTH:
		case MSFF_REST_THIRTYSECOND:
			DrawRestBitmap(pDC, event, notev, color);
			break;
		case MSFF_REST_HALF:
			notev += 4;
			r.SetRect(
				EVENT_OFFSET+EVENT_WIDTH*event,
				notev,
				EVENT_OFFSET+EVENT_WIDTH*event+8,
				notev - 6);
			if (NeedsLine())
			{
				pDC->MoveTo(EVENT_OFFSET + EVENT_WIDTH * event - 2, notev);
				pDC->LineTo(EVENT_OFFSET + EVENT_WIDTH * event + 10, notev);
			}
			pDC->Rectangle(&r);
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
			notev += 5;
			;
			r.SetRect(
				EVENT_OFFSET + EVENT_WIDTH * event,
				notev,
				EVENT_OFFSET + EVENT_WIDTH * event + 8,
				notev + 6
			);
			r.NormalizeRect();
			if (NeedsLine())
			{
				pDC->MoveTo(EVENT_OFFSET + EVENT_WIDTH * event - 2, notev);
				pDC->LineTo(EVENT_OFFSET + EVENT_WIDTH * event + 10, notev);
			}
			pDC->Rectangle(&r);
			
			break;
		default:
			printf("Opps Dur=%d\n", GetDuration());
			break;
		}
	}
	else	///draw note on staff
	{
		UINT rectX1, rectY1, rectX2, rectY2;
		CMsNote* pSecInterval;
		//---------------------------------------------
		// Draw Note Head
		//--------------------------------------------
		if ((pSecInterval = IsSecondInterval()) != NULL)
		{
			if (IsHeadFlipped())
				pSecInterval->SetHeadFlipped(0);
			else
				pSecInterval->SetHeadFlipped(1);
		}
		if (IsHeadFlipped())
		{
			notev = NoteToPosition();
			rectX1 = EVENT_OFFSET + EVENT_WIDTH * event + NOTE_LINE_OFFSET;
			rectY1 = notev;
			rectX2 = rectX1 + NOTE_HEAD_WIDTH;
			rectY2 = notev + NOTE_HEAD_HIEGTH;
			r.SetRect(rectX1, rectY1, rectX2, rectY2);
			pDC->Ellipse(&r);
		}
		else        //Note Head Normal
		{
			notev = NoteToPosition();
			rectX1 = EVENT_OFFSET + EVENT_WIDTH * event + NOTE_LINE_OFFSET - NOTE_HEAD_WIDTH;
			rectY1 = notev;
			rectX2 = rectX1 + NOTE_HEAD_WIDTH;
			rectY2 = notev + NOTE_HEAD_HIEGTH;
			r.SetRect(rectX1, rectY1, rectX2, rectY2);
			pDC->Ellipse(&r);
		}
		if(NeedsLine())
		{
			int y;
			int NeesALine = NeedsLine();
			if(NeesALine < 0)
			{
				f = -1;
				NeesALine = -NeesALine;
			}
			else
				f = 1;
			for(i=0;i< NeesALine;++i)
			{
				if(IsOnLine())
					y = notev+i*8*f;
				else
					y = notev+4+i*8*f;
				pDC->MoveTo(28 + EVENT_WIDTH*event,y);
				pDC->LineTo(44+EVENT_WIDTH*event,y);
			}
		}
		if(NeedsTail())
		{
			int n;
			UINT x = EVENT_OFFSET + EVENT_WIDTH * event + NOTE_LINE_OFFSET;
			if(IsUpsideDown())
			{
				pDC->MoveTo(x,notev+4);
				pDC->LineTo(x,notev+28);
			}
			else
			{
				pDC->MoveTo(x,notev+4);
				pDC->LineTo(x,notev-20);
			}
			if((n=NeedsFlags()))
			{
				for(i=0;i<n;++i)
				{
					if(GetUpsideDown())
					{
						pDC->MoveTo(EVENT_OFFSET+EVENT_WIDTH*event,notev+28-(i*5));
						pDC->LineTo(EVENT_OFFSET+EVENT_WIDTH*event+4,notev+24-(i*5));
					}
					else
					{
						pDC->MoveTo(EVENT_OFFSET+EVENT_WIDTH*event+9,notev-20+i*5);
						pDC->LineTo(EVENT_OFFSET+EVENT_WIDTH*event+13,notev-14+i*5);
					}
				}
			}
		}
		if(DurTab[GetDuration()].Dotted)
		{
			pDC->SetPixel(EVENT_OFFSET+EVENT_WIDTH*event+12,notev+6,color);
			pDC->SetPixel(EVENT_OFFSET+EVENT_WIDTH*event+13,notev+6,color);
			pDC->SetPixel(EVENT_OFFSET+EVENT_WIDTH*event+11,notev+6,color);
			pDC->SetPixel(EVENT_OFFSET+EVENT_WIDTH*event+12,notev+7,color);
			pDC->SetPixel(EVENT_OFFSET+EVENT_WIDTH*event+12,notev+5,color);
		}
		else if (DurTab[GetDuration()].Triplet)
		{
			int x;
			x = EVENT_OFFSET+EVENT_WIDTH*event;

			pDC->SetPixel(x+12,notev+6,color);
			pDC->SetPixel(x+13,notev+6,color);
			pDC->SetPixel(x+14,notev+7,color);
			pDC->SetPixel(x+13,notev+8,color);
			pDC->SetPixel(x+14,notev+9,color);
			pDC->SetPixel(x+13,notev+10,color);
			pDC->SetPixel(x+12,notev+10,color);
		}
		if(GetAccent())
		{
			if(IsUpsideDown())
			{
				pDC->MoveTo(EVENT_OFFSET+EVENT_WIDTH*event,notev-4);
				pDC->LineTo(EVENT_OFFSET+EVENT_WIDTH*event+8,notev-7);
				pDC->LineTo(EVENT_OFFSET+EVENT_WIDTH*event,notev-10);
			}
			else
			{
				pDC->MoveTo(EVENT_OFFSET+EVENT_WIDTH*event,notev+12);
				pDC->LineTo(EVENT_OFFSET+EVENT_WIDTH*event+8,notev+15);
				pDC->LineTo(EVENT_OFFSET+EVENT_WIDTH*event,notev+18);
			}
		}	//end of if(IsAccent())
		if(IsTieEnd())
		{
			CMsNote *pN;
			pN = FindTieBegin();
			if(pN)
			{
				CMsEvent *pEv = pN->GetParentEvent();
				CRect r;
				int widthEv = GetParentEvent()->GetIndex() - pEv->GetIndex();
				int width = EVENT_WIDTH * widthEv;
				int x = 5 + EVENT_OFFSET + (event-widthEv) * EVENT_WIDTH;
				r.SetRect(x,notev,x+width,notev+16);
				if(GetUpsideDown())
				{
					End = CPoint(x+1,notev-8);
					Start = CPoint (x+width-1,notev-8);
				}
				else
				{
					Start= CPoint (x+1,notev+8);
					End = CPoint (x+width-1,notev+8);
				}
				pDC->Arc(r,Start,End);
			}
		}
		switch(GetAccidental())
		{
			case MSFF_ACCIDENTAL_INKEY:
				break;
			case MSFF_ACCIDENTAL_NATURAL:
				{
					CMsNatural nat;
					nat.Draw(pDC,color,EVENT_OFFSET+EVENT_WIDTH*event-8,notev);
				}
				break;
			case MSFF_ACCIDENTAL_SHARP:
				{
					CMsSharp sharp;
					sharp.Draw(pDC,color,EVENT_OFFSET+EVENT_WIDTH*event-8,notev);
				}
				break;
			case MSFF_ACCIDENTAL_FLAT:
				{
					CMsFlat flat;
					flat.Draw(pDC,color,EVENT_OFFSET+EVENT_WIDTH*event-8,notev);
				}
				break;
		}
	}
	pDC->SelectObject(oldpen);
	pDC->SelectObject(oldbrush);
}

void CMsNote::DrawRestBitmap(CDC* pDC, int event, int notev, COLORREF color)
{
	CMyBitmap* pOldBm;
	int x, y;
	CDC dc;
	CSize bmSize;

	if (m_BitmapFlag)
	{
		dc.CreateCompatibleDC(pDC);
		bmSize = m_RestBitmap.GetBmDim();
		pOldBm = (CMyBitmap*)dc.SelectObject(&m_RestBitmap);
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
		int notev = NoteToPosition();
		rV = (STAVE_OFFSET - notev)/8;
	}
	return rV;
}

UINT OnLine[12] = {
	1,	///C
	1,	///C#
	0,	///D
	0,	///D#
	1,	///E
	0,	///F
	0,	///F#
	1,	///G
	1,	///G#
	0,	///A
	0,	///A#
	1	///B
};


bool CMsNote::IsTriplet()
{
	bool rV = false;
	if(DurTab[GetDuration()].Triplet) rV = true;
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

//------------------------------------
// Byte 0 Midi Status (NOTEON,NOTEOFF)
// Byte 1 Midi Note
// Byte 2 Velocity
//-------------------------------------
int RangeLUT[6] = { 0,-24,-12,0,12,24 };

void CMsNote::NoteOn(CMsKeySignature *pKS, UINT Velociry)
{
	int chan = GETMIDIINFO->GetChannel(GetTrack());
	int note = GetPitch() + RangeLUT[GETMIDIINFO->GetRange(GetTrack())];
	note = pKS->GetKeySigCorrection(note, GetAccidental());
	int DeviceID = GETMIDIINFO->GetMidiOutDeviceId(GetTrack());
	GETMIDIOUTTABLE.NoteOn(DeviceID, chan, note, Velociry);
}

void CMsNote::ChangePatch(int PatchNumber)
{
	int chan = GETMIDIINFO->GetChannel(GetTrack());
	int DeviceID = GETMIDIINFO->GetMidiOutDeviceId(GetTrack());
	GETMIDIOUTTABLE.PgmChange(DeviceID, chan, PatchNumber);
}

void CMsNote::NoteOff( CMsKeySignature *pKS, UINT Velociry)
{
	int chan = GETMIDIINFO->GetChannel(GetTrack());
	int note = GetPitch() + RangeLUT[GETMIDIINFO->GetRange(GetTrack())];
	note = pKS->GetKeySigCorrection(note, GetAccidental());
	int DeviceID = GETMIDIINFO->GetMidiOutDeviceId(GetTrack());
	GETMIDIOUTTABLE.NoteOff(DeviceID, chan, note, Velociry);
}

int NoteInc[12] = {
	2,	//C
	2,	//C#
	2,	//D
	2,	//D#
	1,	//E
	2,	//F
	2,	//F#
	2,	//g
	2,	//G#
	2,	//A
	2,	//A#
	1	//B
};

void CMsNote::IncrNote()
{
	int Pitch = GetPitch();
	int i = Pitch % 12;
	Pitch += NoteInc[i];
	SetPitch(Pitch);
}

int NoteDec[12] = {
	1,	//C
	1,	//C#
	2,	//D
	2,	//D#
	2,	//E
	1,	//F
	1,	//F#
	2,	//g
	2,	//G#
	2,	//A
	2,	//A#
	2	//B
};

void CMsNote::DecrNote()
{
	int Pitch = GetPitch();
	int i = Pitch % 12;
	Pitch -= NoteDec[i];
	SetPitch(Pitch);
}

CMsObject * CMsNote::Copy()
{
	CMsNote *pN = new CMsNote();

	pN->GetData().CopyData(GetData());
	pN->SetParentEvent(GetParentEvent()->GetIndex());

	if (m_BitmapFlag)
		pN->GetBitmap()->LoadBitmapW(CMidiSeqMSApp::RestBmIdsTypes[GetShape()]);
	return pN;
}

int CMsNote::MouseLButtonDown(int DrawState, CPoint pointMouse)
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
	case CChildViewStaff::DRAWSTATE_SET_ATTRIBUTES:
		break;
	case CChildViewStaff::DRAWSTATE_WAITFORMOUSE_DOWN:
		break;
	case CChildViewStaff::DRAWSTATE_PLACE:
		break;
	}
	GetStaffView()->Invalidate();
	return DrawState;
}

int CMsNote::MouseLButtonUp(int DrawState, CPoint pointMouse)
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
	case CChildViewStaff::DRAWSTATE_SET_ATTRIBUTES:
		csText.Format(_T("Configure Note Prameters"));
		GetStaffView()->GetStatusBar()->SetText(csText);
		Dlg.SetNoteToEdit(this);
		if ((Id = Dlg.DoModal()) == IDOK)
		{
			DrawState = CChildViewStaff::DRAWSTATE_WAITFORMOUSE_DOWN;
			GetStaffView()->Invalidate();
		}
		break;
	case CChildViewStaff::DRAWSTATE_WAITFORMOUSE_DOWN:
//		m_P1 = m_P2 = pASV->m_SnapPos;
//		pASV->EnableAutoScroll(1);
//		DrawState = DRAWSTATE_PLACE;;
		GetStaffView()->Invalidate();
		break;
	case CChildViewStaff::DRAWSTATE_PLACE:
		GetSong()->AddObjectToSong(GetStaffView()->GetDrawEvent(), this);
		{
			CMsNote* pN = new CMsNote;
			CMsNote* pNote = (CMsNote * )GetStaffView()->GetDrawObject();

			SetPitch(pNote->GetPitch());
			if (IsRest())
				pN->Create(CMidiSeqMSApp::RestBmIdsTypes[pNote->GetShape()], GetSong(), GetStaffView()->GetDrawEvent());	// Create Rest
			else
				pN->Create(0, GetSong(), GetStaffView()->GetDrawEvent());	// Create Note
			pNote->SetParentEvent(GetStaffView()->GetDrawEvent());
			//-----------------------------
			// Copy attributes
			//------------------------------
			pN->GetData().CopyData(GetStaffView()->GetNoteData());
			GetStaffView()->SetDrawObject(pN);
			GetStaffView()->CheckAndDoScroll(pointMouse);
		}
		DrawState = CChildViewStaff::DRAWSTATE_WAITFORMOUSE_DOWN;
		GetStaffView()->Invalidate();
		break;
	}
	return DrawState;
}

int CMsNote::MouseMove(int DrawState, CPoint pointMouse)
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
	INT note = -1;
	CString csStatusString, csTemp;

	switch (DrawState)
	{
	case CChildViewStaff::DRAWSTATE_SET_ATTRIBUTES:
		break;
	case CChildViewStaff::DRAWSTATE_WAITFORMOUSE_DOWN:
		note = GetStaffView()->YtoNote(pointMouse.y);

//		pNote = (CMsNote*)GetStaffView()->GetDrawObject();
//		printf("@@@@@@@@@@@@ LasrPitch = %d  Note = %d @@@@@@@@@@@\n", GetLastPitch(), note);
		if (GetStaffView()->GetLastPitch())
		{
			if (GetStaffView()->GetLastPitch() != note)
			{
				SetPitch(note);
				if (!IsRest())
				{
					printf("    Note Off %d\n", GetPitch());
					GetStaffView()->MidiPlayNote(this, 0);// Note Off
				}
				SetPitch(note);
				if (!IsRest())
				{
					printf("    Note On %d\n", GetPitch());
					GetStaffView()->MidiPlayNote(this, 1);// Note On
				}
				GetStaffView()->SetLastPitch(note);
				if (IsRest())
				{
					csStatusString.Format(_T("Draw Rest at Event %d"), GetStaffView()->GetDrawEvent());
				}
				else
				{
					ObjectToString(csTemp);
					csStatusString.Format(_T("%lS at Event %d: "), csTemp.GetString(), GetStaffView()->GetDrawEvent());
				}
			}
		}
		else if (!GetStaffView()->LastPitchIsValid() && GetStaffView()->IsMouseInEditRegion())
		{
			SetPitch(note);
			if (!IsRest())
				GetStaffView()->MidiPlayNote(this, 1);// Note On
			GetStaffView()->SetLastPitch(note);
		}
		GetStaffView()->Invalidate();
		break;
	case CChildViewStaff::DRAWSTATE_MOVE_OBJECT_AROUND:
		break;
	case CChildViewStaff::DRAWSTATE_PLACE:
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
	if(GetUpsideDown()) Byte3 |= MSFF_NOTE_UPSIDE_DOWN;
	fputc(Byte1,pO);
	fputc(Byte2,pO);
	fputc(Byte3,pO);
}

bool CMsNote::IsDotted()
{
	return DurTab[GetDuration()].Dotted?true:false;
}

void CMsNote::SetTick(int Duration, int tempo)
{
	///----------------------------
	/// Set the number of ticks for
	/// the ticker.  This is the
	/// product of the duration
	/// and the tempo
	///----------------------------
	m_Ticks = DurTab[Duration].DurTime * tempo;
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
	UINT DeleteObject = 0;

//	GetData().PrintData(stdout,  GetDataPointer());
	if (GetTick() == 0)	//Object just added
	{
		SetTick(GetDuration(), 1);
		//-----------------------------------
		// Change the pitch according to the
		// Key Signature and or acidentals
		//-----------------------------------
		NoteOn(GetSong()->GetCurrentKeySignature(),GetSong()->GetCurrentLoudness()->GetLoudness());
	}
	else if (GetTick() == GetNoteOffTick())
	{
		if (IsNote() && (GetTieNoteNext() == NULL))
			NoteOff(
				GetSong()->GetCurrentKeySignature(), 
				GetSong()->GetCurrentLoudness()->GetLoudness()
			);
		DecTick();
	}
	else if (0 == DecTick())
	{
		if (!GetTieNoteNext())
			DeleteObject = 1;
	}
	return DeleteObject;
}

// ???
BOOL CMsNote::AddToQueue()
{
	return TRUE;
}

UINT CMsNote::Process()
{
	//----------------------------------------------
	// This method over rides the defalt method in
	// the base class CMsObject.
	//----------------------------------------------
	UINT NoteAddedToQueue = 0;
	int flag = 0, Velocity;
	BOOL loop;
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
		loop = TRUE;
		while (pNote && loop)
		{
			if (pNote->GetTieNotePrev())
				pNote = pNote->GetTieNotePrev();
			else
				//---------------------------
				// we have found the start
				// of the Note Tie chain
				//---------------------------
				loop = FALSE;
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
		GetSong()->AddObjectToPlayerQueue(this);
		NoteAddedToQueue = 1;
		SetTick(0,1);
	}
	//-------------------------------------
	// Do we need to change the patch?
	//-------------------------------------
	if(GetSong()->CheckChan(GetTrack(), GETMIDIINFO->GetChannel(GetTrack())))
	{
		ChangePatch(
			GETMIDIINFO->GetPatch(GetTrack())
		);
	}
//	printf("MsNote added %d objects\n",NoteAddedToQueue);
	return NoteAddedToQueue;
}

BOOL CMsNote::RemoveFromQueue()
{
	BOOL rB = FALSE;

	if ((GetTick() == 0) && !IsTieBeg())
		rB = TRUE;
	else if ((GetTick() == 0) && IsTieBeg())
	{
		rB = FALSE;
	}
	return rB;
}

void CMsNote::ObjectRectangle(CRect& rect, UINT Event)
{
	UINT noteVerticalPos;
	noteVerticalPos = NoteToPosition();
	rect.SetRect(EVENT_OFFSET + EVENT_WIDTH * Event, noteVerticalPos, EVENT_OFFSET + EVENT_WIDTH * Event + 10, noteVerticalPos + 8);
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

