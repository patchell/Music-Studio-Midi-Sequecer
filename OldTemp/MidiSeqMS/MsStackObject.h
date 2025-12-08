// MsRepeatStackObject.h: interface for the CMsRepeatStackObject class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

class CMsStackObject  
{
	void* m_pDataItem;
	CMsStackObject* m_pPrevStackObject;
	CMsStackObject* m_pNextStackObject;	//next Object on stack
public:
	CMsStackObject();
	virtual ~CMsStackObject();

	CMsStackObject* GetNext() { return m_pNextStackObject; }
	void SetNext(CMsStackObject* pSO) { m_pNextStackObject = pSO; }
	CMsStackObject* GetPrev() { return m_pPrevStackObject; }
	void SetPrev(CMsStackObject* pSO) { m_pPrevStackObject = pSO; }
	void* GetDataItem() { return m_pDataItem; }
	void SetDataItem(void* pDI) { m_pDataItem = pDI; }
};
