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
 *  File:		GameEngine.cpp
 *  Author:		Richard Zou
 *	Created on:	2015-03-01
 */

#include "GameEngine.h"
#include "../Include/SimpleIni.h"

#include "DXImage.h"
#include "DXButton.h"
#include "DXLabel.h"
#include "DXListCtrl.h"

CGameEngine g_GameEngine;

CGameEngine::CGameEngine(void)
{
    m_pD3D = NULL;
    m_pD3DDevice = NULL;
}

CGameEngine::~CGameEngine(void)
{
    SAFE_RELEASE(m_pD3D);
    SAFE_RELEASE(m_pD3DDevice);
}

void CGameEngine::Init( HWND pHwnd, int nWidth, int nHeight )
{
    // 创建IDirect3D9接口对象
    m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
    if (!m_pD3D)
    {
        MessageBox(NULL, "Direct3DCreate9() failed", "创建设备失败", MB_OK);
        return;
    }

    // 获取硬件设备信息
    D3DCAPS9 caps;
    int vp = 0;
    m_pD3D->GetDeviceCaps( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps );
    if( caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT )
    {
        vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;
    }
    else
    {
        vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
    }

    // 创建Direct3D设备接口
    D3DPRESENT_PARAMETERS d3dpp; 
    ZeroMemory(&d3dpp, sizeof(d3dpp));
    d3dpp.BackBufferWidth            = nWidth;
    d3dpp.BackBufferHeight           = nHeight;
    d3dpp.BackBufferFormat           = D3DFMT_A8R8G8B8;
    d3dpp.BackBufferCount            = 1;
    d3dpp.MultiSampleType            = D3DMULTISAMPLE_NONE;
    d3dpp.MultiSampleQuality         = 0;
    d3dpp.SwapEffect                 = D3DSWAPEFFECT_DISCARD; 
    d3dpp.hDeviceWindow              = pHwnd;
    d3dpp.Windowed                   = true;
    d3dpp.EnableAutoDepthStencil     = true; 
    d3dpp.AutoDepthStencilFormat     = D3DFMT_D24S8;
    d3dpp.Flags                      = 0;
    d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
    d3dpp.PresentationInterval       = D3DPRESENT_INTERVAL_IMMEDIATE;

    HRESULT lRet = m_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, pHwnd, vp, &d3dpp, &m_pD3DDevice);

    if (FAILED(lRet))
    {
        return;
    }

    m_pD3DDevice->SetRenderState( D3DRS_ZENABLE, TRUE );
    m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    m_pD3DDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
    m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA); 
    m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA); 
}

LPDIRECT3DDEVICE9 CGameEngine::GetDevice()
{
    return m_pD3DDevice;
}

bool CGameEngine::BeginShow()
{
    if (m_pD3DDevice != NULL)
    {
        m_pD3DDevice->BeginScene();
        m_pD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(255, 255, 255), 1.0f, 0);
    }

    return true;
}

bool CGameEngine::EndShow()
{
    if (m_pD3DDevice != NULL)
    {
        m_pD3DDevice->EndScene();
        m_pD3DDevice->Present(NULL, NULL, NULL, NULL);
    }

    return true;
}

void CGameEngine::ParseFile( const char *FileName )
{
    CSimpleIni Ini;
    Ini.LoadFile(FileName);

    CSimpleIniA::TNamesDepend Sections;
    Ini.GetAllSections(Sections);

    CSimpleIniA::TNamesDepend::iterator it = Sections.begin();
    const char *strTextureFile;
    const char *strTextureFileSelected;
    const char *strTextureFileDisabled;
    const char *strItemTextureFile;
    const char *strListItemBasename;
    const char *strColumnWidth;

    bool bVisible;
    int nDepth;
    int nWidgetType;
    int nLeft;
    int nTop;
    int nWidth;
    int nHeight;
    int nTextLeft;
    int nTextTop;
    int nTextWidth;
    int nTextHeight;
    int nFontType;
    int nColumnCount;
    int nListItemCount;
    int szColumnWidth[s_nMaxListItemColumn];

    for (; it != Sections.end(); ++it)
    {
        CDXWidget *pWidget = NULL;

        nWidgetType = Ini.GetLongValue(it->pItem, "WidgetType", 0);
        if (nWidgetType == 0)
        {
            return;
        }

        strTextureFile = Ini.GetValue(it->pItem, "TextureFile", NULL);
        strTextureFileSelected = Ini.GetValue(it->pItem, "TextureFileSelected", NULL);
        strTextureFileDisabled = Ini.GetValue(it->pItem, "TextureFileDisabled", NULL);

        bVisible = Ini.GetBoolValue(it->pItem, "Visible", false);
        nDepth = Ini.GetLongValue(it->pItem, "Depth", 0);
        nLeft = Ini.GetLongValue(it->pItem, "Left", 0);
        nTop = Ini.GetLongValue(it->pItem, "Top", 0);
        nWidth = Ini.GetLongValue(it->pItem, "Width", 0);;
        nHeight = Ini.GetLongValue(it->pItem, "Height", 0);
        
        //For text area
        nTextLeft = Ini.GetLongValue(it->pItem, "TextLeft", 0);
        nTextTop = Ini.GetLongValue(it->pItem, "TextTop", 0);
        nTextWidth = Ini.GetLongValue(it->pItem, "TextWidth", 0);
        nTextHeight = Ini.GetLongValue(it->pItem, "TextHeight", 0);
        nFontType = Ini.GetLongValue(it->pItem, "FontType", 0);

        //For listctrl
        strItemTextureFile = Ini.GetValue(it->pItem, "ItemTextureFile", NULL);
        strListItemBasename = Ini.GetValue(it->pItem, "ListItemBasename", NULL);
        strColumnWidth = Ini.GetValue(it->pItem, "ColumnWidth", NULL);
        if (strColumnWidth != NULL)
        {
            memset(szColumnWidth, 0, sizeof(int) * s_nMaxListItemColumn);
            StringToIntArray(strColumnWidth, szColumnWidth, ',');
        }
        nColumnCount = Ini.GetLongValue(it->pItem, "ColumnCount", 0);
        nListItemCount = Ini.GetLongValue(it->pItem, "ListItemCount", 0);

        switch(nWidgetType)
        {
        case 1:         //CDXImage
            {
                pWidget = new CDXImage;
                ((CDXImage *)pWidget)->Init(it->pItem, strTextureFile, nLeft, nTop,
                                            nWidth, nHeight, bVisible, nDepth);
            }
            break;

        case 2:         //CDXLabel
            {
                pWidget = new CDXLabel;
                ((CDXLabel *)pWidget)->Init(it->pItem, strTextureFile, nLeft, nTop,
                                            nWidth, nHeight, nFontType, nTextLeft, nTextTop,
                                            nTextWidth, nTextHeight, bVisible, nDepth);
            }
            break;

        case 3:         //CDXButton
            {
                pWidget = new CDXButton;
                ((CDXButton *)pWidget)->Init(it->pItem, strTextureFile, strTextureFileSelected, strTextureFileDisabled,
                                            nLeft, nTop,nWidth, nHeight, bVisible, nDepth);
            }
            break;
        case 4:
            {
                pWidget = new CDXListCtrl;
                ((CDXListCtrl *)pWidget)->Init(it->pItem, strItemTextureFile, strListItemBasename, 
                                                nLeft, nTop, nWidth, nHeight, nFontType, 
                                                nListItemCount, nColumnCount, szColumnWidth,
                                                bVisible, nDepth);
            }

        default:
            break;
        }


        m_mapWidget.insert(make_pair(it->pItem, pWidget));
    }
}

void CGameEngine::Shutdown()
{
    for (vector<TextureInfo *>::iterator it = m_vecTexture.begin();
        it != m_vecTexture.end(); ++it)
    {
        SAFE_RELEASE((*it)->pTexture);
        SAFE_DELETE(*it);
    }
    m_vecTexture.clear();

    for (vector<FontInfo *>::iterator it = m_vecFont.begin(); 
        it != m_vecFont.end(); ++it)
    {
        SAFE_RELEASE((*it)->pFont);
        SAFE_DELETE(*it);
    }
    m_vecFont.clear();

    for (map<string, CDXWidget*>::iterator it = m_mapWidget.begin();
        it != m_mapWidget.end(); ++it)
    {
        (it->second)->Shutdown();
        SAFE_DELETE(it->second);
    }
    m_mapWidget.clear();
}

CDXWidget *CGameEngine::GetWidgetByName( const char *strName )
{
    map<string, CDXWidget*>::iterator it;
    for (it = m_mapWidget.begin(); it != m_mapWidget.end(); ++it)
    {
        if (it->first.compare(strName) == 0)
        {
            return it->second;
        }
    }

    return NULL;
}

void CGameEngine::AddWidget( CDXWidget *pWidget )
{
    if (pWidget != NULL)
    {
        m_mapWidget.insert(make_pair(pWidget->GetWidgetName(), pWidget));
    }
}

void CGameEngine::Render()
{
    map<string, CDXWidget*>::iterator it = m_mapWidget.begin();
    multimap<int, CDXWidget*,  greater<int>> GraphicRes;
    for (; it != m_mapWidget.end(); ++it)
    {
        GraphicRes.insert(make_pair(it->second->GetDepth(), it->second));
    }

    multimap<int, CDXWidget*,  greater<int>>::iterator mit = GraphicRes.begin();
    for (; mit != GraphicRes.end(); ++mit)
    {
        mit->second->Render();
    }
}

LPD3DXFONT CGameEngine::GetFont( int nFontType )
{
    LPD3DXFONT pFont = NULL;
    HRESULT hRet;

    vector<FontInfo *>::iterator it = m_vecFont.begin();
    for (; it != m_vecFont.end(); ++it)
    {
        if ((*it)->nFontType == nFontType)
        {
            pFont = (*it)->pFont;
        }
    }

    //如果没有字体则创建
    if (pFont == NULL)
    {
        FontInfo *pInfo = new FontInfo;
        ZeroMemory(pInfo, sizeof(FontInfo));
        pInfo->nFontType = nFontType;

        switch(nFontType)
        {
        case ARIAL_BOLD_16:
            {
                hRet = D3DXCreateFont(m_pD3DDevice, 16, 0, 1000, 0, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 0, "Arial", &pInfo->pFont);
            }
            break;

        case ARIAL_BOLD_24:
            {                
                hRet = D3DXCreateFont(m_pD3DDevice, 24, 0, 1000, 0, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 0, "Arial", &pInfo->pFont);
            }
            break;

        case ARIAL_BOLD_32:
            {
                hRet = D3DXCreateFont(m_pD3DDevice, 32, 0, 1000, 0, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 0, "Arial", &pInfo->pFont);
            }
            break;

        case ARIAL_BOLD_40:
            {
                hRet = D3DXCreateFont(m_pD3DDevice, 40, 0, 1000, 0, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 0, "Arial", &pInfo->pFont);
            }
            break;

        case ARIAL_BOLD_48:
            {
                hRet = D3DXCreateFont(m_pD3DDevice, 48, 0, 1000, 0, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 0, "Arial", &pInfo->pFont);
            }
            break;

        default:
            break;
        }
        
        if (FAILED(hRet))
        {
            MessageBox(NULL, "D3DXCreateFont() failed", "创建字体失败", MB_OK);
            return NULL;
        }
        else if (hRet == S_OK)
        {
            pFont = pInfo->pFont;
            m_vecFont.push_back(pInfo);
        }

    }

    return pFont;
}

LPDIRECT3DTEXTURE9 CGameEngine::GetTexture( const char *strFileName, int &nWidth, int &nHeight)
{
    LPDIRECT3DTEXTURE9 pTexture = NULL;

    vector<TextureInfo *>::iterator it = m_vecTexture.begin();
    for (; it != m_vecTexture.end(); ++it)
    {
        if (strcmp(strFileName, (*it)->szFileName) == 0)
        {
            pTexture = (*it)->pTexture;
            nHeight = (*it)->nHeight;
            nWidth = (*it)->nWidth;
        }
    }

    //如果纹理不存在，则创建
    if (pTexture == NULL)
    {
        TextureInfo *pInfo = new TextureInfo;
        ZeroMemory(pInfo, sizeof(TextureInfo));
        strcpy(pInfo->szFileName, strFileName);
        D3DXIMAGE_INFO ImageInfo;
        ZeroMemory(&ImageInfo, sizeof(ImageInfo));

        if (FAILED(D3DXGetImageInfoFromFile(pInfo->szFileName, &ImageInfo)))
        {
            MessageBox(NULL, "D3DXGetImageInfoFromFile() failed", "获取图像信息失败", MB_OK);
            return NULL;
        }

        HRESULT hRet = D3DXCreateTextureFromFileEx(m_pD3DDevice, 
                                                   pInfo->szFileName, 
                                                   ImageInfo.Width,
                                                   ImageInfo.Height,
                                                   ImageInfo.MipLevels,
                                                   0,
                                                   ImageInfo.Format,
                                                   D3DPOOL_MANAGED,
                                                   D3DX_FILTER_NONE,
                                                   D3DX_FILTER_NONE,
                                                   0,
                                                   &ImageInfo,
                                                   NULL, 
                                                   &pInfo->pTexture);
        if (FAILED(hRet))
        {
            MessageBox(NULL, "D3DXCreateTextureFromFileEx() failed", "创建纹理失败", MB_OK);
            return NULL;
        }
        else
        {
            pInfo->nHeight = ImageInfo.Height;
            pInfo->nWidth = ImageInfo.Width;
            pTexture = pInfo->pTexture;
            nHeight = ImageInfo.Height;
            nWidth = ImageInfo.Width;
        }
        m_vecTexture.push_back(pInfo);
    }

    return pTexture;
}

void CGameEngine::StringToIntArray( const char *str, int szArr[], char chDelimiter )
{
    const char *p = str;
    int i = 0;
    int j = 0;
    char strDst[256];
    char strDigit[5];
    memset(strDst, 0, 256);
    memset(strDigit, 0, 5);


    //去掉其他字符
    while(*p != '\0')
    {
        if (*p == chDelimiter || *p >= '0' || *p <= '9')
        {
            strDst[i] = *p;
            i++;
        }

        p++;
    }

    p = strDst;
    i = 0;

    //40,90,70
    while (*p != '\0')
    {
        if (*p == ',')
        {
            szArr[i] = atoi(strDigit);
            i++;
            memset(strDigit, 0, 5);
            j = 0;
        }
        else
        {
            strDigit[j] = *p;
            j++;
        }

        p++;
    }
    szArr[i] = atoi(strDigit);
}

void CGameEngine::CreateTexFromDir( const char *strDir )
{
    WIN32_FIND_DATA FindFileData;
    HANDLE hListFile;

    char szFilePath[MAX_PATH];
    char szFileFullPath[MAX_PATH];
    strcpy(szFilePath, strDir);
    strcat(szFilePath, "*.png");

    hListFile = FindFirstFile(szFilePath, &FindFileData);
    if (hListFile == INVALID_HANDLE_VALUE)
    {
        return;
    }
    else
    {
        do 
        {
            TextureInfo *pInfo = new TextureInfo;
            ZeroMemory(pInfo, sizeof(TextureInfo));
            sprintf(pInfo->szFileName, "%s%s", strDir, FindFileData.cFileName);
            D3DXIMAGE_INFO ImageInfo;
            ZeroMemory(&ImageInfo, sizeof(ImageInfo));

            if (FAILED(D3DXGetImageInfoFromFile(pInfo->szFileName, &ImageInfo)))
            {
                MessageBox(NULL, "D3DXGetImageInfoFromFile() failed", "获取图像信息失败", MB_OK);
                continue;
            }

            HRESULT hRet = D3DXCreateTextureFromFileEx(m_pD3DDevice, 
                pInfo->szFileName, 
                ImageInfo.Width,
                ImageInfo.Height,
                ImageInfo.MipLevels,
                0,
                ImageInfo.Format,
                D3DPOOL_MANAGED,
                D3DX_FILTER_NONE,
                D3DX_FILTER_NONE,
                0,
                &ImageInfo,
                NULL, 
                &pInfo->pTexture);
            if (FAILED(hRet))
            {
                MessageBox(NULL, "D3DXCreateTextureFromFileEx() failed", "创建纹理失败", MB_OK);
                continue;
            }
            pInfo->nHeight = ImageInfo.Height;
            pInfo->nWidth = ImageInfo.Width;
            m_vecTexture.push_back(pInfo);

        } while (FindNextFile(hListFile, &FindFileData));
    }

}
