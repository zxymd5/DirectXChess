#ifndef CGAME_ENGINE_H
#define CGAME_ENGINE_H

#include "Common.h"
#include <map>
#include <vector>
#include <string>

class CDXWidget;
using namespace std;

class CGameEngine
{
public:
    CGameEngine(void);
    ~CGameEngine(void);
    void                Init(HWND pHwnd, int nWidth, int nHeight);
    LPDIRECT3DDEVICE9   GetDevice();
    bool                BeginShow();
    bool                EndShow();

    void                ParseFile(const char *strFileName);
    void                Shutdown();
    CDXWidget           *GetWidgetByName(const char *strName);
    void                AddWidget(CDXWidget *pWidget);
    LPD3DXFONT          GetFont(int nFontType);
    LPDIRECT3DTEXTURE9  GetTexture( const char *strFileName, int &nWidth, int &nHeight);
    void                Render();

private:
    LPDIRECT3D9         m_pD3D;
    LPDIRECT3DDEVICE9   m_pD3DDevice;
    map<string, CDXWidget *> m_mapWidget;
    vector<TextureInfo *>   m_vecTexture;
    vector<FontInfo *> m_vecFont;
};

extern CGameEngine g_GameEngine;

#endif
