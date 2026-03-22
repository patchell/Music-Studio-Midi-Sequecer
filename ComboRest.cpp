#include "pch.h"

CComboRest::CComboRest() : CComboDropDown(CComboDropDown::ComboDropDownTypes::REST_TYPE)
{
}

CComboRest::~CComboRest()
{
}

bool CComboRest::Create(CPoint ptUpperLeft, CWnd* pParentWnd)
{
	CSize szItemSize;
	int nTotalItems;
	int i;

	nTotalItems = GETAPP->GetNumRestTypes();
	szItemSize = GETAPP->bmGetRestType(0)->GetBmDim();
	szItemSize += CSize(4, 4);
	CComboDropDown::Create(
		nTotalItems - 2, // Number of items to display
		nTotalItems,	// Total Items
		szItemSize,		//size of items in combobox
		ptUpperLeft,	//uper left corner
		CSize(16, szItemSize.cy),	//size of drop arrow
		pParentWnd,				//parent window
		IDC_COMBO_RESTTYPES	// control ID
	);
	for (i = 0; i < nTotalItems; ++i)
		AddBitmap(GETAPP->bmGetRestType(i));
	SetName("Rest Type");
	return true;
}