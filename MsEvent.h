// MsEvent.h: interface for the CMsEvent class.
//
//////////////////////////////////////////////////////////////////////
#pragma once

class CChildViewStaff;
class CMsSong;

class CMsEvent
{
	UINT m_Selected;
	CMsEvent *m_pNext;
	CMsEvent *m_pPrev;
	CMsObject * m_pEventObjectListHead;	//event queue head
	CMsObject * m_pEventObjectListTail;	//event queue end
	CChildViewStaff* m_pView;
	CMsSong* m_pParentSong;
	int m_Index;	// posisiton in chaihn
	UINT m_EventID;
	INT m_NumberOfObjects;
public:
	CMsEvent();
	virtual ~CMsEvent();
	void Create(CMsSong* pParentSong, CChildViewStaff* pCV, UINT Event);
	virtual void Draw(CDC *pDC, int event, int maxevent);
	//--------------------------------------
	// Object management functions
	//--------------------------------------
	void SetEventObjectHead(CMsObject* pMO) { m_pEventObjectListHead = pMO; }
	CMsObject* GetEventObjectHead(void) { return m_pEventObjectListHead; }
	void SetEventObjectTail(CMsObject* pMO) { m_pEventObjectListTail = pMO; }
	CMsObject* GetEventObjectTail() { 
		return m_pEventObjectListTail; 
	}
	void SetIndex(int I) { m_Index = I; }
	int GetIndex() { return m_Index; }
	UINT GetEventID() { return m_EventID; }
	//------------ Linked List ----------------
	void AddObjectAtEnd(CMsObject *pO);
	void AddObjectAtStart(CMsObject* pO);
	void AddObjectAtHead(CMsObject* pO);
	void AddObjectAtTail(CMsObject* pO);
	void InsertObjectAfter(CMsObject* pO, CMsObject* pAfterObj);
	void InsertObjectBefore(CMsObject* pO, CMsObject* pBeforeObj);
	bool AreThereAnyNotesInThisEvent();
	bool IsThereOnlyOneNoteInThisEvent();
	void AddNoteInOrder(CMsObject* pO);
	CMsNote* FindFirstNote();
	CMsNote* FindNextNote(CMsNote* pPrevNote);
	int RemoveObject(CMsObject *pObj);
	CMsObject* ObjectAlreadyHere(CMsObject* pObj);
	//--------------------------------------------
	CChildViewStaff* GetParentView() { return m_pView; }
	void SetParentView(CChildViewStaff* pCV) { m_pView = pCV; }
	CMsSong* GetSong() { return m_pParentSong; }
	void SetSong(CMsSong* pMS) { m_pParentSong = pMS; }
	// Function returns Note of a given pitch
	CMsNote* FindNote(int Note, int Accidental);
	// Determines if the paricular object is in this event
	CMsObject* ContainsObjectType(int ObjectType);
	CMsObject* ContainsRepeatObject();
	//------------ Event Management --------------------
	void SetNext(CMsEvent *pE){m_pNext = pE;}
	CMsEvent *GetNext(void){return m_pNext;}
	void SetPrev(CMsEvent *pE){m_pPrev = pE;}
	CMsEvent *GetPrev(void){return m_pPrev;}
	//--------------------------------------
	// Selection management
	//--------------------------------------
	UINT IsSelected(void){return m_Selected;}
	void SetSelected(UINT s){m_Selected = s;}
	int AreObjectsSelected();
	CMsObject * GetFirstSelectedObject();
	CMsObject * GetNextSelectedObject(CMsObject * pLastSelectedObject);
	//--------------------------------------
	// Flip Note Heads
	//------------------------------------- 
	bool FlipNoteHeads();
	//--------------------------------------
	// Used for debugging
	//---------------------------------------
	void Print(FILE *pO);
	void PrintEvents(const char*pTitle);
};
