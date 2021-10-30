// Polygon.h: interface for the CMyPolygon class.
//
//////////////////////////////////////////////////////////////////////

#pragma once



#ifndef DLLexport
#define DLLexport	__declspec(dllexport)
#endif

class CMyPolygon  
{
	CPoint * m_pVertexRef;
	CPoint* m_pVertex;
	int m_Xoffset;
	int m_nVertexCount;
	int m_MaxY;
	int m_MinY;
	int m_MaxX;
	int m_MinX;
	COLORREF m_clrFill;
	COLORREF m_clrOutline;
	int m_nOutlineWidth;
public:
	CMyPolygon();
	virtual ~CMyPolygon();
	void Create(int size, CPoint* ptArray);
//	void SetPos(int X, int Y) { m_nX = X; m_nY = Y; }
	void Draw(CDC* pDC);
	void DrawTick(CDC* pDC, COLORREF color);
	void Offset(int x);
	BOOL PointEnclosed(CPoint ptP);
};
