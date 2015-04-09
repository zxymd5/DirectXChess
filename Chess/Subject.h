#ifndef _SUBJECT_H
#define _SUBJECT_H

#include <list>
using namespace std;

class CObserver;

class CSubject
{
public:
    virtual ~CSubject();
    virtual void Attach(CObserver *pObv);
    virtual void Detach(CObserver *pObv);
    virtual void Notify(int nEvent);
    
protected:
    CSubject();

private:
    list<CObserver *> m_lstObv;
};

#endif