
// MainFrm.h : interface of the CMainFrame class
//

#pragma once

class CMainFrame : public CMDIFrameWnd
{
	DECLARE_DYNAMIC(CMainFrame)
	CChildFrameBase* m_pHead;
	CChildFrameBase* m_pTail;
	int m_NumChildren;
public:
	CMainFrame() noexcept;
	virtual ~CMainFrame();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	void OnInitialUpdate();
	void UpdateColors();
	//-----------------------------------------
	// Methodes to manage those naugty children
	//-----------------------------------------
	int GetNumberOfChildren() { return m_NumChildren; }
	inline CChildFrameBase* GetHead() { return m_pHead; }
	inline void SetHead(CChildFrameBase* pCF) { m_pHead = pCF; }
	inline CChildFrameBase* GetTail() { return m_pTail; }
	inline void SetTail(CChildFrameBase* pCF) { m_pTail = pCF; }
	void AddChild(CChildFrameBase* pCF);
	void RemoveChild(CChildFrameBase* pCF);
	BOOL IsChildStillAlive(unsigned childID);
	//------------- Debug Stuff --------------------
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
protected:  // control bar embedded members
	CToolBar          m_wndToolBar;
	CStatusBar        m_wndStatusBar;
// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()
};


