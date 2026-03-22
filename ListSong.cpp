#include "pch.h"

CListSong::CListSong()
{
}

CListSong::~CListSong()
{
}

bool CListSong::Create()
{
	return false;
}

CMsSong* CListSong::FindSongByID(int SongID)
{
	CListSongItem* pItem = 0;
	CMsSong* pSongFound = 0;
	bool bFound = false;

	pItem = FindSongItemByID(SongID);
	if (pItem)
		pSongFound = pItem->GetSong();
	return pSongFound;
}

CListSongItem* CListSong::FindSongItemByID(int SongID)
{
	CListSongItem* pItem = (CListSongItem*)GetHead();
	CMsSong* pSong = 0;
	bool bFound = false;

	while (pItem && !bFound)
	{
		pSong = pItem->GetSong();
		if (pSong->GetSongId() == SongID)
		{
			bFound = true;
		}
		else
			pItem = (CListSongItem*)pItem->GetNext();
	}
	return pItem;
}
