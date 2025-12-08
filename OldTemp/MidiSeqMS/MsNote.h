// MsNote.h: interface for the CMsNote class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

extern UINT OnLine[12];

constexpr auto NOTE_SHAPE_WHOLE = 0;
constexpr auto NOTE_SHAPE_HALF = 1;
constexpr auto NOTE_SHAPE_QUARTER = 2;
constexpr auto NOTE_SHAPE_EIGHTH = 3;
constexpr auto NOTE_SHAPE_SIXTEENTH = 4;
constexpr auto NOTE_SHAPE_THIRTYSECOND = 5;

constexpr auto NOTE_TAIL = 1;
constexpr auto NOTE_NOTAIL = 0;

constexpr auto NOTE_FLAGS_0 = 0;
constexpr auto NOTE_FLAGS_1 = 1;
constexpr auto NOTE_FLAGS_2 = 2;
constexpr auto NOTE_FLAGS_3 = 3;

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
	bool m_Stacato;	// stacato note
	bool m_Legato;	// legato note
	bool m_StemDown;	// note stem points down
	UINT m_HeadFlipped;	// note head points to the right
	UINT m_Velocity;
	UINT m_NoteOffTick;		// Number of ticks for NOTEOFF
	INT m_Accidental;		// sharp, flat or natural
	UINT m_Dotted;
	INT m_Track;		//value is 1->15, 0 is not used
	INT m_Duration;
	UINT m_Pitch;		// Location on the music staff
	UINT m_CorrectedPitch; // Pitch after applying key signature, etc
	INT m_FlagsOff;		// flags for eigth, sixteenth, etc. not not drawn
	INT m_Flags;		// Number of flags to draw on note
	UINT m_Triplet;
	int m_MidiOutID;
public:
	NoteData() {
		m_MidiOutID = 0;
		m_Rest = 0;
		m_TieBeg = 0;
		m_TieEnd = 0;
		m_Accent = 0;
		m_Stacato = 0;
		m_Legato = 0;
		m_Velocity = 100;
		m_NoteOffTick = 2;		// Number of ticks for NOTEOFF
		m_StemDown = 0;		// Default note points up
		m_HeadFlipped = 0;		// Default note head not flipped
		m_Accidental = MSFF_ACCIDENTAL_INKEY;
		m_Track = COMBO_Index_INST_7;
		m_Duration = COMBO_NOTE_QUARTER;
		m_Pitch = 0;
		m_CorrectedPitch = 0;
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
		m_Stacato = source.GetStacato();
		m_Legato = source.GetLegato();
		m_Velocity = source.GetVelocity();
		m_NoteOffTick = source.GetNoteOffTick();		// Number of ticks for NOTEOFF
		m_StemDown = source.GetStemDown();
		m_HeadFlipped = source.GetHeadFlipped();
		m_Accidental = source.GetAccidental();
		m_Track = source.GetTrack();
		m_Duration = source.GetDuration();
		m_Pitch = source.GetPitch();
		m_CorrectedPitch = source.GetCorrectedPitch();	
		m_Dotted = source.GetDotted();
		m_FlagsOff = 0;	//default is  not flags OFF (Flags ON)
		m_Flags = source.GetFlags();
		m_Triplet = source.GetTriplet();
	}
	UINT GetCorrectedPitch() const { return m_CorrectedPitch; }
	void SetCorrectedPitch(UINT v) { m_CorrectedPitch = v; }

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

	void SetStacato(int v) { m_Stacato = v; }
	int GetStacato() { return m_Stacato; }

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

	void SetTrack(INT v) { m_Track = v; }
	INT GetTrack() { return m_Track;}

	void SetDuration(INT v) {m_Duration = v;}
	INT GetDuration() { return m_Duration; }

	void SetPitch(int v) { m_Pitch = v; }
	int GetPitch() { return m_Pitch; }

	void SetDotted(int d) { m_Dotted = d; }
	int GetDotted() { return m_Dotted; }

	void SetTriplet(int t) { m_Triplet = t; }
	int GetTriplet() { return m_Triplet; }

	void PrintData(FILE* pO, NoteData* nd, int Indent);
};

class CMsNote : public CMsObject
{
public:
	struct DUR {
		UINT NoteShapeIndex;
		UINT Dotted;
		UINT Triplet;
		UINT Solid;	// Note circle open or solid
		UINT Flags;
		UINT Tail;
		UINT DurTime;	// clock ticks
		const char* pName;
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
	// tail		=	True/False Needs a tail or stem
	// DurationTime	= Clock ticks
	//------------------------------------------
	inline static DUR DurTab[21] = {
		{ -1,0,0,1,0,0,0,"NA"},				//0
		{ NOTE_SHAPE_THIRTYSECOND,	0,1,1,	NOTE_FLAGS_3,NOTE_TAIL,NOTE_TICKS_32nd_TRIPLET,"32nd Trip"},			//1
		{ -1,0,0,1,0,0,0,"NA"},				//2
		{ NOTE_SHAPE_THIRTYSECOND,	0,0,1,	NOTE_FLAGS_3,1,NOTE_TICKS_32nd,"32nd"},		//3 thirty second
		{ NOTE_SHAPE_SIXTEENTH,		0,1,1,	NOTE_FLAGS_2,1,NOTE_TICKS_16th_TRIPLET,"16th Trip"},			//4
		{ NOTE_SHAPE_THIRTYSECOND,	1,0,1,	NOTE_FLAGS_3,1,NOTE_TICKS_32nd_DOTTED,"Dotted 32nd"},			//5 Dotted Thirty Second
		{ NOTE_SHAPE_SIXTEENTH,		0,0,1,	NOTE_FLAGS_2,1,NOTE_TICKS_16th,"16th"},			//6 sixteenth
		{ NOTE_SHAPE_EIGHTH,		0,1,1,	NOTE_FLAGS_1,1,NOTE_TICKS_8th_TRIPLET,"8th Trip"},			//7
		{ NOTE_SHAPE_SIXTEENTH,		1,0,1,	NOTE_FLAGS_2,1,NOTE_TICKS_16th_DOTTED,"Dotted 16th"},		//8	Dotted Sixteenth
		{ NOTE_SHAPE_EIGHTH,		0,0,1,	NOTE_FLAGS_1,1,NOTE_TICKS_8th,"Eighth"},			//9 eighth
		{ NOTE_SHAPE_QUARTER,		0,1,1,	NOTE_FLAGS_0,1,NOTE_TICKS_QUARTER_TRIPLET,"1/4 Trip"},			//10
		{ NOTE_SHAPE_EIGHTH,		1,0,1,	NOTE_FLAGS_1,1,NOTE_TICKS_8th_DOTTED,"Dotted Eighth"},		//11 Dotted Eighth
		{ NOTE_SHAPE_QUARTER,		0,0,1,	NOTE_FLAGS_0,1,NOTE_TICKS_QUARTER,"Quarter"},			//12 quarter
		{ NOTE_SHAPE_HALF,			0,1,0,	NOTE_FLAGS_0,1,NOTE_TICKS_HALF_TRIPLET,"1/2 Trip"},			//13
		{ NOTE_SHAPE_QUARTER,		1,0,1,	NOTE_FLAGS_0,1,NOTE_TICKS_QUARTER_DOTTED,"Dotted Quarter"},	//14 Dotted Quarter Note
		{ NOTE_SHAPE_HALF,			0,0,0,	NOTE_FLAGS_0,1,NOTE_TICKS_HALF,"Half"},				//15 half
		{ NOTE_SHAPE_WHOLE,			0,1,0,	NOTE_FLAGS_0,0,NOTE_TICKS_WHOLE_TRIPLET,"Whole Trip"},		//16
		{ NOTE_SHAPE_HALF,			1,0,0,	NOTE_FLAGS_0,1,NOTE_TICKS_HALF_DOTTED,"Dotted Half"},		//17 Dotted Half
		{ NOTE_SHAPE_WHOLE,			0,0,0,	NOTE_FLAGS_0,0,NOTE_TICKS_WHOLE,"Whole"},			//18 whole
		{ -1,0,0,0,0,0,0,"NA"},				//19
		{ NOTE_SHAPE_WHOLE,			1,0,0,	NOTE_FLAGS_0,0,NOTE_TICKS_WHOLE_DOTTED,"Dotted Whole"}		//20 dotted whole
	};
	//-------------------------------
	CMyBitmap m_RestBitmap;
	bool m_BitmapFlag;
	NoteData m_Data;
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
	virtual bool IsTimedObject() {
		return true;
	};
	virtual bool DoesSomething() {
		return false;
	}
	//------------------------------------------------------
	void LoadRestBitmap(int Selection);
	virtual void Draw(CDC* pDC, int event, int maxevent);
	void DrawRestBitmap(CDC* pDC, int event, int notev, COLORREF color);
	virtual void Print(FILE* pO, int Indent);
	virtual void Save(FILE* pO);
	virtual CMsObject* Copy(void);
	//----------- Draw Funcrions ----------------------
	virtual int MouseLButtonDown(int DrawState,CPoint pointMouse);
	virtual int MouseLButtonUp(int DrawState, CPoint pointMouse);
	virtual int MouseMove(int DrawState, CPoint pointMouse);
	CChildViewStaff* GetStaffView() { return  GetSong()->GetStaffChildView(); }

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

	UINT IsStacato(void) { return GetData().GetStacato(); }
	void SetStacato(UINT v) { GetData().SetStacato(v); }

	UINT IsLegato(void) { return GetData().GetLegato(); }
	void SetLegato(UINT v) { GetData().SetLegato(v); }

	INT GetAccidental(void) { return GetData().GetAccidental(); }
	void SetAccidental(INT k) { GetData().SetAccidental(k); }

	INT GetTrack(void) { return GetData().GetTrack(); }
	void SetTrack(INT t) { GetData().SetTrack(t); }

	INT GetDuration(void) { return GetData().GetDuration(); }
	void SetDuration(INT d) { GetData().SetDuration(d); }

	int GetPitch(void) { return GetData().GetPitch(); }
	void SetPitch(int p) { GetData().SetPitch(p); }

	bool IsStemDown(void) { return GetData().IsStemDown(); }
	void SetStemDown(bool u) { GetData().SetStemDown(u); }
	bool GetStemDown(void) { return GetData().GetStemDown(); }

	void SetHeadFlipped(UINT HdFlipped) { GetData().SetHeadFlipped(HdFlipped); }
	UINT GetHeadFlipped() { return GetData().GetHeadFlipped(); }
	bool IsHeadFlipped() { return GetData().IsHeadFlipped();}

	void SetDotted(int d) { GetData().SetDotted(d); }
	int GetDotted() { return GetData().GetDotted(); }

	int GetMidiOutID(int Track) { return GETMIDIINFO->GetMidiOutDeviceId(Track); }
	int GetShape() { return  DurTab[(int)GetDuration()].NoteShapeIndex; }

	int NearestLine();
	int IsOnLine() {
		int rV;
		int oct = GetPitch() / 12;
		int noteonline = OnLine[GetPitch() % 12];
		if (oct & 1) noteonline ^= 1;
		rV = noteonline ? 1 : 0;
		return rV;
	}
	//---------------------------------------------------
	// This function converts the note value to a string
	//--------------------------------------------------
	UINT ObjectToString(CString& csString, UINT mode = 0);
	//--------------------------------------
	// returns true for various attributes
	//-------------------------------------
	CMsNote* IsSecondInterval( );
	bool IsDotted();
	bool IsSolid(void) { return DurTab[(int)GetDuration()].Solid ? true : false; }
	bool IsTriplet();
	INT NeedsFlags(void);
	bool NeedsTail(void) { return DurTab[(int)GetDuration()].Tail ? true : false; }
	int NeedsLine(void);
	//----------------
	// Send to Midi
	//---------------
	void NoteOff(CMsKeySignature* pKS, UINT Velociry);
	void NoteOn(CMsKeySignature* pKS, UINT Velociry);
	void ChangePatch(int PatchNumber);
	//---------- Graphic methodes ------------
	void ChangeRestColor(CDC* pDC, COLORREF c, int w, int h);
	int HalfRestOffset();
	int WholeRestOffset(int pitch);
	int NoteToPosition();
	CMyBitmap* GetBitmap() { return &m_RestBitmap; }
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
	virtual void ObjectRectangle(CRect& rect, UINT Event);
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
};

