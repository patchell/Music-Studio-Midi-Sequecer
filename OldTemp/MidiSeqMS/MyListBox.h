#pragma once


// CMyListBox
class CComboBitmap;

class CMyListBox : public CListBox
{
	DECLARE_DYNAMIC(CMyListBox)

public:
	CMyListBox();
	virtual ~CMyListBox();

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
	CComboBitmap *m_pComboBox;
//	virtual bool DestroyWindow();
//	afx_msg void OnNcDestroy();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	int GetCheckBoxRect(int nItem, CRect & rcCB);
	int WhereIsTheMouse(CPoint p);
};


