#pragma once

class CMsSongInfo;


class CMsTrack
{
	//-------------------------------
	// Logical track settings
	//-------------------------------
	inline static int DefaultMidiChannel[16] = {
		-1,		//0		Track 0 not used
		1,		//1
		2,		//2
		3,		//3
		4,		//4
		5,		//5
		6,		//6
		7,		//7
		8,		//8
		9,		//9	
		10,		//10 Percussion track is on channel 10
		11,		//11
		12,		//12
		13,		//13
		14,		//14
		15		//15
	};

	inline static int DefaultPitchRange[16] = {
		-1,		//0	Track 0 not used
		2,		//1
		3,		//2
		3,		//3
		3,		//4
		3,		//5
		3,		//6
		3,		//7
		3,		//8
		3,		//9
		2,		//10 Percussion track is on channel 10
		3,		//11
		3,		//12
		3,		//13
		3,		//14
		1		//15
	};
	//----------------------------------------
	inline static COLORREF DefaultColors[16] = {
		RGB(0,0,0),			//Not used
		RGB(0,0,0),			//Track 1
		RGB(0,0,192),		//Track 2
		RGB(100,0,192),		//Track 3
		RGB(0,100,192),		//Track 4
		RGB(0,96,255),		//Track 5
		RGB(0,192,0),		//Track 6
		RGB(96,208,0),		//Track 7
		RGB(0,208,96),		//Track 8
		RGB(96,208,128),	//Track 9
		RGB(208,255,64),	//Track 10 Percussion track
		RGB(192,0,0),		//Track 11
		RGB(192,0,128),		//Track 12
		RGB(224,86,96),		//Track 13
		RGB(224,67,148),	//Track 14
		RGB(255,80,192)		//Track 15
	};

	inline static COLORREF DefaultTextColors[16] = {
		RGB(255,255,255),					//Not used
		RGB(255,255,255),					//Track 1
		RGB(255,255,255 - 192),				//Track 2
		RGB(155,255,255 - 192),				//Track 3
		RGB(255,155,255 - 192),				//Track 4
		RGB(255,255 - 96,0),				//Track 5
		RGB(255,255 - 192,255),				//Track 6
		RGB(255 - 96,255 - 208,255),		//Track 7
		RGB(255,255 - 208,255 - 96),		//Track 8
		RGB(255 - 96,255 - 208,155 - 128),	//Track 9
		RGB(255 - 208,0,255 - 64),			//Track 10 Percussion track
		RGB(255 - 192,255,255),				//Track 11
		RGB(255 - 192,255,255 - 128),		//Track 12
		RGB(255 - 224,255 - 86,255 - 96),	//Track 13
		RGB(255 - 224,255 - 67,255 - 148),	//Track 14
		RGB(0,255 - 222,255 - 192)			//Track 15
	};

	inline static CMidi::GeneralMidiPatchID DefaultPatch[16] = {
		CMidi::GeneralMidiPatchID(-1),						// Not used
		CMidi::GeneralMidiPatchID::Celesta,					//1	Harmonica
		CMidi::GeneralMidiPatchID::BaratoneSax,				//2 Sax
		CMidi::GeneralMidiPatchID::CleanElectricGuitar,		//3	Gutar
		CMidi::GeneralMidiPatchID::Clarinet,				//4 Clarinet
		CMidi::GeneralMidiPatchID::Flute,					//5 Flute
		CMidi::GeneralMidiPatchID::NewAgePad,				//6 Pads
		CMidi::GeneralMidiPatchID::AcousticGrandPiano,		//7	Piano
		CMidi::GeneralMidiPatchID::ChurchOrgan,				//8	Organ
		CMidi::GeneralMidiPatchID::Trumpet,					//9	Trumpet
		CMidi::GeneralMidiPatchID::StringEnsemble1,			//10	Strings
		CMidi::GeneralMidiPatchID::Vibraphone,				//11	Vibes
		CMidi::GeneralMidiPatchID::TubularBells,			//12	Bells
		CMidi::GeneralMidiPatchID::OrchestralHarp,			//13	Harp
		CMidi::GeneralMidiPatchID::ElectricBassFingered,	//14	Bass
		CMidi::GeneralMidiPatchID::StringEnsemble1			//15	Percusion
	};
	CMsSongInfo* m_pParentSongInfo;
	CMyBitmap m_bmTrack;
	char* bmBits;			// Image bits
	COLORREF m_Color;		// Background color
	COLORREF m_TextColor;	// Text color
	int m_Number;			// Instument number/ Track ID/Number
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
	int m_MidiChannel;		// Logical Mdi channel for this track values 1 -> 16
	int m_PatchNumber;		// Patch number for this track
	int m_PitchRange;		//Pitchcorrection for track
public:
	CMsTrack() {
		m_pParentSongInfo = nullptr;
		m_extentFlag = 1;
		m_Number = 0;
		m_Color = RGB(0, 0, 0);
		m_TextColor = RGB(255, 255, 255);
		m_MidiDeviceID = 0;
		m_PitchRange = 2;
		m_MidiChannel = 0;
		m_PatchNumber = 0;
		bmBits = 0;
		m_InstrumentName[0] = 0;
	}
	~CMsTrack() {}
	bool Create(
		CMsSongInfo* pParentSongInfo, // Parent song info
		CSize szTrackIconSize,		// size of track bitmap Icon
		int MidiOutDevId,	// Midi output device ID
		int TrackID,		// Track number 1->15
		int Range,			// Pitch range
		int MidiChannel,	// Midi channel
		int Patch			// Patch number
	);
	bool Create(
		CMsSongInfo* pParentSongInfo, // Parent song info
		CSize szTrackIconSize,		// size of track bitmap Icon
		int MidiOutDevId,	// Midi output device ID
		int TrackID		// Track number 1->15
	);
	void GetDefaultTrackSettings(int TrackNumber, int MidiOutDviceID = 0);
	void GetColorSettings();
	void SaveColorSettings();
	void Save(FILE* pO);
	//--------------------------------------------------
	// Graphics Methods
	//--------------------------------------------------
	void Draw(CDC* pDC);
	CMyBitmap* GetBitmap() { 
		return &m_bmTrack; 
	}
	COLORREF GetColor() const { return m_Color; }
	void SetColor(COLORREF c) { 
		m_Color = c;
		UpdateTrackBitmap();
	}
	COLORREF GetTextColor() const { return m_TextColor; }
	void SetTextColor(COLORREF cTc) { m_TextColor = cTc; }
	//---------------------------------------------------
	// Track/Instrument attributes
	//---------------------------------------------------
	char* GetInstrumentName() {return m_InstrumentName; }
	void SetInstrumentName(const char* pName) {
		strcpy_s(m_InstrumentName, pName);
	}
	void SetPatch(int Patch) { m_PatchNumber = Patch; }
	int GetPatch() const { return m_PatchNumber; }
	int* GetPatchPointer() { return &m_PatchNumber; }
	void SetMidiOutDeviceID(int MidiOutDevID) { m_MidiDeviceID = MidiOutDevID; }
	int GetMidiOutDeviceID() const { return m_MidiDeviceID; }
	void SetChannel(int LogicalMidiChannel) { m_MidiChannel = LogicalMidiChannel; }
	int GetChannel() const { return m_MidiChannel; }	// return logical midi channel 1->16
	int* GetChannelPointer() { return &m_MidiChannel; }
	void SetPitchRange(int nPr) { m_PitchRange = nPr; };
	int GetPitchRange() const	{ return m_PitchRange; }
	int* GetPitchRangePointer() { return &m_PitchRange; }
	void UpdateTrackBitmap();
	CMsSongInfo* GetParentSongInfo() { return m_pParentSongInfo; }
};

