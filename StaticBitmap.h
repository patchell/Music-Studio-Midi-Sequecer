#pragma once
//
// StaticBitmap.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CStaticBitmap window

class CStaticBitmap : public CStatic
{
	COLORREF m_Color;
	CWnd* m_pMaster;
	int m_Msg;		// Message Parameter
	int m_WMsg;		// Windows Message command
	CFont m_Textfont;
	CString m_csTextString;
	// Construction
public:
	CStaticBitmap();
	virtual ~CStaticBitmap();
	void SetColorAndText(CString& csText, COLORREF crColor) {
		SetColor(crColor);
		SetTextString(csText);
		Invalidate();
	}
	void SetColor(COLORREF c){m_Color = c;Invalidate();}
	COLORREF GetColor(void){return m_Color;}
	void SetupMessaging(CWnd *pW,int Wmsg,int Smsg);
	void SendChange(void);
	void SetTheMaster(CWnd *W){m_pMaster = W;}
	void SetMsgValue(int m) { m_Msg = m; }
	void SetWMsgValue(int m){m_WMsg = m;}
	void SetTextString(CString& csString) {
		m_csTextString = csString;
		Invalidate();
	}
	void Init();
protected:
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

	DECLARE_MESSAGE_MAP()
};

