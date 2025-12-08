// MyComboBox.cpp : implementation file
//

#include "pch.h"

// CMyComboBox

IMPLEMENT_DYNAMIC(CMyComboBox, CComboBox)

CMyComboBox::CMyComboBox()
{
	m_nItemHeight = 0;
	m_nItemWidth = 0;
}

CMyComboBox::~CMyComboBox()
{
}


BEGIN_MESSAGE_MAP(CMyComboBox, CComboBox)
	ON_WM_CREATE()
END_MESSAGE_MAP()



// CMyComboBox message handlers




void CMyComboBox::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);

	if (!IsWindowEnabled())
	{
		CBrush brDisabled(RGB(192, 192, 192)); // light gray
		CBrush* pOldBrush = pDC->SelectObject(&brDisabled);
		CPen penDisabled(PS_SOLID, 1, RGB(192, 192, 192));
		CPen* pOldPen = pDC->SelectObject(&penDisabled);
		OutputBitmap(lpDrawItemStruct);
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
		OutputBitmap(lpDrawItemStruct);
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
		OutputBitmap(lpDrawItemStruct);
		//clean up DC
		pDC->SelectObject(pOldBrush);
		pDC->SelectObject(pOldPen);
	}
	// Focus
	if (lpDrawItemStruct->itemAction & ODA_FOCUS)
		pDC->DrawFocusRect(&lpDrawItemStruct->rcItem);
}


void CMyComboBox::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	lpMeasureItemStruct->itemWidth = (m_nItemWidth + 2);
	lpMeasureItemStruct->itemHeight = (m_nItemHeight + 2);
}


void CMyComboBox::DrawBitmap(CDC * pDC, CMyBitmap * pBM, CPoint & point)
{
	BITMAP bm;
	pBM->GetBitmap(&bm);
	int w = bm.bmWidth;
	int h = bm.bmHeight;
	CDC memDC; memDC.CreateCompatibleDC(pDC);
	CMyBitmap *pBmpOld = (CMyBitmap *) memDC.SelectObject(pBM);
	pDC->BitBlt(point.x, point.y, w, h, &memDC, 0, 0, SRCCOPY);
	memDC.SelectObject(pBmpOld);
}

void CMyComboBox::DrawBitmap(CDC *pDC, CMyBitmap *pBM, CRect &rect)
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

int CMyComboBox::AddBitmap(CMyBitmap * pBM, CString csString)
{
	return InsertBitmap(CComboBox::GetCount(), pBM, csString);
}


int CMyComboBox::InsertBitmap(int nIndex, CMyBitmap * pBM, CString csString)
{
	int n = CComboBox::InsertString(nIndex, csString);
	if (n != CB_ERR && n != CB_ERRSPACE)
	{
		SetItemData(n, (DWORD)pBM);
		BITMAP bm;
		pBM->GetBitmap(&bm);
		SetSize(bm.bmWidth, bm.bmHeight);
	}
	return n;
}


void CMyComboBox::OutputBitmap(LPDRAWITEMSTRUCT lpDIS)
{
	CMyBitmap *pBM = (CMyBitmap*)(lpDIS->itemData);
	if (pBM && pBM != (const CMyBitmap *)(0xffffffff))
	{
		CDC* pDC = CDC::FromHandle(lpDIS->hDC);
		CString string;
		//----------------------------------------------
		// If the listbox isn't  empty (itemID == -1)
		// AND if we are not drawing the editbox 
		// portion of the ComboBox, then get the string
		// if any, to display
		//----------------------------------------------
		if ((lpDIS->itemID != -1) && !(lpDIS->itemState & ODS_COMBOBOXEDIT))
			GetLBText(lpDIS->itemID, string);
		if (string.IsEmpty())
		{
			CRect rect(lpDIS->rcItem);
			DrawBitmap(pDC, pBM, rect);
		}
		else
		{
			CPoint point;
			point.x = lpDIS->rcItem.left + 2;
			point.y = lpDIS->rcItem.top + ((lpDIS->rcItem.bottom - lpDIS->rcItem.top) / 2) - (m_nItemHeight / 2);
			DrawBitmap(pDC, pBM, point);
			CRect rcText(lpDIS->rcItem);
			rcText.DeflateRect(m_nItemWidth + 4, 0, 0, 0);
			pDC->DrawText(string, rcText, DT_SINGLELINE | DT_VCENTER);
		}
	}
}


void CMyComboBox::SetSize(int width, int height)
{
	if (width > m_nItemWidth)
		m_nItemWidth = width;
	if (height > m_nItemHeight)
		m_nItemHeight = height;
	for (int i = -1; i < GetCount(); i++)
		SetItemHeight(i, m_nItemHeight + 6);
}


int CMyComboBox::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CComboBox::OnCreate(lpCreateStruct) == -1)
		return -1;
	return 0;
}


BOOL CMyComboBox::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	return CComboBox::Create(dwStyle, rect, pParentWnd, nID);
}
