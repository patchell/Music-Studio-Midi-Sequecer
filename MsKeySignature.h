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
	virtual DRAWSTATE MouseMove(DRAWSTATE DrawState, CPoint pointMouse, MouseRegions Region, MouseRegionTransitionState Transition);
	virtual int IsTimedObject() { return 0; }
	virtual bool DoesSomething() {
		return true;
	}
	virtual void Draw(CDC* pDC);
	virtual StaffMouseStates StaffTransition(CPoint pointMouse, int NewNote, CMsEvent* pEvent);
	//------------------------------------------------------
	virtual void Print(FILE *pO, int Indent);
	virtual void Save(FILE *pO);
	virtual UINT ObjectToString(CString& csString, UINT mode = 0) { return 0; }
	virtual void ObjectRectangle(CRect& rect, UINT Event);
	int GetKeySignature(){return m_KeySignature;}
	void SetKeySignature(UINT k){m_KeySignature = k;}
	UINT GetKeySigCorrection(UINT note, INT accidental);
	void SetKeySigCorrection();
	void Copy(CMsObject* Source);
	int NoteToPosition(int Note);
	//--------------------------------------------------
	inline static CString KeySigStringTab[APP_NUM_KEYSIGNATURES + 1] = {
			_T("ERROR"),	//5
			_T("C MAJ"),	//1X
			_T("G MAJ"),	//2X
			_T("D MAJ"),	//3X
			_T("A MAJ"),	//4X
			_T("E MAJ"),	//5X
			_T("B MAJ"),	//6X
			_T("F# MAJ"),	//7X
			_T("C# MAJ"),	//8X
			_T("F MAJ"),	//9X
			_T("Bb MAJ"),	//10X
			_T("Eb MAJ"),	//11X
			_T("Ab MAJ"),	//12X
			_T("Db MAJ"),	//13X
			_T("Gb MAJ"),	//14X
			_T("Cb MAJ")	//15X
	};

};
