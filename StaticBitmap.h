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
	char m_aTextString[10];
	// Construction
public:
	CStaticBitmap();
	virtual ~CStaticBitmap();
	void SetColorAndText(const char* t, COLORREF c) {
		SetColor(c);
		SetTextString(t);
		Invalidate();
	}
	void SetColor(COLORREF c){m_Color = c;Invalidate();}
	COLORREF GetColor(void){return m_Color;}
	void SetupMessaging(CWnd *pW,int Wmsg,int Smsg);
	void SendChange(void);
	void SetTheMaster(CWnd *W){m_pMaster = W;}
	void SetMsgValue(int m) { m_Msg = m; }
	void SetWMsgValue(int m){m_WMsg = m;}
	void SetTextString(const char* pS) {
		strcpy_s(m_aTextString, 9, pS);
		Invalidate();
	}
	void Init();
protected:
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

	DECLARE_MESSAGE_MAP()
};

