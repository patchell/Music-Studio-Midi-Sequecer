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
	MMRESULT Close() {
		return midiOutClose(m_MidiOutHandle);
	}
	MMRESULT Send(DWORD msg) {
		return midiOutShortMsg(m_MidiOutHandle, msg);
	}
	MMRESULT NoteOn(int channel, int note, int vel) {
		MidiMsg msg;
		return Send(msg.CreateMidiMsg((char)MIDI_NOTEON, (char)channel, (char)note, (char)vel));
	}
	MMRESULT NoteOff(int channel, int note, int vel) {
		MidiMsg msg;
		return Send(msg.CreateMidiMsg((char)MIDI_NOTEOFF, (char)channel, (char)note, (char)vel));
	}
	MMRESULT PgmChange(int channel, int patch) {
		MidiMsg msg;
		return Send(msg.CreateMidiMsg((char)MIDI_PGMCHANGE, (char)channel, (char)patch));
	}
	MMRESULT Start() {
		MidiMsg msg;
		return Send(msg.CreateMidiMsg((char)MIDI_START));
	}
	MMRESULT Stop()	{
		MidiMsg msg;
		return Send(msg.CreateMidiMsg((char)MIDI_STOP));
	}
	MMRESULT Reset() {
		MidiMsg msg;
		return Send(msg.CreateMidiMsg((char)MIDI_RESET));
	}
	MMRESULT Clock() {
		MidiMsg msg;
		return Send(msg.CreateMidiMsg((char)MIDI_RESET));
	}
	MMRESULT Continue() {
		MidiMsg msg;
		return Send(msg.CreateMidiMsg((char)MIDI_CONTINUE));
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

