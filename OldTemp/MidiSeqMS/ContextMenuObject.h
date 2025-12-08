#pragma once

class CContextMenuObject
{
public:
	int m_nItemID;
	CMsObject *m_pObject;
	CContextMenuObject();
	virtual ~CContextMenuObject();
};

