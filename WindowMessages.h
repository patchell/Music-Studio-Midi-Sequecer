#pragma once

enum {
	DRAWSTATE_SET_ATTRIBUTES,
	DRAWSTATE_WAITFORMOUSE_DOWN,
	DRAWSTATE_MOVE_OBJECT_AROUND,
	DRAWSTATE_PLACE
};

enum {
	SEARCH_REVERSE,
	SEARCH_FORWARD
};

//*************************************
// Midi Messages
//*************************************
union MidiMsg {
	struct {
		char cmd_chan;
		char note;
		char vel;
	}parts;
	DWORD msg;
	DWORD CreateMidiMsg(char cmd, char chan = 0, char note = 0, char vel = 0) {
		msg = 0;
		if ((cmd & 0xf0) == 0xf0)
			parts.cmd_chan = cmd;
		else
		{
			parts.cmd_chan = cmd | chan;
//			parts.cmd_chan = int(cmd) | int(chan);
			parts.note = note;
			parts.vel = vel;
		}
		return msg;
	}
};

//----------------------------------
// Song Playing State Defines
// --------------------------------
enum SongPlayingStates {
	SONG_NOT_PLAYING,
	SONG_IS_PLAYING,
	SONG_STOP	// This Status means we need
				// to wait for the events
				// in the playlist queue
				// to time out.
};

//------------------------------
// Midi In Dispatcher Midi
// Status Byte Look Up Table
//------------------------------

struct MIDI_STATUS {
	CString m_csName;
	unsigned m_MidiStatus:8;
	unsigned m_Data1_flag : 1;
	unsigned m_Data2_flag : 1;
	unsigned m_Channel_flag : 1;
	CString m_csData1;
	CString m_csData2;
} ;

//----------------------------------
// Midi channel Messages
//----------------------------------
constexpr auto MIDI_NOTEOFF = (0x80);
constexpr auto MIDI_NOTEON = (0x90);
constexpr auto MIDI_POLYPRESS = (0xa0);
constexpr auto MIDI_CTRLCHNG = (0xb0);
constexpr auto MIDI_PGMCHANGE = (0xc0);
constexpr auto MIDI_CHNLPRESS = (0xd0);
constexpr auto MIDI_PITCHBEND = (0xe0);
//-------------------------------------
// Submessages for MIDI_CTRLCHNG
//-------------------------------------

constexpr auto MIDI_CTRLCHNG_ALLOFF = (120);
constexpr auto MIDI_CTRLCHNG_RESETALL = (121);
constexpr auto MIDI_CTRLCHNG_LOCAL = (122);
constexpr auto MIDI_CTRLCHNG_LOCAL_OFF = (0);		//sub message to MIDI_CTRLCHNG_LOCAL;
constexpr auto MIDI_CTRLCHNG_LOCAL_ON = (127);	//sub message to MIDI_CTRLCHNG_LOCAL
constexpr auto MIDI_CTRLCHNG_NOTESOFF = (123);
constexpr auto MIDI_CTRLCHNG_OMNION = (124);
constexpr auto MIDI_CTRLCHNG_OMNIOFF = (125);
constexpr auto MIDI_CTRLCHNG_MONO_ON = (126);
constexpr auto MIDI_CTRLCHNG_POLY_ON = (127);

//-------------------------------------
// Midi Real Time Messages
//-------------------------------------
constexpr auto MIDI_CLOCK = (0xf8);
constexpr auto MIDI_RTM_RSVRD_F9 = 0xf9;
constexpr auto MIDI_START = (0xfa);
constexpr auto MIDI_CONTINUE = (0xfb);
constexpr auto MIDI_STOP = (0xfc);
constexpr auto MIDI_RTM_RSVRD_FD = 0xfd;
constexpr auto MIDI_ACTIVE_SENSING = 0xfe;
constexpr auto MIDI_RESET = (0xff);
//-------------------------------------
// Midi System Exclusive
//--------------------------------------
constexpr auto MIDI_SYSEX = (0xf0);
constexpr auto MIDI_SYSEXEND = (0xf7);
//--------------------------------------
// Midi System Messages
// --------------------------------------
constexpr auto MIDI_TIMECODE = 0xf1;
constexpr auto MIDI_SONGPOSITION = 0xf2;
constexpr auto MIDI_SONGSELECT = 0xf3;
constexpr auto MIDI_RESERVED_F4 = 0xf4;
constexpr auto MIDI_RESERVED_F5 = 0xf5;
constexpr auto MIDI_TUNEREQUEST = 0xf6;
//****************************************
// Midi Message Decoding
//***************************************
template<typename T>
constexpr auto CMD(T x) { return (x & 0x0f0); }
template<typename T>
constexpr auto CHAN(T x) { return (x & 0x0f); }
template<typename T>
constexpr auto NOTE(T x) { return ((x & 0x0ff00)>>8); }
template<typename T>
constexpr auto VEL(T x) { return ((x & 0x0ff0000) >> 16); }
//-------------- Message Encoding -------------------
template<typename T1, typename T2, typename T3, typename T4>
constexpr auto MAKEMIDIMSG(T1 CaManD, T2 CHANnel, T3 Pitch, T4 Velocity) { return (CaManD | CHANnel | (Pitch << 8) | (Velocity << 16)); }
//**********************************************
// Windows Messages
//**********************************************
enum WindowMessages{
//------------------------------------
// Channel number change in Midi Setup
//-------------------------------------
	WM_CHANCHANGE = WM_USER + 1,
//------------------------------------
// Midi In Message
//------------------------------------
	WM_LONGMIDIMSG,
	WM_SHORTMIDIMSG,
//-------------------------------------
// Message for CChildViewStaff that tells
// where to highlight staff to show
// current song position
//-------------------------------------
	WM_STAFF_DISP_EVENT,		
	WM_MY_CONTROL_MESSAGES,
	WM_STAFF_MIDI_EDIT,
	WM_STAFF_MIDI_TRIGGER,
	WM_CHILDVIEW_PLAYERTHREAD,
	//---------------------------
	// Player Thread Message
	//--------------------------
	WM_THREAD_PLAYER_TIMER,
	WM_THREAD_PLAYER_TEMPO,
	WM_THREAD_PLAYER_ADD_SONG,
	WM_THREAD_PLAYER_DELETE_SONG,
	WM_THREAD_PLAYER_ENABLE_TIMER,
	//-------------------------
	// CMyButton Notification
	//------------------------
	WM_MYBUTTON_UNLATCH,
	//-------------------------
	// All Thread Messages
	//--------------------------
	WM_KILLTHREAD,
	//--------------------------
	// CMyScrollBar mesages
	//--------------------------
	WM_MYSCROLLBAR_MSG,
	WM_MYOWNSCROLLBAR_INIT
};
//-------------------------------------------------------
// Subcommands for CChildViewStaff::OnChildviewPlayerthread
// WM_CHILDVIEW_PLAYERTHREAD
//--------------------------------------------------------
enum {
	CHILD_VIEW_PLAYERTHREAD_SONGDONE
};

enum SongObjectIDs{
	PLAYER_SONG_BAR,
	PLAYER_SONG_END,
	PLAYER_SONG_ERROR,
	PLAYER_SONG_REPEAT,
	PLAYER_SONG_REPEAT_END,
	PLAYER_SONG_TEMPO,
	PLAYER_SONG_NOTE,
	PLAYER_SONG_PATCHCHNG,
	PLAYER_SONG_TICKDEBUG,
	PLAYER_SONG_EVENTEND,
	PLAYER_SONG_DISPEVENT,
	PLAYER_SONG_PLAYBUTTON,
	PLAYER_SONG_NOTEONOFF,
};

//--------------------------------------
// Subfunctions for WM_MIDI_EDIT
//--------------------------------------

enum {
	MIDIEDIT_NOTE,
	MIDIEDIT_REST,
	MIDIEDIT_DECORATIONS,
	MIDIEDIT_DECORATIONS_ACCENT,
	MIDIEDIT_ACCIDENTALS,
	MIDIEDIT_INSTRUMENT,
	MIDIEDIT_TIMESIGNATURE,
	MIDIEDIT_KEYSIGNATURE,
	MIDIEDIT_MISC
};

//SubSub functions for MIDIEDIT_NOTE and MIDIEDIT_REST

enum {
	MIDIEDIT_WHOLE,
	MIDIEDIT_HALF,
	MIDIEDIT_QUARTER,
	MIDIEDIT_EIGTH,
	MIDIEDIT_SIXTEENTH,
	MIDIEDIT_THIRTYSECOND
};

enum {
	MIDIEDIT_ACCENT_OFF,
	MIDIEDIT_ACCENT_ON
};
enum {
	MIDIEDIT_DECORATIONS_NONE = 1,
	MIDIEDIT_DECORATIONS_DOTTED,
	MIDIEDIT_DECORATIONS_TRIPLET
};

enum {
	MIDIEDIT_ACCIDENTALS_NONE,
	MIDIEDIT_ACCIDENTALS_SHARP,
	MIDIEDIT_ACCIDENTALS_FLAT,
	MIDIEDIT_ACCIDNETALS_NATURAL
};

enum {
	MIDIEDIT_INST1,
	MIDIEDIT_INST2,
	MIDIEDIT_INST3,
	MIDIEDIT_INST4,
	MIDIEDIT_INST5,
	MIDIEDIT_INST6,
	MIDIEDIT_INST7,
	MIDIEDIT_INST8,
	MIDIEDIT_INST9,
	MIDIEDIT_INST10,
	MIDIEDIT_INST11,
	MIDIEDIT_INST12,
	MIDIEDIT_INST13,
	MIDIEDIT_INST14,
	MIDIEDIT_INST15,
};

enum {
	MIDIEDIT_TIMESIG_2_2,
	MIDIEDIT_TIMESIG_3_2,
	MIDIEDIT_TIMESIG_2_4,
	MIDIEDIT_TIMESIG_3_4,
	MIDIEDIT_TIMESIG_4_4,
	MIDIEDIT_TIMESIG_5_4,
	MIDIEDIT_TIMESIG_6_8
};

enum {
	MIDIEDIT_KEY_CMAJ,	//1X
	MIDIEDIT_KEY_GMAJ,	//2X
	MIDIEDIT_KEY_DMAJ,	//3X
	MIDIEDIT_KEY_AMAJ,	//4X
	MIDIEDIT_KEY_EMAJ,	//5X
	MIDIEDIT_KEY_BMAJ,	//6X
	MIDIEDIT_KEY_FSMAJ,	//7X
	MIDIEDIT_KEY_CSMAJ,	//8X
	MIDIEDIT_KEY_FMAJ,	//9X
	MIDIEDIT_KEY_BFMAJ,	//10X
	MIDIEDIT_KEY_EFMAJ,	//11X
	MIDIEDIT_KEY_AFMAJ,	//12X
	MIDIEDIT_KEY_DFMAJ,	//13X
	MIDIEDIT_KEY_GFMAJ,	//14X
	MIDIEDIT_KEY_CFMAJ	//15X

};

enum {
	MIDIEDIT_MISC_MEASUREBAR,
	MIDIEDIT_MISC_NOTETIE,
	MIDIEDIT_MISC_LOUDNESS,
	MIDIEDIT_MISC_TEMPO,
	MIDIEDIT_MISC_INST_CHANGE
};
//--------------------------------------
enum {
	DRAW_NOP,
	DRAW_NOTE,
	DRAW_REST,
	DRAW_ENDBAR,
	DRAW_BAR,
	DRAW_TIE,
	DRAW_COPY,
	DRAW_MOVE,
	DRAW_REPEAT,
	DRAW_TEMPO,
	DRAW_TIMESIG,
	DRAW_KEYSIG,
	DRAW_LOUDNESS,
	DRAW_INSTCHANGE,
	DRAW_CHANGEDUR,
	DRAW_INCREASEDUR,
	DRAW_DECREASEDUR,	//16
	DRAW_INCRPITCH,		//17
	DRAW_DECRPITCH,		//18
	DRAW_INSERTBLOCK,	//19
	DRAW_DRAWNOTESVIAMIDI	//20
};

// decorations combo box indexes

enum {
	COMBO_DECOR_ACCENT,
	COMBO_DECOR_NONE,
	COMBO_DECOR_DOT,
	COMBO_DECOR_TRIPLET
};

//-----------------------------------
// DRAW_NOTE LPARAM Format
//-----------------------------------

constexpr auto DRAW_NOTE_DURATION = (0x1f);
constexpr auto DRAW_NOTE_ACCENT = (0x20);
constexpr auto DRAW_NOTE_ACCENT_SHIFT = (5);
constexpr auto DRAW_NOTE_KEY = (0x0c0);
constexpr auto DRAW_NOTE_KEY_SHIFT = (6);
constexpr auto DRAW_NOTE_TRACK = (0x0f00);
constexpr auto DRAW_NOTE_TRACK_SHIFT = (8);
constexpr auto DRAW_NOTE_REST = (0x1000);
constexpr auto DRAW_NOTE_REST_SHIFT = (12);
constexpr auto DRAW_NOTE_ENDTIE = (0x2000);	//not used
constexpr auto DRAW_NOTE_BEGTIE = (0x4000);	//not used


constexpr auto STAFF_DISP_EVENT_NEXT = 0;
constexpr auto STAFF_DISP_EVENT_END = 1;

//--------------------------------------
// IDs for Combo Boxes in Staff view
//--------------------------------------

enum {
	IDC_COMBO_ACCIDENTALS = 600,	// Accidentals
	IDC_COMBO_BLOCKOPS,			// Block Operations
	IDC_COMBO_DECORATIONS,		// Decpratopms
	IDC_COMBO_INSTRUMENT,		// Instruments
	IDC_COMBO_NOTETYPES,		// Note Types
	IDC_COMBO_RESTTYPES,		// Rest Types
	IDC_COMBO_MISCSTUFF,		// Misc stuff
	IDC_COMBO_KEYSIG,			// Key Signatures
	IDC_COMBO_TIMESIG,			// Time Signatures
	IDC_BUTTON_PLAY,			// Play Button
	IDC_BUTTON_STOP,				// Stop Button
	IDC_STATIC_DEBUGDISP,
	IDC_STATIC_STATUSDISP
};

