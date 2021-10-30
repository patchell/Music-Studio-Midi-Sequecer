// HorzScrollBar.cpp : implementation file
//

#include "pch.h"


static void DefaultFormat(void *p,char *s,int l,int v,int)
{
	//--------------------------------------------------
	// displays text value in a string
	// this is the default callback function
	//
	// parameter:
	//	p.........user defined parameter, dependent on callback function
	//	s.........points to the character array buffer where text is going to be put
	//	l.........maximum size of character array where text is put
	//	v.........value to display...somehow
	//----------------------------------------------------
	sprintf_s(s,l,"%d",v);	//default formating
}

/////////////////////////////////////////////////////////////////////////////
// CMyScrollBar

CMyScrollBar::CMyScrollBar():CScrollBar()
{
	m_FormatFunction = DefaultFormat;			//make sure pointers are initialized
	m_EditFormatFunction = DefaultFormat;
	m_NumEditFormatFunction = DefaultFormat;
	m_pUserFormatParam = 0;
	m_nBufferLen = 256;
	m_pBuffer = new char[m_nBufferLen];
	m_bUserBufferFlag = FALSE;
	m_UserIntParam = 0;
	m_pUserFormatParam = 0;

	m_pMsgDestinationWindow = 0;

	m_pCStaticTextDisplay = 0;
	m_pCEditDisplayText = 0;
	m_CNumEditDisplayText = 0;

	m_TrackInc = 10;
	m_Inc = 1;
	m_Min = 0;
	m_Max = 100;
	m_Pos = 0;
	m_bModified = FALSE;
	m_pMasterValue = 0;

	m_WM_UserMessage = 0;
	m_SubCommand = 0;
	m_pMsgDestinationWindow = 0;
}

CMyScrollBar::~CMyScrollBar()
{
	if(!m_bUserBufferFlag) delete[] m_pBuffer;
}

BOOL CMyScrollBar::Create(DWORD dwStyle, const RECT& rect, CWnd* pParent, UINT nId)
{
	dwStyle |= SBS_HORZ;
	return CScrollBar::Create(dwStyle, rect, pParent, nId);
}

BEGIN_MESSAGE_MAP(CMyScrollBar, CScrollBar)
	ON_WM_HSCROLL_REFLECT()
	ON_WM_VSCROLL_REFLECT()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyScrollBar message handlers

void CMyScrollBar::VScroll(UINT nSBCode, UINT nPos)
{
	HScroll(nSBCode, nPos);
}

void CMyScrollBar::HScroll(UINT nSBCode, UINT nPos) 
{
	m_pBuffer[0]=0;
	m_Pos = GetScrollPos();
	int min,max;
	GetScrollRange(&min,&max);
	switch (nSBCode)
	{
		case SB_BOTTOM:
			m_Pos = min;
			break;
		case SB_ENDSCROLL:
			break;
		case SB_LINEDOWN:
			m_Pos += m_Inc;
			if(m_Pos > max)m_Pos = max;
			SetScrollPos(m_Pos);	//set scrollbar position
			break;
		case SB_LINEUP:
			m_Pos -= m_Inc;
			if(m_Pos < min) m_Pos = min;
			SetScrollPos(m_Pos);	//set scrollbar position
			break;
		case SB_PAGEDOWN:
			m_Pos +=  m_TrackInc;
			if(m_Pos > max) m_Pos = max;
			SetScrollPos(m_Pos);	//set scrollbar position
			break;
		case SB_PAGEUP:
			m_Pos -= m_TrackInc;
			if(m_Pos < min) m_Pos = min;
			SetScrollPos(m_Pos);	//set scrollbar position
			break;
		case SB_THUMBPOSITION:
		case SB_THUMBTRACK:
			if(max < 32768)
				m_Pos = nPos;
			SetScrollPos(m_Pos);	//set scrollbar position
			break;
		case SB_TOP:
			m_Pos = max;
			break;
	}
}

void CMyScrollBar::SetScrollPos(int v)
{
	m_Pos = v;
	int Id = GetDlgCtrlID();
	m_bModified = TRUE;
	if(m_pMasterValue)
		*m_pMasterValue = m_Pos;
	if (m_pMsgDestinationWindow)
	{
		m_pMsgDestinationWindow->PostMessageW(
			m_WM_UserMessage,
			Id,
			m_Pos
		);
	}
	Update();
	CScrollBar::SetScrollPos(v);
}

void CMyScrollBar::UpdateScrollPos(int v)
{
	m_Pos = v;
	m_bModified = TRUE;
	if(m_pMasterValue)
		*m_pMasterValue = m_Pos;
	Update();
	CScrollBar::SetScrollPos(m_Pos);
}

void CMyScrollBar::SetScrollRange(int minval, int maxval)
{
	m_Min = minval;
	m_Max = maxval;
	int span = (maxval - minval);
	if(span > 32768)
	{
		m_Inc = span / 3000;
		m_TrackInc = span/300;
	}
	else if (span > 3276)
	{
		m_Inc = span / 500;
		m_TrackInc = span / 50;
	}
	else if (span > 327)
	{
		m_Inc = span / 100;
		m_TrackInc = span / 10;
	}
	else
	{
		m_Inc = 1;
		m_TrackInc = 10;
	}
	CScrollBar::SetScrollRange(m_Min,m_Max);
}

void CMyScrollBar::Update()
{
	if(m_pCStaticTextDisplay)
	{
		(*m_FormatFunction)(
			m_pUserFormatParam, 
			m_pBuffer, 
			m_nBufferLen,
			m_Pos,
			m_UserIntParam
		);
		m_pCStaticTextDisplay->SetWindowText(CString(m_pBuffer));
	}
	if(m_pCEditDisplayText)
	{
		(*m_EditFormatFunction)(
			m_pUserFormatParam,
			m_pBuffer,
			m_nBufferLen,
			m_Pos,
			m_UserIntParam
		);
		m_pCEditDisplayText->SetWindowText(CString(m_pBuffer));
	}
	if(m_CNumEditDisplayText)
	{
		(*m_NumEditFormatFunction)(
			m_pUserFormatParam,
			m_pBuffer,
			m_nBufferLen,
			m_Pos,
			m_UserIntParam
		);
		m_CNumEditDisplayText->SetWindowText(CString(m_pBuffer));
	}
}

