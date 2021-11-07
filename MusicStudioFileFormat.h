#pragma once


//**************************************
// Music Studio File Format
//**************************************

struct DUR {
	UINT NoteShapIndex;
	UINT Dotted;
	UINT Triplet;
	UINT Solid;	// Note circle open or solid
	UINT Flags;
	UINT Tail;
	UINT DurTime;	// clock ticks
	char* pName;
};

//----------------------------------
//
// File Format:
// Header:
// BYTE0:		$CD
// BYTE1-7		"Mstudio"
// BYTE 8-9		$CD-$02
// BYTE10-159		Atari ST instrument names
//				This is an array of 15 names
//				Each name is 10 bytes
// BYTE 160-279	Atari instrument settings.
//				This is an array of 15 blocks
//				of 8 bytes each which correspond
//				to each of the 15 instruments
// BYTE 280-429	Instrument names for MIDI devices
//				This is an array of 15 blocks of 10
//				bytes each.
// BYTE 430-444	An array of 15 bytes containing the
//				midi channel and the range.  The range is
//				the upper four bits and the channel is
//				the lower four bits.
// BYTE 445-459 An array of 15 bytes that contain the
//				patch number for the instrument
// BYTE 460-479 Displacements to the lyric lines
// BYTE 480-511	32 Byte Song Title
// End of header
// BYTE 512-519 These are evidently the track enable words
// BYTE 520     This is where the actual song data starts
//              Why do we need four sets of track enables?
//-----------------------------------------------------

//Offset Defines
constexpr auto  MSFF_MIDIINSTURMENTS_OFFSET = 280;
constexpr auto  MSFF_SONGDATA_OFFSET = 520;

//-------------------------------------------
// time Signature Values
//-------------------------------------------

enum TimeSig {
	TIMESIG_2_2 = 1,
	TIMESIG_3_2,
	TIMESIG_2_4,
	TIMESIG_3_4,
	TIMESIG_4_4,
	TIMESIG_5_4,
	TIMESIG_6_8
};

//-----------------------------------------------------
// values for creating note dur from combo boxes
//-----------------------------------------------------

constexpr auto  MSFF_WHOLE_NOTE = 18;
constexpr auto  MSFF_HALF_NOTE = 15;
constexpr auto  MSFF_QUARTER_NOTE = 12;
constexpr auto  MSFF_EIGTH_NOTE = 9;
constexpr auto  MSFF_SIXTEENTH_NOTE = 6;
constexpr auto  MSFF_THIRTYSEC_NOTE = 3;

constexpr auto  MSFF_DOTTED_NOTE = (2);
constexpr auto  MSFF_TRIPLET_NOTE = (-2);
constexpr auto  MSFF_NOTE_C2 = 0x24;
constexpr auto  MSFF_NOTE_D2 = 0x26;
constexpr auto  MSFF_NOTE_E2 = 0x28;
constexpr auto  MSFF_NOTE_F2 = 0x29;
constexpr auto  MSFF_NOTE_G2 = 0x2B;
constexpr auto  MSFF_NOTE_A2 = 0x2D;
constexpr auto  MSFF_NOTE_B2 = 0x2F;
constexpr auto  MSFF_NOTE_C3 = 0x30;

//------------------------------------------------------
// Key Signatures
//-----------------------------------------------------

enum KeySig {
	MSFF_CMAJ = 1,
	MSFF_GMAJ,
	MSFF_DMAJ,
	MSFF_AMAJ,
	MSFF_EMAJ,
	MSFF_BMAJ,
	MSFF_FSMAJ,
	MSFF_CSMAJ,
	MSFF_FMAJ,
	MSFF_BFMAJ,
	MSFF_EFMAJ,
	MSFF_AFMAJ,
	MSFF_DFMAJ,
	MSFF_GFMAJ,
	MSFF_CFMAJ
};

//--------------------------------------
// Music Studio Token values for parser
//--------------------------------------

constexpr auto  MSFF_TOKEN_EVENT_END = 0x0;
constexpr auto  MSFF_TOKEN_KEY_SIGNATURE = 0x80;
constexpr auto 	MSFF_TOKEN_TEMPO = 0x81;
constexpr auto 	MSFF_TOKEN_BAR = 0x82;
constexpr auto 	MSFF_TOKEN_TIME_SIGNATURE = 0x83;
constexpr auto 	MSFF_TOKEN_LOUDNESS = 0x84;
constexpr auto 	MSFF_TOKEN_REPEAT_START = 0x85;
constexpr auto 	MSFF_TOKEN_REPEAT_STOP = 0x86;
constexpr auto	MSFF_TOKEN_PORTAMENTO_START = 0X87;
constexpr auto	MSFF_TOKEN_PORTAMENTO_END = 0X88;
constexpr auto	MSFF_TOKEN_GLISANDO_START = 0X89;
constexpr auto	MSFF_TOKEN_GLISANDO_END = 0X8A;
constexpr auto  MSFF_TOKEN_END = 0xFF;

//---------------------------
// defines for note tokens
//---------------------------

// BYTE 0
//bit 0 track lsb
//bit 1 track
//bit 2 track
//bit 3 track msb
constexpr auto  MSFF_REST_FLAG = (0x10);	//bit 4
constexpr auto  MSFF_ENDTIE_FLAG = (0x20);	//bit 5
constexpr auto  MSFF_BEGTIE_FLAG = (0x40);	//bit 6
constexpr auto  MSFF_TRACK_MASK = (0x0f);						//bit 7 not used

// BYTE 1

constexpr auto  MSFF_SHARP = 0x80;
constexpr auto 	MSFF_FLAT = 0xc0;
constexpr auto 	MSFF_INKEY = 0x00;
constexpr auto 	MSFF_NATURAL = 0x40;
constexpr auto  MSFF_KEY_MASK = (0xc0);
constexpr auto  MSFF_KEY_SHIFT = 6;
constexpr auto  MSFF_ACCENT_FLAG = (0x20);
constexpr auto  MSFF_DURATION_MASK = (0x1f);

// BYTE 2

constexpr auto  MSFF_NOTE_UPSIDE_DOWN = 0x80;

//--------------------------------------
// Note Defines
//-------------------------------------

enum Notes{
	MSFF_NOTE_C,
	MSFF_NOTE_CS,
	MSFF_NOTE_D,
	MSFF_NOTE_DS,
	MSFF_NOTE_E,
	MSFF_NOTE_F,
	MSFF_NOTE_FS,
	MSFF_NOTE_G,
	MSFF_NOTE_GS,
	MSFF_NOTE_A,
	MSFF_NOTE_AS,
	MSFF_NOTE_B
};


constexpr auto MSFF_REST_THIRTYSECOND = 3;
constexpr auto MSFF_REST_SIXTEENTH = 6;
constexpr auto  MSFF_REST_EIGTH = 9;
constexpr auto  MSFF_REST_QUARTER = 12;
constexpr auto  MSFF_REST_HALF = 15;
constexpr auto  MSFF_REST_WHOLE = 18;


extern DUR DurTab[];

typedef struct {
	char Name[10];
} Instrument;

typedef struct {
	Instrument Name[15];		//Instrument names
	char chan_range[15];	/*	array contains data that coresponds to channel and range	*/
	char patch[15];			//	array that indicates midi patch for instrument
	long lyrics[5];			//	pointers to lyics
	char Title[32];			// Song Title
	UINT TrackEnables[4];
}MIDI_INFO;

constexpr auto  CHAN_RANGE_CHAN_MASK = 0x0f;
constexpr auto  CHAN_RANGE_RANGE_MASK = 0xf0;
constexpr auto  CHAN_RANGE_RANGE_SHIFT = 4;

//-----------------------------------------------
// Chords
//-----------------------------------------------

enum chords {
	CHORD_MAJOR_TRIAD,
	CHORD_MAJOR_SIXTH,
	CHORD_DOMINANT_SEVENTH,
	CHORD_MAJOR_SEVENTH,
	CHORD_AUGMENTED_TRIAD,
	CHORD_AUGMENTED_SEVENTH,
	CHORD_MINOR_TRIAD,
	CHORD_MINOR_SIXTH,
	CHORD_MINOR_SEVENTH,
	CHORD_MINOR_MAJOR_SEVENTH,
	CHORD_DIMINISHED_TRIAD,
	CHORD_DIMINISHED_SEVENTH,
	CHORD_HALF_DIMINISHED_SEVENTH
};

struct CHORD {
	INT Interval1;	// Interval from Root
	INT Interval2;	// Interval from Interval1
	INT Interval3;	// Interval from Interval2
	INT Interval4;	// Interval from Interval3
	CString Name;
	CString Shorthand;
};

constexpr auto NUMBER_OF_CHORDS = CHORD_HALF_DIMINISHED_SEVENTH + 1;

//---------------------------------------------------
// Intervals
//---------------------------------------------------

constexpr auto NUMBER_OF_INTERVALS = 23;

enum  {
	INTERVAL_NONE = -1,
	INTERVAL_ROOT,
	INTERVAL_m2,
	INTERVAL_M2,
	INTERVAL_A2,
	INTERVAL_d2,
	INTERVAL_m3,
	INTERVAL_M3,
	INTERVAL_A3,
	INTERVAL_d3,
	INTERVAL_P4,
	INTERVAL_A4,
	INTERVAL_d4,
	INTERVAL_P5,
	INTERVAL_A5,
	INTERVAL_d5,
	INTERVAL_m6,
	INTERVAL_M6,
	INTERVAL_A6,
	INTERVAL_d6,
	INTERVAL_m7,
	INTERVAL_M7,
	INTERVAL_A7,
	INTERVAL_d7
};

struct IntervalLUTentry {
	CString Name;
	CString ShortHand;
	UINT Semitones;
};

extern IntervalLUTentry IntervalLUT[NUMBER_OF_INTERVALS];
