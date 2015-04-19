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
 *  File:		GameView.h
 *  Author:		Richard Zou
 *	Created on:	2015-03-01
 */

#ifndef GAME_VIEW_H
#define GAME_VIEW_H

#include "CommDef.h"
#include "Observer.h"
#include "../GameEngine/DXImage.h"
#include "../GameEngine/DXButton.h"
#include "../GameEngine/DXLabel.h"
#include "../GameEngine/DXListCtrl.h"
#include "../GameEngine/SoundPlayer.h"

#include <Windows.h>
#include <map>
#include <algorithm>

using namespace std;

class CGameView : public CObserver
{
public:
    CGameView(void);
    virtual ~CGameView(void);

    CDXWidget *GetResponseMsgWidget();
    void MsgResponse(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam);
    void HandleLButtonUp();

    virtual void ProcessEvent(CSubject *pSub, int nEvent);
    void ProcessUpdateChessManEvent(CSubject *pSub);
    void ProcessUpdateMoveRouteEvent(CSubject *pSub);

    void UpdateChessMan(int szChessMan[s_nChessBoardRow][s_nChessBoardColumn]);
    void UpdateMoveRoute(const MoveRoute &stRoute, int szChessMan[s_nChessBoardRow][s_nChessBoardColumn]);
    void PlayTipSound(const MoveRoute &stRoute, int nGameResult);
    void UpdateGeneralStatus(int nGameResult);
    void UpdateMoveHistory();
    void ShowResultView(int nGameResult);

    void Init(HWND hWnd);
    void ChangeChessManPos();
    void Shutdown();
    void Render();

    static void OnStart(void * pParam);
    static void OnPauseGame(void * pParam);
    static void OnContinue(void * pParam);
    static void OnNewGame(void * pParam);
    static void OnOpen(void * pParam);
    static void OnSave(void * pParam);
    static void OnFallback(void * pParam);
    static void OnTie(void * pParam);
    static void OnLose(void * pParam);
    static void OnSettings(void * pParam);
    static void OnExit(void * pParam);
    static void OnConfirm(void * pParam);

private:
    CDXImage    *m_pChessBackground;
    CDXImage    *m_pChessBoard;
    CDXImage    *m_pChessBoardCover;
    CDXImage    *m_pBlackSide;
    CDXImage    *m_pRedSide;
    CDXImage    *m_pszGamingChessMan[s_nChessBoardRow][s_nChessBoardColumn];
    CDXImage    *m_pRightHistoryBG;
    CDXImage    *m_pLeftHistoryBG;
    CDXButton   *m_pStart;
    CDXButton   *m_pPauseGame;
    CDXButton   *m_pContinue;
    CDXButton   *m_pNewGame;
    CDXButton   *m_pOpen;
    CDXButton   *m_pSave;
    CDXButton   *m_pFallback;
    CDXButton   *m_pTie;
    CDXButton   *m_pLose;
    CDXButton   *m_pSettings;
    CDXButton   *m_pExit;
    CDXLabel    *m_pMessageBox;
    CDXButton   *m_pConfirm;
    CDXWidget   *m_pCurrFocusWidget;
    CDXListCtrl *m_pLeftMoveHistory;
    CDXListCtrl *m_pRightMoveHistory;
    CDXButton   *m_pLeftPrevPage;
    CDXButton   *m_pLeftPrevRecord;
    CDXButton   *m_pLeftNextPage;
    CDXButton   *m_pLeftNextRecord;
    CDXLabel    *m_pLeftPageInfo;
    CDXButton   *m_pRightPrevRecord;
    CDXButton   *m_pRightPrevPage;
    CDXButton   *m_pRightNextPage;
    CDXButton   *m_pRightNextRecord;
    CDXLabel    *m_pRightPageInfo;
    CSoundPlayer m_clSoundPlayer;

    HWND        m_hWnd;
    multimap<int, CDXWidget *, greater<int>> m_mapWidget;

    bool        m_bGameStarted;
    bool        m_bGamePaused;
    bool        m_bGameOver;

};

extern CGameView g_GameView;

#endif