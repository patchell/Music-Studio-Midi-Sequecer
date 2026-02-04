
// ChildFrm.h : interface of the CChildFrameStaff class
//

#pragma once

class CChildFrameStaff : public CChildFrameBase
{
	CChildViewStaff m_wndStaffView;
	DECLARE_DYNCREATE(CChildFrameStaff)
protected:
// Splitter window is not used, YET!
//	CSplitterWnd m_wndSplitter;
public:
	CChildFrameStaff() noexcept;
	virtual ~CChildFrameStaff();
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	virtual void UpdateColors();
	int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual CChildViewStaff* GetView() { return &m_wndStaffView; }
protected:
	afx_msg void OnFileClose();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	// Generated message map functions
	DECLARE_MESSAGE_MAP()
};
