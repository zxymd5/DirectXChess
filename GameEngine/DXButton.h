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
 *  File:       DXButton.h
 *  Author:     Richard Zou
 *  Created on: 2015-03-01
 */

#ifndef DX_BUTTON_H
#define DX_BUTTON_H

#include "DXWidget.h"

class CDXButton :
    public CDXWidget
{
public:
    CDXButton(void);
    virtual ~CDXButton(void);
    void Init(const char *pWidgetName, const char *pTextureFile,
              const char *pTextureFileSelected, const char *pTextureFileDisabled,
              int nLeft, int nTop, 
              int nWidth, int nHeight, 
              bool bVisible, int nDepth);
    const int GetCurrState();
    void SetCurrState(int nCurrState);
    virtual void HandleFocus(bool bFocus);

    virtual void Render();
    virtual void MsgResponse(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
    int                 m_nCurrState;
    LPDIRECT3DTEXTURE9  m_lpszTexture[STATE_COUNT];
};

#endif
