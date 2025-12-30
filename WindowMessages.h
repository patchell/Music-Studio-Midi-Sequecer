#pragma once


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
enum class MidiChannelCmds : int {
	NOTEOFF = 0x80,
	NOTEON = 0x90,
	POLYPRESS = 0xa0,
	CTRLCHNG = 0xb0,
	PGMCHANGE = 0xc0,
	CHNLPRESS = 0xd0,
	PITCHBEND = 0xe0
};

//-------------------------------------
// Submessages for MIDI_CTRLCHNG
//-------------------------------------

enum class MidiCC : int {
	BANK_SELECT_MSB = 0,
	MODWHEEL_MSB = 1,
	BREATHCTRL_MSB = 2,
	FOOTCTRL_MSB = 4,
	PORTAMENTO_TIME_MSB = 5,
	DATA_ENTRY_MSB = 6,
	VOLUME_MSB = 7,
	PAN_MSB = 10,
	EXPRESSION_MSB = 11,
	EFFECT1_MSB = 12,
	EFFECT2_MSB = 13,
	GENERAL_PURPOSE1_MSB = 16,
	GENERAL_PURPOSE2_MSB = 17,
	GENERAL_PURPOSE3_MSB = 18,
	GENERAL_PURPOSE4_MSB = 19,

	BANK_SELECT_LSB = 32,
	MODWHEEL_LSB = 33,
	BREATHCTRL_LSB = 34,
	FOOTCTRL_LSB = 36,
	PORTAMENTO_TIME_LSB = 37,
	DATA_ENTRY_LSB = 38,
	VOLUME_LSB = 39,
	PAN_LSB = 42,
	EXPRESSION_LSB = 43,
	EFFECT1_LSB = 44,
	EFFECT2_LSB = 45,
	GENERAL_PURPOSE1_LSB = 48,
	GENERAL_PURPOSE2_LSB = 49,
	GENERAL_PURPOSE3_LSB = 50,
	GENERAL_PURPOSE4_LSB = 51,

	DAMPER_PEDAL = 64,
	PORTAMENTO_ONOFF = 65,
	SOSTENUTO_PEDAL = 66,
	SOFT_PEDAL = 67,
	LEGATO_FOOTSWITCH = 68,
	HOLD2 = 69,
	SOUND_CONTROLLER1 = 70,
	SOUND_CONTROLLER2 = 71,
	SOUND_CONTROLLER3 = 72,
	SOUND_CONTROLLER4 = 73,
	SOUND_CONTROLLER5 = 74,
	SOUND_CONTROLLER6 = 75,
	SOUND_CONTROLLER7 = 76,
	SOUND_CONTROLLER8 = 77,
	SOUND_CONTROLLER9 = 78,
	SOUND_CONTROLLER10 = 79,
	GENERAL_PURPOSE5 = 80,
	GENERAL_PURPOSE6 = 81,
	GENERAL_PURPOSE7 = 82,
	GENERAL_PURPOSE8 = 83,
	PORTAMENTO_CONTROL = 84,
	EFFECTS1_DEPTH = 91,
	EFFECTS2_DEPTH = 92,
	EFFECTS3_DEPTH = 93,
	EFFECTS4_DEPTH = 94,
	EFFECTS5_DEPTH = 95,
	DATA_INCREMENT = 96,
	DATA_DECREMENT = 97,
	NON_REGISTERED_PARAM_MSB = 98,	
	NON_REGISTERED_PARAM_LSB = 99,
	REGISTERED_PARAM_MSB = 100,
	REGISTERED_PARAM_LSB = 101,
	ALL_SOUND_OFF = 120,
	RESET_ALL_CONTROLLERS = 121,
	LOCAL_CONTROL = 122,
	ALL_NOTES_OFF = 123,
	OMNI_MODE_ON = 124,
	OMNI_MODE_OFF = 125,
	MONO_MODE_ON = 126,
	POLY_MODE_ON = 127
};;


//-------------------------------------
// Midi Real Time Messages
//-------------------------------------

enum class MidiRealTimeMsgs : int {
	CLOCK = 0xf8,
	RSVRD_F9,
	START,
	CONTINUE,
	STOP,
	RSVRD_FD,
	ACTIVE_SENSING,
	RESET
};

//--------------------------------------
// Midi System Messages
// --------------------------------------
enum class MidiSystemCmds : int {
	SYSEX = 0xf0,
	TIMECODE,
	SONG_POSITION,
	SONG_SELECT,
	RSVRD_F4,
	RSVRD_F5,
	TUNE_REQUEST,
	SYSEXEND
};

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
	IDC_BUTTON_PAUSE,			// Pause Button
	IDC_BUTTON_STOP,			// Stop Button
	IDC_STATIC_DEBUGDISP,
	IDC_STATIC_STATUSDISP
};

