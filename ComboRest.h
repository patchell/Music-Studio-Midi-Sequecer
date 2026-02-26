#pragma once

class CComboRest :  public CComboDropDown
{
public:
	CComboRest();
	virtual ~CComboRest();
	bool Create(CPoint ptUpperLeft, CWnd* pParentWnd);
};

