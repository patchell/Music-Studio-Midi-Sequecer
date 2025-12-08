void CMsSong::AddEventAtEnd(CMsEvent* pE)
{
	if (m_pEventListHead == 0)
	{
		m_pEventListHead = pE;
		m_pEventListTail = pE;
	}
	else
	{
		pE->SetPrev(m_pEventListTail);
		m_pEventListTail->SetNext(pE);
		m_pEventListTail = pE;
	}
}

void CMsSong::AddEventAtStart(CMsEvent* pE)
{
	if (m_pEventListHead == 0)
	{
		m_pEventListHead = pE;
		m_pEventListTail = pE;
	}
	else
	{
		pE->SetNext(m_pEventListHead);
		m_pEventListHead->SetPrev(pE);
		m_pEventListHead = pE;
	}
}
