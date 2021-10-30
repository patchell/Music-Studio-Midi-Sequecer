#pragma once

class CMsStack
{
	UINT m_ItemCount;	//items on Stack
	CMsStackObject* m_pHeadObject;
	CMsStackObject* m_pTailObject;
public:
	CMsStack();
	virtual ~CMsStack();
	void* PopFIFO();
	void* PopLIFO();
	void* PeakTop();
	void* PeakBottom();
	void Push(void* pDataItem);
};

