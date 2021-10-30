
#include "pch.h"

CSysExTX816::CSysExTX816()
{
	m_pOutPort = 0;
}

CSysExTX816::~CSysExTX816()
{

}

void CSysExTX816::Create(CMidiOutDevice* pOutPort)
{

}

void CSysExTX816::TX816_VoiceBulkData(int channel, UCHAR* pData)
{

}

void CSysExTX816::TX816_PerformanceBulkData(int channel, UCHAR* pData)
{

}

void CSysExTX816::TX816_PerformanceBulkData64(int channel, UCHAR* pData)
{

}

void CSysExTX816::TX816_VoiceBulkData32(int channel, UCHAR* pData)
{

}

void CSysExTX816::TX816_ParameterChange(int channel, int groupNumber, int subgroup, int parameterNumber, int parameter)
{
	//---------------------------------------
	// TX816_ParameterChange
	//	Poorly documented (wrong) in the TX816 user
	//	Manual.  Seems to actually be the following
	// 0xf0.............SysEx Status
	// 0x43.............ID
	// 0x1c.............Sub ID/Channel  c=> channel
	// 0xgh.............Group/Subgroup
	//                  h=> 0->3 mask 0x03
	//                  g=> 0->31 mask 0x7c shift 2
	// 0xnn.............nn=> 0-> 127 parameter number
	// 0xvv.............Value vv=>0->127
	// 0xf7.............SysEX End
	//------------------------------------------------
}

void CSysExTX816::DumpRequest(int chan, int format)
{

}