#pragma once


// CMyOwnCheckBox

class CMyOwnCheckBox : public CWnd
{
	int m_CheckState;
	CRect m_rectBox;
	CString m_csCaption;
	COLORREF m_colorBackGround;
	COLORREF m_colorBox;
	COLORREF m_colorCheck;
	COLORREF m_colorText;
	CPoint m_pointOrigin;
	CFont m_fontCaption;
	CString m_stringTrueCaption;
	CString m_stringFalseCaption;
	unsigned m_ControlMessage;
	DECLARE_DYNAMIC(CMyOwnCheckBox)
protected:
	DECLARE_MESSAGE_MAP()
public:
	CMyOwnCheckBox();
	virtual ~CMyOwnCheckBox();
	virtual BOOL Create(
		CString& stringTrueCaption,	//String to display when checked (TRUE)
		CString& stringFalseCaption,//String to display when unchecked (false)
		const CRect& rect,	//outline rectangle of control
		CWnd* pParentWnd,	//parent of the control
		UINT nID,	//ID of the control
		COLORREF colorBackGround,	// background color
		COLORREF colorBox,	//color of the box outline
		COLORREF colorCheck,//color of the check mark
		COLORREF colorText,	//color of the text
		unsigned ControlMessage	//message to send back to parent
	);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	void SetCheck(int checkstate)
	{
		m_CheckState = checkstate;
	}
	int GetCheck() { return m_CheckState; }
	void Draw(CDC* pDC);
};


