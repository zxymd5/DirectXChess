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
 *  File:       main.cpp
 *  Author:     Richard Zou
 *  Created on: 2015-03-01
 */

#include "SockWrap.h"
#include "GameView.h"
#include "GameHandle.h"
#include "GameSettings.h"
#include "../GameEngine/GameEngine.h"
#include "resource.h"

#include <Commctrl.h>
#include <stdio.h>

#define WINDOW_CLASS    "CHESS"
#define WINDOW_NAME     "CHESS BY DIRECTX"
HWND g_Hwnd;
HINSTANCE g_hInstance;

using namespace std;

void RenderScene()
{
    g_GameEngine.BeginShow();

    g_GameView.Render();

    g_GameEngine.EndShow();
}

LRESULT CALLBACK SettingsProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
    case WM_INITDIALOG:
        {
            RECT rc;
            GetWindowRect(hWnd, &rc);
            OffsetRect(&rc, 512 - (rc.right - rc.left) / 2, 384 - (rc.bottom - rc.top) / 2);
            MoveWindow(hWnd, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, TRUE);

            HWND hComboBox = GetDlgItem(hWnd, IDC_COMBO_COMPITIOR);
            SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"人机对战");
            SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"自由下棋");
            SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"网络对战");
            SendMessage(hComboBox, CB_SETCURSEL, (WPARAM)(g_GameSettings.m_nGameType - 1), 0);

            hComboBox = GetDlgItem(hWnd, IDC_COMBO_COMPITIORSIDE);
            SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"黑");
            SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"红");
            SendMessage(hComboBox, CB_SETCURSEL, (WPARAM)(g_GameSettings.m_nCompetitorSide - 1), 0);

            hComboBox = GetDlgItem(hWnd, IDC_COMBO_AHEAD);
            SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"黑方");
            SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"红方");
            SendMessage(hComboBox, CB_SETCURSEL, (WPARAM)(g_GameSettings.m_nAhead - 1), 0);

            hComboBox = GetDlgItem(hWnd, IDC_COMBO_SIDE);
            SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"客户端");
            SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"服务器端");
            SendMessage(hComboBox, CB_SETCURSEL, (WPARAM)(g_GameSettings.m_nServerOrClient - 1), 0);

            SetDlgItemInt(hWnd, IDC_STEPTIMEEDIT, g_GameSettings.m_nStepTime, FALSE);
            SetDlgItemText(hWnd, IDC_IPADDREDIT, g_GameSettings.m_szIpAddr);
            SetDlgItemInt(hWnd, IDC_PORTEDIT, g_GameSettings.m_nPort, FALSE);
        }
        break;

    case WM_COMMAND:
        {
            if (wParam == IDOK)
            {
                HWND hComboBox = GetDlgItem(hWnd, IDC_COMBO_COMPITIOR);
                g_GameSettings.m_nGameType = SendMessage(hComboBox, CB_GETCURSEL, 0, 0) + 1;

                hComboBox = GetDlgItem(hWnd, IDC_COMBO_COMPITIORSIDE);
                int nCompetitorSide = SendMessage(hComboBox, CB_GETCURSEL, 0, 0) + 1;

                hComboBox = GetDlgItem(hWnd, IDC_COMBO_AHEAD);
                g_GameSettings.m_nAhead = SendMessage(hComboBox, CB_GETCURSEL, 0, 0) + 1;

                hComboBox = GetDlgItem(hWnd, IDC_COMBO_SIDE);
                g_GameSettings.m_nServerOrClient = SendMessage(hComboBox, CB_GETCURSEL, 0, 0) + 1;
                
                if (g_GameSettings.m_nCompetitorSide != nCompetitorSide)
                {
                    g_GameSettings.m_nCompetitorSide = nCompetitorSide;
                    g_GameView.ChangeChessManPos();
                }
                
                g_GameSettings.m_nStepTime = GetDlgItemInt(hWnd, IDC_STEPTIMEEDIT, NULL, FALSE);
                g_GameSettings.m_nPort = GetDlgItemInt(hWnd, IDC_PORTEDIT, NULL, FALSE);
                GetDlgItemText(hWnd, IDC_IPADDREDIT, g_GameSettings.m_szIpAddr, 32);

                EndDialog(hWnd, wParam);
            }
            else if (wParam == IDCANCEL)
            {
                EndDialog(hWnd, wParam);
            }
        }
        break;

    default:
        break;
    }

    return 0;
}

void OnSettings()
{
    DialogBox(g_hInstance, (LPCTSTR)IDD_DIALOG_SETTINGS, g_Hwnd, (DLGPROC)SettingsProc);
}

void OnFileOperate(UINT uMsg)
{
    OPENFILENAME Ofn;
    char szFileName[MAX_PATH];
    memset(szFileName, 0, MAX_PATH);

    ZeroMemory(&Ofn, sizeof(Ofn));
    Ofn.lStructSize = sizeof(Ofn);
    Ofn.hwndOwner = g_Hwnd;
    Ofn.lpstrFile = szFileName;
    Ofn.nMaxFile = sizeof(szFileName);
    Ofn.lpstrFilter = "文本文件(*.txt)\0*.txt\0数据文件(*.db)\0*.db\0\0";
    Ofn.nFilterIndex = 1;
    Ofn.lpstrFileTitle = NULL;
    Ofn.nMaxFileTitle = 0;
    Ofn.lpstrInitialDir = NULL;

    if (uMsg == WM_OPENFILE)
    {
        Ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
        Ofn.lpstrTitle = "打开文件";
        if (GetOpenFileName(&Ofn))
        {
            g_GameHandle.LoadFromFile(szFileName, Ofn.nFilterIndex);
        }
    }
    else
    {
        Ofn.Flags = OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST |
            OFN_HIDEREADONLY | OFN_NOREADONLYRETURN;
        Ofn.lpstrTitle = "保存文件";
        if (GetSaveFileName(&Ofn))
        {
            g_GameHandle.SaveToFile(szFileName, Ofn.nFilterIndex);
        }
    }
}

LRESULT WINAPI MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
        break;

    case WM_KEYUP:
        if(wParam == VK_ESCAPE) PostQuitMessage(0);
        break;

    case WM_LBUTTONUP:
        break;

    case WM_MOUSEMOVE:
        break;

    case WM_SETTINGS:
        OnSettings();
        break;
    
    case WM_OPENFILE:
    case WM_SAVEFILE:
        OnFileOperate(uMsg);
        break;

    default:
        break;
    }

    g_GameView.MsgResponse(hWnd, uMsg, wParam, lParam);
    g_GameHandle.ProcessMessage();

    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPTSTR    lpCmdLine,
                   int       nCmdShow)
{
    // Register the window class
    WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc, 0L, 0L,
        GetModuleHandle(NULL), LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CHESS)), LoadCursor(NULL, IDC_ARROW), NULL, NULL,
        WINDOW_CLASS, LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CHESS)) };
    RegisterClassEx(&wc);

    // Create the application's window
    HWND hWnd = CreateWindow(WINDOW_CLASS, WINDOW_NAME, WS_POPUPWINDOW,
        WINDOW_START_X, WINDOW_START_Y, WINDOW_WIDTH, WINDOW_HEIGHT,
        GetDesktopWindow(), NULL, wc.hInstance, NULL);
    SetWindowPos(hWnd, HWND_TOP, WINDOW_START_X, WINDOW_START_Y, WINDOW_WIDTH, WINDOW_HEIGHT, SWP_SHOWWINDOW);
    
    ShowWindow(hWnd, SW_SHOWDEFAULT);
    UpdateWindow(hWnd);
    
    g_Hwnd = hWnd;
    g_hInstance = hInstance;

    //初始化网络
    CSockWrap::InitNetwork();

    //GameEngine初始化
    g_GameEngine.Init(hWnd, WINDOW_WIDTH, WINDOW_HEIGHT);
    g_GameEngine.CreateTexFromFiles(PICTURE_FOLDER);
    g_GameEngine.ParseFile(WIDGET_CONFIG_FILE);

    g_GameSettings.LoadSettings(SETTINGS_FILE);

    g_GameView.Init(hWnd);
    g_GameHandle.Attach(&g_GameView);
    g_GameHandle.Init();

    MSG msg;
    ZeroMemory(&msg, sizeof(msg));

    while(msg.message != WM_QUIT)
    {
        if(PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            RenderScene();
        }
    }

    UnregisterClass(WINDOW_CLASS, wc.hInstance);

    g_GameView.Shutdown();
    g_GameEngine.Shutdown();
    g_GameHandle.Shutdown();
    CSockWrap::StopNetwork();

    return (int) msg.wParam;
}