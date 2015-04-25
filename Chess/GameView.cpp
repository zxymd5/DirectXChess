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
 *  File:		GameView.cpp
 *  Author:		Richard Zou
 *	Created on:	2015-03-01
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

    for (int i = 0; i < s_nChessBoardRow; i++)
    {
        for (int j = 0; j < s_nChessBoardColumn; j++)
        {
            m_pszGamingChessMan[i][j] = NULL;
        }
    }

    m_pStart = NULL;
    m_pPauseGame = NULL;
    m_pContinue = NULL;
    m_pNewGame = NULL;
    m_pOpen = NULL;
    m_pSave = NULL;
    m_pFallback = NULL;
    m_pTie = NULL;
    m_pLose = NULL;
    m_pSettings = NULL;
    m_pExit = NULL;
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

    m_bGameStarted = false;
    m_bGamePaused = false;
    m_bGameOver = false;
    m_nCurrentLeftStepOrder = 0;
    m_nCurrentRightStepOrder = 0;
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

    m_pPauseGame = (CDXButton *)g_GameEngine.GetWidgetByName("PauseGame");
    m_pPauseGame->SetCallBackInfo(OnPauseGame, this);
    m_pPauseGame->SetCurrState(STATE_DISABLE);
    m_mapWidget.insert(make_pair(m_pPauseGame->GetDepth(), m_pPauseGame));

    m_pContinue = (CDXButton *)g_GameEngine.GetWidgetByName("Continue");
    m_pContinue->SetCallBackInfo(OnContinue, this);
    m_pContinue->SetCurrState(STATE_DISABLE);
    m_mapWidget.insert(make_pair(m_pContinue->GetDepth(), m_pContinue));

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

    char szChessManName[100];
    for (int i = 0; i < s_nChessBoardRow; i++)
    {
        for (int j = 0; j < s_nChessBoardColumn; j++)
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
    m_clSoundPlayer.AddAudioFile(s_pAudioNewGame);
    m_clSoundPlayer.AddAudioFile(s_pAudioAttackKing);
    m_clSoundPlayer.AddAudioFile(s_pAudioChoose);
    m_clSoundPlayer.AddAudioFile(s_pAudioEat);
    m_clSoundPlayer.AddAudioFile(s_pAudioLoss);
    m_clSoundPlayer.AddAudioFile(s_pAudioWin);
    m_clSoundPlayer.AddAudioFile(s_pAudioMove);

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
}

void CGameView::ProcessEvent( CSubject *pSub, int nEvent )
{
    if (pSub == NULL && nEvent < 0)
    {
        return;
    }

    switch(nEvent)
    {
    case s_nEventUpdateChessMan:
        {
            ProcessUpdateChessManEvent(pSub);
        }
        break;
    case s_nEventUpdateCurrentChessMan:
        {
            ProcessUpdateMoveRouteEvent(pSub);
        }
        break;
    case s_nEventFallback:
        {
            ProcessFallbackEvent(pSub);
        }
        break;
    default:
        break;
    }
}

void CGameView::ProcessUpdateChessManEvent( CSubject *pSub )
{
    CGameHandle *pGameHandle = (CGameHandle *)pSub;
    int szChessMan[s_nChessBoardRow][s_nChessBoardColumn];
    pGameHandle->GetChessMan(szChessMan);
    UpdateChessMan(szChessMan);
}

void CGameView::UpdateChessMan( int szChessMan[s_nChessBoardRow][s_nChessBoardColumn] )
{
    char strChessManPicture[40];
    for (int i = 0; i < s_nChessBoardRow; i++)
    {
        for (int j = 0; j < s_nChessBoardColumn; j++)
        {
            if (szChessMan[i][j] > 0)
            {
                GetChessManPicture(strChessManPicture, szChessMan[i][j], false);
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
            m_bGameStarted && !m_bGamePaused &&
            !m_bGameOver)
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
        RECT stRt = (it->second)->GetPosRect();
        OffsetRect(&stRt, s_nWindowStartX, s_nWindowStartY);
        bool bVisible = (it->second)->IsVisible();

        if (PtInRect(&stRt, pt))
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

    if (!(g_GameSettings.m_nCompetitor == s_nCompititorMachine && 
        g_GameHandle.GetCurrentTurn() == g_GameSettings.m_nCompetitorSide))
    {
        if (GetCoordinate (pt, nRow, nColumn, g_GameSettings.m_nCompetitorSide))
        {
            g_GameHandle.SetCurrentMoveRoute(nRow, nColumn);
        }
    }

}

void CGameView::UpdateMoveRoute( const MoveRoute &stRoute, int szChessMan[s_nChessBoardRow][s_nChessBoardColumn])
{
    char szTextureFile[50];
    if (!IsCompleteMoveRoute(stRoute))
    {
        for (int i = 0; i < s_nChessBoardRow; i++)
        {
            for (int j = 0; j < s_nChessBoardColumn; j++)
            {
                if (IsSameSide(stRoute.nMovingChessMan, szChessMan[i][j]))
                {
                    GetChessManPicture(szTextureFile, szChessMan[i][j], true);
                    if (strcmp(szTextureFile, m_pszGamingChessMan[i][j]->GetTextureFile()) == 0)
                    {
                        GetChessManPicture(szTextureFile, szChessMan[i][j], false);
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
        m_pszGamingChessMan[nRow][nColumn]->SetTexture(IsBlackSide(stRoute.nMovingChessMan) ? s_pBlackIndicator : s_pRedIndicator);

        GetChessManPicture(szTextureFile, stRoute.nMovingChessMan, true);
        m_pszGamingChessMan[stRoute.stToPos.nRow][stRoute.stToPos.nColumn]->SetTexture(szTextureFile);

        //更新MoveHistory
        AddMoveHistory(stRoute);
    }
}

void CGameView::ProcessUpdateMoveRouteEvent( CSubject *pSub )
{
    CGameHandle *pGameHandle = (CGameHandle *)pSub;
    MoveRoute stRoute = pGameHandle->GetCurrentMoveRoute();
    int nGameResult = pGameHandle->GetGameResult();

    int szChessMan[s_nChessBoardRow][s_nChessBoardColumn];
    pGameHandle->GetChessMan(szChessMan);
    //如果移动了完整的一步，则需要先更新整个棋盘
    if (IsCompleteMoveRoute(stRoute))
    {
        UpdateChessMan(szChessMan);
    }

    UpdateMoveRoute(stRoute, szChessMan);
    PlayTipSound(stRoute, nGameResult);
    UpdateGeneralStatus(nGameResult);
    
    m_bGameOver = nGameResult != -1;
    ShowResultView(nGameResult);
}

void CGameView::ProcessFallbackEvent( CSubject *pSub )
{
    CGameHandle *pGameHandle = (CGameHandle *)pSub;
    int szChessMan[s_nChessBoardRow][s_nChessBoardColumn];
    pGameHandle->GetChessMan(szChessMan);
    UpdateChessMan(szChessMan);

    FallbackMoveHistory(false);
    FallbackMoveHistory(true);
}

void CGameView::PlayTipSound( const MoveRoute &stRoute, int nGameResult )
{
    if (nGameResult != -1)
    {
        //对手胜
        if ((nGameResult == s_nResultRedWin && g_GameSettings.m_nCompetitorSide == s_nRedSide) ||
            (nGameResult == s_nResultBlackWin && g_GameSettings.m_nCompetitorSide == s_nBlackSide) )
        {
            m_clSoundPlayer.Play(s_pAudioLoss);
        }
        else
        {
            m_clSoundPlayer.Play(s_pAudioWin);
        }
    }
    else
    {
        if (IsCompleteMoveRoute(stRoute))
        {
            if (stRoute.bAttackGeneral)
            {
                m_clSoundPlayer.Play(s_pAudioAttackKing);
            }
            else
            {
                if (stRoute.nKilledChessMan == 0)
                {
                    m_clSoundPlayer.Play(s_pAudioMove);
                }
                else
                {
                    m_clSoundPlayer.Play(s_pAudioEat);
                }
            }
        }
        else
        {
            m_clSoundPlayer.Play(s_pAudioChoose);
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
}

void CGameView::OnPauseGame( void *pParam )
{
    CGameView *pGameView = (CGameView *)pParam;
    pGameView->m_bGamePaused = true;
    pGameView->m_pPauseGame->SetCurrState(STATE_DISABLE);
    pGameView->m_pContinue->SetCurrState(STATE_ACTIVE);
}

void CGameView::OnContinue( void *pParam )
{
    CGameView *pGameView = (CGameView *)pParam;
    pGameView->m_bGamePaused = false;
    pGameView->m_pPauseGame->SetCurrState(STATE_ACTIVE);
    pGameView->m_pContinue->SetCurrState(STATE_DISABLE);
}

void CGameView::OnNewGame( void *pParam )
{
    CGameView *pGameView = (CGameView *)pParam;
    pGameView->m_bGameStarted = true;
    pGameView->m_bGameOver = false;
    pGameView->m_pPauseGame->SetCurrState(STATE_ACTIVE);
    pGameView->m_pSave->SetCurrState(STATE_ACTIVE);
    pGameView->m_pFallback->SetCurrState(STATE_ACTIVE);
    pGameView->m_pTie->SetCurrState(STATE_ACTIVE);
    pGameView->m_pLose->SetCurrState(STATE_ACTIVE);
    pGameView->m_pSettings->SetCurrState(STATE_DISABLE);

    g_GameHandle.Init();
    pGameView->m_clSoundPlayer.Play(s_pAudioNewGame);

    pGameView->ClearHisotryDisplay();
}

void CGameView::OnOpen( void *pParam )
{
    CGameView *pGameView = (CGameView *)pParam;
    pGameView->m_bGameStarted = true;
    pGameView->m_pPauseGame->SetCurrState(STATE_ACTIVE);
    pGameView->m_pSave->SetCurrState(STATE_ACTIVE);
    pGameView->m_pFallback->SetCurrState(STATE_ACTIVE);
    pGameView->m_pTie->SetCurrState(STATE_ACTIVE);
    pGameView->m_pLose->SetCurrState(STATE_ACTIVE);
    pGameView->m_clSoundPlayer.Play(s_pAudioNewGame);
}

void CGameView::OnSave( void *pParam )
{

}

void CGameView::OnFallback( void *pParam )
{
    g_GameHandle.FallBack();
}

void CGameView::OnTie( void *pParam )
{

}

void CGameView::OnLose( void *pParam )
{

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
}

void CGameView::OnExit( void *pParam )
{
    g_GameSettings.SaveSettings(s_pSettingsFile);
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
    case s_nResultBlackWin:
        {
            if (g_GameHandle.GetDeadOne() == s_nRedDead &&
                g_GameHandle.GetGeneralPosition(s_nRedGeneral, nRow, nColumn) > 0)
            {
                m_pszGamingChessMan[nRow][nColumn]->SetTexture(s_pRedDeadPicture);
                m_pszGamingChessMan[nRow][nColumn]->SetVisible(true);
            }
        }
        break;
    case s_nResultRedWin:
        {
            if (g_GameHandle.GetDeadOne() == s_nBlackDead &&
                g_GameHandle.GetGeneralPosition(s_nBlackGeneral, nRow, nColumn) > 0)
            {
                m_pszGamingChessMan[nRow][nColumn]->SetTexture(s_pBlackDeadPicture);
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
    case s_nResultTie:
        strText = "和局";
        break;

    case s_nResultBlackWin:
        strText = "黑方胜";
        break;

    case s_nResultRedWin:
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
    }

    m_pSettings->SetCurrState(STATE_ACTIVE);
}

void CGameView::ChangeChessManPos()
{
    for (int i = 0; i < s_nChessBoardRow; i++)
    {
        for (int j = 0; j < s_nChessBoardColumn; j++)
        {
            RECT Pos = GetChessManInitPos(i, j, g_GameSettings.m_nCompetitorSide);
            m_pszGamingChessMan[i][j]->SetPosRect(Pos);
        }
    }

    m_pBlackSide->SetPosRect(GetChessManSideInitPos(g_GameSettings.m_nCompetitorSide, s_nBlackSide));
    m_pRedSide->SetPosRect(GetChessManSideInitPos(g_GameSettings.m_nCompetitorSide, s_nRedSide));

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
    for (int i = 0; i < s_nMoveStepPerPage; i++)
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
        int nStartIndex = (nCurrentPage - 1) * s_nMoveStepPerPage;
        int nEndIndex = min(nCurrentPage * s_nMoveStepPerPage, vecMoveHistory.size()) - 1;

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

    char strPageInfo[32];
    sprintf(strPageInfo, "%d/%d", nCurrentPage, nTotalPage);
    pLabel->SetText(strPageInfo);
}

int CGameView::GetMoveHistoryTotalPage(bool bMySide)
{
    int nTotalPage = 0;
    vector<ChineseMoveStep> &vecMoveHistory = bMySide ? m_vecRightMoveHistory : m_vecLeftMoveHistory;

    if (vecMoveHistory.size() % s_nMoveStepPerPage == 0)
    {
        nTotalPage = vecMoveHistory.size() / s_nMoveStepPerPage;
    }
    else
    {
        nTotalPage = vecMoveHistory.size() / s_nMoveStepPerPage + 1;
    }

    return nTotalPage;
}

int CGameView::GetMoveHistoryCurrentPage(bool bMySide)
{
    int nCurrentPage = 0;
    int nCurrentStepOrderNumber = bMySide ? m_nCurrentRightStepOrder : m_nCurrentLeftStepOrder;

    if (nCurrentStepOrderNumber % s_nMoveStepPerPage == 0)
    {
        nCurrentPage = nCurrentStepOrderNumber / s_nMoveStepPerPage;
    }
    else
    {
        nCurrentPage = nCurrentStepOrderNumber / s_nMoveStepPerPage + 1;
    }
    return nCurrentPage;
}

int CGameView::GetMoveHistoryCurrentIndex(bool bMySide)
{
    int nCurrentRecordIndex = 0;    //从1开始
    int nCurrentStepOrderNumber = bMySide ? m_nCurrentRightStepOrder : m_nCurrentLeftStepOrder;
    
    if (nCurrentStepOrderNumber % s_nMoveStepPerPage == 0)
    {
        nCurrentRecordIndex = s_nMoveStepPerPage;
    }
    else
    {
        nCurrentRecordIndex = nCurrentStepOrderNumber % s_nMoveStepPerPage;
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
    nCurrentStepOrderNumber = min(nCurrentStepOrderNumber + s_nMoveStepPerPage, vecMoveHistory.size());

    UpdateMoveHistoryDisplay(bMySide);
}

void CGameView::PrevPage( bool bMySide )
{
    if (GetMoveHistoryCurrentPage(bMySide) == 1)
    {
        return;
    }

    int &nCurrentStepOrderNumber = bMySide ? m_nCurrentRightStepOrder : m_nCurrentLeftStepOrder;
    nCurrentStepOrderNumber = max(nCurrentStepOrderNumber - s_nMoveStepPerPage, 1);

    UpdateMoveHistoryDisplay(bMySide);
}

void CGameView::AddMoveHistory( const MoveRoute &stRoute )
{
    ChineseMoveStep stMoveStep;
    bool bBlack = IsBlackSide(stRoute.nMovingChessMan);
    bool bMySide = false;
    if ((g_GameSettings.m_nCompetitorSide == s_nRedSide && bBlack) ||
        (g_GameSettings.m_nCompetitorSide == s_nBlackSide && !bBlack))
    {
        bMySide = true;
    }
    else
    {
        bMySide =false;
    }
    
    vector<ChineseMoveStep> &vecMoveStep = bMySide ? m_vecRightMoveHistory : m_vecLeftMoveHistory;
    stMoveStep.nOrderNumber = vecMoveStep.size() + 1;
    CMoveRouteGenerator::AlphaFmtToChiness(stRoute.strMoveStepAlpha, stMoveStep.szMoveStepInfo, bBlack);
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

