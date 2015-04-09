#include "DXLabel.h"
#include "GameEngine.h"

CDXLabel::CDXLabel(void)
{
    m_nFontType = 0;
}

CDXLabel::~CDXLabel(void)
{
}

void CDXLabel::Init( const char *strWidgetName, const char *strTextureFile, 
                    int nLeft, int nTop, int nWidth, int nHeight, int nFontType,
                    int nTextLeft, int nTextTop, int nTextWidth, 
                    int nTextHeight, bool bVisible, int nDepth )
{
    if ((m_pTexture = g_GameEngine.GetTexture(strTextureFile, nWidth, nHeight)) != NULL)
    {
        CDXWidget::Init(strWidgetName, nLeft, nTop, nWidth, 
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
    if (m_pTexture && m_bVisible && m_strText.size())
    {
        if (!CDXWidget::PreRender())
        {
            return;
        }

        if (FAILED(g_GameEngine.GetDevice()->SetTexture(0, m_pTexture)))
        {
            MessageBox(NULL, "Set texture", "Set texture failed", MB_OK);
        }

        RECT stTextPos;
        stTextPos.left = m_nTextLeft;
        stTextPos.top = m_nTextTop;
        stTextPos.right = m_nTextLeft + m_nTextWidth;
        stTextPos.bottom = m_nTextTop + m_nTextHeight;
        g_GameEngine.GetFont(m_nFontType)->DrawTextA(NULL, m_strText.c_str(), -1, &stTextPos, m_dwAlignment, m_dwColor);

        CDXWidget::Render();
    }
}

void CDXLabel::Shutdonw()
{
    CDXWidget::Shutdown();
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
    m_dwColor = D3DCOLOR_ARGB(nAlpha, nRed, nGreen, nBlue);
}

void CDXLabel::MsgResponse( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
}
