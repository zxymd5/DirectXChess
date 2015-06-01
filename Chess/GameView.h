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
 *  File:       GameView.h
 *  Author:     Richard Zou
 *  Created on: 2015-03-01
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

#include <map>
#include <algorithm>
#include <vector>

using namespace std;

class CGameView : public CObserver
{
public:
    CGameView(void);
    virtual ~CGameView(void);

    CDXWidget *GetResponseMsgWidget();
    void MsgResponse(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    void HandleLButtonUp();

    virtual void ProcessEvent(CSubject *pSub, int nEvent, void *pParam);
    void ProcessInitBoardEvent(CSubject *pSub);
    void ProcessChangePosEvent(CSubject *pSub);
    void ProcessNewGameEvent(CSubject *pSub);
    void ProcessUpdateMoveRouteEvent(CSubject *pSub);
    void ProcessFallbackEvent( CSubject *pSub );
    void ProcessLoadChessManEvent( CSubject *pSub );
    void ProcessGameResultEvent( CSubject *pSub );
    void ProcessIllegalMoveEvent( CSubject *pSub );
    void ProcessTipEvent(CSubject *pSub, int nEvent, void *pParam);
    void ProcessTipReplyEvent(CSubject *pSub, int nEvent, void *pParam);

    void UpdateChessMan(int arrChessMan[CHESSBOARD_ROW][CHESSBOARD_COLUMN]);
    void UpdateMoveRoute(const MoveRoute &stRoute, int arrChessMan[CHESSBOARD_ROW][CHESSBOARD_COLUMN]);
    void PlayTipSound(const MoveRoute &stRoute, int nGameResult);

    void PlayGameResultSound( int nGameResult );

    void UpdateGeneralStatus(int nGameResult);
    void UpdateMoveHistory();
    void AddMoveHistory(const MoveRoute &stRoute);
    void FallbackMoveHistory(bool bMySide);

    void ClearHisotryDisplay();
    void UpdateMoveHistoryDisplay(bool bMySide);

    int GetMoveHistoryTotalPage(bool bMySide);
    int GetMoveHistoryCurrentPage(bool bMySide);
    int GetMoveHistoryCurrentIndex(bool bMySide);

    void ShowResultView(int nGameResult);
    void ShowTipView(int nTipType);
    void ShowTipReplyView( int nTipType, int nResult );

    void Init(HWND hWnd);
    void ChangeChessManPos();
    void Shutdown();
    void Render();
    void NextRecord(bool bMySide);
    void PrevRecord(bool bMySide);
    void NextPage(bool bMySide);
    void PrevPage(bool bMySide);

    static void OnStart(void * pParam);
    static void OnNewGame(void * pParam);
    static void OnOpen(void * pParam);
    static void OnSave(void * pParam);
    static void OnFallback(void * pParam);
    static void OnTie(void * pParam);
    static void OnLose(void * pParam);
    static void OnSettings(void * pParam);
    static void OnExit(void * pParam);
    static void OnConfirm(void * pParam);
    static void OnPrevPage(void * pParam);
    static void OnPrevRecord(void *pParam);
    static void OnNextRecord(void *pParam);
    static void OnNextPage(void *pParam);
    static void OnConnect(void *pParam);
    static void OnDisconnect(void *pParam);

private:
    CDXImage    *m_pChessBackground;
    CDXImage    *m_pChessBoard;
    CDXImage    *m_pChessBoardCover;
    CDXImage    *m_pBlackSide;
    CDXImage    *m_pRedSide;
    CDXImage    *m_pszGamingChessMan[CHESSBOARD_ROW][CHESSBOARD_COLUMN];
    CDXImage    *m_pRightHistoryBG;
    CDXImage    *m_pLeftHistoryBG;
    CDXButton   *m_pStart;
    CDXButton   *m_pNewGame;
    CDXButton   *m_pOpen;
    CDXButton   *m_pSave;
    CDXButton   *m_pFallback;
    CDXButton   *m_pTie;
    CDXButton   *m_pLose;
    CDXButton   *m_pSettings;
    CDXButton   *m_pExit;
    CDXButton   *m_pConnect;
    CDXButton   *m_pDisconnect;
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
    CDXLabel    *m_pStepTimeLeft;

    HWND        m_hWnd;
    multimap<int, CDXWidget *, greater<int>> m_mapWidget;

    bool        m_bGameStarted;
    bool        m_bGamePaused;
    bool        m_bGameOver;
    vector<ChineseMoveStep> m_vecLeftMoveHistory;
    vector<ChineseMoveStep> m_vecRightMoveHistory;
    int         m_nCurrentLeftStepOrder;  //从1开始
    int         m_nCurrentRightStepOrder; //从1开始
    bool        m_bStepTimeOverNotify;
    int         m_nTipType;
};

extern CGameView g_GameView;

#endif