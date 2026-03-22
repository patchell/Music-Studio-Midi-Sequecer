#include "pch.h"

CComboDecorations::CComboDecorations() : CComboDropDownToggle(ComboDropDownToggleTypes::ACCIDENTALS	)
{
}

CComboDecorations::~CComboDecorations()
{
}

bool CComboDecorations::Create(CPoint ptUpperLeft, CWnd* pParentWnd)
{
	CSize szDropArrowSize;
	int nTotalItems = GETAPP->GetNumDecorations();
	CSize szItemSize;
	int i;

	szItemSize = CMidiSeqMSApp::bmGetCbDecorationNotSel(int(ComboBoxDecorationIndex::NONE))->GetBmDim();
	szItemSize += CSize(8, 8);
	szDropArrowSize = CSize(18, szItemSize.cy);
	CComboDropDownToggle::Create(
		nTotalItems,				//Number of items to display
		nTotalItems,				//Total Items
		szItemSize,					//size of items in combobox
		ptUpperLeft,				//upper left corner
		szDropArrowSize,			//size of drop arrow
		pParentWnd,					//parent window
		IDC_COMBO_DECORATIONS		//control ID
	);
	for (i = 0; i < nTotalItems; ++i)
	{
		AddNotSelBitmapID(CMidiSeqMSApp::GetDecorationsBmCbIdsNotSel(i));
		AddSelBitmapID(CMidiSeqMSApp::GetDecorationsBmCbIdsSel(i));
		SetItemFlags(i, GetDecorationFlags(i));
	}
	SetName("Decorations");
	return true;
}