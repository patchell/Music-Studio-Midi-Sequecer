#pragma once

class CMidiOutTable
{
	CMidiOutDevice** m_ppTable;
	int m_nDevices;
public:
	CMidiOutTable();
	virtual ~CMidiOutTable();
	bool Create();
	int AddItem(CMidiOutDevice* pItem);
	MMRESULT Send(int DeviceID, DWORD message) { return m_ppTable[DeviceID]->Send(message); }
	MMRESULT Open(int DeviceID, int OutID) {
		//-------------------------------------
		// DeviceID....Id of the Output Device
		// OutID........The actuall Windows API
		//				Device number
		//-------------------------------------
		return m_ppTable[DeviceID]->Open(OutID);
	};
	MMRESULT Close(int DeviceID) {
		return m_ppTable[DeviceID]->Close();
	}
	MMRESULT NoteOn(int DeviceID, int channel, int note, int vel) {
		return m_ppTable[DeviceID]->NoteOn(channel, note, vel);
	}
	MMRESULT NoteOff(int DeviceID, int channel, int note, int vel) {
		return m_ppTable[DeviceID]->NoteOff(channel, note, vel);
	}
	MMRESULT PgmChange(int DeviceID, int channel, int patch) {
		return m_ppTable[DeviceID]->PgmChange(channel, patch);
	}
		MMRESULT Clock(int DeviceID) {
		return m_ppTable[DeviceID]->Clock();
	}
	MMRESULT Start(int DeviceID) {
		return m_ppTable[DeviceID]->Start();
	}
	MMRESULT Stop(int DeviceID) {
		return m_ppTable[DeviceID]->Stop();
	}
	MMRESULT Continue(int DeviceID) {
		return m_ppTable[DeviceID]->Continue();
	}
	MMRESULT SendLongMsg(int DeviceID, MIDIHDR* midiHDR) {
		return  m_ppTable[DeviceID]->SendLongMsg(midiHDR);
	}
	HMIDIOUT GetOutHandle(int OutID) {
		m_ppTable[OutID]->GetOutHandle();
	}
	int GetNumDevices() { return m_nDevices; }
	CMidiOutDevice& GetDevice(int DevID) { 
		return *m_ppTable[DevID]; 
	}
};



