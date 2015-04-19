/* 		
 * 	This program is free software: you can redistribute it and/or modify
 * 	it under the terms of the GNU General Public License as published by
 * 	the Free Software Foundation, either version 3 of the License, or
 * 	(at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 * 	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  File:		Subject.cpp
 *  Author:		Richard Zou
 *	Created on:	2015-03-01
 */

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
