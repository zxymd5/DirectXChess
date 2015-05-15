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
 *  File:       DXListCtrl.cpp
 *  Author:     Richard Zou
 *  Created on: 2015-03-01
 */

#include "DXListCtrl.h"
#include <assert.h>

CDXListCtrl::CDXListCtrl(void)
{
    m_nListItemCount = 0;
}

CDXListCtrl::~CDXListCtrl(void)
{
}

void CDXListCtrl::Shutdown()
{
    for (int i = 0; i < MAX_LIST_ITEM; i++)
    {
        m_arrListItem[i].Shutdown();
    }
}

void CDXListCtrl::Init( const char *pWidgetName, const char *pItemTextureFile, const char *pListItemBasename,
                       int nLeft, int nTop, int nWidth, int nHeight, int nFontType, 
                       int nListItemCount, int nColumnCount,
                       int arrColumnWidth[MAX_LIST_ITEM_COLUMN], 
                       bool bVisible, int nDepth )
{
    strcpy(m_szWidgetName, pWidgetName);
    m_bVisible = bVisible;
    m_nDepth = nDepth;
    m_nListItemCount = nListItemCount;
    int nListItemHeight = nHeight / nListItemCount;
    char szListItemName[50];

    for (int i = 0; i < nListItemCount; i++)
    {
        sprintf(szListItemName, "%s%d", pListItemBasename, i);
        m_arrListItem[i].Init(szListItemName, pItemTextureFile, 
                            nLeft, nTop, nWidth, nHeight, 
                            nFontType, nColumnCount, 
                            arrColumnWidth, 
                            bVisible, nDepth);

        nTop += nListItemHeight;
    }
}

void CDXListCtrl::Render()
{
    for (int i = 0; i < m_nListItemCount; i++)
    {
        m_arrListItem[i].Render();
    }
}

int CDXListCtrl::GetListItemCount() const
{
    return m_nListItemCount;
}

CDXListItem & CDXListCtrl::GetListItem( int nListItemIndex )
{
    assert(nListItemIndex < m_nListItemCount && nListItemIndex >= 0);
    return m_arrListItem[nListItemIndex];
}
