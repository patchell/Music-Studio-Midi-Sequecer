#pragma once

class CComboMisc : public CComboDropDown
{
public:
	CComboMisc();
	virtual ~CComboMisc();
	bool Create(CPoint ptULCorner, CWnd* pParentWnd);
};

