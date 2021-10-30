#pragma once

// MyScrollBar.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMyScrollBar
//	This is version 2.0, revised January 24, 2021
//	This class was originally created sometime
// around 1993 for Windows 3.1 using Borland C++ 3.1
//----------------------------------------------------------------

class CNumEdit;

class CMyScrollBar : public CScrollBar
{
	void (*m_FormatFunction)(
		void *pUserParam,
		char *pBufferArray,
		int nBurrerSize,
		int nVal,
		int nAux
	);
	void (*m_EditFormatFunction)(
		void * pUserParam,
		char * pBufferArray,
		int nBurrerSize,
		int nVal,
		int nAux
	);
	void (*m_NumEditFormatFunction)(
		void* pUserParam,
		char* pBufferArray,
		int nBurrerSize,
		int nVal,
		int nAux
		);
	void* m_pUserFormatParam;
	int m_UserIntParam;
	BOOL m_bUserBufferFlag;
	char* m_pBuffer;
	int m_nBufferLen;
	//-----------------------------
	// Slider Display Objects
	//-----------------------------
	CStatic *m_pCStaticTextDisplay;
	CEdit* m_pCEditDisplayText;
	CNumEdit* m_CNumEditDisplayText;
	//------------------
	// Slider Params
	//------------------
	int m_Max;	// maximum value
	int m_Min;	// Min imum value
	int m_Pos;		// Slider Position
	int m_TrackInc;	//increment when track is clicked
	int m_Inc;	// increment when arrow is clicked
	BOOL m_bModified;
	int *m_pMasterValue;	// pointer to value to control
	//----------------------
	// User Windows Message
	//----------------------
	CWnd* m_pMsgDestinationWindow;
	UINT m_WM_UserMessage;	// message to post
	WPARAM m_SubCommand;	// submessage
	// Construction
public:
	CMyScrollBar();
	virtual ~CMyScrollBar();
	virtual BOOL Create(DWORD dwStyle,const RECT& rect, CWnd* pParent,UINT nId);
	void UpdateScrollPos(int v);
	// Methods
	void SetWindowMessage(CWnd* pDest, UINT message, WPARAM SubCommand = 0) {
		m_pMsgDestinationWindow = pDest;
		m_WM_UserMessage = message;
		m_SubCommand = SubCommand;
	}
	BOOL IsModified(void) { return m_bModified; }
	void ClearModify(void) { m_bModified = FALSE; }
	void SetUserFormatParam(void* pParam, int IntParam) {
		m_UserIntParam = IntParam;
		m_pUserFormatParam = pParam;

	}
	void SetMasterValue(int* pV) { m_pMasterValue = pV; };
	void Update(void);
	void SetNumEditDisplay(CNumEdit* pNE) { m_CNumEditDisplayText = pNE; }
	void SetScrollRange(int minval, int maxval);
	void SetDisplayEdit(CEdit* pE) { m_pCEditDisplayText = pE; }
	void SetDisplayText(CStatic* pCStatic) {m_pCStaticTextDisplay = pCStatic;}	//set pointer to CStatic object to display string
	void SetScrollPos(int position);
	void SetEditDisplayFormatFunction(void(__cdecl* pfunc)(void*, char*, int, int, int)) {
		m_EditFormatFunction = pfunc;
	}
	void SetDisplayFormatFunction(void(__cdecl* pfunc)(void*, char*, int, int, int)) {
		m_FormatFunction = pfunc;
	}

protected:
	afx_msg void VScroll(UINT nSBCode, UINT nPos);
	afx_msg void HScroll(UINT nSBCode, UINT nPos);
	DECLARE_MESSAGE_MAP()
};
