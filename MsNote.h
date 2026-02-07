// MsNote.h: interface for the CMsNote class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

constexpr auto NOTE_C4 = 60;	// MIDI note number for middle C
constexpr auto NOTE_C2 = 36;	// MIDI note number for lowest note on staff
constexpr auto NOTE_C6 = 84;	// MIDI note number for highest note on staff
constexpr auto NOTE_C0 = 12;	// MIDI note number for C0

//-------------------------------
// Constants for drawing notes
// on the music staff.
//-------------------------------

constexpr auto NOTE_STEM_OFFSET = EVENT_WIDTH / 2 + EVENT_WIDTH / 4;
constexpr auto NOTE_HEAD_WIDTH = 10;
constexpr auto NOTE_HEAD_HEIGHT = 8;
constexpr auto NOTE_HEAD_RECT_P1_X = NOTE_STEM_OFFSET - NOTE_HEAD_WIDTH - 2;
constexpr auto NOTE_HEAD_RECT_P1_Y = -NOTE_HEAD_HEIGHT / 2;
constexpr auto NOTE_FLIPPED_HEAD_RECT_P1_X = NOTE_STEM_OFFSET;
constexpr auto NOTE_FLIPPED_HEAD_RECT_P1_Y = -NOTE_HEAD_HEIGHT / 2;
constexpr auto NOTE_ACCIDENTAL_OFFSET = 8;


constexpr auto HALF_REST_WIDTH = 8;
constexpr auto HALF_REST_HEIGHT = 6;

class CChildViewStaff;

extern UINT OnLine[12];

enum class NoteStem : int {
	NoStem,
	Stem
};

enum class NoteFlags : int {
	FLAG_QUARTER,
	FLAG_8th,
	FLAG_16th,
	FLAG_32nd
};



//--------------------------------
// Note Tick Definitions arranged
// from shortest to longest
//--------------------------------
constexpr auto NOTE_TICKS_32nd_TRIPLET = 4;
constexpr auto NOTE_TICKS_32nd = 6;
constexpr auto NOTE_TICKS_32nd_DOTTED = 9;
constexpr auto NOTE_TICKS_16th_TRIPLET = 8;
constexpr auto NOTE_TICKS_16th = 12;
constexpr auto NOTE_TICKS_16th_DOTTED = 18;
constexpr auto NOTE_TICKS_8th_TRIPLET = 16;
constexpr auto NOTE_TICKS_8th = 24;
constexpr auto NOTE_TICKS_8th_DOTTED = 36;
constexpr auto NOTE_TICKS_QUARTER_TRIPLET = 32;
constexpr auto NOTE_TICKS_QUARTER = 48;
constexpr auto NOTE_TICKS_QUARTER_DOTTED = 72;
constexpr auto NOTE_TICKS_HALF_TRIPLET = 64;
constexpr auto NOTE_TICKS_HALF = 96;
constexpr auto NOTE_TICKS_HALF_DOTTED = 144;
constexpr auto NOTE_TICKS_WHOLE_TRIPLET = 128;
constexpr auto NOTE_TICKS_WHOLE = 192;
constexpr auto NOTE_TICKS_WHOLE_DOTTED = 288;


constexpr auto MSFF_ACCIDENTAL_INKEY = 0;
constexpr auto MSFF_ACCIDENTAL_NATURAL = 1;
constexpr auto MSFF_ACCIDENTAL_SHARP = 2;
constexpr auto MSFF_ACCIDENTAL_FLAT = 3;


class NoteData {

	UINT m_Rest;	//Note 0 - Rest 1
	UINT m_TieBeg;	// begining of a tied note
	UINT m_TieEnd;	// end of a tied note
	bool m_Accent;	// accent note
	bool m_Staccato;	// stacato note
	bool m_Legato;	// legato note
	bool m_StemDown;	// note stem points down
	UINT m_HeadFlipped;	// note head points to the right
	UINT m_Velocity;
	UINT m_NoteOffTick;		// Number of ticks for NOTEOFF
	INT m_Accidental;		// sharp, flat or natural
	UINT m_Dotted;
	INT m_Track;		//value is 1->15, 0 is not used
	INT m_Duration;		// Token for the note duration
	UINT m_Pitch;		// Pitch of note to use for note
	INT m_FlagsOff;		// flags for eigth, sixteenth, etc. not not drawn
	INT m_Flags;		// Number of flags to draw on note
	UINT m_Triplet;
	int m_MidiOutID;	// Identidy of the MIDI output device
public:
	NoteData() {
		m_MidiOutID = 0;
		m_Rest = 0;
		m_TieBeg = 0;
		m_TieEnd = 0;
		m_Accent = 0;
		m_Staccato = 0;
		m_Legato = 0;
		m_Velocity = 100;
		m_NoteOffTick = 2;		// Number of ticks for NOTEOFF
		m_StemDown = 0;		// Default note points up
		m_HeadFlipped = 0;		// Default note head not flipped
		m_Accidental = MSFF_ACCIDENTAL_INKEY;
		m_Track = COMBO_Index_INST_7;
		m_Duration = COMBO_NOTE_QUARTER;
		m_Pitch = 0;
		m_Dotted = 0;
		m_Triplet = 0;
		m_FlagsOff = 0;	// default is  not flags OFF (Flags ON)
		m_Flags = 0;	// number of flags to draw on note
	}
	virtual ~NoteData() {}
	void CopyData(NoteData& source) {
		//--------------------------------
		// CopyData
		//
		// Copies data from the source
		// note data into this notes
		// note data structure
		//
		// parameter:
		//	source....note data to copy from
		//-----------------------------------
		m_MidiOutID = source.GetMidiOutID();
		m_Rest = source.GetRest();
		m_TieBeg = source.GetTieBeg();
		m_TieEnd = source.GetTieEnd();
		m_Accent = source.GetAccent();
		m_Staccato = source.GetStaccato();
		m_Legato = source.GetLegato();
		m_Velocity = source.GetVelocity();
		m_NoteOffTick = source.GetNoteOffTick();		// Number of ticks for NOTEOFF
		m_StemDown = source.GetStemDown();
		m_HeadFlipped = source.GetHeadFlipped();
		m_Accidental = source.GetAccidental();
		m_Track = source.GetTrack();
		m_Duration = source.GetDuration();
		m_Pitch = source.GetPitch();
		m_Dotted = source.GetDotted();
		m_FlagsOff = 0;	//default is  not flags OFF (Flags ON)
		m_Flags = source.GetFlags();
		m_Triplet = source.GetTriplet();
	}

	void SetFlags(INT v) { m_Flags = v; }
	INT GetFlags() const { return m_Flags; }	

	void SetFlagsOff(INT v) { m_FlagsOff = v; }
	INT GetFlagsOff() const { return m_FlagsOff; }	

	void SetMidiOutID(int  MId) { m_MidiOutID = 0; }
	int GetMidiOutID() { return m_MidiOutID; }

	void SetRest(int v) {m_Rest = v;}
	int GetRest() { return m_Rest; }
	bool IsRest() { return m_Rest; }
	bool IsNote() { return !m_Rest; }

	void SetTieBeg(UINT v) { m_TieBeg = v; }
	UINT GetTieBeg() { return m_TieBeg; }

	void SetTieEnd(int v) { m_TieEnd = v; }
	int GetTieEnd() { return m_TieEnd; }

	void SetAccent(int v) { m_Accent = v; }
	int GetAccent() { return m_Accent; }

	void SetStaccato(int v) { m_Staccato = v; }
	int GetStaccato() const { return m_Staccato; }

	void SetLegato(int v) { m_Legato = v; }
	int GetLegato() { return m_Legato; }

	void SetVelocity(UINT v) {
		m_Velocity = v;
	}
	UINT GetVelocity() {
		return m_Velocity;
	}

	void SetNoteOffTick(UINT Tick) {
		m_NoteOffTick = Tick;
	}
	UINT GetNoteOffTick() {
		return m_NoteOffTick;
	}

	void SetStemDown(bool v) { m_StemDown = v; }
	 
	bool GetStemDown() const { return m_StemDown; }
	bool IsStemDown() { return m_StemDown; }

	void SetHeadFlipped(UINT HdFlipped) { 
		m_HeadFlipped = HdFlipped; 
	}
	UINT GetHeadFlipped() { return m_HeadFlipped; }
	bool IsHeadFlipped() { return m_HeadFlipped; }

	void SetAccidental(INT v) { m_Accidental = v; }
	INT GetAccidental() const {return m_Accidental;}

	void SetTrack(INT v) { 
		m_Track = v; 
	}
	INT GetTrack() { 
		return m_Track;
	}

	void SetDuration(INT v) {m_Duration = v;}
	INT GetDuration() { return m_Duration; }

	void SetPitch(int v) { 
		m_Pitch = v; 
	}
	int GetPitch() { return m_Pitch; }

	void SetDotted(int d) { m_Dotted = d; }
	int GetDotted() { return m_Dotted; }

	void SetTriplet(int t) { m_Triplet = t; }
	int GetTriplet() { return m_Triplet; }

	void PrintData(FILE* pO, NoteData* nd, int Indent);
};

class CMsNote : public CMsObject
{
	enum class ExtraLinesLocation: int {
		NotNeeded,
		AboveTreble,
		BelowBass,
		MiddleC,
	};
	struct NeedsLinesItem {
		int m_Lines;
		ExtraLinesLocation m_Location;
		NeedsLinesItem() {
			m_Lines = 0;
			m_Location = ExtraLinesLocation::NotNeeded;
		}
		NeedsLinesItem(int Lines, ExtraLinesLocation Location) {
			m_Lines = Lines;
			m_Location = Location;
		}
	};
public:

	inline static int NoteAdjustLUT[12] = {
		0,
		1,
		0,
		1,
		0,
		0,
		1,
		0,
		1,
		0,
		1,
		0
	};

	//------------------------------------
	// Lookup table to convert note to
	// semitone offset from C
	//------------------------------------
	inline static int NoteLut[7] = {
		0,	//C
		2,	//D
		4,	//E
		5,	//F
		7,	//G
		9,	//A
		11	//B
	};
	enum class NoteShape : int {
		DNA = -1,
		WHOLE,
		HALF,
		QUARTER,
		EIGHTH,
		SIXTEENTH,
		THIRTYSECOND
	};
	//------------------------------------
	// Byte 0 Midi Status (NOTEON,NOTEOFF)
	// Byte 1 Midi Note
	// Byte 2 Velocity
	//-------------------------------------
	inline static int RangeLUT[6] = { 
		0,
		-24,
		-12,
		0,
		12,
		24 
	};

	struct DUR {
		NoteShape NoteShapeIndex;
		UINT Dotted;
		UINT Triplet;
		UINT Solid;	// Note circle open or solid
		NoteFlags Flags;
		NoteStem Stem;	// Stem direction
		UINT NoteDurClockTicks;	// clock ticks
		const char* pName;
		int DurationTimeTicks() const { return NoteDurClockTicks; }
	};
private:
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
	// Stem		=	True/False Needs a stem
	// DurationTime	= Clock ticks
	//------------------------------------------
	inline static DUR DurTab[21] = {
		{ NoteShape::DNA,0,0,1,NoteFlags(0),NoteStem(0),0,"NA"},				//0
		{ NoteShape::THIRTYSECOND,	0,1,1,	NoteFlags::FLAG_32nd,NoteStem::Stem,NOTE_TICKS_32nd_TRIPLET,"32nd Trip"},			//1
		{ NoteShape::DNA,0,0,1,NoteFlags(0),NoteStem(0),0,"NA"},				//2
		{ NoteShape::THIRTYSECOND,	0,0,1,	NoteFlags::FLAG_32nd,NoteStem::Stem,NOTE_TICKS_32nd,"32nd"},		//3 thirty second
		{ NoteShape::SIXTEENTH,		0,1,1,	NoteFlags::FLAG_16th,NoteStem::Stem,NOTE_TICKS_16th_TRIPLET,"16th Trip"},			//4
		{ NoteShape::THIRTYSECOND,	1,0,1,	NoteFlags::FLAG_32nd,NoteStem::Stem,NOTE_TICKS_32nd_DOTTED,"Dotted 32nd"},			//5 Dotted Thirty Second
		{ NoteShape::SIXTEENTH,		0,0,1,	NoteFlags::FLAG_16th,NoteStem::Stem,NOTE_TICKS_16th,"16th"},			//6 sixteenth
		{ NoteShape::EIGHTH,		0,1,1,	NoteFlags::FLAG_8th,NoteStem::Stem,NOTE_TICKS_8th_TRIPLET,"8th Trip"},			//7
		{ NoteShape::SIXTEENTH,		1,0,1,	NoteFlags::FLAG_16th,NoteStem::Stem,NOTE_TICKS_16th_DOTTED,"Dotted Sixteenth"},		//8	Dotted Sixteenth
		{ NoteShape::EIGHTH,		0,0,1,	NoteFlags::FLAG_8th,NoteStem::Stem,NOTE_TICKS_8th,"Eighth"},			//9 eighth
		{ NoteShape::QUARTER,		0,1,1,	NoteFlags::FLAG_QUARTER,NoteStem::Stem,NOTE_TICKS_QUARTER_TRIPLET,"1/4 Trip"},			//10
		{ NoteShape::EIGHTH,		1,0,1,	NoteFlags::FLAG_8th,NoteStem::Stem,NOTE_TICKS_8th_DOTTED,"Dotted Eighth"},		//11 Dotted Eighth
		{ NoteShape::QUARTER,		0,0,1,	NoteFlags::FLAG_QUARTER,NoteStem::Stem,NOTE_TICKS_QUARTER,"Quarter"},			//12 quarter
		{ NoteShape::HALF,			0,1,0,	NoteFlags::FLAG_QUARTER,NoteStem::Stem,NOTE_TICKS_HALF_TRIPLET,"1/2 Trip"},			//13
		{ NoteShape::QUARTER,		1,0,1,	NoteFlags::FLAG_QUARTER,NoteStem::Stem,NOTE_TICKS_QUARTER_DOTTED,"Dotted Quarter"},	//14 Dotted Quarter Note
		{ NoteShape::HALF,			0,0,0,	NoteFlags::FLAG_QUARTER,NoteStem::Stem,NOTE_TICKS_HALF,"Half"},				//15 half
		{ NoteShape::WHOLE,			0,1,0,	NoteFlags::FLAG_QUARTER,NoteStem::Stem,NOTE_TICKS_WHOLE_TRIPLET,"Whole Trip"},		//16
		{ NoteShape::HALF,			1,0,0,	NoteFlags::FLAG_QUARTER,NoteStem::Stem,NOTE_TICKS_HALF_DOTTED,"Dotted Half"},		//17 Dotted Half
		{ NoteShape::WHOLE,			0,0,0,	NoteFlags::FLAG_QUARTER,NoteStem::Stem,NOTE_TICKS_WHOLE,"Whole"},			//18 whole
		{ NoteShape::DNA,0,0,0,NoteFlags(0),NoteStem(0),0,"NA"},				//19
		{ NoteShape::WHOLE,			1,0,0,	NoteFlags::FLAG_QUARTER,NoteStem::NoStem,NOTE_TICKS_WHOLE_DOTTED,"Dotted Whole"}		//20 dotted whole
	};
	//-------------------------------
	StaffMouseStates m_MouseStaffTransition;
	CMyBitmap* m_pRestBitmap;
	bool m_BitmapFlag;
	NoteData m_Data;
	int m_NotePlayed;	// Note that was used in NoteON
	//-------------------------------------
	// Note data that concerns playing song
	//--------------------------------------
	CMsNote* m_NoteTieNext; //pointer to note object is the next tied note
	CMsNote* m_NoteTiePrev;	//pointer to previous note that is tied
	int m_Ticks;            // timer ticks
public:
	CMsNote();
	virtual ~CMsNote();
	bool Create(int nBitmapID, CMsSong* pSong, CMsEvent* pParentEvent);
	//-------------------------------------------------
	// Pure Virtual Methods
	//-------------------------------------------------
	virtual UINT Process();
	virtual UINT Play();
	virtual DRAWSTATE MouseLButtonDown(DRAWSTATE DrawState, CPoint pointMouse);
	virtual DRAWSTATE MouseLButtonUp(DRAWSTATE DrawState, CPoint pointMouse);
	virtual DRAWSTATE MouseMove(DRAWSTATE DrawState, CPoint pointMouse, MouseRegions Region, MouseRegionTransitionState Transition);
	virtual int IsTimedObject();
	virtual bool DoesSomething() {
		return false;
	}
	virtual void Draw(CDC* pDC);
	UINT ObjectToString(CString& csString, UINT mode = 0);
	virtual StaffMouseStates StaffTransition(CPoint pointMouse, int NewNote, CMsEvent* pEvent);
	//------------------------------------------------------
	char* NoteToString(char* pStr, int l);
	static char* NoteToString(char* pStr, int l, int Note, CMsSong* pSong);

	void LoadRestBitmap(int Selection);

	//------ Note/Rest Drawing Utility Functions -----------------
	void DrawNote(
		CDC* pDC, 
		int NoteY, 
		COLORREF Color
	);
	void DrawNoteHead(
		CDC* pDC, 
		int NoteY, 
		COLORREF 
		Color
	);
	void DrawNoteLines(
		CDC* pDC, 
		int NoteY, 
		COLORREF 
		Color
	);
	void DrawNoteStem(
		CDC* pDC, 
		int NoteY, 
		COLORREF 
		Color
	);
	void DrawNoteFlags(
		CDC* pDC, 
		int NoteY, 
		COLORREF 
		Color
	);
	void DrawNoteAccidental(
		CDC* pDC, 
		int NoteY, 
		COLORREF 
		Color
	);
	void DrawNoteAccent(
		CDC* pDC, 
		int NoteY, 
		COLORREF Color
	);
	void DrawNoteDots(
		CDC* pDC, 
		int NoteY, 
		COLORREF Color
	);
	void DrawNoteTriplet(
		CDC* pDC, 
		int NoteY, 
		COLORREF Color
	);
	void DrawNoteTie(
		CDC* pDC, 
		int NoteY, 
		COLORREF Color
	);

	void DrawRest(
		CDC* pDC, 
		int NoteY, 
		COLORREF Color
	);

	void DrawRestBitmap(
		CDC* pDC, 
		int notev, 
		COLORREF color
	);

	void DrawHalfRest(
		CDC* pDC, 
		int NoteY, 
		COLORREF Color
	);

	void DrawWholeRest(
		CDC* pDC, 
		int NoteY, 
		COLORREF Color
	);

	//-------------------------------
	virtual void Print(FILE* pO, int Indent);
	virtual void Save(FILE* pO);
	virtual void Copy(CMsNote* pNote);
	int MouseYToNote(int MouseY);
	static int QuantizeY(int Note) { return Note / 4; }
	//----------- Draw Functions ----------------------

	void SetNoteOffTick(UINT NOT) { GetData().SetNoteOffTick(NOT); }
	UINT GetNoteOffTick() { return GetData().GetNoteOffTick(); }

	void DecrNote(void);
	void IncrNote(void);

	INT GetFlags() { return GetData().GetFlags(); }
	void SetFlags(INT v) { GetData().SetFlags(v); }

	INT GetFlagsOff() { return GetData().GetFlagsOff(); }
	void SetFlagsOff(INT v) { GetData().SetFlagsOff(v); }

	bool IsNote() { return GetData().IsNote(); }
	UINT IsRest(void) { return GetData().IsRest(); }
	void SetRest(UINT v) { GetData().SetRest(v); }

	UINT IsTieBeg() { return GetData().GetTieBeg(); }
	void SetTieBeg(UINT TB) { GetData().SetTieBeg(TB); }

	UINT IsTieEnd(void) { return GetData().GetTieEnd(); }
	void SetTieEnd(UINT v) { GetData().SetTieEnd(v); }

	bool IsAccentted() { return GetData().GetAccent(); }
	UINT GetAccent(void) { return GetData().GetAccent(); }
	void SetAccent(UINT v) { GetData().SetAccent(v); }

	UINT IsStaccato(void) { return GetData().GetStaccato(); }
	void SetStaccato(UINT v) { GetData().SetStaccato(v); }

	UINT IsLegato(void) { return GetData().GetLegato(); }
	void SetLegato(UINT v) { GetData().SetLegato(v); }

	INT GetAccidental(void) { return GetData().GetAccidental(); }
	void SetAccidental(INT k) { GetData().SetAccidental(k); }

	INT GetTrack(void) { 
		int Track = GetData().GetTrack();
		return Track; 
	}
	void SetTrack(INT t) { GetData().SetTrack(t); }

	INT GetDuration(void) { return GetData().GetDuration(); }
	void SetDuration(INT d) { GetData().SetDuration(d); }

	int GetPitch(void);
	int SetPitch(int p);
	bool IsStemDown(void) { return GetData().IsStemDown(); }
	void SetStemDown(bool u) { GetData().SetStemDown(u); }
	bool GetStemDown(void) { return GetData().GetStemDown(); }

	void SetHeadFlipped(UINT HdFlipped) { GetData().SetHeadFlipped(HdFlipped); }
	UINT GetHeadFlipped() { return GetData().GetHeadFlipped(); }
	bool IsHeadFlipped() { return GetData().IsHeadFlipped();}

	void SetDotted(int d) { GetData().SetDotted(d); }
	int GetDotted() { return GetData().GetDotted(); }

	int GetShape() { return  (int)DurTab[(int)GetDuration()].NoteShapeIndex; }

	int NearestLine();
	int IsOnLine() {
		int rV;
		int oct = GetPitch() / 12;
		int noteonline = OnLine[GetPitch() % 12];
		if (oct & 1) noteonline ^= 1;
		rV = noteonline ? 1 : 0;
		return rV;
	}
	//--------------------------------------
	// returns true for various attributes
	//-------------------------------------
	CMsNote* IsSecondInterval( );
	bool IsDotted() {
		return DurTab[GetDuration()].Dotted ? true : false;
	};
	bool IsSolid(void) { return DurTab[(int)GetDuration()].Solid ? true : false; }
	bool IsTriplet();
	bool IsDuplicate();
	int NeedsFlags(void);
	bool NeedsStem(void) { return DurTab[(int)GetDuration()].Stem == NoteStem::Stem ? true : false; }
	int NeedsLine(void);
	//----------------
	// Send to Midi
	//---------------
	void NoteOff(int Velocity);
	void NoteOn(int Velocity);
	//---------- Graphic methodes ------------
	void ChangeRestColor(CDC* pDC, COLORREF c, int w, int h);
	int HalfRestOffset();
	int WholeRestOffset(int pitch);
	int NoteToPosition(int Note);
	CMyBitmap* GetRestBitmap() { return m_pRestBitmap; }
	void SetRestBitmap(CMyBitmap* pBM) { m_pRestBitmap = pBM; }
	//------------ Note Tie Stuff ----------------------
	CMsNote* FindTieBegin();

	void SetTieNoteNext(CMsNote* pMN) { m_NoteTieNext = pMN; }
	CMsNote* GetTieNoteNext() { return m_NoteTieNext; }

	void SetTieNotePrev(CMsNote* pMN) { m_NoteTiePrev = pMN; }
	CMsNote* GetTieNotePrev() { return m_NoteTiePrev; }
	//-------------------------------
	NoteData& GetData() { return m_Data; }
	NoteData* GetDataPointer() { return &m_Data; }
	//-----------------------------------
	// Methodes that concern playing song
	//------------------------------------
	void SetVelocity(UINT V) { GetData().SetVelocity(V); }
	UINT GetVelocity(void) { return GetData().GetVelocity(); }
	void SetTick(int Duration, int tempo);
	int GetTick(void) { return m_Ticks; }
	int DecTick(void) { return --m_Ticks; }
	int GetChannel(void);
	//---------- Process and Play ------------
	virtual bool RemoveFromQueue();
	//-------------------------------------------
	virtual void ObjectRectangle(CRect& rect);
	static DUR* GetDurationTable() { return DurTab; }
	UINT CorrectPitchWithKeySignature();
	//---------------------------------
	// accidental encoding
	//---------------------------------
	inline static INT AccedentalsLUT[APP_NUM_ACCIDENTALTYPES] = {
		MSFF_ACCIDENTAL_INKEY,
		MSFF_ACCIDENTAL_SHARP,
		MSFF_ACCIDENTAL_FLAT,
		MSFF_ACCIDENTAL_NATURAL
	};

	inline static int NoteDurLut[APP_NUM_NOTETYPES] = {
		MSFF_WHOLE_NOTE,
		MSFF_HALF_NOTE,
		MSFF_QUARTER_NOTE,
		MSFF_EIGTH_NOTE,
		MSFF_SIXTEENTH_NOTE,
		MSFF_THIRTYSEC_NOTE
	};

	inline static int NoteInc[12] = {
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

	inline static int NoteDec[12] = {
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
	inline static int NotePos[12] = {
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
	inline static UINT OnLine[12] = {
		1,	//C
		1,	//C#
		0,	//D
		0,	//D#
		1,	//E
		0,	//F
		0,	//F#
		1,	//G
		1,	//G#
		0,	//A
		0,	//A#
		1	//B
	};
	inline static CString NoteLUT[12] = {
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

	inline static const char* NoteAnsiLUTSharp[12] = {
		"C",
		"C#",
		"D",
		"D#",
		"E",
		"F",
		"F#",
		"G",
		"G#",
		"A",
		"A#",
		"B"
	};

	inline static const char* NoteAnsiLUTFlat[12] = {
	"C",
	"Db",
	"D",
	"Eb",
	"E",
	"F",
	"Gb",
	"G",
	"Ab",
	"A",
	"Bb",
	"B"
	};

	inline static int AccidentalLUT[4] = {
		' ',
		'N',
		'#',
		'b'
	};

	inline static int NearestLineOdd[12] = {
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

	inline static int NearestLineEven[12] = {
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

	inline static NeedsLinesItem LinesNeededLUT[49] = {
		{2, ExtraLinesLocation::BelowBass}, //C3
		{2, ExtraLinesLocation::BelowBass},	//C#3
		{1, ExtraLinesLocation::BelowBass},	//D3
		{1, ExtraLinesLocation::BelowBass},	//D#3
		{0, ExtraLinesLocation::NotNeeded},	//E3
		{0, ExtraLinesLocation::NotNeeded},	//F3
		{0, ExtraLinesLocation::NotNeeded},	//F#3
		{0, ExtraLinesLocation::NotNeeded},	//G3
		{0, ExtraLinesLocation::NotNeeded},	//G#3
		{0, ExtraLinesLocation::NotNeeded},	//A3
		{0, ExtraLinesLocation::NotNeeded},	//A#3
		{0, ExtraLinesLocation::NotNeeded},	//B3
		{0, ExtraLinesLocation::NotNeeded},	//C4
		{0, ExtraLinesLocation::NotNeeded},	//C#4
		{0, ExtraLinesLocation::NotNeeded},	//D4
		{0, ExtraLinesLocation::NotNeeded},	//D#4
		{0, ExtraLinesLocation::NotNeeded},	//E4
		{0, ExtraLinesLocation::NotNeeded},	//F4
		{0, ExtraLinesLocation::NotNeeded},	//F#4
		{0, ExtraLinesLocation::NotNeeded},	//G4
		{0, ExtraLinesLocation::NotNeeded},	//G#4
		{0, ExtraLinesLocation::NotNeeded},	//A4
		{0, ExtraLinesLocation::NotNeeded},	//A#4
		{0, ExtraLinesLocation::NotNeeded},	//B4
		{1, ExtraLinesLocation::MiddleC},	//C5
		{0, ExtraLinesLocation::NotNeeded},	//C#5
		{0, ExtraLinesLocation::NotNeeded},	//D5
		{0, ExtraLinesLocation::NotNeeded},	//D#5
		{0, ExtraLinesLocation::NotNeeded},	//E5
		{0, ExtraLinesLocation::NotNeeded},	//F5
		{0, ExtraLinesLocation::NotNeeded},	//F#5
		{0, ExtraLinesLocation::NotNeeded},	//G5
		{0, ExtraLinesLocation::NotNeeded},	//G#5
		{0, ExtraLinesLocation::NotNeeded},	//A5
		{0, ExtraLinesLocation::NotNeeded},  //A#5
		{0, ExtraLinesLocation::NotNeeded},  //B5
		{0, ExtraLinesLocation::NotNeeded},  //C6
		{0, ExtraLinesLocation::NotNeeded},  //C#6
		{0, ExtraLinesLocation::NotNeeded},  //D6
		{0, ExtraLinesLocation::NotNeeded},  //D#6
		{0, ExtraLinesLocation::NotNeeded},  //E6
		{0, ExtraLinesLocation::NotNeeded},  //F6
		{0, ExtraLinesLocation::NotNeeded},  //F#6
		{0, ExtraLinesLocation::NotNeeded},  //G6
		{0, ExtraLinesLocation::NotNeeded},  //G#6
		{1, ExtraLinesLocation::AboveTreble},  //A6
		{1, ExtraLinesLocation::AboveTreble},  //A#6
		{1, ExtraLinesLocation::AboveTreble},  //B6,
		{2, ExtraLinesLocation::AboveTreble}	//C7
	};
	//-------------------------------
	const char* GetNoteName(); 
	static const char* GetNoteName(int note, CMsSong* pSong);
};

