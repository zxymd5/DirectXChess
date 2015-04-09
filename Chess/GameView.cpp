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
    m_pHistoryLeft = NULL;
    m_pHistoryRight = NULL;

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

    m_bGameStarted = false;
    m_bGamePaused = false;
    m_bGameOver = false;
}

CGameView::~CGameView(void)
{
}

void CGameView::Init(HWND hWnd)
{
    m_hWnd = hWnd;

    m_pChessBackground = (CDXImage *)g_GameEngine.GetWidgetByName("Background");
    m_mapWidget.insert(make_pair(m_pChessBackground->GetDepth(), m_pChessBackground));

    m_pHistoryLeft = (CDXImage *)g_GameEngine.GetWidgetByName("HistoryLeft");
    m_mapWidget.insert(make_pair(m_pHistoryLeft->GetDepth(), m_pHistoryLeft));

    m_pHistoryRight = (CDXImage *)g_GameEngine.GetWidgetByName("HistoryRight");
    m_mapWidget.insert(make_pair(m_pHistoryRight->GetDepth(), m_pHistoryRight));

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

    m_pConfirm = (CDXButton *)g_GameEngine.GetWidgetByName("Confirm");
    m_pConfirm->SetCallBackInfo(OnConfirm, this);
    m_mapWidget.insert(make_pair(m_pConfirm->GetDepth(), m_pConfirm));

    char szChessManName[100];
    for (int i = 0; i < s_nChessBoardRow; i++)
    {
        for (int j = 0; j < s_nChessBoardColumn; j++)
        {
            sprintf(szChessManName, "ChessMan%d_%d", i, j);
            CDXImage *pChessMan = new CDXImage;

            pChessMan->SetWidgetName(szChessManName);
//             RECT Pos = GetChessManInitPos(i, j, g_GameSettings.m_nCompetitorSide);
//             pChessMan->SetPosRect(Pos);
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
}

void CGameView::OnOpen( void *Param )
{
    CGameView *pGameView = (CGameView *)Param;
    pGameView->m_bGameStarted = true;
    pGameView->m_pPauseGame->SetCurrState(STATE_ACTIVE);
    pGameView->m_pSave->SetCurrState(STATE_ACTIVE);
    pGameView->m_pFallback->SetCurrState(STATE_ACTIVE);
    pGameView->m_pTie->SetCurrState(STATE_ACTIVE);
    pGameView->m_pLose->SetCurrState(STATE_ACTIVE);
    pGameView->m_clSoundPlayer.Play(s_pAudioNewGame);
}

void CGameView::OnSave( void *Param )
{

}

void CGameView::OnFallback( void *Param )
{
    g_GameHandle.FallBack(g_GameSettings.m_nCompetitorSide == s_nRedSide ? s_nBlackSide : s_nRedSide);
}

void CGameView::OnTie( void *Param )
{

}

void CGameView::OnLose( void *Param )
{

}

void CGameView::OnSettings( void *Param )
{
    CGameView *GameView = (CGameView *)Param;
    PostMessage(GameView->m_hWnd, WM_SETTINGS, 0, 0);
}

void CGameView::OnConfirm( void *Param )
{
    CGameView *GameView = (CGameView *)Param;
    GameView->m_pConfirm->SetVisible(false);
    GameView->m_pMessageBox->SetVisible(false);
}

void CGameView::OnExit( void *Param )
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
}
