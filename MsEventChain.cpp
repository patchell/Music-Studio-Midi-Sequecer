#include "pch.h"


void CMsEventChain::CreateChain(
	UINT nEvents,	//n nuimber of evets to create
	CMsSong* pSong, 
	CChildViewStaff* pStaffView, 
	UINT StartIndex
)
{
	UINT i;
	CMsEvent* pEV;

	for (i = 0; i < nEvents; ++i)
	{
		pEV = new CMsEvent;
		pEV->Create(pSong, pStaffView);
		pEV->SetEventIndex(StartIndex + i);
		AddEventAtEnd(pEV);
	}
}

void CMsEventChain::AddEventAtEnd(CMsEvent* pEV)
{
	if (!GetHead() && !GetTail())
	{
		SetTail(pEV);
		SetHead(pEV);
	}
	else
	{
		pEV->SetPrev(GetTail());
		GetTail()->SetNext(pEV);
		SetTail(pEV);
	}
	++m_TotalEvents;
}

void CMsEventChain::UnHookChain()
{
	if(GetTail()->GetNext())
		GetTail()->GetNext()->SetPrev(GetHead()->GetPrev());
	if(GetHead()->GetPrev())
		GetHead()->GetPrev()->SetNext(GetTail()->GetNext());
	GetTail()->SetNext(0);
	GetHead()->SetPrev(0);
}

void CMsEventChain::SetSelected(int SelFlag)
{
	CMsEvent* pEV = GetHead();
	int loop = 1;

	while (loop && pEV)
	{
		pEV->SetSelected(SelFlag);
		if (pEV == GetTail())
			loop = 0;
		else
			pEV = pEV->GetNext();
	}
}

bool CMsEventChain::Cut(CMsEvent* pFirst, CMsEvent* pLast)
{
	CMsEvent* pEvPrev = 0;
	CMsEvent* pEvNext = 0;
	bool bRv = false;

	if (pFirst && pLast)
	{
		if (GetHead())
		{
			if (bOriginalChain)
			{
				m_pHead = 0;
				m_pTail = 0;
				m_TotalEvents = 0;
				bOriginalChain = false;
			}
			else
			{
				CMsEvent* pEv = GetHead();
				while (pEv)
				{
					pEvPrev = pEv;
					pEv = pEv->GetNext();
					if(pEvPrev) 
					{ 
						delete pEvPrev; 
						pEvPrev = 0; 
					}
				}
				m_pHead = 0;
				m_pTail = 0;
				m_TotalEvents = 0;
				bOriginalChain = false;
			}
		}
		pEvPrev = pFirst->GetPrev();
		pEvNext = pLast->GetNext();
		if(pEvPrev)
			pEvPrev->SetNext(pEvNext);
		else
			SetHead(pEvNext);
		if(pEvNext)
			pEvNext->SetPrev(pEvPrev);
		else
			SetTail(pEvPrev);
		pFirst->SetPrev(0);
		pLast->SetNext(0);
		bOriginalChain = true;
		//-------------------------------------
		// Count the number of events in
		// the chain
		//--------------------------------------
		CMsEvent* pEv = GetHead();
		m_TotalEvents = 0;
		while (pEv)
		{
			++m_TotalEvents;
			pEv = pEv->GetNext();
		}
	}
	return bRv;
}

bool CMsEventChain::Copy(CMsEvent* pFirst, CMsEvent* pLast)
{
	bool bRv = false;

	if (pFirst && pLast)
	{
		if (GetHead())
		{
			if (bOriginalChain)
			{
				m_pHead = 0;
				m_pTail = 0;
				m_TotalEvents = 0;
				bOriginalChain = false;
			}
			else
			{
				CMsEvent* pEv = GetHead();
				while (pEv)
				{
					CMsEvent* pEvTemp;
					pEvTemp = pEv;
					pEv = pEv->GetNext();
					if(pEvTemp) 
					{ 
						delete pEvTemp; 
						pEvTemp = 0; 
					}
				}
				m_pHead = 0;
				m_pTail = 0;
				m_TotalEvents = 0;
				bOriginalChain = false;
			}
			CMsEvent* pEv = pFirst;
			while (pEv)
			{
				CMsEvent* pEvTemp;
				pEvTemp = new CMsEvent;
				pEvTemp->CopyObjectsFromEvent(pEv);
				AddEventAtEnd(pEvTemp);
				if (pEv == pLast)
					pEv = 0;
				else
					pEv = pEv->GetNext();
			}
		}
		bRv = true;
	}
	return bRv;
}

bool CMsEventChain::Paste(CMsEvent* pInsertionPoint)
{
	bool bRv = false;
	CMsEvent* pEvNext = 0;
	CMsEvent* pEv = 0;
	CMsSong* pSong = 0;
	CChildViewStaff* pCVS = 0;

	if (pInsertionPoint && GetHead())
	{
		pSong = pInsertionPoint->GetSong();
		pCVS = pInsertionPoint->GetParentView();
		pEvNext = pInsertionPoint->GetNext();
		pEv = GetHead();
		while(pEv)
		{
			CMsEvent* pEvTemp;
			pEvTemp = new CMsEvent;
			pEvTemp->Create(pSong, pCVS);
			pEvTemp->CopyObjectsFromEvent(pEv);
			//-------------------------------
			pInsertionPoint->GetSong()->InsertEvent(pEvTemp, pInsertionPoint, CMsSong::Insert::AFTER);
			pInsertionPoint = pEvTemp;
			pEv = pEv->GetNext();
		}
	}
	return bRv;
}
