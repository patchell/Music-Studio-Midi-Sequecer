// MsEvent.h: interface for the CMsEvent class.
//
//////////////////////////////////////////////////////////////////////
#pragma once

class CChildViewStaff;
class CMsSong;

class CMsEvent
{
	UINT m_Selected;	// Number of selected objects in this event
	//------------------------------
	// Linked List of Events
	//------------------------------
	CMsEvent *m_pNext;
	CMsEvent *m_pPrev;
	int m_NumberOfObjects;
	//------------------------------
	// Linked List of Music Objects
	//------------------------------
	CMsObject * m_pEventMsObjectListHead;	//event queue head
	CMsObject * m_pEventMsObjectListTail;	//event queue end
	//------------------------------
	// Event Identifierws
	//------------------------------
	int m_Index;	// posisiton in chaihn
	UINT m_EventID;
	//------------------------------
	CChildViewStaff* m_pView;	// Parent View
	CMsSong* m_pParentSong;		// Parent Song
public:
	CMsEvent();
	virtual ~CMsEvent();
	bool Create(CMsSong* pParentSong, CChildViewStaff* pCV);
	virtual void Draw(CDC *pDC);
	void DrawStaffLines(CDC* pDC);	
	//--------------------------------------
	// Object management functions
	//--------------------------------------
	void SetEventMsObjectHead(CMsObject* pMO) { 
		m_pEventMsObjectListHead = pMO; 
	}
	CMsObject* GetEventMsObjectHead(void) { 
		return m_pEventMsObjectListHead; 
	}
	void SetEventMsObjectTail(CMsObject* pMO) { 
		m_pEventMsObjectListTail = pMO; 
	}
	CMsObject* GetEventMsObjectTail() { 
		return m_pEventMsObjectListTail; 
	}
	//------------ Event Index Management ----------------
	void SetEventIndex(int I) { 
		m_Index = I; 
	}
	int GetIndex() const { 
		return m_Index; 
	}
	int GetPhysicalIndex();
	UINT GetEventID() { return m_EventID; }
	//------------ Music Object Linked List ----------------
	void AddObject(CMsObject* pO);
private:
	void AddObjectAtEnd(CMsObject *pO);
	void AddObjectAtStart(CMsObject* pO);
	void AddObjectAtHead(CMsObject* pO);
	void AddObjectAtTail(CMsObject* pO);
	void AddNoteInOrder(CMsObject* pO);
public:
	void InsertObjectAfter(CMsObject* pO, CMsObject* pAfterObj);
	void InsertObjectBefore(CMsObject* pO, CMsObject* pBeforeObj);
	int AreThereAnyNotesInThisEvent();
	bool IsThisObjectInThisEvent(CMsObject* pO);
	bool IsThereOnlyOneNoteInThisEvent();
	CMsObject* FindFirstObjectOfType(CMsObject::MsObjType ObjectType);
	CMsObject* FindNextObjectOfType(CMsObject::MsObjType ObjectType, CMsObject* pPrevObj);
	CMsNote* FindFirstNote();
	CMsNote* FindNextNote(CMsNote* pPrevNote);
	int RemoveObject(CMsObject *pObj);
	CMsObject* ObjectAlreadyHere(CMsObject* pObj);
	int GetMinEventDuration();
	//--------------------------------------------
	CChildViewStaff* GetParentView() { return m_pView; }
	void SetParentView(CChildViewStaff* pCV) { m_pView = pCV; }
	CMsSong* GetSong() { return m_pParentSong; }
	void SetSong(CMsSong* pMS) { m_pParentSong = pMS; }
	// Function returns Note of a given pitch
	CMsNote* FindNote(int Note, int Accidental);
	// Determines if the paricular object is in this event
	CMsObject* ContainsObjectType(CMsObject::MsObjType ObjectType);
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
	void Print(FILE *pO, const char* s, int Indent);
	void PrintEvents(FILE* pO, const char*pTitle, int Indent);
};
