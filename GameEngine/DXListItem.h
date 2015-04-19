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
 *  File:		DXListItem.h
 *  Author:		Richard Zou
 *	Created on:	2015-03-01
 */

#ifndef DX_LIST_ITEM_H
#define DX_LIST_ITEM_H

#include "dxlabel.h"

static const int s_nMaxListItemColumn = 10;
static const int s_nMaxTextLen = 32;

class CDXListItem : public CDXLabel
{
public:
    CDXListItem(void);
    virtual ~CDXListItem(void);
    void Init(const char *strWidgetName, 
            const char *strTextureFile,
            int nLeft, int nTop, 
            int nWidth, int nHeight, int nFontType,
            int nColumnCount, int szColumnWidth[s_nMaxListItemColumn],
            bool bVisible, int nDepth);
    void SetText(const char *strText, int nColumn);

    virtual void Render();

private:

    int m_nColumnCount;
    int m_szColumnWidth[s_nMaxListItemColumn];
    char m_szText[s_nMaxListItemColumn][s_nMaxTextLen];
    RECT m_szTextPos[s_nMaxListItemColumn];
};

#endif