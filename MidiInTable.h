#pragma once

class CMidiInTable
{
	CMidiInDevice** m_ppTable;
	int m_nDevices;
public:
	CMidiInTable();
	~CMidiInTable();
	int AddItem(CMidiInDevice* pItem);
	int GetNumDevices() { return m_nDevices; }
	CMidiInDevice* GetMidiInDevice(int DeviceID) { return m_ppTable[DeviceID]; }
	MMRESULT Start(int MidiPortDeviceID) {
		m_ppTable[MidiPortDeviceID]->Start();
	}
	MMRESULT Open(int MidiPortDeviceID, int InID) {
		return m_ppTable[MidiPortDeviceID]->Open(InID);
	};
	MMRESULT Close(int MidiPortDeviceID) {
		return m_ppTable[MidiPortDeviceID]->Close();
	}
};

