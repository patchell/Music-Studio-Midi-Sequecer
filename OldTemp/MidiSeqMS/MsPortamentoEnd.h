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
	virtual int MouseLButtonDown(int DrawState, CPoint pointMouse) = 0;
	virtual int MouseLButtonUp(int DrawState, CPoint pointMouse) = 0;
	virtual int MouseMove(int DrawState, CPoint pointMouse) = 0;
	virtual bool IsTimedObject() {
		return false;
	};
	virtual bool DoesSomething() {
		return true;
	}
	//------------------------------------------------------
	virtual CMsObject* Copy(void);
	virtual void Draw(CDC* pDC, int event, int maxevent);
	virtual void Save(FILE* pO);
	virtual void Print(FILE* pO, int Indent);

};

