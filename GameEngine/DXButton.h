#ifndef C2D_BUTTON_H
#define C2D_BUTTON_H

#include "DXWidget.h"

class CDXButton :
    public CDXWidget
{
public:
    CDXButton(void);
    virtual ~CDXButton(void);
    void Init(const char *strWidgetName, const char *strTextureFile,
              const char *strTextureFileSelected, const char *strTextureFileDisabled,
              int nLeft, int nTop, 
              int nWidth, int nHeight, 
              bool bVisible, int nDepth);
    const int GetCurrState();
    void SetCurrState(int nCurrState);
    virtual void HandleFocus(bool bFocus);

    virtual void Render();
    virtual void MsgResponse(HWND Hwnd, UINT Message, WPARAM WParam, LPARAM LParam);

private:
    int             m_nCurrState;
    LPDIRECT3DTEXTURE9  m_szTexture[STATE_COUNT];
};

#endif
