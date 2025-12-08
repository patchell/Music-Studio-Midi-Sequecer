#pragma once

// CComboBitmap

class CComboBitmap : public CComboBox
{
	DECLARE_DYNAMIC(CComboBitmap)
public:	///attributes
	int m_nItemHeight;
	int m_nItemWidth;
	CMyListBox m_ListBox;
public:
	CComboBitmap();
	virtual ~CComboBitmap();
	void SetSize(int width, int height);
	//-----------------------------------
	// Bitmap functions
	//----------------------------------
	void OutputBitmap(LPDRAWITEMSTRUCT lpDIS, bool selected);
	int InsertBitmap(int nIndex, CMyBitmap *pBM, char *string);
	int AddBitmap(CMyBitmap *pBM, char *string);
	void DrawBitmap(CDC *pDC, CMyBitmap *pBM, CRect &rect);
	void DrawBitmap(CDC *pDC, CMyBitmap *pBM, CPoint &point);
	//--------------------------------------
	// checkbox functions
	//--------------------------------------
	void EnableCheckbox(int nItem, int nFlag);
	int CheckBoxIsEnable(int nItem);
	void SetCheck(int nItem, int nFlag);
	int GetCheck(int nItem);
	int GetCheckBoxRect(int Item, CRect & rect);
	//--------------------------------------
	// Overrides
	//---------------------------------------
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
	virtual void MeasureItem(LPMEASUREITEMSTRUCT /*lpMeasureItemStruct*/);
	virtual void PreSubclassWindow();
	virtual BOOL DestroyWindow();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
protected:
	DECLARE_MESSAGE_MAP()
};


