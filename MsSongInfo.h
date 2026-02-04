#pragma once

class CMsSong;

class CMsSongInfo
{
	char m_SongTitle[32];
	CMsTrack Tracks[16];
	int m_TracksEnable[4];		//Enable Flags
	CMsSong* m_pParentSong;
public:
	CMsSongInfo();
	virtual ~CMsSongInfo();
	bool Create(
		CMsSong* pSong, //parent song
		CSize szTrackIconSize		//size of icon
	);
	void Copy(CMsSongInfo* pSource);
	void EditSongInfo(int InstID);
	//-------------------------------------
	CMsTrack* GetTrack(int TrackID);
	//-------------------------------------
	COLORREF GetInstColor(int Inst) { return GetTrack(Inst)->GetColor(); }
	void SetInstColor(int InstID, COLORREF Color) { GetTrack(InstID)->SetColor(Color); }
	COLORREF GetTrackTextColor(int Inst) { return GetTrack(Inst)->GetTextColor(); }
	void SetTrackTextColor(int InstID, COLORREF Color) { GetTrack(InstID)->SetTextColor(Color); }
	int GetPatch(int InstID) { return GetTrack(InstID)->GetPatch(); }
	void SetPatch(int InstID, int Patch) { GetTrack(InstID)->SetPatch(Patch); }
	int GetChannel(int InstID) { return GetTrack(InstID)->GetChannel(); }
	void SetChannel(int InstID, int Chan) { GetTrack(InstID)->SetChannel(Chan); }
	int GetRange(int InstID) { return GetTrack(InstID)->GetPitchRange(); }
	void SetRange(int InstID, int Range) { GetTrack(InstID)->SetPitchRange(Range); }
	//-------------------------------------
	void SetSongTitle(const char* pTitle);
	const char* GetSongTitle() { return m_SongTitle; }
	//-------------------------------------
	void SetTrackEnable(int InstID, int Bank, int Enable);
	bool GetTrackEnable(int InstID, int Bank);
	CMsSong* GetParentSong() { return m_pParentSong; }
	int GetNumberOfTracks() const { return 15; }
};

