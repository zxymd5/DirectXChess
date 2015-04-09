#include "DXImage.h"
#include "GameEngine.h"

#include <assert.h>

CDXImage::CDXImage(void)
{
    m_pTexture = NULL;
    memset(m_szTextureFile, 0, nMaxNameLen);
}

CDXImage::~CDXImage(void)
{
}

void CDXImage::MsgResponse( HWND Hwnd, UINT Message, WPARAM WParam, LPARAM LParam )
{

}

void CDXImage::Render()
{
    if (m_pTexture)
    {
        if (!CDXWidget::PreRender())
        {
            return;
        }

        if (FAILED(g_GameEngine.GetDevice()->SetTexture(0, m_pTexture)))
        {
            MessageBox(NULL, "Set texture", "Set texture failed", MB_OK);
        }

        CDXWidget::Render();
    }
}

void CDXImage::Init(const char *strWidgetName, const char *strTextureFile,
                    int nLeft, int nTop, 
                    int nWidth, int nHeight, 
                    bool bVisible, int nDepth)
{
    m_pTexture = g_GameEngine.GetTexture(strTextureFile, nWidth, nHeight);
    strcpy(m_szTextureFile, strTextureFile);
    CDXWidget::Init(strWidgetName, nLeft, nTop, nWidth, 
                        nHeight, bVisible, nDepth);       
}

void CDXImage::SetTexture( const char *pPicture )
{
    assert(pPicture != NULL);
    m_pTexture = g_GameEngine.GetTexture(pPicture, m_nWidth, m_nHeight);
    strcpy(m_szTextureFile, pPicture);
}

void CDXImage::SetTexture( LPDIRECT3DTEXTURE9 pTexture )
{
    m_pTexture = pTexture;
}

const char * CDXImage::GetTextureFile()
{
    return m_szTextureFile;
}
