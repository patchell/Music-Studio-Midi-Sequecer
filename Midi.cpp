#include "pch.h"


bool CMidi::IsChannelStatus(CMidi::MidiStatusDispatchIDs StatusID)
{
	bool rV = false;

	switch (StatusID)
	{
	case CMidi::MidiStatusDispatchIDs::DISPATCH_STATUS_ID_MIDI_NOTEOFF:
	case CMidi::MidiStatusDispatchIDs::DISPATCH_STATUS_ID_MIDI_NOTEON:
	case CMidi::MidiStatusDispatchIDs::DISPATCH_STATUS_ID_MIDI_POLYPRESS:
	case CMidi::MidiStatusDispatchIDs::DISPATCH_STATUS_ID_MIDI_CTRLCHNG:
	case CMidi::MidiStatusDispatchIDs::DISPATCH_STATUS_ID_MIDI_PGMCHANGE:
	case CMidi::MidiStatusDispatchIDs::DISPATCH_STATUS_ID_MIDI_CHNLPRESS:
	case CMidi::MidiStatusDispatchIDs::DISPATCH_STATUS_ID_MIDI_PITCHBEND:
		rV = true;
		break;
	case CMidi::MidiStatusDispatchIDs::DISPATCH_STATUS_ID_MIDI_TIMECODE:
	case CMidi::MidiStatusDispatchIDs::DISPATCH_STATUS_ID_MIDI_SONGPOSITION:
	case CMidi::MidiStatusDispatchIDs::DISPATCH_STATUS_ID_MIDI_SONGSELECT:
	case CMidi::MidiStatusDispatchIDs::DISPATCH_STATUS_ID_MIDI_RESERVED_F4:
	case CMidi::MidiStatusDispatchIDs::DISPATCH_STATUS_ID_MIDI_RESERVED_F5:
	case CMidi::MidiStatusDispatchIDs::DISPATCH_STATUS_ID_MIDI_TUNEREQUEST:
	case CMidi::MidiStatusDispatchIDs::DISPATCH_STATUS_ID_MIDI_CLOCK:
	case CMidi::MidiStatusDispatchIDs::DISPATCH_STATUS_ID_MIDI_RTM_RSVRD_F9:
	case CMidi::MidiStatusDispatchIDs::DISPATCH_STATUS_ID_MIDI_START:
	case CMidi::MidiStatusDispatchIDs::DISPATCH_STATUS_ID_MIDI_CONTINUE:
	case CMidi::MidiStatusDispatchIDs::DISPATCH_STATUS_ID_MIDI_STOP:
	case CMidi::MidiStatusDispatchIDs::DISPATCH_STATUS_ID_MIDI_RTM_RSVRD_FD:
	case CMidi::MidiStatusDispatchIDs::DISPATCH_STATUS_ID_MIDI_ACTIVE_SENSING:
	case CMidi::MidiStatusDispatchIDs::DISPATCH_STATUS_ID_MIDI_RESET:
	case CMidi::MidiStatusDispatchIDs::DISPATCH_STATUS_ID_MIDI_SYSEX:
	case CMidi::MidiStatusDispatchIDs::DISPATCH_STATUS_ID_MIDI_SYSEXEND:
		rV = false;
		break;
	}
	return rV;
}


CMidi::MidiStatusDispatchIDs CMidi::MidiStatusToMidiID(int Status)
{
	MidiStatusDispatchIDs status =MidiStatusDispatchIDs::DISPATCH_STATUS_ID_MIDI_ERROR;

	switch (Status)
	{
	case (int)CMidi::MidiChannelCmds::NOTEOFF:
		status = MidiStatusDispatchIDs::DISPATCH_STATUS_ID_MIDI_NOTEOFF;
		break;
	case (int)CMidi::MidiChannelCmds::NOTEON:
		status = MidiStatusDispatchIDs::DISPATCH_STATUS_ID_MIDI_NOTEON;
		break;
	case (int)CMidi::MidiChannelCmds::POLYPRESS:
		status = MidiStatusDispatchIDs::DISPATCH_STATUS_ID_MIDI_POLYPRESS;
		break;
	case (int)CMidi::MidiChannelCmds::CTRLCHNG:
		status = MidiStatusDispatchIDs::DISPATCH_STATUS_ID_MIDI_CTRLCHNG;
		break;
	case (int)CMidi::MidiChannelCmds::PGMCHANGE:
		status = MidiStatusDispatchIDs::DISPATCH_STATUS_ID_MIDI_PGMCHANGE;
		break;
	case (int)CMidi::MidiChannelCmds::CHNLPRESS:
		status = MidiStatusDispatchIDs::DISPATCH_STATUS_ID_MIDI_CHNLPRESS;
		break;
	case (int)CMidi::MidiChannelCmds::PITCHBEND:
		status = MidiStatusDispatchIDs::DISPATCH_STATUS_ID_MIDI_PITCHBEND;
		break;
	case (int)CMidi::MidiSystemCmds::TIMECODE:
		status = MidiStatusDispatchIDs::DISPATCH_STATUS_ID_MIDI_TIMECODE;
		break;
	case (int)CMidi::MidiSystemCmds::SONG_POSITION:
		status = MidiStatusDispatchIDs::DISPATCH_STATUS_ID_MIDI_SONGPOSITION;
		break;
	case (int)CMidi::MidiSystemCmds::SONG_SELECT:
		status = MidiStatusDispatchIDs::DISPATCH_STATUS_ID_MIDI_SONGSELECT;
		break;
	case (int)CMidi::MidiSystemCmds::RSVRD_F4:
		status = MidiStatusDispatchIDs::DISPATCH_STATUS_ID_MIDI_RESERVED_F4;
		break;
	case (int)CMidi::MidiSystemCmds::RSVRD_F5:
		status = MidiStatusDispatchIDs::DISPATCH_STATUS_ID_MIDI_RESERVED_F5;
		break;
	case (int)CMidi::MidiSystemCmds::TUNE_REQUEST:
		status = MidiStatusDispatchIDs::DISPATCH_STATUS_ID_MIDI_TUNEREQUEST;
		break;
	case (int)CMidi::MidiRealTimeMsgs::CLOCK:
		status = MidiStatusDispatchIDs::DISPATCH_STATUS_ID_MIDI_CLOCK;
		break;
	case (int)CMidi::MidiRealTimeMsgs::RSVRD_F9:
		status = MidiStatusDispatchIDs::DISPATCH_STATUS_ID_MIDI_RTM_RSVRD_F9;
		break;
	case (int)CMidi::MidiRealTimeMsgs::START:
		status = MidiStatusDispatchIDs::DISPATCH_STATUS_ID_MIDI_START;
		break;
	case (int)CMidi::MidiRealTimeMsgs::CONTINUE:
		status = MidiStatusDispatchIDs::DISPATCH_STATUS_ID_MIDI_CONTINUE;
		break;
	case (int)CMidi::MidiRealTimeMsgs::STOP:
		status = MidiStatusDispatchIDs::DISPATCH_STATUS_ID_MIDI_STOP;
		break;
	case (int)CMidi::MidiRealTimeMsgs::RSVRD_FD:
		status = MidiStatusDispatchIDs::DISPATCH_STATUS_ID_MIDI_RTM_RSVRD_FD;
		break;
	case (int)CMidi::MidiRealTimeMsgs::ACTIVE_SENSING:
		status = MidiStatusDispatchIDs::DISPATCH_STATUS_ID_MIDI_ACTIVE_SENSING;
		break;
	case (int)CMidi::MidiRealTimeMsgs::RESET:
		status = MidiStatusDispatchIDs::DISPATCH_STATUS_ID_MIDI_RESET;
		break;
	case (int)CMidi::MidiSystemCmds::SYSEX:
		status = MidiStatusDispatchIDs::DISPATCH_STATUS_ID_MIDI_SYSEX;
		break;
	case (int)CMidi::MidiSystemCmds::SYSEXEND:
		status = MidiStatusDispatchIDs::DISPATCH_STATUS_ID_MIDI_SYSEXEND;
		break;
	}
	return status;
}
