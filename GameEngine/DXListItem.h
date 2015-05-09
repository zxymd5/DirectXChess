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

class CDXListItem : public CDXLabel
{
public:
    CDXListItem(void);
    virtual ~CDXListItem(void);
    void Init(const char *pWidgetName, 
            const char *pTextureFile,
            int nLeft, int nTop, 
            int nWidth, int nHeight, int nFontType,
            int nColumnCount, int arrColumnWidth[MAX_LIST_ITEM_COLUMN],
            bool bVisible, int nDepth);
    void SetText(const char *pText, int nColumn);
    void SetNumber(int nNumber, int nColumn);
    void ClearText(int nColumn);
    virtual void Render();

private:

    int m_nColumnCount;
    int m_arrColumnWidth[MAX_LIST_ITEM_COLUMN];
    char m_pszText[MAX_LIST_ITEM_COLUMN][MAX_ITEM_TEXT_LEN];
    RECT m_arrTextPos[MAX_LIST_ITEM_COLUMN];
};

#endif