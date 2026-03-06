#include "pch.h"

CComboTimeSignature::CComboTimeSignature() : CComboDropDown(CComboDropDown::ComboDropDownTypes::TIME_SIGNATURE)
{
}

CComboTimeSignature::~CComboTimeSignature()
{
}

bool CComboTimeSignature::Create(
	CPoint pointUpperLeft,
	CWnd* pParentWnd
)
{
	CSize szDropArrowSize;
	int nTotalItems = GETAPP->GetNumTimeSig();
	CSize szItemSize;
	int i;

	szItemSize = GETAPP->bmGetCbTimeSig(int(CBoxTimeSig::TIMESIG_2_2))->GetBmDim();
	szItemSize += CSize(8, 4);
	szDropArrowSize = CSize(16, szItemSize.cy);
	CComboDropDown::Create(
		nTotalItems - 2,			//Number of items to display
		nTotalItems,				//Total Items
		szItemSize,					//size of items in combobox
		pointUpperLeft,				//upper left corner
		szDropArrowSize,			//size of drop arrow
		pParentWnd,					//parent window
		IDC_COMBO_TIMESIG			//control ID
	);
	for (i = 0; i < nTotalItems; ++i)
		AddBitmap(GETAPP->bmGetCbTimeSig(i));
	SetName("<Time Signature>");
	return true;
}
