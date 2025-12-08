#pragma once


// CMyComboBox

class CMyComboBox : public CComboBox
{
	DECLARE_DYNAMIC(CMyComboBox)
public:
	int m_nItemWidth;
	int m_nItemHeight;

public:
	CMyComboBox();
	virtual ~CMyComboBox();

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
	virtual void MeasureItem(LPMEASUREITEMSTRUCT /*lpMeasureItemStruct*/);
	void DrawBitmap(CDC * pDC, CMyBitmap * pBM, CPoint & point);
	void DrawBitmap(CDC *pDC, CMyBitmap *pBM, CRect &rect);
	int AddBitmap(CMyBitmap * pBM, CString csString);
	int InsertBitmap(int nIndex, CMyBitmap * pBM, CString csString);
	void OutputBitmap(LPDRAWITEMSTRUCT lpDIS);
	void SetSize(int width, int legnth);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);
};


