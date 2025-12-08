// MyListBox.cpp : implementation file
//

#include "pch.h"

// CMyListBox

IMPLEMENT_DYNAMIC(CMyListBox, CListBox)

CMyListBox::CMyListBox()
{
	m_pComboBox = 0;
}

CMyListBox::~CMyListBox()
{
}


BEGIN_MESSAGE_MAP(CMyListBox, CListBox)
//	ON_WM_NCDESTROY()
END_MESSAGE_MAP()



// CMyListBox message handlers

void CMyListBox::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);

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
		m_pComboBox->OutputBitmap(lpDrawItemStruct, true);
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
		m_pComboBox->OutputBitmap(lpDrawItemStruct, false);
		//clean up DC
		pDC->SelectObject(pOldBrush);
		pDC->SelectObject(pOldPen);
	}
	// Focus
	if (lpDrawItemStruct->itemAction & ODA_FOCUS)
		pDC->DrawFocusRect(&lpDrawItemStruct->rcItem);
}


LRESULT CMyListBox::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	int item;
	static CPoint LastPoint;

	switch (message)
	{
		case WM_MOUSEMOVE:
			{
				CPoint p;
				CRect clientrect;
				int item;

				GetClientRect(&clientrect);
				p.x = LOWORD(lParam);
				p.y = HIWORD(lParam);
				LastPoint = p;

				if (clientrect.PtInRect(p))
				{
					item = WhereIsTheMouse(p);
					if (m_pComboBox->CheckBoxIsEnable(item))
						return 0;
				}
			}
			break;
		case WM_LBUTTONDOWN:
			{
				CPoint p;
				CRect clientrect,rect,rcCB;

				p.x = LOWORD(lParam);
				p.y = HIWORD(lParam);
				item = WhereIsTheMouse(p);
				GetItemRect(item, &rect);
				GetCheckBoxRect(item, rcCB);
				if (rcCB.PtInRect(p) && m_pComboBox->CheckBoxIsEnable(item) )
				{
					int Id = m_pComboBox->GetDlgCtrlID();
					m_pComboBox->SetCheck(item, m_pComboBox->GetCheck(item) ^ 1);
					m_pComboBox->GetParent()->PostMessageW(
						WM_COMMAND,
						MAKELONG(Id, CBN_SELCHANGE), 
						LPARAM(m_pComboBox->m_hWnd)
					);
					Invalidate();
					m_pComboBox->Invalidate();
					return 0;
				}
				else
				{
					m_pComboBox->ShowDropDown(0);
					m_pComboBox->Invalidate();
				}
			}
			break;
		case WM_CHAR:
			if (wParam == VK_SPACE)
			{
				CRect clientrect;
				GetClientRect(&clientrect);
				if (clientrect.PtInRect(LastPoint))
				{
					item = WhereIsTheMouse(LastPoint);
					if (m_pComboBox->CheckBoxIsEnable(item))
					{
						m_pComboBox->SetCheck(item, m_pComboBox->GetCheck(item) ^ 1);
						m_pComboBox->GetParent()->PostMessageW(
							WM_COMMAND,
							MAKELONG(m_pComboBox->GetDlgCtrlID(), CBN_SELCHANGE),
							LPARAM(m_pComboBox->m_hWnd));
						Invalidate();
						m_pComboBox->Invalidate();
						return 0;
					}
				}
			}
			break;
	}	//end of switch(message)
	return CListBox::WindowProc(message, wParam, lParam);
}

int CMyListBox::GetCheckBoxRect(int nItem, CRect & rcCB)
{
	CRect rect;
	GetItemRect(nItem, &rect);
	int h = rect.Height();
	h -= 16;
	h /= 2;
	rcCB.SetRect(2, rect.top + h, 18, rect.bottom - h);
	return 0;
}

int CMyListBox::WhereIsTheMouse(CPoint p)
{
	int rV;	//holds the item number
	int nItemHeight;
	int nTopIndex;

	nItemHeight = GetItemHeight(0);
	nTopIndex = GetTopIndex();
	rV = nTopIndex + p.y / nItemHeight;
	return rV;
}