#pragma once

class CMsPortamentoEnd : public CMsObject
{
public:
	CMsPortamentoEnd();
	virtual ~CMsPortamentoEnd();
	virtual CMsObject* Copy(void);
	virtual void Draw(CDC* pDC, int event, int maxevent);
	virtual void Save(FILE* pO);
	virtual void Print(FILE* pO, int Indent);

};

