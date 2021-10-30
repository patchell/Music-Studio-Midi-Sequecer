#include "pch.h"

void CALLBACK CMMtimer::TimerCallback(
	UINT TimerID,
	UINT message_DontUse,
	DWORD UserData,
	DWORD DontUse1,
	DWORD DontUse2
)
{
	CMMtimer* pMMtmr = (CMMtimer*)UserData;
	pMMtmr->DoCallback(TimerID);
}
