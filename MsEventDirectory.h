#pragma once

constexpr auto EVENT_DIR_CHUNK_SIZE = 8192;

class CMsEventDirectory
{
	int m_nTotalEvents;
	int m_nDirectorySize;
	CMsEvent** m_pEventDirectory;
	CMsSong* m_pParentSong;
public:
	CMsEventDirectory();
	virtual ~CMsEventDirectory();
	bool Create(CMsSong* pParentSong);
	bool AddEventsAtEnd(CMsEvent* pEv);
	CMsEvent* GetEvent(int Event);
	bool SetEvent(CMsEvent* pE);
	int GetTotalEvents() const { return m_nTotalEvents; }
	bool ResizeEventDirectory();
	bool UpdateEventDirectory();
	void BubbbleSortEvents();
	int Print(FILE* pO, const char* pTStr, int Indent);
	int PrintEvent(char* pS, int l, int Event, int Indent);
};

