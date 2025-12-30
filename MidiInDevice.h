#pragma once

class CMidiInDevice
{
	HMIDIIN m_hMidiIn;
	MIDIHDR m_mhMidiIn1;
	MIDIHDR m_mhMidiIn2;
	CString m_csPortName;
	CMyEvent m_evMidiLongDone;
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
	//--------------------------
	// Midi In Thread
	//--------------------------
	HANDLE m_hMidiInThreadHandle;
	DWORD m_nMidiInThreadID;
	CWnd* m_pDestWindow;
	CMidiInMessageDispatcher* m_pHead;
	CMidiInMessageDispatcher* m_pTail;
	static UINT MidiInWorkerThread(LPVOID pP);
public:
	CMidiInDevice();
	~CMidiInDevice();
	MMRESULT Open(int InId);
	MMRESULT Close() { return ::midiInClose(m_hMidiIn); }
	MMRESULT Start() { return ::midiInStart(m_hMidiIn); }
	UINT MidiInThread();
	int KillThread();
	CMyEvent& GetMidiLongComplete() { return m_evMidiLongDone; }
	//-------------------
	//	Attribute methods
	//-------------------
	void SetMidiInHandle(HMIDIIN hIn) { m_hMidiIn = hIn; }
	HMIDIIN GetMidiInHandle() { return m_hMidiIn; }
	void SetPortName(CString csName) { m_csPortName = csName;}
	CString& GetPortName() { return m_csPortName; }
	//-------------------------------------
	// Dispatcher Methods
	//-------------------------------------
	CMidiInMessageDispatcher* GetDispatcherHead() { return m_pHead; }
	CMidiInMessageDispatcher* GetDispatcherTail() { return m_pTail; }
protected:
	void SetDispatcherHead(CMidiInMessageDispatcher* pH) { m_pHead = pH; }
	void SetDispatcherTail(CMidiInMessageDispatcher* pT) { m_pTail = pT; }
public:
	void AddDispatcherToHead(CMidiInMessageDispatcher* pMIMD);
	void AddDispatcherToTail(CMidiInMessageDispatcher* pMIMD);
	void RemoveDispatcher(CMidiInMessageDispatcher* pMIMD);
	int CheckDispatcherList();
	void DispatchShortMessage(WPARAM wparam, LPARAM lparam);
	void DispatchLongMessage(WPARAM wParam, LPARAM lParam);
};

