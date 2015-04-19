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
 *  File:		DXImage.h
 *  Author:		Richard Zou
 *	Created on:	2015-03-01
 */

#ifndef DX_IMAGE_H
#define DX_IMAGE_H

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