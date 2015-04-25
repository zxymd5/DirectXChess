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
 *  File:		main.cpp
 *  Author:		Richard Zou
 *	Created on:	2015-03-01
 */

#include "../GameEngine/GameEngine.h"
#include "GameView.h"
#include "GameHandle.h"
#include "GameSettings.h"
#include "resource.h"

#include <windows.h>
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

LRESULT CALLBACK SettingsProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch(msg)
    {
    case WM_INITDIALOG:
        {
            RECT rt;
            GetWindowRect(hWnd, &rt);
            OffsetRect(&rt, 512 - (rt.right - rt.left) / 2, 384 - (rt.bottom - rt.top) / 2);
            MoveWindow(hWnd, rt.left, rt.top, rt.right - rt.left, rt.bottom - rt.top, TRUE);

            HWND ComboBox = GetDlgItem(hWnd, IDC_COMBO_COMPITIOR);
            SendMessage(ComboBox, CB_ADDSTRING, 0, (LPARAM)"电脑");
            SendMessage(ComboBox, CB_ADDSTRING, 0, (LPARAM)"人");
            SendMessage(ComboBox, CB_SETCURSEL, (WPARAM)(g_GameSettings.m_nCompetitor - 1), 0);

            ComboBox = GetDlgItem(hWnd, IDC_COMBO_COMPITIORSIDE);
            SendMessage(ComboBox, CB_ADDSTRING, 0, (LPARAM)"黑");
            SendMessage(ComboBox, CB_ADDSTRING, 0, (LPARAM)"红");
            SendMessage(ComboBox, CB_SETCURSEL, (WPARAM)(g_GameSettings.m_nCompetitorSide - 1), 0);

            ComboBox = GetDlgItem(hWnd, IDC_COMBO_AHEAD);
            SendMessage(ComboBox, CB_ADDSTRING, 0, (LPARAM)"黑方");
            SendMessage(ComboBox, CB_ADDSTRING, 0, (LPARAM)"红方");
            SendMessage(ComboBox, CB_SETCURSEL, (WPARAM)(g_GameSettings.m_nAhead - 1), 0);
        }
        break;

    case WM_COMMAND:
        {
            if (wParam == IDOK)
            {
                HWND ComboBox = GetDlgItem(hWnd, IDC_COMBO_COMPITIOR);
                g_GameSettings.m_nCompetitor = SendMessage(ComboBox, CB_GETCURSEL, 0, 0) + 1;

                ComboBox = GetDlgItem(hWnd, IDC_COMBO_COMPITIORSIDE);
                int nCompetitorSide = SendMessage(ComboBox, CB_GETCURSEL, 0, 0) + 1;

                ComboBox = GetDlgItem(hWnd, IDC_COMBO_AHEAD);
                g_GameSettings.m_nAhead = SendMessage(ComboBox, CB_GETCURSEL, 0, 0) + 1;
                
                if (g_GameSettings.m_nCompetitorSide != nCompetitorSide)
                {
                    g_GameSettings.m_nCompetitorSide = nCompetitorSide;
                    g_GameView.ChangeChessManPos();
                }

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

LRESULT WINAPI MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch(msg)
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
    }

    g_GameView.MsgResponse(hWnd, msg, wParam, lParam);

    return DefWindowProc(hWnd, msg, wParam, lParam);
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
        s_nWindowStartX, s_nWindowStartY, nWindowWidth, nWindowHeight,
        GetDesktopWindow(), NULL, wc.hInstance, NULL);
    SetWindowPos(hWnd, HWND_TOP, s_nWindowStartX, s_nWindowStartY, nWindowWidth, nWindowHeight, SWP_SHOWWINDOW);
    
    ShowWindow(hWnd, SW_SHOWDEFAULT);
    UpdateWindow(hWnd);
    
    g_Hwnd = hWnd;
    g_hInstance = hInstance;

    //GameEngine初始化
    g_GameEngine.Init(hWnd, nWindowWidth, nWindowHeight);
    g_GameEngine.ParseFile(s_pWidgetConfigFile);

    g_GameSettings.LoadSettings(s_pSettingsFile);

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

    return (int) msg.wParam;
}