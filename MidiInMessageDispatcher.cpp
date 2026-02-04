#include "pch.h"

CMidiInMessageDispatcher::CMidiInMessageDispatcher()
{
    m_DestWindowSet = 0;
    m_MidiChannelSet = 0;
    m_WM_MessageSet = 0;
    m_MidiMessageSet = 0;
    m_pDestWindow = 0;
    m_DestWindowID = 0;
    m_idDestThread = 0;
    m_WM_MessageID = 0;	//index of entry in LUT
    m_MidiDeviceID = 0;
    m_MidiChannel = 0;
    m_MidiStatusID = 0;	    //index of entry in LUT
    m_MidiData1Min = 0;
    m_MidiData1Max = 0;
    m_MidiData2Min = 0;
    m_MidiData2Max = 0;
    m_pNext = 0;
    m_pPrev = 0;
}

CMidiInMessageDispatcher::~CMidiInMessageDispatcher()
{
}

int CMidiInMessageDispatcher::GetStatusByteValue()
{
    return 0;
}

bool CMidiInMessageDispatcher::DispatchMidiInMessage(int data1, int data2)
{
    bool rV = false;
    bool DATA1 = false;
    bool DATA2 = false;
    MidiMsg Msg;

    DATA1 = ((m_MidiData1Min == 0) && (m_MidiData1Max == 0)) || //all data1 values
        ((m_MidiData1Max == m_MidiData1Min) && (m_MidiData1Max == data1)) || //Just 1 data1 Value
        ((m_MidiData1Min <= data1) && (data1 <= m_MidiData1Max));   //range of values
    DATA2 = ((m_MidiData2Min == 0) && (m_MidiData2Max == 0)) || //all data1 values
        ((m_MidiData2Max == m_MidiData2Min) && (m_MidiData2Max == data2)) || //Just 1 data1 Value
        ((m_MidiData2Min <= data2) && (data2 <= m_MidiData2Max));   //range of values
    if (DATA1 && DATA2)  //in range, post message
    {
        if (m_pDestWindow)
        {
            m_pDestWindow->PostMessageW(
                m_WM_MessageID, 
                Msg.CreateMidiMsg(
                    CMidiSeqMSApp::GetMidiStatusCommands()[m_MidiStatusID].m_MidiStatus, 
                    m_MidiChannel, data1,
                    data2
                )
            );
            rV = true;
        }
     }
    return rV;
}
