#ifndef DX_LABEL_H
#define DX_LABEL_H

#include <string>
#include "DXWidget.h"

using namespace std;

class CDXLabel :
    public CDXWidget
{
public:
    CDXLabel(void);
    void Init(const char *strWidgetName, 
              const char *strTextureFile,
                int nLeft, int nTop, 
                int nWidth, int nHeight, int nFontType,
                int nTextLeft, int nTextTop,
                int nTextWidth, int nTextHeight,
                bool bVisible, int nDepth);

    virtual void MsgResponse(HWND Hwnd, UINT Message, WPARAM WParam, LPARAM LParam);
    virtual ~CDXLabel(void);

    virtual void Render();
    virtual void Shutdonw();
    void SetText(const string &strText);
    void SetAlignment(DWORD dwAlignment);
    void SetFontColor(int nAlpha, int nRed, int nGreen, int nBlue);

protected:
    LPDIRECT3DTEXTURE9  m_pTexture;
    int         m_nFontType;
    DWORD       m_dwAlignment;
    D3DCOLOR    m_dwColor;

private:
    string      m_strText;
    int         m_nTextLeft;
    int         m_nTextTop;
    int         m_nTextWidth;
    int         m_nTextHeight;
};

#endif