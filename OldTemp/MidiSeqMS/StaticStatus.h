#pragma once

// StaticStatus.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CStaticStatus window

class CStaticStatus : public CWnd
{
	COLORREF color_BG;
	COLORREF color_Text;
	COLORREF color_Border;
	CPoint m_ptUpperLeft;
	CSize m_szRectSize;
	CString m_csDisplayText;
	// Construction
public:
	CStaticStatus();
	virtual ~CStaticStatus();
	void SetText(CString& csText);
	void SetColors(COLORREF bGrn, COLORREF text, COLORREF Border);
	virtual bool Create(const CRect& rect, CWnd* pParentWnd, UINT nID = 0xffff);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	void OnDraw(CDC* pDC);
protected:
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
};

