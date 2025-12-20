#include "pch.h"

CMsEventDirectory::CMsEventDirectory()
{
	m_nTotalEvents = 0;
	m_nDirectorySize = 0;
	m_pEventDirectory = nullptr;
	m_pParentSong = nullptr;
}

CMsEventDirectory::~CMsEventDirectory()
{
	if (m_pEventDirectory)
	{
		delete[] m_pEventDirectory;
		m_pEventDirectory = nullptr;
	}
}

bool CMsEventDirectory::Create(CMsSong* pParentSong)
{
	m_nTotalEvents = EVENT_DIR_CHUNK_SIZE;
	m_pParentSong = pParentSong;
	m_pEventDirectory = new CMsEvent * [m_nTotalEvents];
	for (int i = 0; i < m_nTotalEvents; ++i)
		m_pEventDirectory[i] = nullptr;
	return true;
}

bool CMsEventDirectory::AddEventsAtEnd(CMsEvent* pEv)
{
	int idx = 0;
	bool rV = true;

	while (pEv)
	{
		idx = pEv->GetIndex();
		if(m_pEventDirectory[idx] == nullptr)
		{
			SetEvent(pEv);
			m_nTotalEvents++;
		}
		pEv = pEv->GetNext();
	}
	return rV;
}
CMsEvent* CMsEventDirectory::GetEvent(int Event)
{
	return m_pEventDirectory[Event];
}

bool CMsEventDirectory::SetEvent(CMsEvent* pE)
{
	bool rV = true;
	int Event = pE->GetIndex();

	if (Event < m_nDirectorySize)
	{
		m_pEventDirectory[Event] = pE;
	}
	else
	{
		ResizeEventDirectory();
		m_pEventDirectory[Event] = pE;
	}
	return rV;
}

bool CMsEventDirectory::ResizeEventDirectory()
{
	bool rV = true;
	
	if (m_pEventDirectory)
	{
		CMsEvent** pNewDirectory = new CMsEvent * [m_nDirectorySize + EVENT_DIR_CHUNK_SIZE];
		if (pNewDirectory)
		{
			for (int i = 0; i < m_nDirectorySize; ++i)
				pNewDirectory[i] = m_pEventDirectory[i];

			delete[] m_pEventDirectory;
			m_pEventDirectory = pNewDirectory;
			m_nDirectorySize += EVENT_DIR_CHUNK_SIZE;
		}
		else
			return rV = false;

	}
	return rV;
}

bool CMsEventDirectory::UpdateEventDirectory()
{
	bool rV = true;
	CMsEvent* pEv = m_pParentSong->GetEventListHead();

	if (pEv &&	m_pEventDirectory)
	{
		while(pEv)
		{
			int idx = pEv->GetIndex();
			if (idx >= m_nDirectorySize)
			{
				if (!ResizeEventDirectory())
				{
					rV = false;
					break;
				}
			}
			m_pEventDirectory[idx] = pEv;
			pEv = pEv->GetNext();
		}
	}
	return rV;
}

void CMsEventDirectory::BubbbleSortEvents()
{
	//----------------------------------
	// Bubble Sort Algorithm
	// Sorts events in order of their
	// index value.  From lowest to highest
	// value
	//----------------------------------
	int i, j;
	CMsEvent* temp;

	for (i = 0; i < m_nTotalEvents - 1; ++i)
	{
		for (j = 0; j < m_nTotalEvents - i - 1; ++j)
		{
			if (m_pEventDirectory[j]->GetIndex() > m_pEventDirectory[j + 1]->GetIndex())
			{
				temp = m_pEventDirectory[j];
				m_pEventDirectory[j] = m_pEventDirectory[j + 1];
				m_pEventDirectory[j + 1] = temp;
			}
		}
	}
}

int CMsEventDirectory::PrintEvent(char* pS, int l, int Event, int Indent)
{
	int ls = 0;
	char* pIndent = new char[256];

	GETAPP->IndentString(pIndent, 256, Indent);
	ls = sprintf_s(pS, l, "%sEvent %d:", pIndent, m_pEventDirectory[Event]->GetIndex());
	return 0;
}

int CMsEventDirectory::Print(FILE* pO, const char* pTStr, int Indent)
{
	char* pIndent = new char[256];
	int i;
	char* pS = new char[1024];

	GETAPP->IndentString(pIndent, 256, Indent);
	if (pTStr)
	{
		fprintf(pO, "%s\n", pTStr);
		Indent += 2;
		GETAPP->IndentString(pIndent, 256, Indent);
	}
	for (i = 0; i < m_nTotalEvents; ++i)
	{
		if (m_pEventDirectory[i])
		{
			PrintEvent(pS, 1024, i, Indent);
			fprintf(pO, "%s\n", pS);
		}
	}
	return 0;
}
