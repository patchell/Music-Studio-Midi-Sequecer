// Polygon.cpp: implementation of the CMyPolygon class.
//
//////////////////////////////////////////////////////////////////////

#include "pch.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMyPolygon::CMyPolygon()
{
	m_pVertex = 0;		//pointer to array of vertexes
	m_pVertexRef = 0;

	m_MinX = m_MinY = m_MaxX = m_MaxY = 0;
	m_clrFill = RGB(255, 0, 0);
	m_clrOutline = RGB(0, 0, 255);
	m_nOutlineWidth = 2;
	m_nVertexCount = 0;
	m_Xoffset = 0;
}

CMyPolygon::~CMyPolygon()
{
	if(m_pVertex)delete[] m_pVertex;
}

void CMyPolygon::Create(int Count, CPoint* ptArray)
{
	m_nVertexCount = Count;
	m_pVertex = new CPoint[Count];
	m_pVertexRef = new CPoint[Count];
	for (int i = 0; i < Count; ++i)
	{
		m_pVertex[i] = ptArray[i];
		m_pVertexRef[i] = ptArray[i];
		if (m_pVertex[i].x < (m_MinX + m_Xoffset)) m_MinX = m_pVertex[i].x;
		else if (m_pVertex[i].x > (m_MaxX + m_Xoffset)) m_MaxX = m_pVertex[i].x;
		if (m_pVertex[i].y < m_MinY)m_MinY = m_pVertex[i].y;
		else if (m_pVertex[i].y > m_MaxY)m_MaxY = m_pVertex[i].y;
	}
}

BOOL CMyPolygon::PointEnclosed(CPoint nP)
{
	/*****************************************
	** PointEnclosed
	**	This function determines if a point
	** is enclosed within a polygon.
	**
	** parameters:
	**	nP....point to test
	** Returns: TRUE if point inside
	**          FALSE if point is outside
	*****************************************/
	int   i, j= m_nVertexCount -1 ;
	BOOL  Enclosed=0      ;
	int Xintercept;

	//--------------------------------------------
	// Do a precheck agains the rectangle that
	// encloses the polygon
	//--------------------------------------------
	if((m_MinX + m_Xoffset) < nP.x && (m_MaxX + m_Xoffset) > nP.x && m_MinY < nP.y && m_MaxY > nP.y)
	{
		//----------------------------------
		// very good chance now that point
		// is in polygon, so make a 
		// thorough check
		//---------------------------------
		for (i=0; i< m_nVertexCount; i++)
		{
			if (( (m_pVertex[i].y< nP.y && m_pVertex[j].y>=nP.y)
				|| (m_pVertex[j].y< nP.y && m_pVertex[i].y>=nP.y))
				&&  (m_pVertex[i].x<=nP.x || m_pVertex[j].x<=nP.x) )
			{
				Xintercept = m_pVertex[i].x+((nP.y-m_pVertex[i].y)*(m_pVertex[j].x-m_pVertex[i].x))/(m_pVertex[j].y-m_pVertex[i].y);
				Enclosed ^= Xintercept < nP.x;
			}
			j=i;
		}
	}
	return Enclosed;
}

void CMyPolygon::Draw(CDC* pDC)
{
	CPen BorderPen, * pOldPen;
	CBrush FillBrush, * pOldBrush;

	BorderPen.CreatePen(PS_SOLID, m_nOutlineWidth, m_clrOutline);
	pOldPen = pDC->SelectObject(&BorderPen);
	FillBrush.CreateSolidBrush(m_clrFill);
	pOldBrush = pDC->SelectObject(&FillBrush);
	pDC->Polygon(m_pVertex, m_nVertexCount);
	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldBrush);
}

void CMyPolygon::DrawTick(CDC* pDC, COLORREF color)
{
	int x, y;

	CPen pen, * oldpen;
	pen.CreatePen(PS_SOLID, 1, color);
	oldpen = pDC->SelectObject(&pen);
	x = m_Xoffset + (m_MaxX - m_MinX) / 2;
	y = m_MaxY;
	pDC->MoveTo(x, y);
	pDC->LineTo(x, y - 6);
	pDC->SelectObject(oldpen);
}

void CMyPolygon::Offset(int x)
{
	int i;

	m_Xoffset = x;
	for (i = 0; i < m_nVertexCount; ++i)
	{
		m_pVertex[i].x = m_pVertexRef[i].x + x;
	}

}

