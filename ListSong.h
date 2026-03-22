#pragma once

class CListSong : public CMsList
{
public:
	CListSong();
	virtual ~CListSong();
	bool Create();
	CMsSong* FindSongByID(int SongID);
	CListSongItem* FindSongItemByID(int SongID);
};

