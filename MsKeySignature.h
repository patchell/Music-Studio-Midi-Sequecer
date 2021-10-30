// MsKeySignature.h: interface for the CMsKeySignature class.
//
//////////////////////////////////////////////////////////////////////
#pragma once

class CMsKeySignature : public CMsObject  
{
	UINT m_KeySigCorrection[12];
	UINT m_KeySignature;
public:
	CMsKeySignature();
	virtual ~CMsKeySignature();
	void Create(CMsSong*pSong,UINT ParentEvent, UINT key) { 
		m_KeySignature = key; 
		SetKeySigCorrection();
		CMsObject::Create(pSong, ParentEvent);
	}
	virtual void Print(FILE *pO);
	virtual void Save(FILE *pO);
	virtual void Draw(CDC *pDC, int event, int maxevent);
	virtual UINT Process();
	virtual UINT ObjectToString(CString& csString, UINT mode = 0) { return 0; }
	virtual void ObjectRectangle(CRect& rect, UINT Event);
	int GetKeySignature(){return m_KeySignature;}
	void SetKeySignature(UINT k){m_KeySignature = k;}
	UINT GetKeySigCorrection(UINT note, UINT accidental);
	void SetKeySigCorrection();
	CMsObject * Copy();
	int NoteToPosition(int Note);
};
