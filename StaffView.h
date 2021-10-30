#if !defined(AFX_STAFFVIEW_H__D269DF16_18EB_4F1F_81C8_D1F291DD4243__INCLUDED_)
#define AFX_STAFFVIEW_H__D269DF16_18EB_4F1F_81C8_D1F291DD4243__INCLUDED_


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StaffView.h : header file
//
#include "MsObject.h"
#include "MsSong.h"
#include "DisplayStatic.h"
#include "StaticStatus.h"
//#include "MSPlayerDlg.h"

#define DRAWMODE_NOP		0
#define DRAWMODE_NOTE		1
#define DRAWMODE_BAR		2
#define DRAWMODE_KKEYSIG	3
#define DRAWMODE_TIMESIG	4
#define DRAWMODE_TIE		5
#define DRAWMODE_LOUDNESS	6
#define DRAWMODE_TEMPO		7
#define DRAWMODE_COPY		8
#define DRAWMODE_MOVE		9
#define DRAWMODE_MIDINOTEIN	10

///-------------------------------
/// States for DRAWMODE_TIE
///-------------------------------

#define DRAWSTATE_TIE_FIRSTNOTE		0
#define DRAWSTATE_TIE_SECONDNOTE	1

/////////////////////////////////////////////////////////////////////////////
// CStaffView view
#define STAFFVIEW_MOUSEUP	0
#define STAFFVIEW_MOUSEDOWN	1

class CStaffView : public CMDIChildWnd
{
private:
	int m_nLastSongPosition;
	friend class CMSPlayerDlg;
	int m_SongScrollPos;
protected:
	CStaffView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CStaffView)

// Attributes
public:
	int m_FirstSelectedEvent;
	int m_LastSelectedEventIndex;
	CStaticStatus * m_pStatus;
	CMsKeySignature * m_pKeySig;
	CPoint m_TieEndPoint;
	CPoint m_TieStartPoint;
	CMsNote * m_pSecondTieNote;
	CMsNote * m_pFirstTieNote;
	int m_nDrawState;
	int m_ExitEditRegion;
	int m_lastpitch;
	HMIDIOUT m_hMidiOut;
	CMsNote *pLastNote;
	int m_nDrawEvent;
	CMsObject * m_pDrawObject;
	int m_nDrawMode;
	int m_MouseInEditRegion;
	int m_MaxEvents;		//maximum number of events that can be displayed
	CDisplayStatic * m_pDisp;
	CMsSong * m_pSong;
	// Keeps track of the number of notes pressed down for an event in Midi Input Mode
	int m_nMidiNotesOn;
	int m_nMidiInputNoteSetup;
	// Pointer to window that has the midi program change message handler
	CWnd *m_pPgmChng;
	// Last note that was hovered over
	CMsNote *m_pLastSelNote;
	CMsObject *m_pLoudnessSelected;
	CMsObject *m_pTimeSigSelected;
	CMsObject *m_pKeySigSelected;
	CMsRepeatStart *m_pRepeatStartSelected;
	CMsRepeatEnd *m_pRepeatEndSelected;
	CMsObject  *m_pTempoSelected;

// Operations
public:
	void MoveBlock(int dest);
	void CopyBlock(int dest);
	void ChangeDuration(int From, int To);
	void ChangeInst(int From, int To);
	void AddRepeat(int n);
	void InsertBlock(void);
	void DecrPitch();
	void IncrPitch(void);
	void IncreaseDuration(void);
	int MouseInRegion(CPoint p);
	int QuantizeY(int y);
	void CalcMaxEvents(void);
	int XtoEventIndex(int x);
	int YtoNote(int y);

// Overrides
public:
	virtual void OnInitialUpdate();
protected:
	virtual void PreSubclassWindow();
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
// Implementation
protected:
	virtual ~CStaffView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	LRESULT OnInitStaff(WPARAM wp, LPARAM lp);
	LRESULT OnDrawMessage(WPARAM wp, LPARAM lp);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	DECLARE_MESSAGE_MAP()
	afx_msg LRESULT OnShortmidimsg(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnStaffDispEvent(WPARAM wParam, LPARAM lParam);
public:
	// State of the mouse, UP, DOWN
	int m_nMouseState;
	void UpdateNoteInfo(int RestFlag);
	int IsEventDisplayed(CMsEvent * pEV);
	void UpdateScrollbarInfo(int TotalEvents);
	int MouseOverObject(int nEvent, int nObjectType, CMsObject ** ppObjectSelected);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STAFFVIEW_H__D269DF16_18EB_4F1F_81C8_D1F291DD4243__INCLUDED_)
