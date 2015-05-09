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
 *  File:		DXWidget.cpp
 *  Author:		Richard Zou
 *	Created on:	2015-03-01
 */

#include "DXWidget.h"
#include "GameEngine.h"
#include <assert.h>

CDXWidget::CDXWidget(void)
{
    m_lpVertexBuffer = NULL;
    m_pfnCallback = NULL;
    m_pCallbackParam = NULL;

    memset(m_arrVertex, 0, 4 * sizeof(CUSTOMVETEX));

    m_bVisible = false;
    m_nDepth = 0;
    m_nLeft = 0;
    m_nTop = 0;
    m_nHeight = 0;
    m_nWidth = 0;
    memset(m_szWidgetName, 0, MAX_NAME_LEN);
}

CDXWidget::~CDXWidget(void)
{
    Shutdown();
}

void CDXWidget::Shutdown()
{
    SAFE_RELEASE(m_lpVertexBuffer);
}

void CDXWidget::Init(const char *pWidgetName, 
                     int nLeft, int nTop, 
                     DWORD dwWidth, DWORD dwHeight, 
                     bool bVisible, int nDepth)
{
    strcpy(m_szWidgetName, pWidgetName);
    m_bVisible = bVisible;
    m_nDepth = nDepth;

    SetPosRect(nLeft, nTop, dwWidth, dwHeight);
    ResetVertex();
    CreateVertexBuffer();
}

void CDXWidget::ResetVertex()
{
    m_arrVertex[0].stPos.x = static_cast<float>(m_nLeft);
    m_arrVertex[0].stPos.y = static_cast<float>(m_nTop);
    m_arrVertex[0].stPos.z = 0.5f;
    m_arrVertex[0].fRhw = 1;
    m_arrVertex[0].dwColor = D3DCOLOR_ARGB(255, 255, 255, 255);
    m_arrVertex[0].u = 0.0f;
    m_arrVertex[0].v = 0.0f;

    m_arrVertex[1].stPos.x = static_cast<float>(m_nLeft + m_nWidth);
    m_arrVertex[1].stPos.y = static_cast<float>(m_nTop);
    m_arrVertex[1].stPos.z = 0.5f;
    m_arrVertex[1].fRhw = 1;
    m_arrVertex[1].dwColor = D3DCOLOR_ARGB(255, 255, 255, 255);
    m_arrVertex[1].u = 1.0f;
    m_arrVertex[1].v = 0.0f;

    m_arrVertex[2].stPos.x = static_cast<float>(m_nLeft);
    m_arrVertex[2].stPos.y = static_cast<float>(m_nTop + m_nHeight);
    m_arrVertex[2].stPos.z = 0.5f;
    m_arrVertex[2].fRhw = 1;
    m_arrVertex[2].dwColor = D3DCOLOR_ARGB(255, 255, 255, 255);
    m_arrVertex[2].u = 0.0f;
    m_arrVertex[2].v = 1.0f;

    m_arrVertex[3].stPos.x = static_cast<float>(m_nLeft + m_nWidth);
    m_arrVertex[3].stPos.y = static_cast<float>(m_nTop + m_nHeight);
    m_arrVertex[3].stPos.z = 0.5f;
    m_arrVertex[3].fRhw = 1;
    m_arrVertex[3].dwColor = D3DCOLOR_ARGB(255, 255, 255, 255);
    m_arrVertex[3].u = 1.0f;
    m_arrVertex[3].v = 1.0f;
}

void CDXWidget::Render()
{
    g_GameEngine.GetDevice()->SetStreamSource(0, m_lpVertexBuffer, 0, sizeof(CUSTOMVETEX));
    g_GameEngine.GetDevice()->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);
    g_GameEngine.GetDevice()->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, m_arrVertex, sizeof(CUSTOMVETEX));
}

void CDXWidget::SetVisible( bool bVisible )
{
    m_bVisible = bVisible;
}

void CDXWidget::SetPosRect( int nLeft, int nTop, DWORD dwWidth, DWORD dwHeight )
{
    m_nLeft = nLeft;
    m_nTop = nTop;
    m_nHeight = dwHeight;
    m_nWidth = dwWidth;
}

void CDXWidget::SetPosRect( const RECT &rcPos )
{
    m_nLeft = rcPos.left;
    m_nTop = rcPos.top;
    m_nHeight = rcPos.bottom - m_nTop;
    m_nWidth = rcPos.right - m_nLeft;
}

void CDXWidget::MsgResponse( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{

}

void CDXWidget::SetCallBackInfo( CallBackFunc pfnFunc, void *pCallBackParam )
{
    m_pfnCallback = pfnFunc;
    m_pCallbackParam = pCallBackParam;
}

bool CDXWidget::IsVisible() const
{
    return m_bVisible;
}

void CDXWidget::CreateVertexBuffer()
{
    //´´½¨¶¥µã»º´æ
    if (FAILED(g_GameEngine.GetDevice()->CreateVertexBuffer(sizeof(m_arrVertex), 0, 
        D3DFVF_XYZRHW | D3DFVF_TEX1, D3DPOOL_DEFAULT, 
        &m_lpVertexBuffer, NULL)))
    {
        return;
    }

    void *ptr;
    if (FAILED(m_lpVertexBuffer->Lock(0, sizeof(m_arrVertex), (void**)&ptr, 0)))
    {
        return;
    }

    memcpy(ptr, m_arrVertex, sizeof(m_arrVertex));
    m_lpVertexBuffer->Unlock();
}

bool CDXWidget::PreRender()
{
    if(!m_bVisible)
    {
        return false;
    }

    ResetVertex();
    return true;
}

const char * CDXWidget::GetWidgetName()
{
    return m_szWidgetName;
}

RECT CDXWidget::GetPosRect()
{
    RECT rc;
    rc.left = m_nLeft;
    rc.right = m_nLeft + m_nWidth;
    rc.top = m_nTop;
    rc.bottom = m_nTop + m_nHeight;

    return rc;
}

void CDXWidget::HandleFocus( bool bFocus )
{

}

const int CDXWidget::GetDepth()
{
    return m_nDepth;
}

void CDXWidget::SetWidgetName( const char *pWidgetName )
{
    assert(pWidgetName != NULL);
    strcpy(m_szWidgetName, pWidgetName);
}

void CDXWidget::SetTexture( const char *pPicture )
{

}

void CDXWidget::SetTexture( LPDIRECT3DTEXTURE9 lpTexture )
{
    
}

void CDXWidget::SetDepth( int nDepth )
{
    m_nDepth = nDepth;
}
