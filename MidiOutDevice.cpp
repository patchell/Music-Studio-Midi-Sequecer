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
