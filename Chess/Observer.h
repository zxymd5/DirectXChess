#ifndef _OBSERVER_H
#define _OBSERVER_H  

#include "Subject.h"
using namespace std;

class CObserver
{
public:
    virtual ~CObserver();
    virtual void ProcessEvent(CSubject *pSub, int nEvent) = 0;

protected:
    CObserver();
    CSubject    *m_pSubject;
};

#endif