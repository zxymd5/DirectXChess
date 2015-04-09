#ifndef COMMON_H
#define COMMON_H

#include <d3dx9math.h>

#define SAFE_DELETE(p)       { if (p) { delete (p);     (p)=NULL; } }
#define SAFE_DELETE_ARRAY(p) { if (p) { delete[] (p);   (p)=NULL; } }
#define SAFE_RELEASE(p)  {if(p) { (p)->Release(); (p) = NULL; }}

static const int nMaxNameLen = 50;
static const int nMaxFilePathLen = 260;
static const int nWindowWidth = 1024;
static const int nWindowHeight = 768;

enum FontType
{
    ARIAL_BOLD_16,
    ARIAL_BOLD_24,
    ARIAL_BOLD_32,
    ARIAL_BOLD_40,
    ARIAL_BOLD_48
};

enum WidgetType
{
    WIDGET_IMAGE = 1,
    WIDGET_LABEL = 2,
    WIDGET_BUTTON = 3,
};

struct TextureInfo
{
    char szFileName[nMaxFilePathLen];
    int nWidth;
    int nHeight;
    LPDIRECT3DTEXTURE9 pTexture;
};

struct FontInfo
{
    int nFontType;
    LPD3DXFONT pFont;
};

struct CUSTOMVETEX
{
    D3DXVECTOR3 stPos; 
    float fRhw;
    DWORD dwColor;
    float u;
    float v;
};

#define D3DFVF_VERTEX (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)

typedef void (*CallBackFunc)(void *);

enum BUTTON_STATE
{
    STATE_ACTIVE = 0,
    STATE_SELECT,
    STATE_DISABLE,
    STATE_COUNT,
};

#endif