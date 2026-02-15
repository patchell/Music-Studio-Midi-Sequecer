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
	MidiMsg() : msg(0) {}
	DWORD CreateMidiMsg(
		char cmd, 
		char chan = 0,	// Logical channel number
		char note = 0, 
		char vel = 0
	) {
		msg = 0;
		if ((cmd & 0xf0) == 0xf0)
			parts.cmd_chan = cmd;
		else
		{
			// convert logical channel number
			// to physical channel number by
			// subtracting 1
			parts.cmd_chan = cmd | (chan - 1);
//			parts.cmd_chan = int(cmd) | int(chan);
			parts.note = note;
			parts.vel = vel;
		}
		return msg;
	}
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

