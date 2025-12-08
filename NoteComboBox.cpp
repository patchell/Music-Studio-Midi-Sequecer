// NoteComboBox.cpp : implementation file
//

#include "pch.h"


/////////////////////////////////////////////////////////////////////////////
// CNoteComboBox

CNoteComboBox::CNoteComboBox()
{
}

CNoteComboBox::~CNoteComboBox()
{
}


BEGIN_MESSAGE_MAP(CNoteComboBox, CComboBox)
	//{{AFX_MSG_MAP(CNoteComboBox)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNoteComboBox message handlers

int ItemID[6] = {
	IDB_NOTE_WHOLE,
	IDB_NOTE_HALF,
	IDB_NOTE_QUARTER,
	IDB_NOTE_EIGTH,
	IDB_NOTE_SIXTEENTH,
	IDB_NOTE_THIRTYSECOND
};

void CNoteComboBox::DrawItem(LPDRAWITEMSTRUCT lpdis) 
{
    CDC dc;
    dc.Attach (lpdis->hDC);
    CRect rect = lpdis->rcItem;
    UINT nIndex = lpdis->itemID;


//    if (lpdis->itemState & ODS_FOCUS)
//        dc.DrawFocusRect (rect);
    if (nIndex != (UINT) -1) {
		CMyBitmap bm;
		bm.LoadBitmap(ItemID[nIndex]);
		CDC memdc;
		dc.CreateCompatibleDC(&memdc);
		memdc.SelectObject(&bm);
		dc.BitBlt(0,0,16,16,&memdc,0,0,SRCCOPY);
        // Draw the item.
    }
    dc.Detach ();
}

void CNoteComboBox::MeasureItem(LPMEASUREITEMSTRUCT lpmis) 
{
    lpmis->itemHeight = 16;    // Item height in pixels
}

BOOL CNoteComboBox::PreCreateWindow(CREATESTRUCT& cs) 
{
    if (!CComboBox::PreCreateWindow (cs))
        return FALSE;
	cs.style &= ~(LBS_OWNERDRAWVARIABLE | LBS_SORT);
	cs.style |= LBS_OWNERDRAWFIXED;
    return TRUE;
}

int CNoteComboBox::AddItem()
{
    int nIndex = AddString (_T (""));
	if((nIndex == LB_ERR) || (nIndex == LB_ERRSPACE))
		MessageBox(_T("Combo Box Error"),_T("DEBUG") );
    return nIndex;

}
