#pragma once

class CMidiControllerObject
{
	CMidiControllerObject* m_pNext;
	CMidiControllerObject* m_pPrev;
	int m_MidiInDeviceId;
	int m_MidiOutDeviceId;
	CString m_csName;
public:
	CMidiControllerObject();
	virtual ~CMidiControllerObject();
	virtual void Create();
	CMidiControllerObject* GetPrev() { return m_pPrev; }
	CMidiControllerObject* GetNext() { return m_pNext; }
	void SetPrev(CMidiControllerObject* pP) { m_pPrev = pP; }
	void SetNext(CMidiControllerObject* pN) { m_pNext = pN; }
	CString& GetName() { return m_csName; }
	void SetMidiInDevID(int id) { m_MidiInDeviceId = id; }
	int GetMidiInDevID() { return m_MidiInDeviceId; }
	void SetMidiOutDevID(int id) { m_MidiOutDeviceId = id; }
	int GetMidiOutDevID() { return m_MidiOutDeviceId; }
};

