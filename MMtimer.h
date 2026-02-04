#pragma once

class CMMtimer
{
	UINT m_TimerID;
	UINT m_Resolution;
	static void CALLBACK TimerCallback(
		UINT TimerID,
		UINT message_DontUse,
		DWORD UserData,
		DWORD DontUse1,
		DWORD DontUse2
	);
public:
	CMMtimer() {
		m_TimerID = 0;
		m_Resolution = 0;
	};
	virtual ~CMMtimer() {}
	virtual void Kill() {
		timeKillEvent(m_TimerID);
		EndPeriod(m_Resolution);
	}
	MMRESULT BeginPeriod(DWORD timerRes) {
		return timeBeginPeriod(timerRes);
	}
	MMRESULT EndPeriod(DWORD timerRes) {
		return timeEndPeriod(timerRes);
	}
	virtual MMRESULT Create(
		UINT delay,
		UINT resolution,
		UINT TimerType = TIME_PERIODIC
	)
	{
		MMRESULT mmResult;

		mmResult = BeginPeriod(resolution);
		if (TIMERR_NOCANDO == mmResult)
		{
			if (LogFile()) 
				fprintf(LogFile(), "Could not begin Period Bad Resolution(?) %d\n", resolution);
		}
		else
		{
			mmResult = timeSetEvent(
				delay,
				resolution,
				(LPTIMECALLBACK)CMMtimer::TimerCallback,
				DWORD_PTR(this),
				TimerType
			);
			if (0 < mmResult)
				m_TimerID = mmResult;
			else
				EndPeriod(resolution);
		}
		return mmResult;
	}
	DWORD GetTime() { return timeGetTime(); }
	MMRESULT GetSystemTime(MMTIME* SysTime, UINT cbmmt) {
		return timeGetSystemTime(SysTime, cbmmt);
	}
	MMRESULT GetDevCaps(TIMECAPS* timeCaps, UINT cbtc) {
		return timeGetDevCaps(timeCaps, cbtc);
	}
	virtual void DoCallback(
		UINT TimerID
	) {}
};

