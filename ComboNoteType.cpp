#include "pch.h"

CComboNoteType::CComboNoteType() : CComboDropDown(CComboDropDown::ComboDropDownTypes::NOTE_TYPE)
{
}

CComboNoteType::~CComboNoteType()
{
}

bool CComboNoteType::Create(CPoint ptUpperLeft, CWnd* pParentWnd)
{
	CSize szItemSize;
	int nTotalItems;
	int i;

	nTotalItems = GETAPP->GetNumNoteTypes();
	szItemSize = GETAPP->bmGetNoteType(0)->GetBmDim();
	szItemSize += CSize(4, 4);
	CComboDropDown::Create(
		nTotalItems - 2, // Number of items to display
		nTotalItems,	// Total Items
		szItemSize,		//size of items in combobox
		ptUpperLeft,	//uper left corner
		CSize(16, szItemSize.cy),	//size of drop arrow
		pParentWnd,				//parent window
		IDC_COMBO_NOTETYPES	// control ID
	);
	for (i = 0; i < nTotalItems; ++i)
		AddBitmap(GETAPP->bmGetNoteType(i));
	return true;
}
