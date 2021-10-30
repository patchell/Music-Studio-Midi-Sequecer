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
		pEV->Create(pSong, pStaffView, StartIndex++);
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
