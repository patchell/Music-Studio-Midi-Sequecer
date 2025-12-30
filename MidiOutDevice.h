#pragma once


class CMidiOutDevice
{
	HMIDIOUT m_MidiOutHandle;
	CMidiOutDevice* m_Next;
	CMidiOutDevice* m_Prev;
	CString m_csName;
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
				(char)MidiChannelCmds::CTRLCHNG, 
				(char)channel, 
				(char)MidiCC::ALL_NOTES_OFF, 
				(char)0)
		);
	}

	MMRESULT Close() {
		return midiOutClose(m_MidiOutHandle);
	}
	MMRESULT Send(DWORD msg) {
		return midiOutShortMsg(m_MidiOutHandle, msg);
	}
	MMRESULT CtrlChange(int channel, int ctrl, int val) {
		MidiMsg msg;
		return Send(msg.CreateMidiMsg((char)MidiChannelCmds::CTRLCHNG, (char)channel, (char)ctrl, (char)val));
	}
	MMRESULT NoteOn(int channel, int note, int vel) {
		MidiMsg msg;
		return Send(msg.CreateMidiMsg((char)MidiChannelCmds::NOTEON, (char)channel, (char)note, (char)vel));
	}
	MMRESULT NoteOff(int channel, int note, int vel) {
		MidiMsg msg;
		return Send(msg.CreateMidiMsg((char)MidiChannelCmds::NOTEOFF, (char)channel, (char)note, (char)vel));
	}
	MMRESULT PgmChange(int channel, int patch) {
		MidiMsg msg;
		return Send(msg.CreateMidiMsg((char)MidiChannelCmds::PGMCHANGE, (char)channel, (char)patch));
	}
	MMRESULT Start() {
		MidiMsg msg;
		return Send(msg.CreateMidiMsg((char)MidiRealTimeMsgs::START));
	}
	MMRESULT Stop()	{
		MidiMsg msg;
		return Send(msg.CreateMidiMsg((char)MidiRealTimeMsgs::STOP));
	}
	MMRESULT Reset() {
		MidiMsg msg;
		return Send(msg.CreateMidiMsg((char)MidiRealTimeMsgs::RESET));
	}
	MMRESULT Clock() {
		MidiMsg msg;
		return Send(msg.CreateMidiMsg((char)MidiRealTimeMsgs::RESET));
	}
	MMRESULT Continue() {
		MidiMsg msg;
		return Send(msg.CreateMidiMsg((char)MidiRealTimeMsgs::CONTINUE));
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
};

