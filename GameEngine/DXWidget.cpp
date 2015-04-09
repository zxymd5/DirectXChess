#include "DXWidget.h"
#include "GameEngine.h"
#include <assert.h>

CDXWidget::CDXWidget(void)
{
    m_pVertexBuffer = NULL;
    m_pfnCallback = NULL;
    m_pCallbackParam = NULL;

    memset(m_szVertex, 0, 4 * sizeof(CUSTOMVETEX));

    m_bVisible = false;
    m_nDepth = 0;
    m_nLeft = 0;
    m_nTop = 0;
    m_nHeight = 0;
    m_nWidth = 0;
    memset(m_szWidgetName, 0, nMaxNameLen);
}

CDXWidget::~CDXWidget(void)
{
    Shutdown();
}

void CDXWidget::Shutdown()
{
    SAFE_RELEASE(m_pVertexBuffer);
}

void CDXWidget::Init(const char *strWidgetName, 
                     int nLeft, int nTop, 
                     DWORD dwWidth, DWORD dwHeight, 
                     bool bVisible, int nDepth)
{
    strcpy(m_szWidgetName, strWidgetName);
    m_bVisible = bVisible;
    m_nDepth = nDepth;

    SetPosRect(nLeft, nTop, dwWidth, dwHeight);
    ResetVertex();
    CreateVertexBuffer();
}

void CDXWidget::ResetVertex()
{
    m_szVertex[0].stPos.x = static_cast<float>(m_nLeft);
    m_szVertex[0].stPos.y = static_cast<float>(m_nTop);
    m_szVertex[0].stPos.z = 0.5f;
    m_szVertex[0].fRhw = 1;
    m_szVertex[0].dwColor = D3DCOLOR_ARGB(255, 255, 255, 255);
    m_szVertex[0].u = 0.0f;
    m_szVertex[0].v = 0.0f;

    m_szVertex[1].stPos.x = static_cast<float>(m_nLeft + m_nWidth);
    m_szVertex[1].stPos.y = static_cast<float>(m_nTop);
    m_szVertex[1].stPos.z = 0.5f;
    m_szVertex[1].fRhw = 1;
    m_szVertex[1].dwColor = D3DCOLOR_ARGB(255, 255, 255, 255);
    m_szVertex[1].u = 1.0f;
    m_szVertex[1].v = 0.0f;

    m_szVertex[2].stPos.x = static_cast<float>(m_nLeft);
    m_szVertex[2].stPos.y = static_cast<float>(m_nTop + m_nHeight);
    m_szVertex[2].stPos.z = 0.5f;
    m_szVertex[2].fRhw = 1;
    m_szVertex[2].dwColor = D3DCOLOR_ARGB(255, 255, 255, 255);
    m_szVertex[2].u = 0.0f;
    m_szVertex[2].v = 1.0f;

    m_szVertex[3].stPos.x = static_cast<float>(m_nLeft + m_nWidth);
    m_szVertex[3].stPos.y = static_cast<float>(m_nTop + m_nHeight);
    m_szVertex[3].stPos.z = 0.5f;
    m_szVertex[3].fRhw = 1;
    m_szVertex[3].dwColor = D3DCOLOR_ARGB(255, 255, 255, 255);
    m_szVertex[3].u = 1.0f;
    m_szVertex[3].v = 1.0f;
}

void CDXWidget::Render()
{
    g_GameEngine.GetDevice()->SetStreamSource(0, m_pVertexBuffer, 0, sizeof(CUSTOMVETEX));
    g_GameEngine.GetDevice()->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);
    g_GameEngine.GetDevice()->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, m_szVertex, sizeof(CUSTOMVETEX));
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

void CDXWidget::SetPosRect( const RECT &Pos )
{
    m_nLeft = Pos.left;
    m_nTop = Pos.top;
    m_nHeight = Pos.bottom - m_nTop;
    m_nWidth = Pos.right - m_nLeft;
}

void CDXWidget::MsgResponse( HWND Hwnd, UINT Message, WPARAM WParam, LPARAM LParam )
{

}

void CDXWidget::SetCallBackInfo( CallBackFunc Func, void *CallBackParam )
{
    m_pfnCallback = Func;
    m_pCallbackParam = CallBackParam;
}

bool CDXWidget::IsVisible() const
{
    return m_bVisible;
}

void CDXWidget::CreateVertexBuffer()
{
    //´´½¨¶¥µã»º´æ
    if (FAILED(g_GameEngine.GetDevice()->CreateVertexBuffer(sizeof(m_szVertex), 0, 
        D3DFVF_XYZRHW | D3DFVF_TEX1, D3DPOOL_DEFAULT, 
        &m_pVertexBuffer, NULL)))
    {
        return;
    }

    void *ptr;
    if (FAILED(m_pVertexBuffer->Lock(0, sizeof(m_szVertex), (void**)&ptr, 0)))
    {
        return;
    }

    memcpy(ptr, m_szVertex, sizeof(m_szVertex));
    m_pVertexBuffer->Unlock();
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
    RECT Pos;
    Pos.left = m_nLeft;
    Pos.right = m_nLeft + m_nWidth;
    Pos.top = m_nTop;
    Pos.bottom = m_nTop + m_nHeight;

    return Pos;
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

void CDXWidget::SetTexture( LPDIRECT3DTEXTURE9 pTexture )
{
    
}

void CDXWidget::SetDepth( int nDepth )
{
    m_nDepth = nDepth;
}
