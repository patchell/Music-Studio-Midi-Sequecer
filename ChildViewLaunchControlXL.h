#pragma once


// CChildViewLaunchControlXL

class CChildViewLaunchControlXL : public CWnd
{
	DECLARE_DYNAMIC(CChildViewLaunchControlXL)

public:
	CChildViewLaunchControlXL();
	virtual ~CChildViewLaunchControlXL();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};


