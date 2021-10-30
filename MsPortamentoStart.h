#pragma once

class CMsPortamentoStart : public CMsObject
{
public:
	CMsPortamentoStart();
	~CMsPortamentoStart();
	virtual void Save(FILE* pO);
	virtual CMsObject* Copy(void);
	virtual void Draw(CDC* pDC, int event, int maxevent);
	virtual void Print(FILE* pO);
};

