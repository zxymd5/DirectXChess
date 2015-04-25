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
 *  File:		DXListItem.cpp
 *  Author:		Richard Zou
 *	Created on:	2015-03-01
 */

#include "DXListItem.h"
#include "GameEngine.h"
#include <assert.h>

CDXListItem::CDXListItem(void)
{
    m_nColumnCount = 0;
    memset(m_szColumnWidth, 0, sizeof(int) * s_nMaxListItemColumn);
    memset(m_szText, 0, s_nMaxListItemColumn * s_nMaxTextLen);
    memset(m_szTextPos, 0, s_nMaxListItemColumn * sizeof(RECT));
}

CDXListItem::~CDXListItem(void)
{
}

void CDXListItem::Render()
{
    if (m_pTexture && m_bVisible)
    {
        if (!CDXWidget::PreRender())
        {
            return;
        }

        if (FAILED(g_GameEngine.GetDevice()->SetTexture(0, m_pTexture)))
        {
            MessageBox(NULL, "Set texture", "Set texture failed", MB_OK);
        }

        for (int i = 0; i < m_nColumnCount; i++)
        {
            if (strlen(m_szText[i]) > 0)
            {
                g_GameEngine.GetFont(m_nFontType)->DrawTextA(NULL, m_szText[i], -1, &m_szTextPos[i], m_dwAlignment, m_dwFontColor);
            }
        }

        CDXWidget::Render();
    }
}

void CDXListItem::Init( const char *strWidgetName, const char *strTextureFile, 
                       int nLeft, int nTop, int nWidth, int nHeight, 
                       int nFontType, int nColumnCount, 
                       int szColumnWidth[s_nMaxListItemColumn], bool bVisible, int nDepth )
{
    if ((m_pTexture = g_GameEngine.GetTexture(strTextureFile, nWidth, nHeight)) != NULL)
    {
        CDXWidget::Init(strWidgetName, nLeft, nTop, nWidth, 
            nHeight, bVisible, nDepth); 
        m_nFontType = nFontType;
        m_nColumnCount = nColumnCount;
        memcpy(m_szColumnWidth, szColumnWidth, sizeof(int) * s_nMaxListItemColumn);

        for (int i = 0; i < nColumnCount; i++)
        {
            m_szTextPos[i].left = nLeft;
            m_szTextPos[i].right = nLeft + szColumnWidth[i];
            m_szTextPos[i].top = nTop;
            m_szTextPos[i].bottom = nTop + nHeight;

            nLeft = m_szTextPos[i].right;
        }
    }
}

void CDXListItem::SetText( const char *strText, int nColumn )
{
    assert(nColumn >= 0 && nColumn < m_nColumnCount && 
        strText != NULL && strlen(strText) > 0 && 
        strlen(strText) < s_nMaxTextLen);

    strcpy(m_szText[nColumn], strText);
}

void CDXListItem::SetNumber( int nNumber, int nColumn )
{
    assert(nColumn >= 0 && nColumn < m_nColumnCount);
    sprintf(m_szText[nColumn], "%d", nNumber);
}

void CDXListItem::ClearText( int nColumn )
{
    memset(m_szText[nColumn], 0, s_nMaxTextLen);
}
