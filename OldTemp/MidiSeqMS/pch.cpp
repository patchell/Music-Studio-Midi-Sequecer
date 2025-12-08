// pch.cpp: source file corresponding to the pre-compiled header

#include "pch.h"

//---------------------------------------
// Bitmap Tables for music editor
//---------------------------------------

MIDI_STATUS MidiStatusCommands[DISPATCHEDIT__NUM_MIDI_CMDS] = {
	//------------ Channel Status ---------------------
	{CString("Note Off"), MIDI_NOTEOFF,1,1,1, CString("Note"), CString("Velocity")},
	{CString("Note On"), MIDI_NOTEON,1,1,1, CString("Note"), CString("Velocity")},
	{CString("Poly AftrTch"), MIDI_POLYPRESS,1,1,1, CString("Note"), CString("Pres")},
	{CString("Control/Mode"),MIDI_CTRLCHNG,1,1,1, CString("Type"), CString("Value")},
	{CString("Prog Cnance"),MIDI_PGMCHANGE,1,0,1, CString("Prgm"), CString("-----")},
	{CString("After Touch"), MIDI_CHNLPRESS,1,0,1, CString("Pres"), CString("----")},
	{CString("Pitch Bend"),MIDI_PITCHBEND,1,1,1, CString("LSB"), CString("MSB")},
	//----- System Status ----------------------
	{CString("TimeCode"),MIDI_TIMECODE, 0, 0, 0, CString("----"), CString("----")},
	{CString("Song Pos"),MIDI_SONGPOSITION, 1, 1, 0, CString("LSB"), CString("MSB")},
	{CString("Song Sel"),MIDI_SONGSELECT, 1, 0, 0, CString("Song #"), CString("----")},
	{CString("Reserved"),MIDI_RESERVED_F4, 0, 0, 0, CString("----"), CString("----")},
	{CString("Reserved"),MIDI_RESERVED_F5, 0, 0, 0, CString("----"), CString("----")},
	{CString("Tune Request"),MIDI_TUNEREQUEST, 0, 0, 0, CString("----"), CString("----")},
	{CString("Timing Clock"),MIDI_CLOCK, 0, 0, 0, CString("----"), CString("----")},
	{CString("Reserved"),MIDI_RTM_RSVRD_F9, 0, 0, 0, CString("----"), CString("----")},
	{CString("Start"),MIDI_START, 0, 0, 0, CString("----"), CString("----")},
	{CString("Continue"),MIDI_CONTINUE, 0, 0, 0, CString("----"), CString("----")},
	{CString("Stop"),MIDI_STOP, 0, 0, 0, CString("----"), CString("----")},
	{CString("Reserved"),MIDI_RTM_RSVRD_FD, 0, 0, 0, CString("----"), CString("----")},
	{CString("Active Sensing"),MIDI_ACTIVE_SENSING, 0, 0, 0, CString("----"), CString("----")},
	{CString("Reset"),MIDI_RESET, 0, 0, 0, CString("----"), CString("----")},
	{CString("SYSEX"),MIDI_SYSEX, 0, 0, 0, CString("----"), CString("----")},
	{CString("SYSEX END"),MIDI_SYSEXEND, 0, 0, 0, CString("----"), CString("----")}
};


/*************************************************
*	Tbles for TX816 Editor
*************************************************/
UINT TX816_AlgrithmBitmapID[APP_TX816_NUM_ALG_BITMAPS] = {
	IDB_DX7_ALG01,
	IDB_DX7_ALG02,
	IDB_DX7_ALG03,
	IDB_DX7_ALG04,
	IDB_DX7_ALG05,
	IDB_DX7_ALG06,
	IDB_DX7_ALG07,
	IDB_DX7_ALG08,
	IDB_DX7_ALG09,
	IDB_DX7_ALG10,
	IDB_DX7_ALG11,
	IDB_DX7_ALG12,
	IDB_DX7_ALG13,
	IDB_DX7_ALG14,
	IDB_DX7_ALG15,
	IDB_DX7_ALG16,
	IDB_DX7_ALG17,
	IDB_DX7_ALG18,
	IDB_DX7_ALG19,
	IDB_DX7_ALG20,
	IDB_DX7_ALG21,
	IDB_DX7_ALG22,
	IDB_DX7_ALG23,
	IDB_DX7_ALG24,
	IDB_DX7_ALG25,
	IDB_DX7_ALG26,
	IDB_DX7_ALG27,
	IDB_DX7_ALG28,
	IDB_DX7_ALG29,
	IDB_DX7_ALG30,
	IDB_DX7_ALG31,
	IDB_Dx7_ALG32
};

UINT TX816_LFO_WaveBmAPP_TX816_NUM_LFO_WAVES[APP_TX816_NUM_LFO_WAVES] = {
	IDB_TX816_LFO_TRI_0,
	IDB_TX816_LFO_SAWDOWN_1,
	IDB_TX816_LFO_SAWUP_2,
	IDB_TX816_LFO_SQUARE_3,
	IDB_TX816_LFO_SINE_4,
	IDB_TX816_LFO_SAMPLE_HOLD_5
};



//-------------------------------------
// Tx816 Editor
//-------------------------------------

CString MidiNoteToNoteString[127] = {
	_T("C -2"),	// 0
	_T("C#-2"),
	_T("D -2"),
	_T("D#-2"),
	_T("E -2"),
	_T("F -2"),
	_T("F#-2"),
	_T("G -2"),
	_T("G#-2"),
	_T("A -2"),
	_T("A#-2"),
	_T("B -2"),
	_T("C -1"),	// 12
	_T("C#-1"),
	_T("D -1"),
	_T("D#-1"),
	_T("E -1"),
	_T("F -1"),
	_T("F#-1"),
	_T("G -1"),
	_T("G#-1"),
	_T("A -1"),	// 21
	_T("A#-1"),	// 22
	_T("B -1"),	// 23
	_T("C  0"),	// 24
	_T("C# 0"),
	_T("D  0"),
	_T("D# 0"),
	_T("E  0"),
	_T("F  0"),
	_T("F# 0"),
	_T("G  0"),
	_T("G# 0"),
	_T("A  0"),
	_T("A# 0"),
	_T("B  0"),
	_T("C  1"),
	_T("C# 1"),
	_T("D  1"),
	_T("D# 1"),
	_T("E  1"),
	_T("F  1"),
	_T("F# 1"),
	_T("G  1"),
	_T("G# 1"),
	_T("A  1"),
	_T("A# 1"),
	_T("B  1"),
	_T("C  2"),
	_T("C# 2"),
	_T("D  2"),
	_T("D# 2"),
	_T("E  2"),
	_T("F  2"),
	_T("F# 2"),
	_T("G  2"),
	_T("G# 2"),
	_T("A  2"),
	_T("A# 2"),
	_T("B  2"),
	_T("C  3"),
	_T("C# 3"),
	_T("D  3"),
	_T("D# 3"),
	_T("E  3"),
	_T("F  3"),
	_T("F# 3"),
	_T("G  3"),
	_T("G# 3"),
	_T("A  3"),
	_T("A# 3"),
	_T("B  3"),
	_T("C  4"),
	_T("C# 4"),
	_T("D  4"),
	_T("D# 4"),
	_T("E  4"),
	_T("F  4"),
	_T("F# 4"),
	_T("G  4"),
	_T("G# 4"),
	_T("A  4"),
	_T("A# 4"),
	_T("B  4"),
	_T("C  5"),
	_T("C# 5"),
	_T("D  5"),
	_T("D# 5"),
	_T("E  5"),
	_T("F  5"),
	_T("F# 5"),
	_T("G  5"),
	_T("G# 5"),
	_T("A  5"),
	_T("A# 5"),
	_T("B  5"),
	_T("C  6"),
	_T("C# 6"),
	_T("D  6"),
	_T("D# 6"),
	_T("E  6"),
	_T("F  6"),
	_T("F# 6"),
	_T("G  6"),
	_T("G# 6"),
	_T("A  6"),
	_T("A# 6"),
	_T("B  6"),
	_T("C  7"),
	_T("C# 7"),
	_T("D  7"),
	_T("D# 7"),
	_T("E  7"),
	_T("F  7"),
	_T("F# 7"),
	_T("G  7"),
	_T("G# 7"),
	_T("A  7"),
	_T("A# 7"),
	_T("B  7"),
	_T("C  8"),	//120
	_T("C# 8"),
	_T("D  8"),
	_T("D# 8"),
	_T("E  8"),
	_T("F  8"),
	_T("F# 8"),
};

CString TX816_OP_KEYBORD_Level_Scaling_Curve[4] = {
	_T("-LIN"),
	_T("-EXP"),
	_T("+EXP"),
	_T("+LIN")
};

int IsChannelStatus(int StatusID)
{
	int rV = 0;
	switch (StatusID)
	{
	case STATUS_ID_MIDI_NOTEOFF:
	case STATUS_ID_MIDI_NOTEON:
	case STATUS_ID_MIDI_POLYPRESS:
	case STATUS_ID_MIDI_CTRLCHNG:
	case STATUS_ID_MIDI_PGMCHANGE:
	case STATUS_ID_MIDI_CHNLPRESS:
	case STATUS_ID_MIDI_PITCHBEND:
		rV = 1;
		break;
	case STATUS_ID_MIDI_TIMECODE:
	case STATUS_ID_MIDI_SONGPOSITION:
	case STATUS_ID_MIDI_SONGSELECT:
	case STATUS_ID_MIDI_RESERVED_F4:
	case STATUS_ID_MIDI_RESERVED_F5:
	case STATUS_ID_MIDI_TUNEREQUEST:
	case STATUS_ID_MIDI_CLOCK:
	case STATUS_ID_MIDI_RTM_RSVRD_F9:
	case STATUS_ID_MIDI_START:
	case STATUS_ID_MIDI_CONTINUE:
	case STATUS_ID_MIDI_STOP:
	case STATUS_ID_MIDI_RTM_RSVRD_FD:
	case STATUS_ID_MIDI_ACTIVE_SENSING:
	case STATUS_ID_MIDI_RESET:
	case STATUS_ID_MIDI_SYSEX:
	case STATUS_ID_MIDI_SYSEXEND:
		rV = 0;
		break;
	}
	return rV;
}

int MidiStatusToMidiID(int status)
{
	switch (status)
	{
	case MIDI_NOTEOFF:
		status = STATUS_ID_MIDI_NOTEOFF;
		break;
	case MIDI_NOTEON:
		status = STATUS_ID_MIDI_NOTEON;
		break;
	case MIDI_POLYPRESS:
		status = STATUS_ID_MIDI_POLYPRESS;
		break;
	case MIDI_CTRLCHNG:
		status = STATUS_ID_MIDI_CTRLCHNG;
		break;
	case MIDI_PGMCHANGE:
		status = STATUS_ID_MIDI_PGMCHANGE;
		break;
	case MIDI_CHNLPRESS:
		status = STATUS_ID_MIDI_CHNLPRESS;
		break;
	case MIDI_PITCHBEND:
		status = STATUS_ID_MIDI_PITCHBEND;
		break;
	case MIDI_TIMECODE:
		status = STATUS_ID_MIDI_TIMECODE;
		break;
	case MIDI_SONGPOSITION:
		status = STATUS_ID_MIDI_SONGPOSITION;
		break;
	case MIDI_SONGSELECT:
		status = STATUS_ID_MIDI_SONGSELECT;
		break;
	case MIDI_RESERVED_F4:
		status = STATUS_ID_MIDI_RESERVED_F4;
		break;
	case MIDI_RESERVED_F5:
		status = STATUS_ID_MIDI_RESERVED_F5;
		break;
	case MIDI_TUNEREQUEST:
		status = STATUS_ID_MIDI_TUNEREQUEST;
		break;
	case MIDI_CLOCK:
		status = STATUS_ID_MIDI_CLOCK;
		break;
	case MIDI_RTM_RSVRD_F9:
		status = STATUS_ID_MIDI_RTM_RSVRD_F9;
		break;
	case MIDI_START:
		status = STATUS_ID_MIDI_START;
		break;
	case MIDI_CONTINUE:
		status = STATUS_ID_MIDI_CONTINUE;
		break;
	case MIDI_STOP:
		status = STATUS_ID_MIDI_STOP;
		break;
	case MIDI_RTM_RSVRD_FD:
		status = STATUS_ID_MIDI_RTM_RSVRD_FD;
		break;
	case MIDI_ACTIVE_SENSING:
		status = STATUS_ID_MIDI_ACTIVE_SENSING;
		break;
	case MIDI_RESET:
		status = STATUS_ID_MIDI_RESET;
		break;
	case MIDI_SYSEX:
		status = STATUS_ID_MIDI_SYSEX;
		break;
	case MIDI_SYSEXEND:
		status = STATUS_ID_MIDI_SYSEXEND;
		break;
	}
	return status;
}
