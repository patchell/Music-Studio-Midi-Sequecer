// MsKeySignature.h: interface for the CMsKeySignature class.
//
//////////////////////////////////////////////////////////////////////
#pragma once

class CMsNote;

class CMsKeySignature : public CMsObject  
{
	UINT m_KeySigCorrection[12];
	UINT m_KeySignature;
public:
	CMsKeySignature();
	virtual ~CMsKeySignature();
	bool Create(CMsSong* pSong, CMsEvent* pParentEvent, UINT key);
	//-------------------------------------------------
	// Pure Virtual Methods
	//-------------------------------------------------
	virtual UINT Process();
	virtual UINT Play();
	virtual DRAWSTATE MouseLButtonDown(DRAWSTATE DrawState, CPoint pointMouse);
	virtual DRAWSTATE MouseLButtonUp(DRAWSTATE DrawState, CPoint pointMouse);
	virtual DRAWSTATE MouseMove(DRAWSTATE DrawState, CPoint pointMouse);
	virtual int IsTimedObject() { return 0; }
	virtual bool DoesSomething() {
		return true;
	}
	//------------------------------------------------------
	virtual void Print(FILE *pO, int Indent);
	virtual void Save(FILE *pO);
	virtual void Draw(CDC *pDC, int event, int maxevent);
	virtual UINT ObjectToString(CString& csString, UINT mode = 0) { return 0; }
	virtual void ObjectRectangle(CRect& rect, UINT Event);
	int GetKeySignature(){return m_KeySignature;}
	void SetKeySignature(UINT k){m_KeySignature = k;}
	UINT GetKeySigCorrection(UINT note, INT accidental);
	void SetKeySigCorrection();
	CMsObject * Copy();
	int NoteToPosition(int Note);
	//--------------------------------------------------
	static CString KeySigStringTab[APP_NUM_KEYSIGNATURES + 1];
};
