#pragma once

class CMsGlisandoEnd : public CMsObject
{
public:
	CMsGlisandoEnd();
	virtual ~CMsGlisandoEnd();
	bool Create(CMsSong* pSong);
	//-------------------------------------------------
	// Pure Virtual Methods
	//-------------------------------------------------
	virtual UINT Process();
	virtual UINT Play();
	virtual int MouseLButtonDown(int DrawState, CPoint pointMouse);
	virtual int MouseLButtonUp(int DrawState, CPoint pointMouse);
	virtual int MouseMove(int DrawState, CPoint pointMouse);
	virtual bool IsTimedObject() {
		return true;
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

