#include "Subject.h"
#include "Observer.h"
#include <algorithm>

CSubject::~CSubject()
{

}

void CSubject::Attach( CObserver *pObv )
{
    if (pObv == NULL)
    {
        return;
    }

    list<CObserver *>::iterator it;
    it = find(m_lstObv.begin(), m_lstObv.end(), pObv);
    if (it == m_lstObv.end())
    {
        m_lstObv.push_back(pObv);
    }
}

void CSubject::Detach( CObserver *pObv )
{
    if (pObv == NULL)
    {
        return;
    }

    list<CObserver *>::iterator it;
    it = find(m_lstObv.begin(), m_lstObv.end(), pObv);
    if (it != m_lstObv.end())
    {
        m_lstObv.erase(it);
    }
}

void CSubject::Notify(int nEvent)
{
    list<CObserver *>::iterator it;
    for (it = m_lstObv.begin(); it != m_lstObv.end(); ++it)
    {
        (*it)->ProcessEvent(this, nEvent);
    }
}

CSubject::CSubject()
{
}
