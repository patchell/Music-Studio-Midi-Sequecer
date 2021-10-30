#pragma once

class CMsGlisandoStart : public CMsObject
{
public:
	CMsGlisandoStart();
	~CMsGlisandoStart();
	virtual CMsObject* Copy(void);
	virtual void Draw(CDC* pDC, int event, int maxevent);
	virtual void Save(FILE* pO);
	virtual void Print(FILE* pO);
};

