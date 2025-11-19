#pragma once

class CMsGlisandoEnd : public CMsObject
{
public:
	CMsGlisandoEnd();
	virtual ~CMsGlisandoEnd();
	virtual CMsObject* Copy(void);
	virtual void Draw(CDC* pDC, int event, int maxevent);
	virtual void Save(FILE* pO);
	virtual void Print(FILE* pO, int Indent);

};

