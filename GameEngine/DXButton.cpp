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
 *  File:       DXButton.cpp
 *  Author:     Richard Zou
 *  Created on: 2015-03-01
 */

#include "DXButton.h"
#include "GameEngine.h"

CDXButton::CDXButton(void)
{
    m_nCurrState = STATE_ACTIVE;

    for (int i = 0; i < STATE_COUNT; i++)
    {
        m_lpszTexture[i] = NULL;
    }
}

CDXButton::~CDXButton(void)
{
}

void CDXButton::Init(const char *pWidgetName, const char *pTextureFile,
                     const char *pTextureFileSelected, const char *pTextureFileDisabled,
                     int nLeft, int nTop, 
                     int nWidth, int nHeight, 
                     bool bVisible, int nDepth)
{
    m_lpszTexture[STATE_ACTIVE] = g_GameEngine.GetTexture(pTextureFile, nWidth, nHeight);
    m_lpszTexture[STATE_SELECT] = g_GameEngine.GetTexture(pTextureFileSelected, nWidth, nHeight);
    m_lpszTexture[STATE_DISABLE] = g_GameEngine.GetTexture(pTextureFileDisabled, nWidth, nHeight);

    CDXWidget::Init(pWidgetName, nLeft, nTop, nWidth, 
                    nHeight, bVisible, nDepth); 
}


void CDXButton::SetCurrState( int nCurrState )
{
    m_nCurrState = nCurrState;
}

void CDXButton::MsgResponse( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
    if (m_nCurrState == STATE_DISABLE)
    {
        return;
    }

    switch(uMsg)
    {
    case WM_LBUTTONDOWN:
        {
            if (m_pfnCallback)
            {
                m_nCurrState = STATE_SELECT;
                m_pfnCallback(m_pCallbackParam);
            }
        }
        break;
    case WM_MOUSEMOVE:
        {
            if (m_nCurrState != STATE_DISABLE)
            {
                m_nCurrState = STATE_SELECT;
            }
        }
        break;
    default:
        {
            break;
        }
    }
}

const int CDXButton::GetCurrState()
{
    return m_nCurrState;
}

void CDXButton::Render()
{
    if (m_lpszTexture[m_nCurrState])
    {
        if (!CDXWidget::PreRender())
        {
            return;
        }

        if (FAILED(g_GameEngine.GetDevice()->SetTexture(0, m_lpszTexture[m_nCurrState])))
        {
            MessageBox(NULL, "Set texture", "Set texture failed", MB_OK);
        }

        CDXWidget::Render();
    }
}

void CDXButton::HandleFocus( bool bFocus )
{
    if (bFocus)
    {
        if (m_nCurrState != STATE_DISABLE)
        {
            m_nCurrState = STATE_SELECT;
        }
    }
    else
    {
        if (m_nCurrState != STATE_DISABLE)
        {
            m_nCurrState = STATE_ACTIVE;
        }
    }
}
