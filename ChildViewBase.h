#pragma once

struct MessageList {
	int m_WM_messase;
	CString m_csMessageName;
	MessageList() { m_WM_messase = 0; }
};

// CChildViewBase

class CChildViewBase : public CWnd
{
	DECLARE_DYNAMIC(CChildViewBase)
protected:
	MessageList* m_pMessageList;
	int m_NumOfMessListEntries;
	unsigned m_thisID;
public:
	CChildViewBase();
	virtual ~CChildViewBase();

	BOOL PreCreateWindow(CREATESTRUCT& cs);
	int GetNumOfMessageEntries() { return m_NumOfMessListEntries; }
	CString& GetMessageName(int messNum) {
		return m_pMessageList[messNum].m_csMessageName;
	}
	int GetMessageCommand(int messNum) { return m_pMessageList[messNum].m_WM_messase; }
	virtual unsigned GetID() { return m_thisID; }
protected:
	DECLARE_MESSAGE_MAP()
};


