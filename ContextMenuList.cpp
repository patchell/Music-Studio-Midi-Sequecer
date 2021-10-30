#include "pch.h"

CContextMenuList::CContextMenuList()
{
	m_ppList = 0;
	m_Total = 0;
	m_CurrentIndex = 0;
}


CContextMenuList::~CContextMenuList()
{
	if (m_ppList) delete[] m_ppList;
}


CContextMenuList::CContextMenuList(int nN)
{
	m_ppList = (CContextMenuObject **)new CContextMenuObject[nN];
	m_Total = nN;
	m_CurrentIndex = 0;

}


int CContextMenuList::AddObject(CMsObject * pObj, int nID)
{
	int rV = -1;

	if (m_CurrentIndex != m_Total)
	{
		m_ppList[m_CurrentIndex] = new CContextMenuObject;
		m_ppList[m_CurrentIndex]->m_nItemID = nID;
		m_ppList[m_CurrentIndex]->m_pObject = pObj;
		rV = ++m_CurrentIndex;
	}
	return 0;
}


int CContextMenuList::GetID(int nIndex)
{
	return m_ppList[nIndex]->m_nItemID;
}


CMsObject *CContextMenuList::GetObject(int nIndex)
{
	return m_ppList[nIndex]->m_pObject;
}


int CContextMenuList::FindID(int nID)
{
	int rV = -1;
	int i,loop;

	for (i = 0, loop = 1; (i < m_CurrentIndex) && loop; ++i)
	{
		if (m_ppList[i]->m_nItemID == nID)
		{
			rV = i;
			loop = 0;
		}
	}
	return rV;
}
