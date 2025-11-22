#include "pch.h"

CMsPlayerQueueItem::CMsPlayerQueueItem()
{
	m_pNext = nullptr;
	m_pPrev = nullptr;
	m_pMsObject = nullptr;
}

CMsPlayerQueueItem::~CMsPlayerQueueItem()
{
}

bool CMsPlayerQueueItem::Create()
{
	// Create the object queue item
	return true;
}

UINT CMsPlayerQueueItem::Process(CMsSong* pSong)
{
	UINT retVal = QUEUE_STATUS_NOACTION;

	if (m_pMsObject)
	{
		// Process the associated music studio object
		retVal = m_pMsObject->Process();
	}
	return retVal;
}

UINT CMsPlayerQueueItem::Play(CMsSong* pSong)
{
	UINT retVal = QUEUE_STATUS_NOACTION;
	if (m_pMsObject)
	{
		// Play the associated music studio object
		retVal = m_pMsObject->Play();
	}
	return retVal;
}