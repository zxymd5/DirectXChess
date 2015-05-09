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
 *  File:		DXLabel.h
 *  Author:		Richard Zou
 *	Created on:	2015-03-01
 */

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
    void Init(const char *pWidgetName, 
              const char *pTextureFile,
                int nLeft, int nTop, 
                int nWidth, int nHeight, int nFontType,
                int nTextLeft, int nTextTop,
                int nTextWidth, int nTextHeight,
                bool bVisible, int nDepth);

    virtual ~CDXLabel(void);

    virtual void Render();
    void SetText(const string &strText);
    void ClearText();
    void SetNumber(int nNumber);
    void SetAlignment(DWORD dwAlignment);
    void SetFontColor(int nAlpha, int nRed, int nGreen, int nBlue);

protected:
    LPDIRECT3DTEXTURE9  m_lpTexture;
    int         m_nFontType;
    DWORD       m_dwAlignment;
    D3DCOLOR    m_dwFontColor;

private:
    string      m_strText;
    int         m_nTextLeft;
    int         m_nTextTop;
    int         m_nTextWidth;
    int         m_nTextHeight;
};

#endif