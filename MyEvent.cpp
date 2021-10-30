// CMyEvent.cpp: implementation of the CMyEvent class.
//
//////////////////////////////////////////////////////////////////////

#include "pch.h"

CMyEvent::CMyEvent()
{
	m_hHandle = 0;
}

CMyEvent::~CMyEvent()
{
	::CloseHandle(m_hHandle);
}

BOOL CMyEvent::Create(const CString& name)
{
	BOOL rV = FALSE;
	m_hHandle = ::CreateEventW(NULL, FALSE, FALSE, name);
	if (m_hHandle) rV = TRUE;
	return rV;
}

//----------------------------------
//	Post
//		This function is used to
//	the object to the signaled state.
//
// return value
//	TRUE if succesfull
//	FALSE on fail
//--------------------------------------
bool CMyEvent::Post()
{
	if (m_hHandle) {
		if (::SetEvent(m_hHandle))
			return true;
	}
	return false;
}

//-----------------------------------
// Pend
//	This function will wait for an
//	Event to be signaled.
// Parameter:
//	timeout---Time, in milliseconds
//	that the method will wait for
//	the object to be signalled.
//	The default is Infinate
//
//	return value:
//		WAIT_OBJECT_0....Success
//		WAIT_TIMEOUT.....Timeout
//		WAIT_FALED.......ERROR
//		WAIT_MY_HANDLE...ERROR, band handle
//------------------------------------
int CMyEvent::Pend(DWORD timeout)
{
	int rV = WAIT_MY_HANDLE_ERROR;	//object not created error
	if (m_hHandle)
		rV = ::WaitForSingleObject(m_hHandle, timeout);
	return rV;
}


