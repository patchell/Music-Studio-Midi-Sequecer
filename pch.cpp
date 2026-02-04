// pch.cpp: source file corresponding to the pre-compiled header

#include "pch.h"

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
	case (int)MidiChannelCmds::NOTEOFF:
		status = STATUS_ID_MIDI_NOTEOFF;
		break;
	case (int)MidiChannelCmds::NOTEON:
		status = STATUS_ID_MIDI_NOTEON;
		break;
	case (int)MidiChannelCmds::POLYPRESS:
		status = STATUS_ID_MIDI_POLYPRESS;
		break;
	case (int)MidiChannelCmds::CTRLCHNG:
		status = STATUS_ID_MIDI_CTRLCHNG;
		break;
	case (int)MidiChannelCmds::PGMCHANGE:
		status = STATUS_ID_MIDI_PGMCHANGE;
		break;
	case (int)MidiChannelCmds::CHNLPRESS:
		status = STATUS_ID_MIDI_CHNLPRESS;
		break;
	case (int)MidiChannelCmds::PITCHBEND:
		status = STATUS_ID_MIDI_PITCHBEND;
		break;
	case (int)MidiSystemCmds::TIMECODE:
		status = STATUS_ID_MIDI_TIMECODE;
		break;
	case (int)MidiSystemCmds::SONG_POSITION:
		status = STATUS_ID_MIDI_SONGPOSITION;
		break;
	case (int)MidiSystemCmds::SONG_SELECT:
		status = STATUS_ID_MIDI_SONGSELECT;
		break;
	case (int)MidiSystemCmds::RSVRD_F4:
		status = STATUS_ID_MIDI_RESERVED_F4;
		break;
	case (int)MidiSystemCmds::RSVRD_F5:
		status = STATUS_ID_MIDI_RESERVED_F5;
		break;
	case (int)MidiSystemCmds::TUNE_REQUEST:
		status = STATUS_ID_MIDI_TUNEREQUEST;
		break;
	case (int)MidiRealTimeMsgs::CLOCK:
		status = STATUS_ID_MIDI_CLOCK;
		break;
	case (int)MidiRealTimeMsgs::RSVRD_F9:
		status = STATUS_ID_MIDI_RTM_RSVRD_F9;
		break;
	case (int)MidiRealTimeMsgs::START:
		status = STATUS_ID_MIDI_START;
		break;
	case (int)MidiRealTimeMsgs::CONTINUE:
		status = STATUS_ID_MIDI_CONTINUE;
		break;
	case (int)MidiRealTimeMsgs::STOP:
		status = STATUS_ID_MIDI_STOP;
		break;
	case (int)MidiRealTimeMsgs::RSVRD_FD:
		status = STATUS_ID_MIDI_RTM_RSVRD_FD;
		break;
	case (int)MidiRealTimeMsgs::ACTIVE_SENSING:
		status = STATUS_ID_MIDI_ACTIVE_SENSING;
		break;
	case (int)MidiRealTimeMsgs::RESET:
		status = STATUS_ID_MIDI_RESET;
		break;
	case (int)MidiSystemCmds::SYSEX:
		status = STATUS_ID_MIDI_SYSEX;
		break;
	case (int)MidiSystemCmds::SYSEXEND:
		status = STATUS_ID_MIDI_SYSEXEND;
		break;
	}
	return status;
}
