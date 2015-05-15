/*      
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  File:       Common.h
 *  Author:     Richard Zou
 *  Created on: 2015-03-01
 */

#ifndef COMMON_H
#define COMMON_H

#include <d3dx9math.h>

#define SAFE_DELETE(p)       { if (p) { delete (p);     (p)=NULL; } }
#define SAFE_DELETE_ARRAY(p) { if (p) { delete[] (p);   (p)=NULL; } }
#define SAFE_RELEASE(p)  {if(p) { (p)->Release(); (p) = NULL; }}

static const int MAX_NAME_LEN = 50;
static const int MAX_FILE_PATH_LEN = 260;
static const int WINDOW_WIDTH = 1024;
static const int WINDOW_HEIGHT = 768;
static const int MAX_LIST_ITEM = 10;
static const int MAX_LIST_ITEM_COLUMN = 10;
static const int MAX_ITEM_TEXT_LEN = 32;

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
    char szFileName[MAX_FILE_PATH_LEN];
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