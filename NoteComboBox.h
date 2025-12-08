#pragma once

/////////////////////////////////////////////////////////////////////////////
// CNoteComboBox window

class CNoteComboBox : public CComboBox
{
// Construction
public:
	CNoteComboBox();
	virtual ~CNoteComboBox();

	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
public:
	int AddItem(void);
protected:
	DECLARE_MESSAGE_MAP()
};

