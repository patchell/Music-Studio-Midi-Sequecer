#pragma once

class CMsSongInfo;

struct CMsTrackSettings
{
	COLORREF m_Color;
	COLORREF m_TextColor;
};

class CMsTrack
{
	inline static const char* CZ101PatchNames[16] = {
		//-------------------------------
		// CZ-101 Patch Names
		//-------------------------------
		"Brass Ensemble",	//0
		"Strings",			//1
		"Pipe Organ",		//2
		"Electric Piano",	//3
		"Harpsichord",		//4
		"Synth Bass",		//5
		"Cosmic Sound",		//6
		"Metal Synth",		//7
		"Fantasy",			//8
		"Funky Pop",		//9
		"Trumpet",			//10
		"Saxophone",		//11
		"Violin",			//12
		"Flute",			//13
		"Glocken",			//14
		"Drum Set"			//15
	};

	inline static int CZ101PatchToGenMidiPatch(int cz101Patch)
	{
		//---------------------------------------
		// Maps CZ-101 patch numbers to
		// General Midi Patch numbers
		//---------------------------------------
		static const int CZ101ToGenMidiLut[16] = {
			61,	//Brass Ensemble -> Brass Section
			48, //Strings -> String Ensemble 1
			19, //Pipe Organ -> Church Organ
			4,  //Electric Piano -> Electric Piano 1
			7,  //Harpsichord -> Clavicord
			38, //Synth Bass -> Synth Bass 1
			89, //Cosmic Sound -> Warm Pad
			81, //Metal Synth -> Sawtooth Lead
			88, //Fantasy -> New Age Pad
			27, //Funky Pop -> Clean Electric Guitar
			56, //Trumpet -> Trumpet
			65, //Saxophone -> Alto Sax
			40, //Violin -> Violin
			73, //Flute -> Flute
			9,  //Glocken -> Glockenspiel
			118 //Drum Set -> Synth Drum
		};
		if (cz101Patch < 0 || cz101Patch > 15)
			return 0; //default to Acoustic Grand Piano
		return CZ101ToGenMidiLut[cz101Patch];
	}
	//-------------------------------
	// General Midi Patch Names
	//-------------------------------
	inline static const char* GenMidiPatchNames[128] = {
			"Acoustic Grand Piano",		//0
			"Bright Acoustic Piano",	//1
			"Electric Grand Piano",		//2
			"Honky-Tonk Piano",			//3
			"Electric Piano 1",			//4
			"Electric Piano 2",			//5
			"Harpsicord",				//6
			"Clavicord",				//7
			"Celesta",					//8
			"Glockenspiel",				//9
			"Music Box",				//10
			"Vibraphone",				//11
			"Marimba",					//12
			"Xylophone",				//13
			"Tubular Bells",			//14
			"Dulcimer",					//15
			"Drawbar Organ",			//16
			"Percusive Organ",			//17
			"Rock Organ",				//18
			"Church Organ",				//19
			"Reed Organ",				//20
			"Accordian",				//21
			"Harmonica",				//22
			"Tango Accordian",			//23
			"Nylon Acoustic Guitar",	//24
			"Steel Acoustic Guitar",	//25
			"Jazz Electric Guitar",		//26
			"Clean Electric Guitar",	//27
			"Muted Electric Guitar",	//28
			"Overdriven Guitar",		//29
			"Distortion Guitar",		//30
			"Guitar Harmonics",			//31
			"Acoustic Bass",			//32
			"Electric Bass Fingered",	//33
			"Electric Bass Picked",		//34
			"Fretless Bass",			//35
			"Slap Bass 1",				//36
			"Slap Bass 2",				//37
			"Synth Bass 1",				//38
			"Synth Bass 2",				//39
			"Violin",					//40
			"Viola",					//41
			"Cello",					//42
			"Contrabass",				//43
			"Tremolo Strings",			//44
			"Pizzicato Strings",		//45
			"Orchestral Harp",			//46
			"Timpani",					//47
			"String Ensemble 1",		//48
			"String Ensemble 2",		//49
			"Synth Strings 1",			//50
			"Synth Strings 2",			//51
			"Choir Aahs",				//52
			"Voice Oohs",				//53
			"Synth Voice",				//54
			"Orchestra Hit",			//55
			"Trumpet",					//56
			"Trombone",					//57
			"Tuba",						//58
			"Muted Trumpet",			//59
			"French Horn",				//60
			"Brass Section",			//61
			"Synth Brass 1",			//62
			"Synth Brass 2",			//63
			"Soprano Sax",				//64
			"Alto Sax",					//65
			"Tenor Sax",				//66
			"Baratone Sax",				//67
			"Oboe",						//68
			"English Horn",				//69
			"Bassoon",					//70
			"Clarinet",					//71
			"Picolo",					//72
			"Flute",					//73
			"Recorder",					//74
			"Pan Flute",				//75
			"Blown Bottle",				//76
			"Shakulachi",				//77
			"Wistle",					//78
			"Ocarina",					//79
			"Square Lead",				//80
			"Sawtooth Lead",			//81
			"Caliope Lead",				//82
			"Chiff Lead",				//83
			"Charang Lead",				//84
			"Voice Lead",				//85
			"Fifths Lead",				//86
			"Bass + Lead",				//87
			"New Age Pad",				//88
			"Warm Pad",					//89
			"Polysynth Pad",			//90
			"Choir Pad",				//91
			"Bowed Pad",				//92
			"Metalic Pad",				//93
			"Halo Pad",					//94
			"Sweep Pad",				//95
			"FX 1 (rain)",				//96
			"FX 2 (soundtrack)",		//97
			"FX 3 (Crystal)",			//98
			"FX 4 (atmosphere)",		//99
			"FX 5 (brightness)",		//100
			"FX 6 (goblins)",			//101
			"FX 7 (echoes)",			//102
			"FX 8 (sci-fi)",			//103
			"Sitar",					//104
			"Banjo",					//105
			"Shanisen",					//106
			"Koto",						//107
			"Kalinba",					//108
			"Bag Pipe",					//109
			"Fiddle",					//110
			"Shania",					//111
			"Tinkle Bell",				//112
			"Agogo",					//113
			"Steel Drums",				//114
			"Wood Blocks",				//115
			"Taiko Drum",				//116
			"Melodic Tom",				//117
			"Synth Drum",				//118
			"Reverse Cymbal",			//119
			"Guitar Fret Noise",		//120
			"Breath Noise",				//121
			"SeaShore",					//122
			"Bird Tweets",				//123
			"Telephone Ring",			//124
			"Heliocopter",				//125
			"Applause",					//126
			"Gunshot"					//127
	};

	inline static int DefaultPatch[16] = {
		-1,		// Not used
		23,		//1	Harmonica
		67,		//2 Sax
		27,		//3	Gutar
		72,		//4 Clarinet
		74,		//5 Flute
		90,		//6 Pads
		1,		//7	Piano
		17,		//8	Organ
		57,		//9	Trumpet
		51,		//10	Strings
		12,		//11	Vibes
		15,		//12	Bells
		47,		//13	Harp
		39,		//14	Bass
		1		//15	Percusion
	};

	inline static int DefaultMidiChannel[16] = {
		-1,		//0	
		9,		//1
		2,		//2
		3,		//3
		4,		//4
		5,		//5
		6,		//6
		7,		//7
		8,		//8
		10,		//9
		11,		//10
		12,		//11
		13,		//12
		14,		//13
		15,		//14
		1		//15
	};

	inline static int DefaultPitchRange[16] = {
		-1,		//0
		2,		//1
		3,		//2
		3,		//3
		3,		//4
		3,		//5
		3,		//6
		3,		//7
		3,		//8
		3,		//9
		3,		//10
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
		RGB(208,255,64),	//Track 10
		RGB(192,0,0),		//Track 11
		RGB(192,0,128),		//Track 12
		RGB(224,86,96),		//Track 13
		RGB(224,67,148),		//Track 14
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
		RGB(255 - 208,0,255 - 64),			//Track 10
		RGB(255 - 192,255,255),				//Track 11
		RGB(255 - 192,255,255 - 128),		//Track 12
		RGB(255 - 224,255 - 86,255 - 96),	//Track 13
		RGB(255 - 224,255 - 67,255 - 148),	//Track 14
		RGB(0,255 - 222,255 - 192)			//Track 15
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
	int m_MidiChannel;		// Mdi channel for this track values 0->15 => chan 1->16
	int m_PatchNumber;		// Patch number for this track
	int m_PitchRange;		//Pitchcorrection for track
	//-----------------------------
	// Load and Save settings for
	// This Class
	//-----------------------------
	static CMsTrackSettings Settings[16];
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
	static void GetTrackSettings(int TrackNumber);
	static void GetSettings();
	static void SaveSettings();
	void Save(FILE* pO);
	//--------------------------------------------------
	// Graphics Methods
	//--------------------------------------------------
	void Draw(CDC* pDC);
	CMyBitmap* GetBitmap() { 
		return &m_bmTrack; 
	}
	COLORREF GetColor() { return m_Color; }
	void SetColor(COLORREF c) { 
		m_Color = c;
		CMsTrack::Settings[m_Number].m_Color = m_Color;
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
	void SetChannel(int nC) { m_MidiChannel = nC; }
	int GetChannel() const { return m_MidiChannel; }
	int* GetChannelPointer() { return &m_MidiChannel; }
	void SetPitchRange(int nPr) { m_PitchRange = nPr; };
	int GetPitchRange() const	{ return m_PitchRange; }
	int* GetPitchRangePointer() { return &m_PitchRange; }
	void UpdateTrackBitmap();
	CMsSongInfo* GetParentSongInfo() { return m_pParentSongInfo; }
};

