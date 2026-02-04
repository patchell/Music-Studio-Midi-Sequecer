#pragma once


// CChildFrameBase
constexpr auto CHILD_FRAME_WIDTH = 520;	
constexpr auto CHILD_FRAME_HEIGHT = 468;	//403

class CChildFrameBase : public CMDIChildWnd
{
	CChildFrameBase* m_pNext;
	CChildFrameBase* m_pPrev;
	DECLARE_DYNAMIC(CChildFrameBase)
public:
	CChildFrameBase() noexcept;
	virtual ~CChildFrameBase();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual CChildViewBase* GetView() { return 0; };
	//Linked List Methodes
	inline CChildFrameBase* GetNext() { return m_pNext; }
	inline void SetNext(CChildFrameBase* pCF) { m_pNext = pCF; }
	inline CChildFrameBase* GetPrev() { return m_pPrev; }
	inline void SetPrev(CChildFrameBase* pCF) { m_pPrev = pCF; }
	//Sizing Methods
	virtual void UpdateColors() {}
protected:
	DECLARE_MESSAGE_MAP()
};


