// MsRepeatStackObject.cpp: implementation of the CMsRepeatStackObject class.
//
//////////////////////////////////////////////////////////////////////

#include "pch.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMsStackObject::CMsStackObject()
{
	m_pDataItem = 0;
	m_pNextStackObject = 0;
	m_pPrevStackObject = 0;
}

CMsStackObject::~CMsStackObject()
{
	if(m_pNextStackObject)
		delete m_pNextStackObject;
}
