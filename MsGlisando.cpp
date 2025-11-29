#include "pch.h"

CMsGlisando::CMsGlisando():CMsObject()
{
    m_ObjType = MSOBJ_GLISANDO;
	m_pEvEnd = nullptr;
	m_EndPitch = 0;
	m_StartPitch = 0;
	m_TotalIntervals = 0;
	m_CurrentPitch = 0;
	m_TicksPerInterval = 0;
	m_GlisandoState = GlisandoState::END;
	m_GlisandoDirection = GlisandoDirection::UP;	
	m_Track = COMBO_Index_INST_7;
}

CMsGlisando::~CMsGlisando()
{
}

bool CMsGlisando::Create(CMsSong* pSong, CMsEvent* pEvStart, CMsEvent* pEvEnd)
{
	CMsObject::Create(pSong, pEvStart);
	m_pEvEnd = pEvEnd;
	return true;
}

UINT CMsGlisando::Process()
{
	m_CurrentPitch = m_StartPitch;
	m_TotalIntervals = m_StartPitch - m_EndPitch;
	if (m_TotalIntervals < 0)
	{
		m_GlisandoDirection = GlisandoDirection::UP;
		m_TotalIntervals = -m_TotalIntervals;
	}
	m_TotalDurationTicks = CalculateTotalDuration();
	int TicksPerInterval = 0;
	int RemainderTicks = 0;
	if (m_TotalIntervals)
	{
		TicksPerInterval = m_TotalDurationTicks / m_TotalIntervals;
		RemainderTicks = m_TotalDurationTicks % m_TotalIntervals;
		if(RemainderTicks)
		{
			// Adjust ticks per interval to account for remainder
			m_TotalDurationTicks = m_TotalIntervals - RemainderTicks;
			TicksPerInterval = m_TotalDurationTicks / m_TotalIntervals;
		}
	}
	m_GlisandoState = GlisandoState::START;
	return 0;

}

UINT CMsGlisando::Play()
{
	UINT ret = 0;	
	switch(m_GlisandoState)
	{
		case GlisandoState::START:
			m_Ticks = m_TicksPerInterval;
			NoteOn(127);
			m_GlisandoState = GlisandoState::RUNNING;
			break;
		case GlisandoState::RUNNING:
			if (--m_Ticks <= 0)
			{
				NoteOff(0);
				if(m_TotalIntervals--)
				{
					if (m_GlisandoDirection == GlisandoDirection::UP)
						m_CurrentPitch++;
					else
						m_CurrentPitch--;
					NoteOn(127);
					m_Ticks = m_TicksPerInterval;
				}
				else
				{
					m_GlisandoState = GlisandoState::END;
					ret = PLAY_OBJECT_DONE;
				}
			}
			else
			{
				m_Ticks--;
			}
			break;
		case GlisandoState::END:
			break;
	}
	return ret;
}

DRAWSTATE CMsGlisando::MouseLButtonDown(DRAWSTATE DrawState, CPoint pointMouse)
{
	return DrawState;
}

DRAWSTATE CMsGlisando::MouseLButtonUp(DRAWSTATE DrawState, CPoint pointMouse)
{
	return DrawState;
}

DRAWSTATE CMsGlisando::MouseMove(DRAWSTATE DrawState, CPoint pointMouse)
{
	return DrawState;
}

int CMsGlisando::IsTimedObject()
{
	return COMBO_NOTE_QUARTER;
}

void CMsGlisando::NoteOn(UINT Velociry)
{
	GetSong()->IncNoteOnCount();
	int chan = GETMIDIINFO->GetChannel(GetTrack());
	int note = m_CurrentPitch + CMsNote::RangeLUT[GETMIDIINFO->GetRange(GetTrack())];
	int DeviceID = GETMIDIINFO->GetMidiOutDeviceId(GetTrack());
	GETMIDIOUTTABLE.NoteOn(DeviceID, chan, note, Velociry);
}

void CMsGlisando::NoteOff(UINT Velociry)
{
	GetSong()->IncNoteOffCount();
	int chan = GETMIDIINFO->GetChannel(GetTrack());
	int note = m_CurrentPitch + CMsNote::RangeLUT[GETMIDIINFO->GetRange(GetTrack())];
	int DeviceID = GETMIDIINFO->GetMidiOutDeviceId(GetTrack());
	GETMIDIOUTTABLE.NoteOff(DeviceID, chan, note, Velociry);
}

CMsObject* CMsGlisando::Copy(void)
{
    return nullptr;
}

void CMsGlisando::Draw(CDC* pDC, int event, int maxevent)
{
}

void CMsGlisando::Save(FILE* pO)
{
}

void CMsGlisando::Print(FILE* pO, int Indent)
{
}

int CMsGlisando::CalculateTotalDuration()
{
	CMsEvent* pEvent = GetParentEvent();
	CMsEvent* pEvEnd = m_pEvEnd;
	bool Loop = true;
	int Duration = 0;
	int m_Ticks = 0;

	while (Loop)
	{
		Duration = pEvent->GetMinEventDuration();
		if(Duration)
			m_Ticks += CMsNote::GetDurationTable()[Duration].DurationTimeTicks();
		if (pEvent == pEvEnd)
			Loop = false;
		else
			pEvent = pEvent->GetNext();
	}
	return m_Ticks;
}
