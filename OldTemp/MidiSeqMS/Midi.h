#pragma once

class CMidi
{
	static DWORD MidiWorker(LPVOID pP);
	HMIDIIN m_hMidiIn;
	HMIDIOUT m_hMidiOut;

	MIDIHDR m_MidiIn1;
	MIDIHDR m_MidiIn2;

	CWnd* m_pDestWindow;	//window to send message to
	DWORD m_nThreadID;
	HANDLE m_hThreadHandle;
	//-------------------------
	// Edit Mode Variablews
	//-------------------------
	int m_EditNote;
	int m_EditRest;
	int m_EditAccent;
	int m_EditDecorations;
	int m_EditAccidentals;
	int m_EditInstrument;
	int m_EditTimeSignature;
	int m_EditKeySignature;
	int m_EditMisc;
public:
	CMidi();
	~CMidi();
	void Create(int OutId, int InId);
	DWORD MidiInThread();
	int KillThead();
	void DecodeShortMidiMessageShort(int Chan, int Cmd, int Note, int Vel);
	//------------------------------------------
	HMIDIOUT GetMidiOutHandle() { return m_hMidiOut; }
	HMIDIIN	GetMidiInHandle() { return m_hMidiIn; }
	void SetMessageDestWind(CWnd* pW) { m_pDestWindow = pW; }
	//-----------------------------------------
	// Edit Mode Methodes
	//-----------------------------------------
	void ResetModes() {
		m_EditNote = MIDIEDIT_QUARTER;
		m_EditRest = MIDIEDIT_QUARTER;
		m_EditAccent = MIDIEDIT_ACCENT_OFF;
		m_EditDecorations = MIDIEDIT_DECORATIONS_NONE;
		m_EditAccidentals = MIDIEDIT_ACCIDENTALS_NONE;
		m_EditInstrument = MIDIEDIT_INST7;
		m_EditKeySignature = MIDIEDIT_KEY_CMAJ;
		m_EditTimeSignature = MIDIEDIT_TIMESIG_4_4;
		m_EditMisc = MIDIEDIT_MISC_MEASUREBAR;
	}
	void UpdateScreenControls();
};

