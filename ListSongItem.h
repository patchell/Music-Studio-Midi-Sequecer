#pragma once

class CListSongItem : public CMsListItem
{
	CMsSong* m_pSong;
public:
	CListSongItem();
	virtual ~CListSongItem();
	bool Create(CMsSong* pSong);
	CMsSong* GetSong() { return m_pSong; }
	void SetSong(CMsSong* pSong) { m_pSong = pSong; }
};

