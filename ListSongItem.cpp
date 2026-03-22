#include "pch.h"

CListSongItem::CListSongItem()
{
	m_pSong = 0;
}

CListSongItem::~CListSongItem()
{
}

bool CListSongItem::Create(CMsSong* pSong)
{
	m_pSong = pSong;
	return CMsListItem::Create();
}
