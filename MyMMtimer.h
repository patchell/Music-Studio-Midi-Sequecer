#pragma once

class CMyMMtimer :  public CMMtimer
{
	UINT m_Count;
	UINT m_Tempo;
	BOOL m_Enable;
	DWORD m_ThreadID;
	UINT m_Message;
	UINT m_SubMessagbe;
	UINT m_MessageData;
public:
	CMyMMtimer() {
		m_Count = 0;
		m_Tempo = 0;
		m_Enable = 0;
		m_ThreadID = 0;
		m_Message = 0;
		m_SubMessagbe = 0;
		m_MessageData = 0;
	}
	virtual ~CMyMMtimer() {}
	virtual MMRESULT Create(
		DWORD ThreadID,
		UINT Msg,
		UINT SubJsg,
		UINT MsgData,
		UINT delay,
		UINT resolution,
		UINT TimerType = TIME_PERIODIC
	);
	virtual void DoCallback(
		UINT TimerID
	);
	void SetTempo(UINT tempo) { m_Tempo = tempo; }
	void Enable(BOOL en) { m_Enable = en; }
};

