#ifndef C2D_IMAGE_H
#define C2D_IMAGE_H

#include "DXWidget.h"

class CDXImage :
    public CDXWidget
{
public:
    CDXImage(void);
    void Init(const char *strWidgetName, const char *strTextureFile,
                int nLeft, int nTop, 
                int nWidth, int nHeight, 
                bool bVisible, int nDepth);
    void SetTexture(const char *pPicture);
    void SetTexture(LPDIRECT3DTEXTURE9 pTexture);
    const char *GetTextureFile();
    virtual ~CDXImage(void);
    virtual void Render();

    virtual void MsgResponse(HWND Hwnd, UINT Message, WPARAM WParam, LPARAM LParam);

private:
    LPDIRECT3DTEXTURE9  m_pTexture;
    char                m_szTextureFile[nMaxNameLen];
};

#endif