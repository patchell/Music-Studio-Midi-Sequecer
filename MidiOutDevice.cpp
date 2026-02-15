#include "pch.h"

CMidiOutDevice::CMidiOutDevice()
{
	m_MidiOutHandle = 0;
	m_Next = 0;
	m_Prev = 0;
}

CMidiOutDevice::~CMidiOutDevice()
{
}

bool CMidiOutDevice::Create(int OutId)
{
	Open(OutId);
	return false;
}

const char* CMidiOutDevice::GetMidiStatusString(int status)
{
	const char* pStatusString = "Not a Status";

	status &= 0xF0;	// Mask out channel number if it's a channel message
	if (status >= 0x80)
	{
		for (const auto& item : MidiStatusStringLUT) 
		{
			if (item.m_Status == status) 
			{
				pStatusString = item.m_pName;
				break;
			}
		}
	}
	return pStatusString;
}

const int CMidiOutDevice::GetMidiStatusByteCount(int status)
{
	int byteCount = 0;

	status &= 0xF0;	// Mask out channel number if it's a channel message
	if (status >= 0x80)
	{
		for (const auto& item : MidiStatusStringLUT)
		{
			if (item.m_Status == status)
			{
				byteCount = item.m_ByteCount;
				break;
			}
		}
	}
	return byteCount;
}

const char* CMidiOutDevice::MidiCmdToString(int cmd)
{
	return nullptr;
}
