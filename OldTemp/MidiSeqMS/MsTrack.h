#pragma once

struct CMsTrackSettings
{
	COLORREF m_Color;
	COLORREF m_TextColor;
};

class CMsTrack
{
	CMyBitmap m_bmTrack;
	char* bmBits;		// Image bits
	COLORREF m_Color;	// Background color
	COLORREF m_TextColor;	// Text color
	int m_Number;		// Instument number
	CPoint m_ptUpperLeft;	// upper left, parent coordinates
	CSize m_szDimensions;	// Dimensions
	CFont m_Font;
	CString m_csText;
	CPoint m_ptText;
	int m_extentFlag;
	//----------------------------------------
	// Track Midi Attributes
	//----------------------------------------
	char m_InstrumentName[64];
	int m_MidiDeviceID;		// ID for the midi output device
	int m_MidiChannel;		// Mdi channel for this track values 0->15 => chan 1->16
	int m_PatchNumber;		// Patch number for this track
	int m_PitchRange;		//Pitchcorrection for track
	int m_Enable[4];		//Enable Flags
	//-----------------------------
	// Load and Save settings for
	// This Class
	//-----------------------------
	static CMsTrackSettings Settings[16];
public:
	CMsTrack() {
		m_extentFlag = 1;
		m_Number = 0;
		m_Color = RGB(0, 0, 0);
		m_TextColor = RGB(255, 255, 255);
		m_MidiDeviceID = 0;
		memset(m_Enable, 0, 4);
		m_PitchRange = 2;
		m_MidiChannel = 0;
		m_PatchNumber = 0;
		bmBits = 0;
		m_InstrumentName[0] = 0;
	}
	~CMsTrack() {}
	bool Create(
		CDC* pDC,
		int TrackID,
		CRect& rect,
		int MidiOutDevId,
		int Range,
		int MidiChannel,
		int Patch
	);
	static void GetSettings();
	static void SaveSettings();
	//--------------------------------------------------
	// Graphics Methods
	//--------------------------------------------------
	void Draw(CDC* pDC);
	CMyBitmap* GetBitmap() { return &m_bmTrack; }
	COLORREF GetColor() { return m_Color; }
	void SetColor(COLORREF c) { 
		m_Color = c;
		CMsTrack::Settings[m_Number].m_Color = m_Color;
		UpdateTrackBitmap();
	}
	COLORREF GetTextColor() { return m_TextColor; }
	void SetTextColor(COLORREF cTc) { m_TextColor = cTc; }
	//---------------------------------------------------
	// Track/Instrument attributes
	//---------------------------------------------------
	char* GetInstrumentName() {return m_InstrumentName; }
	void SetInstrumentName(const char* pName) {
		strcpy_s(m_InstrumentName, pName);
	}
	void SetPatch(int Patch) { m_PatchNumber = Patch; }
	int GetPatch() { return m_PatchNumber; }
	int* GetPatchPointer() { return &m_PatchNumber; }
	void SetMidiOutDeviceID(int MidiOutDevID) { m_MidiDeviceID = MidiOutDevID; }
	int GetMidiOutDeviceID() { return m_MidiDeviceID; }
	void SetChanel(int nC) { m_MidiChannel = nC; }
	int GetChanel() { return m_MidiChannel; }
	int* GetChannelPointer() { return &m_MidiChannel; }
	void SetPitchRange(int nPr) { m_PitchRange = nPr; };
	int GetPitchRange() { return m_PitchRange; }
	int* GetPitchRangePointer() { return &m_PitchRange; }
	void SetEnable(int index, int nE) { m_Enable[index] = nE; }
	int GetEnable(int index) { return m_Enable[index]; }
	void UpdateTrackBitmap();
};

