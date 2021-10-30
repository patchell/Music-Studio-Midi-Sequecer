// DisplayStatic.cpp : implementation file
//

#include "pch.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDisplayStatic

CDisplayStatic::CDisplayStatic()
{
	m_FifoStrings = 0;
	m_FifoSize = 0;
	m_FifoHead = 0;
	m_FifoTail = 0;
	m_nStrings = 0;
}

CDisplayStatic::~CDisplayStatic()
{
	if(m_FifoStrings)
		delete[] m_FifoStrings;
}


BEGIN_MESSAGE_MAP(CDisplayStatic, CStatic)
	//{{AFX_MSG_MAP(CDisplayStatic)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDisplayStatic message handlers

void CDisplayStatic::SetWindowText(const char *String)
{
	//---------------------------------------------------
	// This function is used to add a string to a Static
	// Text box.  The string will show up at the end of
	// the diplayed text (if any).
	//
	// parameter:
	//		String....string to add to display
	//
	//---------------------------------------------------
	int l;
	char *s;
	//add string to queue
	//remove oldest string (if need be)
	AddString((char *)String);
	//get total size in bytes of all strings
	l = GetTotalSize();
	//allocate enough memory for display sting
	s = new char[l];
	//genereate display string
	GenDisplayString(s,l);
	//display the string
	CStatic::SetWindowText(CString(s));
	delete[] s;
}

int CDisplayStatic::AddString(const char *s)
{
	///-----------------------------------------
	///	AddString
	///		Adds a string to the queue that holds
	///	the strings that need to be displayed.
	///	As new strings are added, old strings
	/// are deleted.
	///
	/// parameter:
	///		s.....string to add
	/// Returns:
	///		Number of strings in queue
	///------------------------------------------------

	int l;
	char *S;

	l = (int)strlen(s);
	S = new char[l+1];
	strcpy_s(S,l + 1,s);
	if(m_nStrings == m_FifoSize)	//is it full yet?
	{
		delete[] m_FifoStrings[m_FifoTail];
		m_FifoTail++;
		if(m_FifoTail == m_FifoSize) m_FifoTail = 0;
		--m_nStrings;
	}
	m_FifoStrings[m_FifoHead++] = S;
	if(m_FifoHead == m_FifoSize) m_FifoHead = 0;
	++m_nStrings;
	return m_nStrings;
}

int CDisplayStatic::GenDisplayString(char *pDispString, int N)
{
	//----------------------------------------------------
	// Concatanate all of the strings togheter
	// use a '\n' char between strings
	//
	// parameter:
	//	pDispString......pointer to dest, must be big
	//					enough for total size of strings
	//----------------------------------------------------

	int l=0,i,t;

	t = m_FifoTail;
	for(i=0;i<m_nStrings;++i)
	{
		l += sprintf_s(&pDispString[l],N-l,"%s\n",m_FifoStrings[t]);
		++t;
		if(t == m_nStrings) t = 0;
	}
	return m_nStrings;
}

int CDisplayStatic::GetTotalSize()
{
	///------------------------------------------
	///	GetTotalSize
	///		Find out how many bytes one will need
	///	in order to display all the lines of text
	///
	///	Returns number of bytes required to
	/// store all text.
	///-----------------------------------------
	int rv = 0;

	int i;
	int h = m_FifoTail;

	for(i=0;i<m_nStrings;++i)
	{
		rv += strlen(m_FifoStrings[h]) + 2;		//add a fudge factor
		h++;
		if(h == m_FifoSize) h = 0;
	}
	return rv;
}

void CDisplayStatic::InitStringFifo(int n)
{
	///---------------------------------------------
	/// InitStringFifo
	///		Set up the string fifo for the qyeyhe
	///
	///	PARAMETER:
	///		n........number of strings in queue
	///--------------------------------------------
	int i;

	m_FifoStrings = new char *[n];
	for(i=0;i<n;++i)
		m_FifoStrings[i] = 0;
	m_FifoSize = n;

}


BOOL CDisplayStatic::Create(CPoint corner,int CornerFlag, int cx, int n, CWnd* pParentWnd, UINT nID)
{
	CRect rect;
	TEXTMETRIC metric;
	int cy;
	CDC* pDC = pParentWnd->GetDC();
	pDC->GetTextMetrics(&metric);
	cy = metric.tmHeight * n;
	if (CornerFlag)	//corner is Lower Left
		corner.y -= cy;
	rect = CRect(corner, CSize(cx, cy));
	return CStatic::Create(_T(""), WS_BORDER, rect, pParentWnd, nID);
}
