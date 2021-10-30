#pragma once


// CMyButton

constexpr auto MYBUTTS_LATCHED = 0x01;

constexpr auto MYBUTTON_XPAD = 10;
constexpr auto MYBUTTON_YPAD = 10;

class CMyButton : public CWnd
{
	DECLARE_DYNAMIC(CMyButton)
	CMyBitmap m_bmButtonPushed;
	CMyBitmap m_bmButtonOut;
	CMyBitmap* m_pBmButtonDisp;
	CPoint m_ptUpLeftCorn;
	CSize m_szButtSize;
	COLORREF m_colorDark;
	COLORREF m_colorLight;
	int m_flagPushed;
	int m_flagLatched;
	int m_styleLatched;
	CWnd* m_pCWndUnLatch;
	int m_ID;
	CWnd* m_pParent;
public:
	CMyButton();
	virtual ~CMyButton();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	virtual void OnDraw(CDC* pDC);
	virtual BOOL Create( 
		const CPoint& rect, 
		CWnd* pParentWnd, 
		UINT nID, 
		UINT nBUbmId, 
		UINT nBDbmId,
		CWnd* pCWndUnLatch = 0,
		int Style = 0
	);
	int GetID() { return m_ID; }
	void LikeA_ButtonPush();
	static CSize GetButtonSize(int  Id);
protected:
	afx_msg LRESULT OnMybuttonUnlatch(WPARAM wParam, LPARAM lParam);
};


