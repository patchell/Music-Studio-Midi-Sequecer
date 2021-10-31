// MsNote.h: interface for the CMsNote class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

extern DUR DurTab[21];
extern UINT OnLine[12];

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


class NoteData {
	UINT m_Rest;	//Note 0 - Rest 1
	UINT m_TieBeg;
	UINT m_TieEnd;
	UINT m_Accent;
	UINT m_Stacato;
	UINT m_Legato;
	UINT m_UpsideDown;
	UINT m_HeadFlipped;
	UINT m_Velocity;
	UINT m_NoteOffTick;		// Number of ticks for NOTEOFF
	UINT m_Accidental;
	UINT m_Dotted;
	UINT m_Track;	//value is 1->15, 0 is not used
	UINT m_Duration;
	UINT m_Pitch;
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
		m_UpsideDown = 0;
		m_HeadFlipped = 0;
		m_Accidental = 0;
		m_Track = COMBO_INST_7;
		m_Duration = COMBO_NOTE_QUARTER;
		m_Pitch = 0;
		m_Dotted = 0;
		m_Triplet = 0;
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
		m_UpsideDown = source.GetUpsideDown();
		m_HeadFlipped = source.GetHeadFlipped();
		m_Accidental = source.GetAccidental();
		m_Track = source.GetTrack();
		m_Duration = source.GetDuration();
		m_Pitch = source.GetPitch();
		m_Dotted = source.GetDotted();
		m_Triplet = source.GetTriplet();
	}
	void SetMidiOutID(int  MId) { m_MidiOutID = 0; }
	int GetMidiOutID() { return m_MidiOutID; }

	void SetRest(int v) {m_Rest = v;}
	int GetRest() { return m_Rest; }

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

	void SetUpsideDown(int v) { m_UpsideDown = v; }
	int GetUpsideDown() { return m_UpsideDown; }

	void SetHeadFlipped(UINT HdFlipped) { m_HeadFlipped = HdFlipped; }
	UINT GetHeadFlipped() { return m_HeadFlipped; }

	void SetAccidental(int v) { m_Accidental = v; }
	int GetAccidental(){return m_Accidental;}

	void SetTrack(int v) { m_Track = v; }
	int GetTrack() { return m_Track;}

	void SetDuration(int v) {m_Duration = v;}
	int GetDuration() { return m_Duration; }

	void SetPitch(int v) { m_Pitch = v; }
	int GetPitch() { return m_Pitch; }

	void SetDotted(int d) { m_Dotted = d; }
	int GetDotted() { return m_Dotted; }

	void SetTriplet(int t) { m_Triplet = t; }
	int GetTriplet() { return m_Triplet; }

	void PrintData(FILE* pO, NoteData* nd)
	{
		fprintf_s(pO, "==================Note Data================\n");
		fprintf_s(pO,"Rest = %d\n",nd->m_Rest);
		fprintf_s(pO, "Tie End = %d\n", nd->m_TieEnd);
		fprintf_s(pO, "Accent = %d\n", nd->m_Accent);
		fprintf_s(pO, "Stacato = %d\n", nd->m_Stacato);
		fprintf_s(pO, "Legato = %d\n", nd->m_Legato);
		fprintf_s(pO, "Velocty = %d\n", nd->m_Velocity);
		fprintf_s(pO, "Note Off Tick Mark = %d\n", nd->m_NoteOffTick);
		fprintf_s(pO, "Upside Down = %d\n", nd->m_UpsideDown);
		fprintf_s(pO, "Head Flipped = %d\n", nd->m_HeadFlipped);
		fprintf_s(pO, "Accidental = %d\n", nd->m_Accidental);
		fprintf_s(pO, "Track = %d\n", nd->m_Track);
		fprintf_s(pO, "Duration = %d\n", nd->m_Duration);
		fprintf_s(pO, "Duration: %s\n", DurTab[nd->m_Duration].pName);
		fprintf_s(pO, "Pitch = %d\n", nd->m_Pitch);;
		fprintf_s(pO, "Dotted = %d\n", nd->m_Dotted);
		fprintf_s(pO, "Triplet = %d\n", nd->m_Triplet);
		fprintf_s(pO, "Midi DeviceID = %d\n", nd->m_MidiOutID);
		fprintf_s(pO, "=================END=================\n");
	}
};

class CMsNote : public CMsObject
{
	CMyBitmap m_RestBitmap;
	BOOL m_BitmapFlag;
	NoteData m_Data;
	//-------------------------------------
	// Note data that concerns playing song
	//--------------------------------------
	CMsNote* m_NoteTieNext; //pointer to note object is the next tied note
	CMsNote* m_NoteTiePrev;	//pointer to previous note that is tied
	int m_Ticks;            /// timer ticks
public:
	CMsNote();
	virtual ~CMsNote();
	void Create(int nBitmapID, CMsSong* pSong, UINT ParentEvent);
	void LoadRestBitmap(int Selection);
	virtual void Draw(CDC* pDC, int event, int maxevent);
	void DrawRestBitmap(CDC* pDC, int event, int notev, COLORREF color);
	virtual void Print(FILE* pO);
	virtual void Save(FILE* pO);
	virtual CMsObject* Copy(void);
	virtual CMsObject* MakeANewObject();
	void SetNoteOffTick(UINT NOT) { GetData().SetNoteOffTick(NOT); }
	UINT GetNoteOffTick() { return GetData().GetNoteOffTick(); }

	void DecrNote(void);
	void IncrNote(void);

	BOOL IsNote() { return !GetData().GetRest(); }
	UINT IsRest(void) { return GetData().GetRest(); }
	void SetRest(UINT v) { GetData().SetRest(v); }

	UINT IsTieBeg() { return GetData().GetTieBeg(); }
	void SetTieBeg(UINT TB) { GetData().SetTieBeg(TB); }

	UINT IsTieEnd(void) { return GetData().GetTieEnd(); }
	void SetTieEnd(UINT v) { GetData().SetTieEnd(v); }

	BOOL IsAccentted() { return GetData().GetAccent(); }
	UINT GetAccent(void) { return GetData().GetAccent(); }
	void SetAccent(UINT v) { GetData().SetAccent(v); }

	UINT IsStacato(void) { return GetData().GetStacato(); }
	void SetStacato(UINT v) { GetData().SetStacato(v); }

	UINT IsLegato(void) { return GetData().GetLegato(); }
	void SetLegato(UINT v) { GetData().SetLegato(v); }

	UINT GetAccidental(void) { return GetData().GetAccidental(); }
	void SetAccidental(UINT k) { GetData().SetAccidental(k); }

	UINT GetTrack(void) { return GetData().GetTrack(); }
	void SetTrack(UINT t) { GetData().SetTrack(t); }

	int GetDuration(void) { return GetData().GetDuration(); }
	void SetDuration(int d) { GetData().SetDuration(d); }

	int GetPitch(void) { return GetData().GetPitch(); }
	void SetPitch(int p) { GetData().SetPitch(p); }

	UINT IsUpsideDown(void) { return GetData().GetUpsideDown(); }
	void SetUpsideDown(int u) { GetData().SetUpsideDown(u); }
	int GetUpsideDown(void) { return GetData().GetUpsideDown(); }

	void SetHeadFlipped(UINT HdFlipped) { GetData().SetHeadFlipped(HdFlipped); }
	UINT GetHeadFlipped() { return GetData().GetHeadFlipped(); }

	void SetDotted(int d) { GetData().SetDotted(d); }
	int GetDotted() { return GetData().GetDotted(); }

	int GetMidiOutID(int Track) { return GETMIDIINFO->GetMidiOutDeviceId(Track); }
	int GetShape() { return  DurTab[GetDuration()].NoteShapIndex; }

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
	bool IsDotted();
	bool IsSolid(void) { return DurTab[GetDuration()].Solid ? true : false; }
	bool IsTriplet();
	int NeedsFlags(void) { return DurTab[GetDuration()].Flags; }
	bool NeedsTail(void) { return DurTab[GetDuration()].Tail ? true : false; }
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
	virtual UINT Play();
	virtual BOOL AddToQueue();
	virtual UINT Process();
	virtual BOOL RemoveFromQueue();
	virtual void ObjectRectangle(CRect& rect, UINT Event);
};