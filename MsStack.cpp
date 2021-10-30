#include "pch.h"

CMsStack::CMsStack()
{
	m_ItemCount = 0;	//items on Stack
	 m_pHeadObject = 0;	//top of stack
	 m_pTailObject = 0;	//bottom of stack
}

CMsStack::~CMsStack()
{
}

void* CMsStack::PopFIFO()	//pop from bottom
{
	CMsStackObject* pSO;
	void* pDataItem = 0;

	if ((pSO = m_pTailObject) != NULL)
	{
		m_pTailObject = pSO->GetPrev();
		m_pHeadObject->SetNext(0);
		pDataItem = pSO->GetDataItem();
		delete pSO;
	}
	return pDataItem;
}

void* CMsStack::PopLIFO()//pop from top
{
	CMsStackObject* pSO;
	void* pDataItem = 0;

	if ((pSO = m_pHeadObject) != NULL)
	{
		m_pHeadObject = pSO->GetNext();
		if(m_pHeadObject)
			m_pHeadObject->SetPrev(0);
		pDataItem = pSO->GetDataItem();
		delete pSO;
	}
	return pDataItem;
}

void* CMsStack::PeakTop()
{
	void* pDataItem = 0;

	if (m_pHeadObject)
	{
		pDataItem = m_pHeadObject->GetDataItem();
	}
	return pDataItem;
}

void* CMsStack::PeakBottom()
{
	void* pDataItem = 0;

	if (m_pTailObject)
	{
		pDataItem = m_pTailObject->GetDataItem();
	}
	return pDataItem;
}

void CMsStack::Push(void* pDataItem)
{
	CMsStackObject* pSO;

	pSO = new CMsStackObject;
	pSO->SetDataItem(pDataItem);
	if (m_pHeadObject)
	{
		m_pHeadObject->SetPrev(pSO);
		pSO->SetNext(m_pHeadObject);
		m_pHeadObject = pSO;
	}
	else
	{
		m_pHeadObject = m_pTailObject = pSO;
	}
}
