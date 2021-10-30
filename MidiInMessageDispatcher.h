#pragma once

class CChildViewBase;

class CMidiInMessageDispatcher
{
	BOOL m_DestWindowSet : 1;
	BOOL m_WM_MessageSet : 1;
	BOOL m_MidiMessageSet : 1;
	BOOL m_MidiChannelSet : 1;
	CChildViewBase* m_pDestWindow;
	unsigned m_DestWindowID;
	DWORD m_idDestThread;
	unsigned m_WM_MessageID;	//index of entry in WM_meesage LUT
	int m_MidiDeviceID;
	int m_MidiChannel;
	int m_MidiStatusID;	//index of entry in Midi Status LUT
	int m_MidiData1Min;
	int m_MidiData1Max;
	int m_MidiData2Min;
	int m_MidiData2Max;
	CString m_csName;
	CMidiInMessageDispatcher* m_pNext;
	CMidiInMessageDispatcher* m_pPrev;
public:
	CMidiInMessageDispatcher();
	~CMidiInMessageDispatcher();
	BOOL DispatchMidiInMessage(int data1, int data2);
	BOOL DestWindowISset() { return m_DestWindowSet; }
	BOOL WMmessageISset() { return m_WM_MessageSet; }
	BOOL MidiStatusByteISset() { return m_MidiMessageSet; }
	BOOL MidiChannelISset() { return m_MidiChannelSet; }
	int GetMidiData1Max() { return m_MidiData1Max; }
	int GetMidiData1Min() { return m_MidiData1Min; }
	void SetMidiData1Max(int max) { m_MidiData1Max = max; }
	void SetMidiData1Min(int min) { m_MidiData1Min = min; }
	int GetMidiData2Max() { return m_MidiData2Max; }
	int GetMidiData2Min() { return m_MidiData2Min; }
	void SetData2Max(int max) { m_MidiData2Max = max; }
	void SetMidiData2Min(int min) { m_MidiData2Min = min; }
	int GetMidiStatusID() { return m_MidiStatusID; }
	void SetMidiStatusID(int StatusID) { 
		m_MidiStatusID = StatusID;
		m_MidiMessageSet = 1;
	}
	int GetMidiChannel() { return m_MidiChannel; }
	void SetMidiChannel(int chan) { 
		m_MidiChannel = chan; 
		m_MidiChannelSet = TRUE;
	}
	void SetDestinationWindow(CChildViewBase* pWnd, int ID) 
	{ 
		m_pDestWindow = pWnd; 
		m_DestWindowID = ID;
		m_DestWindowSet = TRUE;
	}
	unsigned GetDestinationID() { return m_DestWindowID; }
	CChildViewBase* GetDestinationWindow() { return m_pDestWindow; }
	void SetWindowMessageID(unsigned WM_messID) { 
		m_WM_MessageID = WM_messID;
		m_WM_MessageSet = TRUE;
	}
	unsigned GetWindowMessageID() { return m_WM_MessageID; }
	int GetMidiDeviceID() { return m_MidiDeviceID; }
	void SetMidiDeviceID(int Id) { m_MidiDeviceID = Id; }
	CString& GetDispatchName() { return m_csName; }
	void SetDispatchName(CString csName) {
		m_csName = csName;
	}
	int GetStatusByteValue();
	//------------------------------------------
	// Dispatcher List Management
	//------------------------------------------
	void SetNext(CMidiInMessageDispatcher* pNext) { m_pNext = pNext; }
	CMidiInMessageDispatcher* GetNext() { return m_pNext; }
	void SetPrev(CMidiInMessageDispatcher* pPrev) { m_pPrev = pPrev; }
	CMidiInMessageDispatcher* GetPrev() { return m_pPrev; }
};

