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
 *  File:       DXWidget.h
 *  Author:     Richard Zou
 *  Created on: 2015-03-01
 */

#ifndef DX_WIDGET_H
#define DX_WIDGET_H

#include "Common.h"

class CDXWidget
{
public:
    CDXWidget(void);
    virtual ~CDXWidget(void);

    virtual void                Init(const char *pWidgetName, 
                                    int nLeft, int nTop, 
                                    DWORD dwWidth, DWORD dwHeight, 
                                    bool bVisible, int nDepth);

    void                        ResetVertex();
    bool                        PreRender();
    virtual void                Render();

    void                        CreateVertexBuffer();

    void                        SetVisible(bool bVisible);
    bool                        IsVisible() const;
    void                        SetPosRect( int nLeft, int nTop, DWORD dwWidth, DWORD dwHeight );
    void                        SetPosRect(const RECT &rcPos);
    void                        SetWidgetName(const char *pWidgetName);
    void                        SetTexture(const char *pPicture);
    void                        SetTexture(LPDIRECT3DTEXTURE9 lpTexture);
    void                        SetDepth(int nDepth);
    virtual void                MsgResponse(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    virtual void                Shutdown();

    void                        SetCallBackInfo(CallBackFunc pfnCallback, void *pCallbackParam);
    const char                  *GetWidgetName();
    RECT                        GetPosRect();
    virtual void                HandleFocus(bool bFocus);
    const int                   GetDepth();

protected:
    LPDIRECT3DVERTEXBUFFER9     m_lpVertexBuffer;
    CUSTOMVETEX                 m_arrVertex[4];

    CallBackFunc                m_pfnCallback;
    void                        *m_pCallbackParam;

    bool                        m_bVisible;
    int                         m_nDepth;
    int                         m_nLeft;
    int                         m_nTop;
    int                         m_nHeight;
    int                         m_nWidth;
    char                        m_szWidgetName[MAX_NAME_LEN];
};

#endif
