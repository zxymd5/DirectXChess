/*      
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  File:       Subject.h
 *  Author:     Richard Zou
 *  Created on: 2015-03-01
 */

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
    virtual void Notify(int nEvent, void *pParam = NULL);
    
protected:
    CSubject();

private:
    list<CObserver *> m_lstObv;
};

#endif