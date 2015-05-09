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
 *  File:		DXImage.cpp
 *  Author:		Richard Zou
 *	Created on:	2015-03-01
 */

#include "DXImage.h"
#include "GameEngine.h"

#include <assert.h>

CDXImage::CDXImage(void)
{
    m_lpTexture = NULL;
    memset(m_szTextureFile, 0, MAX_NAME_LEN);
}

CDXImage::~CDXImage(void)
{
}

void CDXImage::Render()
{
    if (m_lpTexture)
    {
        if (!CDXWidget::PreRender())
        {
            return;
        }

        if (FAILED(g_GameEngine.GetDevice()->SetTexture(0, m_lpTexture)))
        {
            MessageBox(NULL, "Set texture", "Set texture failed", MB_OK);
        }

        CDXWidget::Render();
    }
}

void CDXImage::Init(const char *pWidgetName, const char *pTextureFile,
                    int nLeft, int nTop, 
                    int nWidth, int nHeight, 
                    bool bVisible, int nDepth)
{
    m_lpTexture = g_GameEngine.GetTexture(pTextureFile, nWidth, nHeight);
    strcpy(m_szTextureFile, pTextureFile);
    CDXWidget::Init(pWidgetName, nLeft, nTop, nWidth, 
                        nHeight, bVisible, nDepth);       
}

void CDXImage::SetTexture( const char *pPicture )
{
    assert(pPicture != NULL);
    m_lpTexture = g_GameEngine.GetTexture(pPicture, m_nWidth, m_nHeight);
    strcpy(m_szTextureFile, pPicture);
}

void CDXImage::SetTexture( LPDIRECT3DTEXTURE9 lpTexture )
{
    m_lpTexture = lpTexture;
}

const char * CDXImage::GetTextureFile()
{
    return m_szTextureFile;
}
