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
 *  File:		DXLabel.cpp
 *  Author:		Richard Zou
 *	Created on:	2015-03-01
 */

#include "DXLabel.h"
#include "GameEngine.h"

CDXLabel::CDXLabel(void)
{
    m_nFontType = 0;
}

CDXLabel::~CDXLabel(void)
{
}

void CDXLabel::Init( const char *pWidgetName, const char *pTextureFile, 
                    int nLeft, int nTop, int nWidth, int nHeight, int nFontType,
                    int nTextLeft, int nTextTop, int nTextWidth, 
                    int nTextHeight, bool bVisible, int nDepth )
{
    if ((m_lpTexture = g_GameEngine.GetTexture(pTextureFile, nWidth, nHeight)) != NULL)
    {
        CDXWidget::Init(pWidgetName, nLeft, nTop, nWidth, 
            nHeight, bVisible, nDepth); 
        m_nTextLeft = nTextLeft;
        m_nTextTop = nTextTop;
        m_nTextWidth = nTextWidth;
        m_nTextHeight = nTextHeight;
        m_nFontType = nFontType;
    }
}

void CDXLabel::Render()
{
    if (m_lpTexture && m_bVisible && m_strText.size())
    {
        if (!CDXWidget::PreRender())
        {
            return;
        }

        if (FAILED(g_GameEngine.GetDevice()->SetTexture(0, m_lpTexture)))
        {
            MessageBox(NULL, "Set texture", "Set texture failed", MB_OK);
        }

        RECT rcTextPos;
        rcTextPos.left = m_nTextLeft;
        rcTextPos.top = m_nTextTop;
        rcTextPos.right = m_nTextLeft + m_nTextWidth;
        rcTextPos.bottom = m_nTextTop + m_nTextHeight;
        g_GameEngine.GetFont(m_nFontType)->DrawTextA(NULL, m_strText.c_str(), -1, &rcTextPos, m_dwAlignment, m_dwFontColor);

        CDXWidget::Render();
    }
}

void CDXLabel::SetText( const string &strText )
{
    m_strText = strText;
}

void CDXLabel::SetAlignment( DWORD dwAlignment )
{
    m_dwAlignment = dwAlignment;
}

void CDXLabel::SetFontColor( int nAlpha, int nRed, int nGreen, int nBlue )
{
    m_dwFontColor = D3DCOLOR_ARGB(nAlpha, nRed, nGreen, nBlue);
}

void CDXLabel::SetNumber( int nNumber )
{
    char szNumber[10];
    memset(szNumber, 0, 10);
    sprintf(szNumber, "%d", nNumber);
    m_strText.assign(szNumber);
}

void CDXLabel::ClearText()
{
    m_strText.clear();
}
