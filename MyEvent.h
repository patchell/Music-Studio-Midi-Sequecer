// GlobalLock.h: interface for the GlobalLock class.
//
//////////////////////////////////////////////////////////////////////


// Basic Event class. The use of the prefix CDM was a semi-random selection.
#pragma once

constexpr auto WAIT_MY_HANDLE_ERROR = (-2);

class CMyEvent  
{
	HANDLE m_hHandle;
public:
	CMyEvent();
	virtual ~CMyEvent();
	virtual BOOL Create(const CString& name);
	bool Post();
	int Pend(DWORD timeout = INFINITE);
};

