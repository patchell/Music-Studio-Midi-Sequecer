// ComboBitmap.cpp : implementation file
//

#include "pch.h"

// CComboBitmap

IMPLEMENT_DYNAMIC(CComboBitmap, CComboBox)

CComboBitmap::CComboBitmap()
{
	m_nItemHeight = 0;
	m_nItemWidth = 0;
}

CComboBitmap::~CComboBitmap()
{
}


BEGIN_MESSAGE_MAP(CComboBitmap, CComboBox)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

// CComboBitmap message handlers

void CComboBitmap::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);

	if (!IsWindowEnabled())
	{
		CBrush brDisabled(RGB(192, 192, 192)); // light gray
		CBrush* pOldBrush = pDC->SelectObject(&brDisabled);
		CPen penDisabled(PS_SOLID, 1, RGB(192, 192, 192));
		CPen* pOldPen = pDC->SelectObject(&penDisabled);
		OutputBitmap(lpDrawItemStruct, false);
		pDC->SelectObject(pOldBrush);
		pDC->SelectObject(pOldPen);
		return;
	}
	// Selected
	if ((lpDrawItemStruct->itemState & ODS_SELECTED)
		&& (lpDrawItemStruct->itemAction & (ODA_SELECT | ODA_DRAWENTIRE)))
	{
		CBrush brHighlight(::GetSysColor(COLOR_HIGHLIGHT));
		CBrush* pOldBrush = pDC->SelectObject(&brHighlight);
		CPen penHighlight(PS_SOLID, 1, ::GetSysColor(COLOR_HIGHLIGHT));
		CPen* pOldPen = pDC->SelectObject(&penHighlight);
		pDC->Rectangle(&lpDrawItemStruct->rcItem);
		pDC->SetBkColor(::GetSysColor(COLOR_HIGHLIGHT));
		pDC->SetTextColor(::GetSysColor(COLOR_HIGHLIGHTTEXT));
		OutputBitmap(lpDrawItemStruct, true);
		//clean up device context
		pDC->SelectObject(pOldBrush);
		pDC->SelectObject(pOldPen);
	}
	// De-Selected
	if (!(lpDrawItemStruct->itemState & ODS_SELECTED)
		&& (lpDrawItemStruct->itemAction & (ODA_SELECT | ODA_DRAWENTIRE)))
	{
		CBrush brWindow(::GetSysColor(COLOR_WINDOW));
		CBrush* pOldBrush = pDC->SelectObject(&brWindow);
		CPen penHighlight(PS_SOLID, 1, ::GetSysColor(COLOR_WINDOW));
		CPen* pOldPen = pDC->SelectObject(&penHighlight);
		pDC->Rectangle(&lpDrawItemStruct->rcItem);
		pDC->SetBkColor(::GetSysColor(COLOR_WINDOW));
		pDC->SetTextColor(::GetSysColor(COLOR_WINDOWTEXT));
		OutputBitmap(lpDrawItemStruct, false);
		//clean up DC
		pDC->SelectObject(pOldBrush);
		pDC->SelectObject(pOldPen);
	}
	// Focus
	if (lpDrawItemStruct->itemAction & ODA_FOCUS)
		pDC->DrawFocusRect(&lpDrawItemStruct->rcItem);
}

void CComboBitmap::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	lpMeasureItemStruct->itemWidth = (m_nItemWidth + 2);
	lpMeasureItemStruct->itemHeight = (m_nItemHeight + 2);
}

void CComboBitmap::PreSubclassWindow()
{
	CComboBox::PreSubclassWindow();
	ASSERT(GetStyle() & CBS_DROPDOWNLIST);
	ASSERT(GetStyle() & CBS_OWNERDRAWVARIABLE);
	ASSERT(GetStyle() & CBS_HASSTRINGS);
}

void CComboBitmap::DrawBitmap(CDC *pDC, CMyBitmap *pBM, CPoint &point)
{
	BITMAP bm;
	pBM->GetBitmap(&bm);
	int w = bm.bmWidth;
	int h = bm.bmHeight;
	CDC memDC;
	memDC.CreateCompatibleDC(pDC);
	CMyBitmap *pBmpOld = (CMyBitmap *)memDC.SelectObject(pBM);
	pDC->BitBlt(point.x, point.y, w, h, &memDC, 0, 0, SRCCOPY);
	memDC.SelectObject(pBmpOld);
}

void CComboBitmap::DrawBitmap(CDC *pDC, CMyBitmap *pBM, CRect &rect)
{
	BITMAP bm;
	pBM->GetBitmap(&bm);
	int w = bm.bmWidth;
	int h = bm.bmHeight;
	CPoint point;
	point.x = rect.left + ((rect.right - rect.left) / 2) - (w / 2);
	point.y = rect.top + ((rect.bottom - rect.top) / 2) - (h / 2);
	DrawBitmap(pDC, pBM, point);
}

int CComboBitmap::AddBitmap(CMyBitmap *pBM, char *string)
{
	return InsertBitmap(CComboBox::GetCount(), pBM, string);
}

int CComboBitmap::InsertBitmap(int nIndex, CMyBitmap *pBM, char *string)
{
	int n = CComboBox::InsertString(nIndex, string ? CString(string) : CString(""));
	if (n != CB_ERR && n != CB_ERRSPACE)
	{
		CComboBoxItem *pCBI = new CComboBoxItem;
		pCBI->m_nCheckboxChecked = 0;
		pCBI->m_nCheckboxEnabled = 0;
		pCBI->m_pbmBitmap = pBM;
		SetItemData(n, (DWORD)pCBI);
		BITMAP bm;
		pBM->GetBitmap(&bm);
		SetSize(bm.bmWidth, bm.bmHeight);
	}
	return n;
}

void CComboBitmap::OutputBitmap(LPDRAWITEMSTRUCT lpDIS, bool selected)
{
	int Width = m_nItemWidth;

	CComboBoxItem *pCBI = (CComboBoxItem *)lpDIS->itemData;
	if(pCBI && pCBI != (CComboBoxItem *)(0xffffffff))
	{
		CMyBitmap *pBM = pCBI->m_pbmBitmap;
		int nId = lpDIS->itemID;
		CDC* pDC = CDC::FromHandle(lpDIS->hDC);
		CString string("");
		//--------------------------------
		// if this is the edit box part,
		// we are going to only draw
		// the selected item bitmap
		// as well as any bitmaps that
		// are checked
		//-------------------------------
		if (lpDIS->itemState & ODS_COMBOBOXEDIT)
		{
			CPoint point;
			point.x = lpDIS->rcItem.left + 2;
			point.y = lpDIS->rcItem.top + ((lpDIS->rcItem.bottom - lpDIS->rcItem.top) / 2) - (m_nItemHeight / 2);
			DrawBitmap(pDC, pBM, point);
			point.x += m_nItemHeight + 6;
			int n = GetCount();
			int i;
			for (i = 0; i<n; ++i)
			{
				if (i != nId)
				{
					pCBI = (CComboBoxItem *)GetItemData(i);
					if (pCBI->m_nCheckboxEnabled && pCBI->m_nCheckboxChecked)
					{
						pBM = pCBI->m_pbmBitmap;
						DrawBitmap(pDC, pBM, point);
						point.x += m_nItemHeight + 2;
					}
				}
			}
		}
		else	//just draw a list box item
		{
			if ((lpDIS->itemID != -1))
				GetLBText(lpDIS->itemID, string);
			CPoint point;
			point.x = lpDIS->rcItem.left + 2;
			point.y = lpDIS->rcItem.top + ((lpDIS->rcItem.bottom - lpDIS->rcItem.top) / 2) - (m_nItemHeight / 2);
			if (pCBI->m_nCheckboxEnabled && !(lpDIS->itemState & ODS_COMBOBOXEDIT))
			{
				//----------------------------
				// Draw a checkbox here
				//-----------------------------
				CRect cbRect;
				int w = m_nItemHeight - 2;
				if (w > 16) w = 16;
				Width += w;
				cbRect.SetRect(point, point + CSize(w, w));
				pDC->DrawFrameControl(&cbRect, DFC_BUTTON, ( (DFCS_BUTTONCHECK) | (pCBI->m_nCheckboxChecked) )? DFCS_CHECKED : 0);
			}
			point.x += Width + 2;
			DrawBitmap(pDC, pBM, point);
			Width += m_nItemWidth + 2;
			if (!string.IsEmpty())
			{
				CRect rcText(lpDIS->rcItem);
				rcText.DeflateRect(Width + 4, 0, 0, 0);
				pDC->DrawText(string, rcText, DT_SINGLELINE | DT_VCENTER);
			}
		}
	}
}

void CComboBitmap::SetSize(int width, int height)
{
	if (width > m_nItemWidth)
		m_nItemWidth = width;
	if (height > m_nItemHeight)
		m_nItemHeight = height;
	for (int i = -1; i < GetCount(); i++)
		SetItemHeight(i, m_nItemHeight + 6);
}


BOOL CComboBitmap::DestroyWindow()
{
	m_ListBox.UnsubclassWindow();
	return CComboBox::DestroyWindow();
}


HBRUSH CComboBitmap::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CComboBox::OnCtlColor(pDC, pWnd, nCtlColor);
	//------------------------------------------------
	// This is where we subclass the controls we want
	// to subclass.
	//-------------------------------------------------
	switch (nCtlColor)
	{
	case CTLCOLOR_LISTBOX:
		if (m_ListBox.GetSafeHwnd() == NULL)
		{
			m_ListBox.m_pComboBox = this;
			m_ListBox.SubclassWindow(pWnd->GetSafeHwnd());
		}
		break;
	case CTLCOLOR_EDIT:
		break;
	}
	return hbr;
}


void CComboBitmap::EnableCheckbox(int nItem, int nFlag)
{
	CComboBoxItem *pCBI = (CComboBoxItem *)GetItemData(nItem);
	pCBI->m_nCheckboxEnabled = nFlag;
}

int CComboBitmap::CheckBoxIsEnable(int nItem)
{
	CComboBoxItem *pCBI = (CComboBoxItem *)GetItemData(nItem);
	return pCBI->m_nCheckboxEnabled;
}

void CComboBitmap::SetCheck(int nItem, int nFlag)
{
	CComboBoxItem *pCBI = (CComboBoxItem *)GetItemData(nItem);
	pCBI->m_nCheckboxChecked = nFlag;
}

int CComboBitmap::GetCheck(int nItem)
{
	CComboBoxItem *pCBI = (CComboBoxItem *)GetItemData(nItem);
	return pCBI->m_nCheckboxChecked;
}

int CComboBitmap::GetCheckBoxRect(int Item, CRect & rect)
{
	CRect lbRect;
	int nObjects;

	m_ListBox.GetClientRect(&lbRect);
	nObjects = GetCount();
	return 0;
}