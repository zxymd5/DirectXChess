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
 *  File:		DXListCtrl.h
 *  Author:		Richard Zou
 *	Created on:	2015-03-01
 */

#ifndef DX_LIST_CTRL_H
#define DX_LIST_CTRL_H

#include "dxwidget.h"
#include "DXListItem.h"
static const int s_nMaxListItem = 10;

class CDXListCtrl : public CDXWidget
{
public:
    CDXListCtrl(void);
    void Init(const char *strWidgetName, 
              const char *strItemTextureFile,
               const char *strListItemBasename,
              int nLeft, int nTop, 
              int nWidth, int nHeight, 
               int nFontType, int nListItemCount, int nColumnCount,
              int szColumnWidth[s_nMaxListItemColumn],
              bool bVisible, int nDepth);

    virtual void Shutdown();
    virtual void Render();
    CDXListItem &GetListItem(int nListItemIndex);
    int GetListItemCount() const;
    virtual ~CDXListCtrl(void);

private:
    CDXListItem m_szListItem[s_nMaxListItem];
    int m_nListItemCount;
};

#endif