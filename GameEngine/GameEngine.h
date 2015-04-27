/* 		
 * 	This program is free software: you can redistribute it and/or modify
 * 	it under the terms of the GNU General Public License as published by
 * 	the Free Software Foundation, either version 3 of the License, or
 * 	(at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 * 	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  File:		GameEngine.h
 *  Author:		Richard Zou
 *	Created on:	2015-03-01
 */

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
    void                CreateTexFromDir(const char *strDir);
    void                StringToIntArray(const char *str, int szArr[], char chDelimiter );
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
