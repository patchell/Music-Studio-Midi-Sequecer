#pragma once

class CMsPortamentoEnd : public CMsObject
{
public:
	CMsPortamentoEnd();
	virtual ~CMsPortamentoEnd();
	bool Create(CMsSong* pSong, CMsEvent* pEvent);
	//-------------------------------------------------
	// Pure Virtual Methods
	//-------------------------------------------------
	virtual UINT Process();
	virtual UINT Play();
	virtual DRAWSTATE MouseLButtonDown(DRAWSTATE DrawState, CPoint pointMouse) = 0;
	virtual DRAWSTATE MouseLButtonUp(DRAWSTATE DrawState, CPoint pointMouse) = 0;
	virtual DRAWSTATE MouseMove(DRAWSTATE DrawState, CPoint pointMouse) = 0;
	virtual int IsTimedObject() {
		return 0;
	}
	virtual bool DoesSomething() {
		return true;
	}
	//------------------------------------------------------
	virtual void Copy(CMsObject* Source);
	virtual void Draw(CDC* pDC, int event, int maxevent);
	virtual void Save(FILE* pO);
	virtual void Print(FILE* pO, int Indent);

};

