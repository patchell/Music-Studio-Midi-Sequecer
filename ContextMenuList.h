#pragma once


class CContextMenuList
{
	int m_Total;
	CContextMenuObject **m_ppList;
public:
	int m_CurrentIndex;
public:
	CContextMenuList();
	CContextMenuList(int nN);
	virtual ~CContextMenuList();
	int AddObject(CMsObject * pObj, int nID);
	int GetID(int nIndex);
	int FindID(int n_ID);
	CMsObject *GetObject(int nIndex);
};

