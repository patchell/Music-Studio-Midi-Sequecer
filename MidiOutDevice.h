#pragma once


class CMidiOutDevice
{
	HMIDIOUT m_MidiOutHandle;
	CMidiOutDevice* m_Next;
	CMidiOutDevice* m_Prev;
	CString m_csName;
	struct MidiStatusStringItem {
		int m_Status;
		int m_ByteCount;
		const char* m_pName;
	};
	inline static MidiStatusStringItem MidiStatusStringLUT[] = {
		{ 0x80, 3, "Note Off" },
		{ 0x90, 3, "Note On" },
		{ 0xA0, 3, "Polyphonic Key Pressure" },
		{ 0xB0, 3, "Control Change" },
		{ 0xC0, 2, "Program Change" },
		{ 0xD0, 2, "Channel Pressure" },
		{ 0xE0, 3, "Pitch Bend Change" },
		{ 0xF0, 1, "System Exclusive" },
		{ 0xF1, 1, "MIDI Time Code Quarter Frame" },
		{ 0xF2, 3, "Song Position Pointer" },
		{ 0xF3, 2, "Song Select" },
		{ 0xF4, 1, "Undefined" },
		{ 0xF5, 1, "Undefined" },
		{ 0xF6, 1, "Tune Request" },
		{ 0xF7, 1, "End of SysEx" },
		{ 0xF8, 1, "Clock" },
		{ 0xF9, 1, "Reserved" },
		{ 0xFA, 1, "Start" },
		{ 0xFB, 1, "Continue" },
		{ 0xFC, 1, "Stop" },
		{ 0xFD, 1, "Reserved" },
		{ 0xFE, 1, "Active Sensing" },
		{ 0xFF, 1, "Reset" }
	};
public:
	CMidiOutDevice();
	~CMidiOutDevice();
	bool Create(int Id);
	HMIDIOUT GetOutHandle() { return m_MidiOutHandle; }
	MMRESULT  Open(UINT OutId) {
		return midiOutOpen(
			&m_MidiOutHandle,
			OutId,
			NULL,
			NULL,
			CALLBACK_NULL
		);
	}
	MMRESULT SendAllNotesOff(int channel) {
		MidiMsg msg;
		return Send(
			msg.CreateMidiMsg(
				(char)CMidi::MidiChannelCmds::CTRLCHNG,
				(char)channel, 
				(char)CMidi::MidiCC::ALL_NOTES_OFF,
				(char)0)
		);
	}

	MMRESULT Close() {
		return midiOutClose(m_MidiOutHandle);
	}
	MMRESULT Send(DWORD msg) {
		printf("Sending MIDI Message: %08x\n", msg);
		return midiOutShortMsg(m_MidiOutHandle, msg);
	}
	MMRESULT CtrlChange(int channel, int ctrl, int val) {
		MidiMsg msg;
		return Send(msg.CreateMidiMsg((char)CMidi::MidiChannelCmds::CTRLCHNG, (char)channel, (char)ctrl, (char)val));
	}
	MMRESULT NoteOn(int channel, int note, int vel) {
		MidiMsg msg;
		return Send(msg.CreateMidiMsg((char)CMidi::MidiChannelCmds::NOTEON, (char)channel, (char)note, (char)vel));
	}
	MMRESULT NoteOff(int channel, int note, int vel) {
		MidiMsg msg;
		return Send(msg.CreateMidiMsg((char)CMidi::MidiChannelCmds::NOTEOFF, (char)channel, (char)note, (char)vel));
	}
	MMRESULT PgmChange(
		int channel,	// logical channel number 1-16, not zero based
		int patch) 
	{
		MMRESULT rV = 0;
		MidiMsg msg;

		msg.CreateMidiMsg((char)CMidi::MidiChannelCmds::PGMCHANGE, (char)channel, (char)patch);
		rV = Send(msg.msg);
		return rV;
	}
	MMRESULT Start() {
		MidiMsg msg;
		return Send(msg.CreateMidiMsg((char)CMidi::MidiRealTimeMsgs::START));
	}
	MMRESULT Stop()	{
		MidiMsg msg;
		return Send(msg.CreateMidiMsg((char)CMidi::MidiRealTimeMsgs::STOP));
	}
	MMRESULT Reset() {
		MidiMsg msg;
		return Send(msg.CreateMidiMsg((char)CMidi::MidiRealTimeMsgs::RESET));
	}
	MMRESULT Clock() {
		MidiMsg msg;
		return Send(msg.CreateMidiMsg((char)CMidi::MidiRealTimeMsgs::RESET));
	}
	MMRESULT Continue() {
		MidiMsg msg;
		return Send(msg.CreateMidiMsg((char)CMidi::MidiRealTimeMsgs::CONTINUE));
	}
	MMRESULT SendLongMsg(MIDIHDR* midiHDR) {
		midiOutPrepareHeader(m_MidiOutHandle, midiHDR, sizeof(MIDIHDR));
		return midiOutLongMsg(m_MidiOutHandle, midiHDR,sizeof(MIDIHDR));
	}
	CMidiOutDevice* GetNext() { return m_Next; }
	void SetNext(CMidiOutDevice* Next) { m_Next = Next; }
	CMidiOutDevice* GetPrev() { return m_Prev; }
	void SetPrev(CMidiOutDevice* Prev) { m_Prev = Prev; }
	CString& csGetName() { return m_csName; }
	void SetName(CString& csName) {
		m_csName = csName;
	}
	static const char* GetMidiStatusString(int status);
	static const int GetMidiStatusByteCount(int status);
	static const char* MidiCmdToString(int cmd);
};

