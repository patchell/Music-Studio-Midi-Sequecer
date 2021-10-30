#if !defined(AFX_NOTECOMBOBOX_H__D9C48F1C_B269_4F50_822A_7FA54F945A47__INCLUDED_)
#define AFX_NOTECOMBOBOX_H__D9C48F1C_B269_4F50_822A_7FA54F945A47__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NoteComboBox.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CNoteComboBox window

class CNoteComboBox : public CComboBox
{
// Construction
public:
	CNoteComboBox();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNoteComboBox)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	int AddItem(void);
	virtual ~CNoteComboBox();

	// Generated message map functions
protected:
	//{{AFX_MSG(CNoteComboBox)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NOTECOMBOBOX_H__D9C48F1C_B269_4F50_822A_7FA54F945A47__INCLUDED_)
