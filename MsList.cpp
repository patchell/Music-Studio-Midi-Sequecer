#include "pch.h"

CMsList::CMsList()
{
	m_pHead = 0;
	m_pTail = 0;
	m_NumberOfItems = 0;
	m_pName = 0;
}

CMsList::~CMsList()
{
	if(m_pName)
	{
		delete[] m_pName;
		m_pName = 0;
	}
}

bool CMsList::Create()
{
	return false;
}

void CMsList::SetName(const char* pName)
{
	int Len = strlen(pName);

	if (m_pName) delete[] m_pName;

	m_pName = new char[Len + 1];
	strcpy_s(m_pName, Len + 1, pName);
}

void CMsList::AddItemToHead(CMsListItem* pItem)
{
	if (!m_pHead)	//nobody home
	{
		m_pHead = pItem;
		m_pTail = pItem;
	}
	else
	{
		GetHead()->SetPrev(pItem);
		pItem->SetNext(GetHead());
		SetHead(pItem);
	}
}

void CMsList::AddItemToTail(CMsListItem* pItem)
{
	if(pItem)
	{
		if (!m_pHead)	//nobody home
		{
			m_pHead = pItem;
			m_pTail = pItem;
		}
		else
		{
			GetTail()->SetNext(pItem);
			pItem->SetPrev(GetTail());
			SetTail(pItem);
		}
	}
	else
	{
		AfxMessageBox(_T("CMsList::AddItemToTail() - pItem is null"));
	}
}

bool CMsList::RemoveItem(CMsListItem* pItem)
{
	if (pItem == GetHead())
	{
		SetHead(GetHead()->GetNext());
		if (GetHead()) GetHead()->SetPrev(0);
	}
	else if (pItem == GetTail())
	{
		SetTail(GetTail()->GetPrev());
		if (GetTail()) GetTail()->SetNext(0);
	}
	else
	{
		pItem->GetPrev()->SetNext(pItem->GetNext());
		pItem->GetNext()->SetPrev(pItem->GetPrev());
	}
	return true;
}
