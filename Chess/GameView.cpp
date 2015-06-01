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
*  File:       GameView.cpp
*  Author:     Richard Zou
*  Created on: 2015-03-01
*/

#include "GameView.h"
#include "GameHandle.h"
#include "GameSettings.h"

#include "../GameEngine/GameEngine.h"

CGameView g_GameView;

CGameView::CGameView(void)
{
    m_hWnd = NULL;
    m_pChessBackground = NULL;
    m_pChessBoardCover = NULL;
    m_pBlackSide = NULL;
    m_pRedSide = NULL;
    m_pLeftHistoryBG = NULL;
    m_pRightHistoryBG = NULL;

    for (int i = 0; i < CHESSBOARD_ROW; i++)
    {
        for (int j = 0; j < CHESSBOARD_COLUMN; j++)
        {
            m_pszGamingChessMan[i][j] = NULL;
        }
    }

    m_pStart = NULL;
    m_pNewGame = NULL;
    m_pOpen = NULL;
    m_pSave = NULL;
    m_pFallback = NULL;
    m_pTie = NULL;
    m_pLose = NULL;
    m_pSettings = NULL;
    m_pExit = NULL;
    m_pConnect = NULL;
    m_pDisconnect = NULL;
    m_pMessageBox = NULL;
    m_pConfirm = NULL;
    m_pCurrFocusWidget = NULL;
    m_pLeftMoveHistory = NULL;
    m_pRightMoveHistory = NULL;
    m_pLeftPrevPage = NULL;
    m_pLeftPrevRecord = NULL;
    m_pLeftNextPage = NULL;
    m_pLeftNextRecord = NULL;
    m_pLeftPageInfo = NULL;
    m_pRightPrevRecord = NULL;
    m_pRightPrevPage = NULL;
    m_pRightNextPage = NULL;
    m_pRightNextRecord = NULL;
    m_pRightPageInfo = NULL;
    m_pStepTimeLeft = NULL;

    m_bGameStarted = false;
    m_bGameOver = false;
    m_nCurrentLeftStepOrder = 0;
    m_nCurrentRightStepOrder = 0;
    m_bStepTimeOverNotify = false;

    m_nTipType = 0;
    m_nTipResult = 0;
    m_pAgree = NULL;
    m_pDisagree = NULL;
}

CGameView::~CGameView(void)
{
}

void CGameView::Init(HWND hWnd)
{
    m_hWnd = hWnd;

    m_pChessBackground = (CDXImage *)g_GameEngine.GetWidgetByName("Background");
    m_mapWidget.insert(make_pair(m_pChessBackground->GetDepth(), m_pChessBackground));

    m_pLeftHistoryBG = (CDXImage *)g_GameEngine.GetWidgetByName("HistoryLeft");
    m_mapWidget.insert(make_pair(m_pLeftHistoryBG->GetDepth(), m_pLeftHistoryBG));

    m_pRightHistoryBG = (CDXImage *)g_GameEngine.GetWidgetByName("HistoryRight");
    m_mapWidget.insert(make_pair(m_pRightHistoryBG->GetDepth(), m_pRightHistoryBG));

    m_pChessBoard = (CDXImage *)g_GameEngine.GetWidgetByName("ChessBoard");
    m_mapWidget.insert(make_pair(m_pChessBoard->GetDepth(), m_pChessBoard));

    m_pChessBoardCover = (CDXImage *)g_GameEngine.GetWidgetByName("ChessBoardCover");
    m_mapWidget.insert(make_pair(m_pChessBoardCover->GetDepth(), m_pChessBoardCover));

    m_pBlackSide = (CDXImage *)g_GameEngine.GetWidgetByName("BlackSide");
    m_mapWidget.insert(make_pair(m_pBlackSide->GetDepth(), m_pBlackSide));

    m_pRedSide = (CDXImage *)g_GameEngine.GetWidgetByName("RedSide");
    m_mapWidget.insert(make_pair(m_pRedSide->GetDepth(), m_pRedSide));

    m_pStart = (CDXButton *)g_GameEngine.GetWidgetByName("Start");
    m_pStart->SetCallBackInfo(OnStart, this);
    m_mapWidget.insert(make_pair(m_pStart->GetDepth(), m_pStart));

    m_pNewGame = (CDXButton *)g_GameEngine.GetWidgetByName("NewGame");
    m_pNewGame->SetCallBackInfo(OnNewGame, this);
    m_pNewGame->SetCurrState(STATE_DISABLE);
    m_mapWidget.insert(make_pair(m_pNewGame->GetDepth(), m_pNewGame));

    m_pOpen = (CDXButton *)g_GameEngine.GetWidgetByName("Open");
    m_pOpen->SetCallBackInfo(OnOpen, this);
    m_pOpen->SetCurrState(STATE_DISABLE);
    m_mapWidget.insert(make_pair(m_pOpen->GetDepth(), m_pOpen));

    m_pSave = (CDXButton *)g_GameEngine.GetWidgetByName("Save");
    m_pSave->SetCallBackInfo(OnSave, this);
    m_pSave->SetCurrState(STATE_DISABLE);
    m_mapWidget.insert(make_pair(m_pSave->GetDepth(), m_pSave));    

    m_pFallback = (CDXButton *)g_GameEngine.GetWidgetByName("Fallback");
    m_pFallback->SetCallBackInfo(OnFallback, this);
    m_pFallback->SetCurrState(STATE_DISABLE);
    m_mapWidget.insert(make_pair(m_pFallback->GetDepth(), m_pFallback));  

    m_pTie = (CDXButton *)g_GameEngine.GetWidgetByName("Tie");
    m_pTie->SetCallBackInfo(OnTie, this);
    m_pTie->SetCurrState(STATE_DISABLE);
    m_mapWidget.insert(make_pair(m_pTie->GetDepth(), m_pTie));

    m_pLose = (CDXButton *)g_GameEngine.GetWidgetByName("Lose");
    m_pLose->SetCallBackInfo(OnLose, this);
    m_pLose->SetCurrState(STATE_DISABLE);
    m_mapWidget.insert(make_pair(m_pLose->GetDepth(), m_pLose));

    m_pSettings = (CDXButton *)g_GameEngine.GetWidgetByName("Settings");
    m_pSettings->SetCallBackInfo(OnSettings, this);
    m_mapWidget.insert(make_pair(m_pSettings->GetDepth(), m_pSettings));

    m_pExit = (CDXButton *)g_GameEngine.GetWidgetByName("Exit");
    m_pExit->SetCallBackInfo(OnExit, this);
    m_mapWidget.insert(make_pair(m_pExit->GetDepth(), m_pExit));

    m_pConnect = (CDXButton *)g_GameEngine.GetWidgetByName("Connect");
    m_pConnect->SetCallBackInfo(OnConnect, this);
    m_mapWidget.insert(make_pair(m_pConnect->GetDepth(), m_pConnect));

    m_pDisconnect = (CDXButton *)g_GameEngine.GetWidgetByName("Disconnect");
    m_pDisconnect->SetCallBackInfo(OnDisconnect, this);
    m_mapWidget.insert(make_pair(m_pDisconnect->GetDepth(), m_pDisconnect));

    m_pMessageBox = (CDXLabel *)g_GameEngine.GetWidgetByName("MessageBox");
    m_pMessageBox->SetFontColor(255, 255, 255, 255);
    m_pMessageBox->SetAlignment(DT_CENTER | DT_VCENTER);
    m_mapWidget.insert(make_pair(m_pMessageBox->GetDepth(), m_pMessageBox));

    m_pLeftMoveHistory = (CDXListCtrl *)g_GameEngine.GetWidgetByName("LeftMoveCtrl");
    for (int i = 0; i < m_pLeftMoveHistory->GetListItemCount(); i++)
    {
        m_pLeftMoveHistory->GetListItem(i).SetFontColor(255, 0, 0, 0);
        m_pLeftMoveHistory->GetListItem(i).SetAlignment(DT_CENTER | DT_VCENTER);
    }
    m_mapWidget.insert(make_pair(m_pLeftMoveHistory->GetDepth(), m_pLeftMoveHistory));

    m_pRightMoveHistory = (CDXListCtrl *)g_GameEngine.GetWidgetByName("RightMoveCtrl");
    for (int i = 0; i < m_pLeftMoveHistory->GetListItemCount(); i++)
    {
        m_pRightMoveHistory->GetListItem(i).SetFontColor(255, 0, 0, 0);
        m_pRightMoveHistory->GetListItem(i).SetAlignment(DT_CENTER | DT_VCENTER);
    }
    m_mapWidget.insert(make_pair(m_pRightMoveHistory->GetDepth(), m_pRightMoveHistory));

    m_pConfirm = (CDXButton *)g_GameEngine.GetWidgetByName("Confirm");
    m_pConfirm->SetCallBackInfo(OnConfirm, this);
    m_mapWidget.insert(make_pair(m_pConfirm->GetDepth(), m_pConfirm));

    m_pAgree = (CDXButton *)g_GameEngine.GetWidgetByName("Agree");
    m_pAgree->SetCallBackInfo(OnAgreeOrDisagree, this);
    m_mapWidget.insert(make_pair(m_pAgree->GetDepth(), m_pAgree));

    m_pDisagree = (CDXButton *)g_GameEngine.GetWidgetByName("Disagree");
    m_pDisagree->SetCallBackInfo(OnAgreeOrDisagree, this);
    m_mapWidget.insert(make_pair(m_pDisagree->GetDepth(), m_pDisagree));

    m_pLeftPrevPage = (CDXButton *)g_GameEngine.GetWidgetByName("LeftPrevPage");
    m_pLeftPrevPage->SetCallBackInfo(OnPrevPage, this);
    m_mapWidget.insert(make_pair(m_pLeftPrevPage->GetDepth(), m_pLeftPrevPage));

    m_pLeftPrevRecord = (CDXButton *)g_GameEngine.GetWidgetByName("LeftPrevRecord");
    m_pLeftPrevRecord->SetCallBackInfo(OnPrevRecord, this);
    m_mapWidget.insert(make_pair(m_pLeftPrevRecord->GetDepth(), m_pLeftPrevRecord));

    m_pLeftNextPage = (CDXButton *)g_GameEngine.GetWidgetByName("LeftNextPage");
    m_pLeftNextPage->SetCallBackInfo(OnNextPage, this);
    m_mapWidget.insert(make_pair(m_pLeftNextPage->GetDepth(), m_pLeftNextPage));

    m_pLeftNextRecord = (CDXButton *)g_GameEngine.GetWidgetByName("LeftNextRecord");
    m_pLeftNextRecord->SetCallBackInfo(OnNextRecord, this);
    m_mapWidget.insert(make_pair(m_pLeftNextRecord->GetDepth(), m_pLeftNextRecord));

    m_pLeftPageInfo = (CDXLabel *)g_GameEngine.GetWidgetByName("LeftPageInfo");
    m_pLeftPageInfo->SetFontColor(255, 0, 0, 0);
    m_pLeftPageInfo->SetAlignment(DT_CENTER | DT_VCENTER);
    m_pLeftPageInfo->SetText("0/0");
    m_mapWidget.insert(make_pair(m_pLeftPageInfo->GetDepth(), m_pLeftPageInfo));

    m_pRightPrevPage = (CDXButton *)g_GameEngine.GetWidgetByName("RightPrevPage");
    m_pRightPrevPage->SetCallBackInfo(OnPrevPage, this);
    m_mapWidget.insert(make_pair(m_pRightPrevPage->GetDepth(), m_pRightPrevPage));

    m_pRightPrevRecord = (CDXButton *)g_GameEngine.GetWidgetByName("RightPrevRecord");
    m_pRightPrevRecord->SetCallBackInfo(OnPrevRecord, this);
    m_mapWidget.insert(make_pair(m_pRightPrevRecord->GetDepth(), m_pRightPrevRecord));

    m_pRightNextPage = (CDXButton *)g_GameEngine.GetWidgetByName("RightNextPage");
    m_pRightNextPage->SetCallBackInfo(OnNextPage, this);
    m_mapWidget.insert(make_pair(m_pRightNextPage->GetDepth(), m_pRightNextPage));

    m_pRightNextRecord = (CDXButton *)g_GameEngine.GetWidgetByName("RightNextRecord");
    m_pRightNextRecord->SetCallBackInfo(OnNextRecord, this);
    m_mapWidget.insert(make_pair(m_pRightNextRecord->GetDepth(), m_pRightNextRecord));

    m_pRightPageInfo = (CDXLabel *)g_GameEngine.GetWidgetByName("RightPageInfo");
    m_pRightPageInfo->SetFontColor(255, 0, 0, 0);
    m_pRightPageInfo->SetAlignment(DT_CENTER | DT_VCENTER);
    m_pRightPageInfo->SetText("0/0");
    m_mapWidget.insert(make_pair(m_pRightPageInfo->GetDepth(), m_pRightPageInfo));

    m_pStepTimeLeft = (CDXLabel *)g_GameEngine.GetWidgetByName("StepTimeLeft");
    m_pStepTimeLeft->SetFontColor(255, 255, 0, 0);
    m_pStepTimeLeft->SetAlignment(DT_CENTER | DT_VCENTER);
    m_pStepTimeLeft->SetText(" ");
    m_mapWidget.insert(make_pair(m_pStepTimeLeft->GetDepth(), m_pStepTimeLeft));

    char szChessManName[100];
    for (int i = 0; i < CHESSBOARD_ROW; i++)
    {
        for (int j = 0; j < CHESSBOARD_COLUMN; j++)
        {
            sprintf(szChessManName, "ChessMan%d_%d", i, j);
            CDXImage *pChessMan = new CDXImage;

            pChessMan->SetWidgetName(szChessManName);
            pChessMan->SetVisible(false);
            pChessMan->SetDepth(97);

            m_pszGamingChessMan[i][j] = pChessMan;
            m_mapWidget.insert(make_pair(pChessMan->GetDepth(), pChessMan));
            g_GameEngine.AddWidget(pChessMan);
        }
    }

    ChangeChessManPos();

    //声音设备初始化
    m_clSoundPlayer.Init(hWnd);
    m_clSoundPlayer.AddAudioFile(AUDIO_NEW_GAME);
    m_clSoundPlayer.AddAudioFile(AUDIO_ATTACK_KING);
    m_clSoundPlayer.AddAudioFile(AUDIO_CHOOSE);
    m_clSoundPlayer.AddAudioFile(AUDIO_EAT);
    m_clSoundPlayer.AddAudioFile(AUDIO_LOSS);
    m_clSoundPlayer.AddAudioFile(AUDIO_WIN);
    m_clSoundPlayer.AddAudioFile(AUDIO_MOVE);
    m_clSoundPlayer.AddAudioFile(AUDIO_TIE);
    m_clSoundPlayer.AddAudioFile(AUDIO_ILLEGAL);

    m_nCurrentLeftStepOrder = m_vecLeftMoveHistory.size();
    m_nCurrentRightStepOrder = m_vecRightMoveHistory.size();
    UpdateMoveHistoryDisplay(false);
    UpdateMoveHistoryDisplay(true);
}

void CGameView::Render()
{
    multimap<int, CDXWidget*, greater<int> >::iterator it;
    for (it = m_mapWidget.begin(); it != m_mapWidget.end(); ++it)
    {
        (it->second)->Render();
    }

    __int64 llCurrentTime = ::timeGetTime();
    int nTimeElapsed = max((llCurrentTime - g_GameHandle.GetCurrentStepStartTime()) / 1000, 0);
    int nTimeLeft = max(g_GameSettings.m_nStepTime - nTimeElapsed, 0);
    char szStepTimeLeft[10];
    char szText[32];
    if (g_GameHandle.GetCurrentStepStartTime() > 0 && g_GameSettings.m_nStepTime > 0)
    {
        ConvertToTimeStr(nTimeLeft, szStepTimeLeft);
        sprintf(szText, "%s%s", g_GameHandle.GetCurrentTurn() == BLACK ? "黑方走棋" : "红方走棋", szStepTimeLeft);
        m_pStepTimeLeft->SetText(szText);

        if (!m_bStepTimeOverNotify && nTimeLeft == 0)
        {
            g_GameHandle.StepTimeOver();
            m_bStepTimeOverNotify = true;
        }
    }
}

void CGameView::ProcessEvent( CSubject *pSub, int nEvent, void *pParam)
{
    if (pSub == NULL && nEvent < 0)
    {
        return;
    }

    switch(nEvent)
    {
    case EVENT_INIT_BOARD:
        {
            ProcessInitBoardEvent(pSub);
        }
        break;
    case EVENT_CHANGE_POS:
        {
            ProcessChangePosEvent(pSub);
        }
        break;
    case EVENT_NEW_GAME:
        {
            ProcessNewGameEvent(pSub);
        }
        break;
    case EVENT_UPDATE_MOVE:
        {
            ProcessUpdateMoveRouteEvent(pSub);
        }
        break;
    case EVENT_FALLBACK:
        {
            ProcessFallbackEvent(pSub);
        }
        break;
    case EVENT_LOAD_CHESSMAN:
        {
            ProcessLoadChessManEvent(pSub);
        }
        break;
    case EVENT_GAME_RESULT:
        {
            ProcessGameResultEvent(pSub);
        }
        break;
    case EVENT_ILLEGAL_MOVE:
        {
            ProcessIllegalMoveEvent(pSub);
        }
        break;
    case EVENT_REQ_FALLBACK:
    case EVENT_REQ_TIE:
    case EVENT_REQ_LOSE:
        {
            ProcessTipEvent(pSub, nEvent, pParam);
        }
        break;
    case EVENT_REQ_FALLBACK_REPLY:
    case EVENT_REQ_TIE_REPLY:
    case EVENT_REQ_LOSE_REPLY:
        {
            ProcessTipReplyEvent(pSub, nEvent, pParam);
        }
        break;
    default:
        break;
    }
}

void CGameView::ProcessInitBoardEvent( CSubject *pSub )
{
    CGameHandle *pGameHandle = (CGameHandle *)pSub;
    int arrChessMan[CHESSBOARD_ROW][CHESSBOARD_COLUMN];
    pGameHandle->GetChessMan(arrChessMan);
    UpdateChessMan(arrChessMan);
}

void CGameView::ProcessNewGameEvent( CSubject *pSub )
{
    CGameHandle *pGameHandle = (CGameHandle *)pSub;
    int arrChessMan[CHESSBOARD_ROW][CHESSBOARD_COLUMN];
    pGameHandle->GetChessMan(arrChessMan);
    UpdateChessMan(arrChessMan);

    if (g_GameSettings.m_nStepTime <= 0)
    {
        m_pStepTimeLeft->SetText(pGameHandle->GetCurrentTurn() == BLACK ? "黑方走棋" : "红方走棋");
    }

    m_bGameStarted = true;
    m_bGameOver = false;
    m_pSave->SetCurrState(STATE_ACTIVE);
    m_pFallback->SetCurrState(STATE_ACTIVE);
    m_pTie->SetCurrState(STATE_ACTIVE);
    m_pLose->SetCurrState(STATE_ACTIVE);
    m_pSettings->SetCurrState(STATE_DISABLE);
}

void CGameView::ProcessIllegalMoveEvent( CSubject *pSub )
{
    m_clSoundPlayer.Play(AUDIO_ILLEGAL);
}

void CGameView::UpdateChessMan( int arrChessMan[CHESSBOARD_ROW][CHESSBOARD_COLUMN] )
{
    char strChessManPicture[40];
    for (int i = 0; i < CHESSBOARD_ROW; i++)
    {
        for (int j = 0; j < CHESSBOARD_COLUMN; j++)
        {
            if (arrChessMan[i][j] > 0)
            {
                GetChessManPicture(strChessManPicture, arrChessMan[i][j], false);
                m_pszGamingChessMan[i][j]->SetTexture(strChessManPicture);
                m_pszGamingChessMan[i][j]->SetVisible(true);
            }
            else
            {
                m_pszGamingChessMan[i][j]->SetTexture(LPDIRECT3DTEXTURE9(NULL));
                m_pszGamingChessMan[i][j]->SetVisible(false);
            }
        }
    }

    m_pFallback->SetCurrState(g_GameHandle.GetLstMoveRoute().size() > 0 ? STATE_ACTIVE : STATE_DISABLE);
}

void CGameView::MsgResponse( HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam )
{
    CDXWidget *pResponseWidget = GetResponseMsgWidget();
    if (pResponseWidget)
    {
        pResponseWidget->MsgResponse(hWnd, uMessage, wParam, lParam);

        if (pResponseWidget != m_pCurrFocusWidget)
        {
            if (m_pCurrFocusWidget)
            {
                m_pCurrFocusWidget->HandleFocus(false);
            }
            pResponseWidget->HandleFocus(true);
            m_pCurrFocusWidget = pResponseWidget;
        }

        if (!m_pChessBoardCover->IsVisible() && 
            m_bGameStarted && !m_bGameOver)
        {
            switch(uMessage)
            {
            case WM_LBUTTONUP:
                {
                    HandleLButtonUp();
                }
                break;
            default:
                break;
            }
        }
    }
}

CDXWidget * CGameView::GetResponseMsgWidget()
{
    POINT pt;
    GetCursorPos(&pt);

    multimap<int, CDXWidget*, greater<int> >::reverse_iterator it;
    for (it = m_mapWidget.rbegin(); it != m_mapWidget.rend(); ++it)
    {
        RECT rc = (it->second)->GetPosRect();
        OffsetRect(&rc, WINDOW_START_X, WINDOW_START_Y);
        bool bVisible = (it->second)->IsVisible();

        if (PtInRect(&rc, pt))
        {
            if (!bVisible)
            {
                continue;
            }

            return it->second;
        }
    }

    return NULL;
}

void CGameView::HandleLButtonUp()
{
    POINT pt;
    GetCursorPos(&pt);

    int nRow = -1;
    int nColumn = -1;

    if (!((g_GameSettings.m_nGameType == COMPITITOR_MACHINE || g_GameSettings.m_nGameType == COMPITITOR_NETWORK)&& 
        g_GameHandle.GetCurrentTurn() == g_GameSettings.m_nCompetitorSide))
    {
        if (GetCoordinate (pt, nRow, nColumn, g_GameSettings.m_nCompetitorSide))
        {
            g_GameHandle.DoMove(nRow, nColumn);
        }
    }

}

void CGameView::UpdateMoveRoute( const MoveRoute &stRoute, int arrChessMan[CHESSBOARD_ROW][CHESSBOARD_COLUMN])
{
    char szTextureFile[50];
    if (!IsCompleteMoveRoute(stRoute))
    {
        for (int i = 0; i < CHESSBOARD_ROW; i++)
        {
            for (int j = 0; j < CHESSBOARD_COLUMN; j++)
            {
                if (IsSameSide(stRoute.nMovingChessMan, arrChessMan[i][j]))
                {
                    GetChessManPicture(szTextureFile, arrChessMan[i][j], true);
                    if (strcmp(szTextureFile, m_pszGamingChessMan[i][j]->GetTextureFile()) == 0)
                    {
                        GetChessManPicture(szTextureFile, arrChessMan[i][j], false);
                        m_pszGamingChessMan[i][j]->SetTexture(szTextureFile);
                    }
                }
            }
        }

        GetChessManPicture(szTextureFile, stRoute.nMovingChessMan, true);
        m_pszGamingChessMan[stRoute.stFromPos.nRow][stRoute.stFromPos.nColumn]->SetTexture(szTextureFile);
    }
    else
    {
        int nRow = stRoute.stFromPos.nRow;
        int nColumn = stRoute.stFromPos.nColumn;
        m_pszGamingChessMan[nRow][nColumn]->SetVisible(true);
        m_pszGamingChessMan[nRow][nColumn]->SetTexture(IsBlackSide(stRoute.nMovingChessMan) ? BLACK_INDICATOR : RED_INDICATOR);

        GetChessManPicture(szTextureFile, stRoute.nMovingChessMan, true);
        m_pszGamingChessMan[stRoute.stToPos.nRow][stRoute.stToPos.nColumn]->SetTexture(szTextureFile);
    }
}

void CGameView::ProcessUpdateMoveRouteEvent( CSubject *pSub )
{
    CGameHandle *pGameHandle = (CGameHandle *)pSub;
    MoveRoute stRoute = pGameHandle->GetCurrentMoveRoute();
    int nGameResult = pGameHandle->GetGameResult();

    int arrChessMan[CHESSBOARD_ROW][CHESSBOARD_COLUMN];
    pGameHandle->GetChessMan(arrChessMan);
    //如果移动了完整的一步，则需要先更新整个棋盘
    if (IsCompleteMoveRoute(stRoute))
    {
        UpdateChessMan(arrChessMan);

        //更新MoveHistory
        AddMoveHistory(stRoute);
        m_bStepTimeOverNotify = nGameResult != -1;
    }

    UpdateMoveRoute(stRoute, arrChessMan);
    PlayTipSound(stRoute, nGameResult);
    UpdateGeneralStatus(nGameResult);

    if (g_GameSettings.m_nStepTime <= 0 && nGameResult == -1)
    {
        m_pStepTimeLeft->SetText(pGameHandle->GetCurrentTurn() == BLACK ? "黑方走棋" : "红方走棋");
    }

    m_bGameOver = nGameResult != -1;
    ShowResultView(nGameResult);
}

void CGameView::ProcessGameResultEvent( CSubject *pSub )
{
    CGameHandle *pGameHandle = (CGameHandle *)pSub;

    int nGameResult = pGameHandle->GetGameResult();
    m_bGameOver = nGameResult != -1;
    PlayGameResultSound(nGameResult);
    ShowResultView(nGameResult);
}

void CGameView::ProcessLoadChessManEvent( CSubject *pSub )
{
    CGameHandle *pGameHandle = (CGameHandle *)pSub;
    //     MoveRoute stRoute = pGameHandle->GetCurrentMoveRoute();
    //     list<MoveRoute> lstMoveRoute = pGameHandle->GetLstMoveRoute();
    int nGameResult = pGameHandle->GetGameResult();

    int arrChessMan[CHESSBOARD_ROW][CHESSBOARD_COLUMN];
    pGameHandle->GetChessMan(arrChessMan);
    UpdateChessMan(arrChessMan);
    //     UpdateMoveRoute(stRoute, arrChessMan);
    // 
    //     list<MoveRoute>::iterator it = lstMoveRoute.begin();
    //     for (; it != lstMoveRoute.end(); ++it)
    //     {
    //         AddMoveHistory(*it);
    //     }

    UpdateGeneralStatus(nGameResult);
    m_bGameStarted = true;
    m_bGameOver = nGameResult != -1;
    ShowResultView(nGameResult);
    m_pSave->SetCurrState(STATE_ACTIVE);
    m_nTipResult = 0;
    m_nTipType = 0;
}

void CGameView::ProcessFallbackEvent( CSubject *pSub )
{
    CGameHandle *pGameHandle = (CGameHandle *)pSub;
    int arrChessMan[CHESSBOARD_ROW][CHESSBOARD_COLUMN];
    pGameHandle->GetChessMan(arrChessMan);
    UpdateChessMan(arrChessMan);

    FallbackMoveHistory(false);
    FallbackMoveHistory(true);

    if (g_GameSettings.m_nStepTime <= 0 && pGameHandle->GetGameResult() == -1)
    {
        m_pStepTimeLeft->SetText(pGameHandle->GetCurrentTurn() == BLACK ? "黑方走棋" : "红方走棋");
    }
}

void CGameView::PlayTipSound( const MoveRoute &stRoute, int nGameResult )
{
    if (nGameResult != -1)
    {
        PlayGameResultSound(nGameResult);
    }
    else
    {
        if (IsCompleteMoveRoute(stRoute))
        {
            if (stRoute.bAttackGeneral)
            {
                m_clSoundPlayer.Play(AUDIO_ATTACK_KING);
            }
            else
            {
                if (stRoute.nKilledChessMan == 0)
                {
                    m_clSoundPlayer.Play(AUDIO_MOVE);
                }
                else
                {
                    m_clSoundPlayer.Play(AUDIO_EAT);
                }
            }
        }
        else
        {
            m_clSoundPlayer.Play(AUDIO_CHOOSE);
        }
    }

}

void CGameView::OnStart( void *pParam )
{
    CGameView *pGameView = (CGameView *)pParam;
    pGameView->m_pChessBoardCover->SetVisible(false);
    pGameView->m_pStart->SetCurrState(STATE_DISABLE);
    pGameView->m_pNewGame->SetCurrState(STATE_ACTIVE);
    pGameView->m_pOpen->SetCurrState(STATE_ACTIVE);

    if (g_GameSettings.m_nGameType == COMPITITOR_NETWORK)
    {
        g_GameHandle.OnStart();
        if (g_GameSettings.m_nServerOrClient == SERVER_SIDE)
        {
            pGameView->m_pConnect->SetCurrState(STATE_DISABLE);
        }
    }

}

void CGameView::OnNewGame( void *pParam )
{
    CGameView *pGameView = (CGameView *)pParam;

    g_GameHandle.NewGame();
    pGameView->m_clSoundPlayer.Play(AUDIO_NEW_GAME);

    pGameView->ClearHisotryDisplay();

    pGameView->m_nTipResult = 0;
    pGameView->m_nTipType = 0;

    //判断是否电脑先走，如果是则电脑走棋
    if (g_GameSettings.m_nAhead == g_GameSettings.m_nCompetitorSide &&
        g_GameSettings.m_nGameType == COMPITITOR_MACHINE)
    {
        g_GameHandle.ComputerMove();
    }

    //判断是否是网络对战
    if (g_GameSettings.m_nGameType == COMPITITOR_NETWORK)
    {
        g_GameHandle.SendNewGameMsg();
    }
}

void CGameView::OnOpen( void *pParam )
{
    CGameView *pGameView = (CGameView *)pParam;
    PostMessage(pGameView->m_hWnd, WM_OPENFILE, 0, 0);
}

void CGameView::OnSave( void *pParam )
{
    CGameView *pGameView = (CGameView *)pParam;
    PostMessage(pGameView->m_hWnd, WM_SAVEFILE, 0, 0);
}

void CGameView::OnFallback( void *pParam )
{
    CGameView *pGameView = (CGameView *)pParam;
    if (g_GameSettings.m_nGameType == COMPITITOR_NETWORK)
    {
        g_GameHandle.SendTipMsg(TIP_REQ_FALLBACK);
        pGameView->m_nTipType = TIP_REQ_FALLBACK;
    }
    else
    {
        g_GameHandle.OnFallback();
    }
}

void CGameView::OnTie( void *pParam )
{    
    CGameView *pGameView = (CGameView *)pParam;
    if (g_GameSettings.m_nGameType == COMPITITOR_NETWORK)
    {
        g_GameHandle.SendTipMsg(TIP_REQ_TIE);
        pGameView->m_nTipType = TIP_REQ_TIE;
    }
    else
    {
        g_GameHandle.OnTie();
    }
}

void CGameView::OnLose( void *pParam )
{    
    CGameView *pGameView = (CGameView *)pParam;
    if (g_GameSettings.m_nGameType == COMPITITOR_NETWORK)
    {
        g_GameHandle.SendTipMsg(TIP_REQ_LOSE);
        pGameView->m_nTipType = TIP_REQ_LOSE;
    }
    else
    {
        g_GameHandle.OnLose();
    }
}

void CGameView::OnSettings( void *pParam )
{
    CGameView *pGameView = (CGameView *)pParam;
    PostMessage(pGameView->m_hWnd, WM_SETTINGS, 0, 0);
}

void CGameView::OnConfirm( void *pParam )
{
    CGameView *pGameView = (CGameView *)pParam;
    pGameView->m_pConfirm->SetVisible(false);
    pGameView->m_pMessageBox->SetVisible(false);

    if (pGameView->m_nTipResult == 1)
    {
        switch(pGameView->m_nTipType)
        {
        case TIP_REQ_FALLBACK:
            g_GameHandle.OnFallback();
            break;
        case TIP_REQ_LOSE:
            g_GameHandle.OnLose();
            break;
        case TIP_REQ_TIE:
            g_GameHandle.OnTie();
            break;
        default:
            break;
        }

        pGameView->m_nTipType = 0;
        pGameView->m_nTipResult = 0;
    }
}

void CGameView::OnExit( void *pParam )
{
    g_GameSettings.SaveSettings(SETTINGS_FILE);
    PostQuitMessage(0);
}

void CGameView::Shutdown()
{
    m_clSoundPlayer.Shutdown();
}

void CGameView::UpdateGeneralStatus( int nGameResult )
{
    int nRow = 0;
    int nColumn = 0;

    switch(nGameResult)
    {
    case BLACK:
        {
            if (g_GameHandle.GetDeadOne() == RED &&
                g_GameHandle.GetGeneralPosition(RED_GENERAL, nRow, nColumn) > 0)
            {
                m_pszGamingChessMan[nRow][nColumn]->SetTexture(RED_DEAD_PICTURE);
                m_pszGamingChessMan[nRow][nColumn]->SetVisible(true);
            }
        }
        break;
    case RED:
        {
            if (g_GameHandle.GetDeadOne() == BLACK &&
                g_GameHandle.GetGeneralPosition(BLACK_GENERAL, nRow, nColumn) > 0)
            {
                m_pszGamingChessMan[nRow][nColumn]->SetTexture(BLACK_DEAD_PICTURE);
                m_pszGamingChessMan[nRow][nColumn]->SetVisible(true);
            }
        }
        break;
    default:
        break;
    }
}

void CGameView::ShowResultView( int nGameResult )
{
    string strText;
    switch(nGameResult)
    {
    case TIE:
        strText = "和局";
        break;

    case BLACK:
        strText = "黑方胜";
        break;

    case RED:
        strText = "红方胜";
        break;

    default:
        break;
    }

    if (strText.size())
    {
        m_pMessageBox->SetVisible(true);
        m_pMessageBox->SetText(strText);
        m_pConfirm->SetVisible(true);
        m_pSave->SetCurrState(STATE_DISABLE);
        m_pSettings->SetCurrState(STATE_ACTIVE);
    }
}

void CGameView::ChangeChessManPos()
{
    for (int i = 0; i < CHESSBOARD_ROW; i++)
    {
        for (int j = 0; j < CHESSBOARD_COLUMN; j++)
        {
            RECT Pos = GetChessManInitPos(i, j, g_GameSettings.m_nCompetitorSide);
            m_pszGamingChessMan[i][j]->SetPosRect(Pos);
        }
    }

    m_pBlackSide->SetPosRect(GetChessManSideInitPos(g_GameSettings.m_nCompetitorSide, BLACK));
    m_pRedSide->SetPosRect(GetChessManSideInitPos(g_GameSettings.m_nCompetitorSide, RED));

    //交换左右两边数据
    int nCurrentOrder = m_nCurrentLeftStepOrder;
    m_nCurrentLeftStepOrder = m_nCurrentRightStepOrder;
    m_nCurrentRightStepOrder = nCurrentOrder;

    vector<ChineseMoveStep> vecMoveHistory = m_vecLeftMoveHistory;
    m_vecLeftMoveHistory = m_vecRightMoveHistory;
    m_vecRightMoveHistory = vecMoveHistory;

    UpdateMoveHistoryDisplay(false);
    UpdateMoveHistoryDisplay(true);
}

void CGameView::UpdateMoveHistory()
{

}

void CGameView::OnPrevPage( void * pParam )
{
    CGameView *pGameView = (CGameView *)pParam;

    pGameView->PrevPage(pGameView->m_pCurrFocusWidget == pGameView->m_pRightPrevPage);
}

void CGameView::OnPrevRecord( void *pParam )
{
    CGameView *pGameView = (CGameView *)pParam;

    pGameView->PrevRecord(pGameView->m_pCurrFocusWidget == pGameView->m_pRightPrevRecord);
}

void CGameView::OnNextRecord( void *pParam )
{
    CGameView *pGameView = (CGameView *)pParam;

    pGameView->NextRecord(pGameView->m_pCurrFocusWidget == pGameView->m_pRightNextRecord);
}

void CGameView::OnNextPage( void *pParam )
{
    CGameView *pGameView = (CGameView *)pParam;

    pGameView->NextPage(pGameView->m_pCurrFocusWidget == pGameView->m_pRightNextPage);
}

void CGameView::UpdateMoveHistoryDisplay(bool bMySide)
{
    int nCurrentPage = GetMoveHistoryCurrentPage(bMySide);
    int nCurrentRecord = GetMoveHistoryCurrentIndex(bMySide);
    int nTotalPage = GetMoveHistoryTotalPage(bMySide);

    vector<ChineseMoveStep> &vecMoveHistory = bMySide ? m_vecRightMoveHistory : m_vecLeftMoveHistory;
    CDXLabel *pLabel = bMySide ? m_pRightPageInfo : m_pLeftPageInfo;
    CDXListCtrl *pListCtrl = bMySide ? m_pRightMoveHistory : m_pLeftMoveHistory;

    //先清除文本
    for (int i = 0; i < MOVE_STEP_PER_PAGE; i++)
    {
        pListCtrl->GetListItem(i).ClearText(0);
        pListCtrl->GetListItem(i).ClearText(1);
        pListCtrl->GetListItem(i).ClearText(2);
    }

    CDXButton *pPrevPage = bMySide ? m_pRightPrevPage : m_pLeftPrevPage;
    CDXButton *pPrevRecord = bMySide ? m_pRightPrevRecord : m_pLeftPrevRecord;
    CDXButton *pNextPage = bMySide ? m_pRightNextPage: m_pLeftNextPage;
    CDXButton *pNextRecord = bMySide ? m_pRightNextRecord : m_pLeftNextRecord;
    if (vecMoveHistory.size() == 0)
    {
        pPrevRecord->SetCurrState(STATE_DISABLE);
        pPrevPage->SetCurrState(STATE_DISABLE);
        pNextRecord->SetCurrState(STATE_DISABLE);
        pNextPage->SetCurrState(STATE_DISABLE);
    }
    else
    {
        int &nCurrentStepOrder = bMySide ? m_nCurrentRightStepOrder : m_nCurrentLeftStepOrder;
        int nStartIndex = (nCurrentPage - 1) * MOVE_STEP_PER_PAGE;
        int nEndIndex = min(nCurrentPage * MOVE_STEP_PER_PAGE, vecMoveHistory.size()) - 1;

        for (int i = nStartIndex; i <= nEndIndex; i++)
        {
            pListCtrl->GetListItem(i % 10).SetNumber(vecMoveHistory.at(i).nOrderNumber, 0);
            pListCtrl->GetListItem(i % 10).SetText(vecMoveHistory.at(i).szMoveStepInfo, 1);
            pListCtrl->GetListItem(i % 10).SetText(vecMoveHistory.at(i).szMoveStepTime, 2);
            pListCtrl->GetListItem(i % 10).SetFontColor(255, 0, 0, 0);
        }

        pListCtrl->GetListItem(nCurrentRecord - 1).SetFontColor(255, 255, 0, 0);

        pPrevRecord->SetCurrState(nCurrentStepOrder == 1 ? STATE_DISABLE : STATE_ACTIVE);
        pPrevPage->SetCurrState(nCurrentPage == 1 ? STATE_DISABLE : STATE_ACTIVE);
        pNextRecord->SetCurrState(nCurrentStepOrder == vecMoveHistory.size() ? STATE_DISABLE : STATE_ACTIVE);
        pNextPage->SetCurrState(nCurrentPage == nTotalPage ? STATE_DISABLE : STATE_ACTIVE);
    }

    char szPageInfo[32];
    sprintf(szPageInfo, "%d/%d", nCurrentPage, nTotalPage);
    pLabel->SetText(szPageInfo);
}

int CGameView::GetMoveHistoryTotalPage(bool bMySide)
{
    int nTotalPage = 0;
    vector<ChineseMoveStep> &vecMoveHistory = bMySide ? m_vecRightMoveHistory : m_vecLeftMoveHistory;

    if (vecMoveHistory.size() % MOVE_STEP_PER_PAGE == 0)
    {
        nTotalPage = vecMoveHistory.size() / MOVE_STEP_PER_PAGE;
    }
    else
    {
        nTotalPage = vecMoveHistory.size() / MOVE_STEP_PER_PAGE + 1;
    }

    return nTotalPage;
}

int CGameView::GetMoveHistoryCurrentPage(bool bMySide)
{
    int nCurrentPage = 0;
    int nCurrentStepOrderNumber = bMySide ? m_nCurrentRightStepOrder : m_nCurrentLeftStepOrder;

    if (nCurrentStepOrderNumber % MOVE_STEP_PER_PAGE == 0)
    {
        nCurrentPage = nCurrentStepOrderNumber / MOVE_STEP_PER_PAGE;
    }
    else
    {
        nCurrentPage = nCurrentStepOrderNumber / MOVE_STEP_PER_PAGE + 1;
    }
    return nCurrentPage;
}

int CGameView::GetMoveHistoryCurrentIndex(bool bMySide)
{
    int nCurrentRecordIndex = 0;    //从1开始
    int nCurrentStepOrderNumber = bMySide ? m_nCurrentRightStepOrder : m_nCurrentLeftStepOrder;

    if (nCurrentStepOrderNumber % MOVE_STEP_PER_PAGE == 0)
    {
        nCurrentRecordIndex = MOVE_STEP_PER_PAGE;
    }
    else
    {
        nCurrentRecordIndex = nCurrentStepOrderNumber % MOVE_STEP_PER_PAGE;
    }

    return nCurrentRecordIndex;
}

void CGameView::NextRecord( bool bMySide )
{
    int &nCurrentStepOrderNumber = bMySide ? m_nCurrentRightStepOrder : m_nCurrentLeftStepOrder;
    vector<ChineseMoveStep> &vecMoveHistory = bMySide ? m_vecRightMoveHistory : m_vecLeftMoveHistory;

    if (nCurrentStepOrderNumber == vecMoveHistory.size())
    {
        return;
    }

    nCurrentStepOrderNumber++;
    UpdateMoveHistoryDisplay(bMySide);
}

void CGameView::PrevRecord( bool bMySide )
{
    int &nCurrentStepOrderNumber = bMySide ? m_nCurrentRightStepOrder : m_nCurrentLeftStepOrder;
    if (nCurrentStepOrderNumber == 1)
    {
        return;
    }

    nCurrentStepOrderNumber--;
    UpdateMoveHistoryDisplay(bMySide);
}

void CGameView::NextPage( bool bMySide )
{
    if (GetMoveHistoryCurrentPage(bMySide) == GetMoveHistoryTotalPage(bMySide))
    {
        return;
    }

    int &nCurrentStepOrderNumber = bMySide ? m_nCurrentRightStepOrder : m_nCurrentLeftStepOrder;
    vector<ChineseMoveStep> &vecMoveHistory = bMySide ? m_vecRightMoveHistory : m_vecLeftMoveHistory;
    nCurrentStepOrderNumber = min(nCurrentStepOrderNumber + MOVE_STEP_PER_PAGE, vecMoveHistory.size());

    UpdateMoveHistoryDisplay(bMySide);
}

void CGameView::PrevPage( bool bMySide )
{
    if (GetMoveHistoryCurrentPage(bMySide) == 1)
    {
        return;
    }

    int &nCurrentStepOrderNumber = bMySide ? m_nCurrentRightStepOrder : m_nCurrentLeftStepOrder;
    nCurrentStepOrderNumber = max(nCurrentStepOrderNumber - MOVE_STEP_PER_PAGE, 1);

    UpdateMoveHistoryDisplay(bMySide);
}

void CGameView::AddMoveHistory( const MoveRoute &stRoute )
{
    ChineseMoveStep stMoveStep;
    bool bBlack = IsBlackSide(stRoute.nMovingChessMan);
    bool bMySide = false;
    if ((g_GameSettings.m_nCompetitorSide == RED && bBlack) ||
        (g_GameSettings.m_nCompetitorSide == BLACK && !bBlack))
    {
        bMySide = true;
    }
    else
    {
        bMySide =false;
    }

    vector<ChineseMoveStep> &vecMoveStep = bMySide ? m_vecRightMoveHistory : m_vecLeftMoveHistory;
    stMoveStep.nOrderNumber = vecMoveStep.size() + 1;
    CMoveRouteGenerator::AlphaFmtToChiness(stRoute.szMoveStepAlpha, stMoveStep.szMoveStepInfo, bBlack);
    CurrentTimeToStr(stMoveStep.szMoveStepTime);
    vecMoveStep.push_back(stMoveStep);

    int &nCurrentStepOrder = bMySide ? m_nCurrentRightStepOrder : m_nCurrentLeftStepOrder;
    nCurrentStepOrder = vecMoveStep.size();
    UpdateMoveHistoryDisplay(bMySide);
}

void CGameView::FallbackMoveHistory( bool bMySide )
{
    vector<ChineseMoveStep> &vecMoveStep = bMySide ? m_vecRightMoveHistory : m_vecLeftMoveHistory;

    if (vecMoveStep.size() > 0)
    {
        vecMoveStep.pop_back();

        int &nCurrentStepOrder = bMySide ? m_nCurrentRightStepOrder : m_nCurrentLeftStepOrder;

        nCurrentStepOrder = vecMoveStep.size();
        UpdateMoveHistoryDisplay(bMySide);
    }
}

void CGameView::ClearHisotryDisplay()
{
    m_vecRightMoveHistory.clear();
    m_vecLeftMoveHistory.clear();
    m_nCurrentRightStepOrder = 0;
    m_nCurrentLeftStepOrder = 0;
    UpdateMoveHistoryDisplay(true);
    UpdateMoveHistoryDisplay(false);
}

void CGameView::PlayGameResultSound( int nGameResult )
{
    //对手胜
    if (nGameResult != TIE)
    {
        if ((nGameResult == RED && g_GameSettings.m_nCompetitorSide == RED) ||
            (nGameResult == BLACK && g_GameSettings.m_nCompetitorSide == BLACK) )
        {
            m_clSoundPlayer.Play(AUDIO_LOSS);
        }
        else
        {
            m_clSoundPlayer.Play(AUDIO_WIN);
        }

    }
    else
    {
        m_clSoundPlayer.Play(AUDIO_TIE);
    }
}

void CGameView::OnConnect( void *pParam )
{

}

void CGameView::OnDisconnect( void *pParam )
{

}

void CGameView::ProcessTipEvent( CSubject *pSub, int nEvent, void *pParam )
{
    switch(nEvent)
    {
    case EVENT_REQ_FALLBACK:
        m_nTipType = TIP_REQ_FALLBACK;
        break;
    case EVENT_REQ_TIE:
        m_nTipType = TIP_REQ_TIE;
        break;
    case EVENT_REQ_LOSE:
        m_nTipType = TIP_REQ_LOSE;
        break;
    default:
        break;
    }

    //弹出提示框
    ShowTipView(m_nTipType);
}

void CGameView::ProcessTipReplyEvent( CSubject *pSub, int nEvent, void *pParam )
{
    MsgTipReply *pMsgTipReply = (MsgTipReply *)pParam;
    m_nTipResult = pMsgTipReply->nResult;
    switch(nEvent)
    {
    case EVENT_REQ_FALLBACK_REPLY:
        {
            m_nTipType = TIP_REQ_FALLBACK;
        }
        break;
    case EVENT_REQ_TIE_REPLY:
        {
            m_nTipType = TIP_REQ_TIE;
        }
        break;
    case EVENT_REQ_LOSE_REPLY:
        {
            m_nTipType = TIP_REQ_LOSE;
        }
        break;
    default:
        break;
    }

    //弹出提示框
    ShowTipReplyView(m_nTipType, m_nTipResult);
}

void CGameView::ShowTipView( int nTipType )
{
    string strTipMsg;
    switch(nTipType)
    {
    case EVENT_REQ_FALLBACK:
        strTipMsg = "对方请求\n悔棋，是否同意？";
        break;
    case EVENT_REQ_TIE:
        strTipMsg = "对方请求\n和棋，是否同意？";
        break;
    case EVENT_REQ_LOSE:
        strTipMsg = "对方请求\n认输，是否同意？";
        break;
    default:
        break;
    }

    if (strTipMsg.size())
    {
        m_pMessageBox->SetVisible(true);
        m_pMessageBox->SetText(strTipMsg);
        m_pAgree->SetVisible(true);
        m_pDisagree->SetVisible(true);
    }
}

void CGameView::ShowTipReplyView( int nTipType, int nResult )
{
    string strTipMsg;
    switch(nTipType)
    {
    case EVENT_REQ_FALLBACK:
        {
            strTipMsg = nResult == 1 ? "对方同意悔棋。" : "对方不同意悔棋。";
        }
        break;
    case EVENT_REQ_TIE:
        {
            strTipMsg = nResult == 1 ? "对方同意和棋。" : "对方不同意和棋。";
        }
        break;
    case EVENT_REQ_LOSE:
        {
            strTipMsg = nResult == 1 ? "对方同意认输。" : "对方不同意认输。";
        }
        break;
    default:
        break;
    }

    if (strTipMsg.size())
    {
        m_pMessageBox->SetVisible(true);
        m_pMessageBox->SetText(strTipMsg);
        m_pConfirm->SetVisible(true);
    }
}

void CGameView::ProcessChangePosEvent( CSubject *pSub )
{
    CGameHandle *pGameHandle = (CGameHandle *)pSub;
    int arrChessMan[CHESSBOARD_ROW][CHESSBOARD_COLUMN];
    pGameHandle->GetChessMan(arrChessMan);
    UpdateChessMan(arrChessMan);
    ChangeChessManPos();
}

void CGameView::OnAgreeOrDisagree( void *pParam )
{
    CGameView *pGameView = (CGameView *)pParam;
    pGameView->m_pAgree->SetVisible(false);
    pGameView->m_pDisagree->SetVisible(false);
    pGameView->m_pMessageBox->SetVisible(false);

    int nResult = 0;
    if (pGameView->m_pCurrFocusWidget == pGameView->m_pAgree)
    {
        nResult = 1;
    }
    else
    {
        nResult = 0;
    }
    
    g_GameHandle.SendTipReplyMsg(pGameView->m_nTipType, nResult);


    if (nResult == 1)
    {
        switch(pGameView->m_nTipType)
        {
        case TIP_REQ_FALLBACK:
            g_GameHandle.OnFallback();
            break;
        case TIP_REQ_LOSE:
            g_GameHandle.OnWin();
            break;
        case TIP_REQ_TIE:
            g_GameHandle.OnTie();
            break;
        default:
            break;
        }
    }
}