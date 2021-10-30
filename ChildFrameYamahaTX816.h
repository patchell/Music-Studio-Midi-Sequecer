#pragma once


// CChildFrameYamahaTX816.h : interface of the CChildFrameTest class
//


class CChildFrameYamahaTX816 : public CChildFrameBase
{
	CChildViewYamahaTX816 m_wndView;
	DECLARE_DYNCREATE(CChildFrameYamahaTX816)
protected:
	CSplitterWnd m_wndSplitter;
public:
	CChildFrameYamahaTX816() noexcept;
	virtual ~CChildFrameYamahaTX816();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	//------------------------------------------------
	CChildViewYamahaTX816* GetView() { return &m_wndView; }
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	afx_msg void OnFileClose();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()

};
