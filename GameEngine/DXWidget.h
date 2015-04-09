#ifndef DX_WIDGET_H
#define DX_WIDGET_H

#include "Common.h"

class CDXWidget
{
public:
    CDXWidget(void);
    virtual ~CDXWidget(void);

    virtual void                Init(const char *strWidgetName, 
                                    int nLeft, int nTop, 
                                    DWORD dwWidth, DWORD dwHeight, 
                                    bool bVisible, int nDepth);

    void                        ResetVertex();
    bool                        PreRender();
    virtual void                Render();

    void                        CreateVertexBuffer();

    void                        SetVisible(bool Visible);
    bool                        IsVisible() const;
    void                        SetPosRect( int nLeft, int nTop, DWORD dwWidth, DWORD dwHeight );
    void                        SetPosRect(const RECT &Pos);
    void                        SetWidgetName(const char *pWidgetName);
    void                        SetTexture(const char *pPicture);
    void                        SetTexture(LPDIRECT3DTEXTURE9 pTexture);
    void                        SetDepth(int nDepth);
    virtual void                MsgResponse(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    virtual void                Shutdown();

    void                        SetCallBackInfo(CallBackFunc pfnCallback, void *pCallbackParam);
    const char                  *GetWidgetName();
    RECT                        GetPosRect();
    virtual void                HandleFocus(bool bFocus);
    const int                   GetDepth();

protected:
    LPDIRECT3DVERTEXBUFFER9     m_pVertexBuffer;
    CUSTOMVETEX                 m_szVertex[4];

    CallBackFunc                m_pfnCallback;
    void                        *m_pCallbackParam;

    bool                        m_bVisible;
    int                         m_nDepth;
    int                         m_nLeft;
    int                         m_nTop;
    int                         m_nHeight;
    int                         m_nWidth;
    char                        m_szWidgetName[nMaxNameLen];
};

#endif
