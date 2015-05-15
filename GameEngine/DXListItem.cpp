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
 *  File:       DXListItem.cpp
 *  Author:     Richard Zou
 *  Created on: 2015-03-01
 */

#include "DXListItem.h"
#include "GameEngine.h"
#include <assert.h>

CDXListItem::CDXListItem(void)
{
    m_nColumnCount = 0;
    memset(m_arrColumnWidth, 0, sizeof(int) * MAX_LIST_ITEM_COLUMN);
    memset(m_pszText, 0, MAX_LIST_ITEM_COLUMN * MAX_ITEM_TEXT_LEN);
    memset(m_arrTextPos, 0, MAX_LIST_ITEM_COLUMN * sizeof(RECT));
}

CDXListItem::~CDXListItem(void)
{
}

void CDXListItem::Render()
{
    if (m_lpTexture && m_bVisible)
    {
        if (!CDXWidget::PreRender())
        {
            return;
        }

        if (FAILED(g_GameEngine.GetDevice()->SetTexture(0, m_lpTexture)))
        {
            MessageBox(NULL, "Set texture", "Set texture failed", MB_OK);
        }

        for (int i = 0; i < m_nColumnCount; i++)
        {
            if (strlen(m_pszText[i]) > 0)
            {
                g_GameEngine.GetFont(m_nFontType)->DrawTextA(NULL, m_pszText[i], -1, &m_arrTextPos[i], m_dwAlignment, m_dwFontColor);
            }
        }

        CDXWidget::Render();
    }
}

void CDXListItem::Init( const char *pWidgetName, const char *pTextureFile, 
                       int nLeft, int nTop, int nWidth, int nHeight, 
                       int nFontType, int nColumnCount, 
                       int arrColumnWidth[MAX_LIST_ITEM_COLUMN], bool bVisible, int nDepth )
{
    if ((m_lpTexture = g_GameEngine.GetTexture(pTextureFile, nWidth, nHeight)) != NULL)
    {
        CDXWidget::Init(pWidgetName, nLeft, nTop, nWidth, 
            nHeight, bVisible, nDepth); 
        m_nFontType = nFontType;
        m_nColumnCount = nColumnCount;
        memcpy(m_arrColumnWidth, arrColumnWidth, sizeof(int) * MAX_LIST_ITEM_COLUMN);

        for (int i = 0; i < nColumnCount; i++)
        {
            m_arrTextPos[i].left = nLeft;
            m_arrTextPos[i].right = nLeft + arrColumnWidth[i];
            m_arrTextPos[i].top = nTop;
            m_arrTextPos[i].bottom = nTop + nHeight;

            nLeft = m_arrTextPos[i].right;
        }
    }
}

void CDXListItem::SetText( const char *pText, int nColumn )
{
    assert(nColumn >= 0 && nColumn < m_nColumnCount && 
        pText != NULL && strlen(pText) > 0 && 
        strlen(pText) < MAX_ITEM_TEXT_LEN);

    strcpy(m_pszText[nColumn], pText);
}

void CDXListItem::SetNumber( int nNumber, int nColumn )
{
    assert(nColumn >= 0 && nColumn < m_nColumnCount);
    sprintf(m_pszText[nColumn], "%d", nNumber);
}

void CDXListItem::ClearText( int nColumn )
{
    memset(m_pszText[nColumn], 0, MAX_ITEM_TEXT_LEN);
}
