#include "pch.h"

CMsSongInfo::CMsSongInfo()
{
	memset(m_TracksEnable, 0, sizeof(int) * 4);
	memset(m_SongTitle, 0, sizeof(m_SongTitle));
	m_pParentSong = nullptr;
}

CMsSongInfo::~CMsSongInfo()
{
}

bool CMsSongInfo::Create(
	CMsSong* pSong, 
	CSize szTrackIconSize
)
{
	int i;
	int DeviceID = 0;

	m_pParentSong = pSong;
	for (i = 1; i < 16; ++i)
	{
		Tracks[i].Create(this, szTrackIconSize, DeviceID, i);
	}
	return true;
}

void CMsSongInfo::Copy(CMsSongInfo* pSource)
{
	memcpy(m_SongTitle, pSource->m_SongTitle, sizeof(m_SongTitle));
	memcpy(m_TracksEnable, pSource->m_TracksEnable, sizeof(m_TracksEnable));
	m_pParentSong = pSource->m_pParentSong;
	memcpy(Tracks, pSource->Tracks, sizeof(Tracks));
}

void CMsSongInfo::EditSongInfo(int InstID)
{
	CDlgMidiInfo dlg;
	int ret;
	CMsSongInfo* pSongInfo = new CMsSongInfo();

	pSongInfo->Copy(this);
	dlg.SetSongInfo(pSongInfo);
	dlg.SetInstrument(InstID);
	ret = (int)dlg.DoModal();
	if(ret == IDOK)
	{
		//---------------------------
		// OK Pressed
		//---------------------------
		this->Copy(pSongInfo);
	}
	delete pSongInfo;
}

CMsTrack* CMsSongInfo::GetTrack(int TrackID)
{
	CMsTrack* pTrack = nullptr;

	if(TrackID >= 0 && TrackID < 16)
		pTrack = &Tracks[TrackID];
	else
		::MessageBoxW(NULL, _T("ERROR"), _T("Track ID out of Range"), MB_OK);
	return pTrack;
}

void CMsSongInfo::SetSongTitle(const char* pTitle)
{
	strncpy_s(m_SongTitle, pTitle, sizeof(m_SongTitle) - 1);
	m_SongTitle[sizeof(m_SongTitle) - 1] = '\0'; // Ensure null termination
}

void CMsSongInfo::SetTrackEnable(int InstID, int Bank, int Enable)
{
	//--------------------------------
	// Select or Deselect Tracks in
	// a given Bank
	//--------------------------------
	if (InstID < 0 || InstID >= 16)
	{
		::MessageBoxW(NULL, _T("ERROR"), _T("Instrument ID out of Range"), MB_OK);
		return;
	}
	else if (Bank < 0 || Bank >= 4)
	{
		::MessageBoxW(NULL, _T("ERROR"), _T("Bank ID out of Range"), MB_OK);
		return;
	}
	else if (Enable)
		m_TracksEnable[Bank] |= (1 << InstID);
	else
		m_TracksEnable[Bank] &= ~(1 << InstID);
}

bool CMsSongInfo::GetTrackEnable(int InstID, int Bank)
{
	bool success = true;

	if (InstID < 0 || InstID >= 16)
	{
		::MessageBoxW(NULL, _T("ERROR"), _T("Instrument ID out of Range"), MB_OK);
		success = false;
	}
	else if (Bank < 0 || Bank >= 4)
	{
		::MessageBoxW(NULL, _T("ERROR"), _T("Bank ID out of Range"), MB_OK);
		success = false;
	}
	else
	{
		success = (m_TracksEnable[Bank] & (1 << InstID)) != 0;
	}
	return success;
}
