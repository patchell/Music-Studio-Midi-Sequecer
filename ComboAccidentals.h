#pragma once

class CComboAccidentals : public CComboDropDown
{
public:
	CComboAccidentals();
	virtual ~CComboAccidentals();
	bool Create(CPoint ptULCorner, CWnd* pParentWnd);
};

