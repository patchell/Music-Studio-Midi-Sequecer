#include "pch.h"

MMRESULT CMyMMtimer::Create(
	DWORD ThreadID,
	UINT Msg,
	UINT SubMsg,
	UINT MsgData,
	UINT delay,
	UINT resolution,
	UINT TimerType
)
{
	m_ThreadID = ThreadID;
	m_Message = Msg;
	m_SubMessagbe = SubMsg;
	m_MessageData = MsgData;
	return CMMtimer::Create(delay, resolution, TimerType);;
}

void CMyMMtimer::DoCallback(UINT TimerID)
{
	if (m_Enable)
	{
		if (0 == m_Count--)
		{
			m_Count = m_Tempo;
			if(m_Message)
				::PostThreadMessageW(m_ThreadID, m_Message, m_SubMessagbe, m_MessageData);
		}
	}
}
