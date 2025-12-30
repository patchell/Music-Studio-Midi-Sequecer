#include "pch.h"

CMidiOutTable::CMidiOutTable()
{
	m_nDevices = 0;
	m_ppTable = 0;
}

CMidiOutTable::~CMidiOutTable()
{
	if (m_ppTable)
	{
		for (int i = 0; i < m_nDevices; ++i)
			delete m_ppTable[i];
		delete[] m_ppTable;
	}
}

bool CMidiOutTable::Create()
{
	m_nDevices = midiOutGetNumDevs();
	MIDIOUTCAPS MidiOut;
	CString csName;
	int i, l = 0;

	m_ppTable = new CMidiOutDevice*[m_nDevices];
	for (i = 0; i < m_nDevices; ++i)
	{
		midiOutGetDevCaps(i, &MidiOut, sizeof(MIDIOUTCAPS));
		m_ppTable[i] = new CMidiOutDevice;
		csName = MidiOut.szPname;
		m_ppTable[i]->SetName(csName);
		m_ppTable[i]->Create(i);
	}
	return true;
}

int CMidiOutTable::AddItem(CMidiOutDevice* pItem)
{
	int DeviceID = m_nDevices;

	if (0 == m_nDevices)
	{
		m_ppTable = new CMidiOutDevice * [m_nDevices + 1];
		m_ppTable[m_nDevices] = pItem;
	}
	else
	{
		CMidiOutDevice** ppTemp;
		ppTemp = new CMidiOutDevice * [m_nDevices + 1];
		int i;
		for ( i = 0; i < m_nDevices; ++i)
			ppTemp[i] = m_ppTable[i];
		delete[]m_ppTable;
		m_ppTable = ppTemp;
		m_ppTable[i] = pItem;
	}
	++m_nDevices;
	return DeviceID;
}
