#pragma once

// DisplayStatic.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDisplayStatic window

class CDisplayStatic : public CStatic
{
	int m_nStrings;
	int m_FifoTail;
	int m_FifoHead;
	int m_FifoSize;
	char ** m_FifoStrings;
	// Construction
public:
	CDisplayStatic();
	void InitStringFifo(int n);
	void SetWindowText(const char *String);
	virtual ~CDisplayStatic();
	// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
private:
	int GetTotalSize(void);
	int GenDisplayString(char *pDispString, int N);
	int AddString(const char *s);
public:
	virtual BOOL Create(CPoint Lcorner,int CornerFlag, int cx, int n, CWnd* pParentWnd, UINT nID);
};

