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
*  File:       GameHandle.cpp
*  Author:     Richard Zou
*  Created on: 2015-03-01
*/

#include "GameHandle.h"
#include "GameSettings.h"
#include "ChessmanValue.h"

#include <assert.h>
#include <fstream>
#include "../Include/sqlite3.h"

CGameHandle g_GameHandle;

CGameHandle::CGameHandle(void)
{
    memset(m_arrChessMan, 0, sizeof(int) * CHESSBOARD_ROW * CHESSBOARD_COLUMN);
    m_nCurrentTurn = -1;
    m_nCurrentSearchMoveTurn = -1;
    m_nWhoIsDead = 0;
    memset(m_szGameInfoSaveFile, 0, MAX_PATH);

    m_hEventSaveGame = INVALID_HANDLE_VALUE;
    m_hEventGameSaved = INVALID_HANDLE_VALUE;
    m_hThreadSaveGame = INVALID_HANDLE_VALUE;
    m_hThreadComputerMove = INVALID_HANDLE_VALUE;
    m_hEventComputerMove = INVALID_HANDLE_VALUE;

    m_llCurrentStepStartTime = 0;
}

CGameHandle::~CGameHandle(void)
{
}

void CGameHandle::Init()
{
    ResetChessManLayout();
    Notify(EVENT_INIT_BOARD);

    m_hEventSaveGame = CreateEvent(NULL, TRUE, FALSE, NULL); 
    m_hEventGameSaved = CreateEvent(NULL, TRUE, TRUE, NULL);  
    m_hThreadSaveGame = (HANDLE)_beginthreadex(NULL, 0, SaveGameFunc, this, 0, NULL);

    if (g_GameSettings.m_nGameType == COMPITITOR_MACHINE)
    {
        m_hEventComputerMove = CreateEvent(NULL, TRUE, FALSE, NULL);
        m_hThreadComputerMove = (HANDLE)_beginthreadex(NULL, 0, ComputerMove, this, 0, NULL);
    }
}

void CGameHandle::OnNewGame()
{
    Reset();

    ResetZobrist();
    ResetMoveRoute(m_stCurrentMoveRoute);
    ResetChessManLayout();
    Notify(EVENT_NEW_GAME);
    m_llCurrentStepStartTime = ::timeGetTime();
}


//置棋子为最初状态
void CGameHandle::ResetChessManLayout()
{
    for (int i = 0; i < CHESSBOARD_ROW; i++)
    {
        for (int j = 0; j < CHESSBOARD_COLUMN; j++)
        {
            if (DEFAULT_CHESSBOARD_LAYOUT[i][j] > 0)
            {
                AddChessMan(DEFAULT_CHESSBOARD_LAYOUT[i][j], i, j);
            }
        }
    }
}

void CGameHandle::GetChessMan( int arrChessMan[][CHESSBOARD_COLUMN] )
{
    memcpy(arrChessMan, m_arrChessMan, sizeof(int) * CHESSBOARD_ROW * CHESSBOARD_COLUMN);
}

//电脑走棋
void CGameHandle::ComputerMove()
{
    SetEvent(m_hEventComputerMove);
}

unsigned int __stdcall CGameHandle::ComputerMove( void *pParam )
{
    CGameHandle *pGameHandle = (CGameHandle *)pParam;

    while(1)
    {
        WaitForSingleObject(pGameHandle->m_hEventComputerMove, INFINITE);
        pGameHandle->MinMaxSearch(MAX_SEARCH_DEPTH, pGameHandle->m_stCurrentMoveRoute);
        pGameHandle->m_clGenerator.GetChessManMoveStepAlpha(pGameHandle->m_arrChessMan, 
            pGameHandle->m_stCurrentMoveRoute.stFromPos.nRow,
            pGameHandle->m_stCurrentMoveRoute.stFromPos.nColumn, 
            pGameHandle->m_stCurrentMoveRoute.stToPos.nRow, 
            pGameHandle->m_stCurrentMoveRoute.stToPos.nColumn,
            pGameHandle->m_stCurrentMoveRoute.szMoveStepAlpha);
        pGameHandle->ApplyCompleteMove();
        ResetEvent(pGameHandle->m_hEventComputerMove);
    }

    return 0;
}

//人走棋
void CGameHandle::DoMove(int nRow, int nColumn)
{
    if ((nRow == -1 || nColumn == -1) || 
        (nRow == m_stCurrentMoveRoute.stFromPos.nRow && 
        nColumn == m_stCurrentMoveRoute.stFromPos.nColumn) ||
        (nRow == m_stCurrentMoveRoute.stToPos.nRow &&
        nColumn == m_stCurrentMoveRoute.stToPos.nColumn) ||
        m_nCurrentTurn == -1)
    {
        return;
    }

    bool bLegal = false;

    if (m_nCurrentTurn == RED)
    {
        bLegal = RedDoMove(nRow, nColumn);
    }
    else
    {
        bLegal = BlackDoMove(nRow, nColumn);
    }

    if (bLegal)
    {
        //如果走了完整的一步
        if (IsCompleteMoveRoute(m_stCurrentMoveRoute))
        {
            ApplyCompleteMove();

            if (m_nGameResult == -1 && g_GameSettings.m_nGameType == COMPITITOR_MACHINE)
            {
                ComputerMove();
            }
        }
        else
        {
            Notify(EVENT_UPDATE_MOVE);

            //发送走棋消息给对方
            if (g_GameSettings.m_nGameType == COMPITITOR_NETWORK)
            {
                SendMoveRouteMsg();
            }
        }
    }
    else
    {
        Notify(EVENT_ILLEGAL_MOVE);
    }
}

bool CGameHandle::BlackDoMove( int nRow, int nColumn )
{
    int nKilledChessMan = m_arrChessMan[nRow][nColumn];
    int nMovingChessMan = m_stCurrentMoveRoute.nMovingChessMan;
    bool bLegal = false;

    if (nMovingChessMan == 0)
    {
        if (IsBlackSide(nKilledChessMan))
        {
            m_stCurrentMoveRoute.nMovingChessMan = nKilledChessMan;
            m_stCurrentMoveRoute.stFromPos.nRow = nRow;
            m_stCurrentMoveRoute.stFromPos.nColumn = nColumn;
            bLegal = true;
        }
    }
    else
    {
        if (IsBlackSide(nKilledChessMan))
        {
            m_stCurrentMoveRoute.nMovingChessMan = nKilledChessMan;
            m_stCurrentMoveRoute.stFromPos.nRow = nRow;
            m_stCurrentMoveRoute.stFromPos.nColumn = nColumn;
            bLegal = true;
        }
        else
        {
            //判断走法是否合理
            if (m_clGenerator.ValidateMoveRoute(m_arrChessMan, m_stCurrentMoveRoute.stFromPos.nRow, 
                m_stCurrentMoveRoute.stFromPos.nColumn, nRow, nColumn))
            {
                //再判断走棋后，自己是否被对方将军，如果自己被对方将军，则走法不合理
                m_arrChessMan[m_stCurrentMoveRoute.stFromPos.nRow][m_stCurrentMoveRoute.stFromPos.nColumn] = 0;
                m_arrChessMan[nRow][nColumn] = m_stCurrentMoveRoute.nMovingChessMan;
                if(m_clGenerator.IsAttackGeneral(m_arrChessMan, BLACK_GENERAL))
                {
                    bLegal = false;
                }
                else
                {
                    //再判断自己是否将对方的军
                    m_stCurrentMoveRoute.bAttackGeneral = m_clGenerator.IsAttackGeneral(m_arrChessMan, RED_GENERAL);
                    bLegal = true;
                }

                m_arrChessMan[m_stCurrentMoveRoute.stFromPos.nRow][m_stCurrentMoveRoute.stFromPos.nColumn] = m_stCurrentMoveRoute.nMovingChessMan;
                m_arrChessMan[nRow][nColumn] = nKilledChessMan;
                if (bLegal)
                {
                    m_stCurrentMoveRoute.nKilledChessMan = nKilledChessMan;
                    m_clGenerator.GetChessManMoveStepAlpha(m_arrChessMan, m_stCurrentMoveRoute.stFromPos.nRow,
                        m_stCurrentMoveRoute.stFromPos.nColumn, nRow, nColumn, 
                        m_stCurrentMoveRoute.szMoveStepAlpha);
                    m_stCurrentMoveRoute.stToPos.nRow = nRow;
                    m_stCurrentMoveRoute.stToPos.nColumn = nColumn;
                }

            }
            else
            {
                bLegal = false;
            }
        }
    }

    return bLegal;
}

bool CGameHandle::RedDoMove( int nRow, int nColumn )
{
    int nKilledChessMan = m_arrChessMan[nRow][nColumn];
    int nMovingChessMan = m_stCurrentMoveRoute.nMovingChessMan;
    bool bLegal = false;

    if (nMovingChessMan == 0)
    {
        if (IsRedSide(nKilledChessMan))
        {
            m_stCurrentMoveRoute.nMovingChessMan = nKilledChessMan;
            m_stCurrentMoveRoute.stFromPos.nRow = nRow;
            m_stCurrentMoveRoute.stFromPos.nColumn = nColumn;
            bLegal = true;
        }
    }
    else
    {
        if (IsRedSide(nKilledChessMan))
        {
            m_stCurrentMoveRoute.nMovingChessMan = nKilledChessMan;
            m_stCurrentMoveRoute.stFromPos.nRow = nRow;
            m_stCurrentMoveRoute.stFromPos.nColumn = nColumn;
            bLegal = true;
        }
        else
        {
            //判断走法是否合理
            if (m_clGenerator.ValidateMoveRoute(m_arrChessMan, m_stCurrentMoveRoute.stFromPos.nRow, 
                m_stCurrentMoveRoute.stFromPos.nColumn, nRow, nColumn))
            {
                //再判断走棋后，自己是否被对方将军，如果自己被对方将军，则走法不合理
                m_arrChessMan[m_stCurrentMoveRoute.stFromPos.nRow][m_stCurrentMoveRoute.stFromPos.nColumn] = 0;
                m_arrChessMan[nRow][nColumn] = m_stCurrentMoveRoute.nMovingChessMan;
                if(m_clGenerator.IsAttackGeneral(m_arrChessMan, RED_GENERAL))
                {
                    bLegal = false;
                }
                else
                {
                    //再判断自己是否将对方的军
                    m_stCurrentMoveRoute.bAttackGeneral = m_clGenerator.IsAttackGeneral(m_arrChessMan, BLACK_GENERAL);
                    bLegal = true;
                }

                m_arrChessMan[m_stCurrentMoveRoute.stFromPos.nRow][m_stCurrentMoveRoute.stFromPos.nColumn] = m_stCurrentMoveRoute.nMovingChessMan;
                m_arrChessMan[nRow][nColumn] = nKilledChessMan;

                if (bLegal)
                {
                    m_stCurrentMoveRoute.nKilledChessMan = nKilledChessMan;
                    m_clGenerator.GetChessManMoveStepAlpha(m_arrChessMan, m_stCurrentMoveRoute.stFromPos.nRow,
                        m_stCurrentMoveRoute.stFromPos.nColumn, nRow, nColumn, 
                        m_stCurrentMoveRoute.szMoveStepAlpha);
                    m_stCurrentMoveRoute.stToPos.nRow = nRow;
                    m_stCurrentMoveRoute.stToPos.nColumn = nColumn;
                }
            }
            else
            {
                bLegal = false;
            }
        }
    }

    return bLegal;
}


void CGameHandle::ApplyCompleteMove()
{
    DoMakeMove(m_stCurrentMoveRoute);

    //判断是否将对方置于死地
    if (m_stCurrentMoveRoute.bAttackGeneral)
    {
        if (m_clGenerator.IsGeneralDead(m_arrChessMan, m_nCurrentTurn == BLACK ? RED : BLACK))
        {
            m_nWhoIsDead = m_nCurrentTurn == BLACK ? RED : BLACK;
            m_nGameResult = m_nCurrentTurn;
        }
    }

    int nRepStatus = RepStatus(3);
    if (nRepStatus > 0)
    {
        int nRepVal = RepValue(nRepStatus);
        if (nRepVal > WIN_VALUE)
        {
            m_nGameResult = m_nCurrentTurn == BLACK ? RED : BLACK;
        }

        if (nRepVal < -WIN_VALUE)
        {
            m_nGameResult = m_nCurrentTurn;
        }
    }

    //发送走棋消息给对方
    if (g_GameSettings.m_nGameType == COMPITITOR_NETWORK)
    {
        SendMoveRouteMsg();
    }

    Notify(EVENT_UPDATE_MOVE);
    ResetMoveRoute(m_stCurrentMoveRoute);
    m_llCurrentStepStartTime = m_nGameResult == -1 ? ::timeGetTime() : 0;
}


const MoveRoute & CGameHandle::GetCurrentMoveRoute()
{
    return m_stCurrentMoveRoute;
}

void CGameHandle::ResetMoveRoute( MoveRoute &stRoute )
{
    stRoute.nMovingChessMan = 0;
    stRoute.nKilledChessMan = 0;
    stRoute.bAttackGeneral = false;
    stRoute.stFromPos.nRow = -1;
    stRoute.stFromPos.nColumn = -1;
    stRoute.stToPos.nRow = -1;
    stRoute.stToPos.nColumn = -1;
    stRoute.dwKey = 0;
    memset(stRoute.szMoveStepAlpha, 0, 5);
}

void CGameHandle::SetGameResult( int nGameResult )
{
    m_nGameResult = nGameResult;
}

int CGameHandle::GetGameResult() const
{
    return m_nGameResult;
}

int CGameHandle::GetCurrentTurn() const
{
    return m_nCurrentTurn;
}

int CGameHandle::GetDeadOne() const
{
    return m_nWhoIsDead;
}

void CGameHandle::SetDeadOne( int nWhoIsDead )
{
    m_nWhoIsDead = nWhoIsDead;
}

int CGameHandle::GetGeneralPosition( int nGeneral, int &nRow, int &nColumn )
{
    ChessManPosition arrPos[5];
    int nChessManCount = m_clGenerator.GetChessManPosition(m_arrChessMan, nGeneral, arrPos);
    if (nChessManCount > 0)
    {
        nRow = arrPos[0].nRow;
        nColumn = arrPos[0].nColumn;
    }

    return nChessManCount;
}

void CGameHandle::OnFallback()
{
    if (m_lstMoveRoute.size() > 0)
    {
        if (m_lstMoveRoute.size() == 1)
        {
            //悔一步棋
            UnDoMakeMove();
        }
        else
        {
            //悔两步棋
            UnDoMakeMove();
            UnDoMakeMove();
        }

        Notify(EVENT_FALLBACK);
        ResetMoveRoute(m_stCurrentMoveRoute);

        if (g_GameSettings.m_nCompetitorSide == m_nCurrentTurn && g_GameSettings.m_nGameType == COMPITITOR_MACHINE)
        {
            ComputerMove();
        }
    }
}

list<MoveRoute> & CGameHandle::GetLstMoveRoute()
{
    return m_lstMoveRoute;
}

void CGameHandle::UnDoMakeMove()
{
    MoveRoute stRoute = m_lstMoveRoute.back();

    int nFromRow = stRoute.stFromPos.nRow;
    int nToRow = stRoute.stToPos.nRow;
    int nFromColumn = stRoute.stFromPos.nColumn;
    int nToColumn = stRoute.stToPos.nColumn;
    int nKilledChessMan = stRoute.nKilledChessMan;
    int nMovingChessMan = stRoute.nMovingChessMan;

    DelChessMan(nMovingChessMan, nToRow, nToColumn);
    AddChessMan(nMovingChessMan, nFromRow, nFromColumn);
    if (nKilledChessMan > 0)
    {
        AddChessMan(nKilledChessMan, nToRow, nToColumn);
    }

    m_lstMoveRoute.pop_back();
    m_nCurrentTurn = (m_nCurrentTurn == BLACK ? RED : BLACK);
    m_nCurrentSearchMoveTurn = m_nCurrentTurn;
    m_clCurrentZobrist.Xor(m_clInitZobrist);

    if (m_lstMoveRoute.size() > 0)
    {
        m_stCurrentMoveRoute = m_lstMoveRoute.back();
    }
}

void CGameHandle::UnDoMakeMove( MoveRoute &stMoveRoute )
{
    int nFromRow = stMoveRoute.stFromPos.nRow;
    int nToRow = stMoveRoute.stToPos.nRow;
    int nFromColumn = stMoveRoute.stFromPos.nColumn;
    int nToColumn = stMoveRoute.stToPos.nColumn;
    int nKilledChessMan = stMoveRoute.nKilledChessMan;
    int nMovingChessMan = stMoveRoute.nMovingChessMan;

    DelChessMan(nMovingChessMan, nToRow, nToColumn);
    AddChessMan(nMovingChessMan, nFromRow, nFromColumn);
    if (nKilledChessMan > 0)
    {
        AddChessMan(nKilledChessMan, nToRow, nToColumn);
    }

    m_nCurrentSearchMoveTurn = (m_nCurrentSearchMoveTurn == BLACK ? RED : BLACK);
    m_clCurrentZobrist.Xor(m_clInitZobrist);
}

void CGameHandle::SaveToFile( const char *pFileName, int nFileType)
{
    assert(pFileName != NULL);
    //txt文件
    if (nFileType == 1)
    {
        //先保存m_arrChessMan
        ofstream fs;
        fs.open(pFileName, ios::out | ios::trunc);
        for (int i = 0; i < CHESSBOARD_ROW; i++)
        {
            for (int j = 0; j < CHESSBOARD_COLUMN - 1; j++)
            {
                fs << m_arrChessMan[i][j] << '\t';
            }
            fs << m_arrChessMan[i][CHESSBOARD_COLUMN - 1] << endl;
        }
        fs << endl;

        //再保存轮到谁走棋，棋局结果
        fs << m_nCurrentTurn << '\t' << m_nGameResult << '\t' << m_nWhoIsDead << endl << endl;

        fs.close();
    }
    else
    {
        SetEvent(m_hEventSaveGame);
        ResetEvent(m_hEventGameSaved);
        strcpy(m_szGameInfoSaveFile, pFileName);
    }
}

void CGameHandle::LoadFromFile( const char *pFileName, int nFileType)
{
    assert(pFileName != NULL);
    memset(m_arrChessMan, 0, sizeof(int) * CHESSBOARD_ROW * CHESSBOARD_COLUMN);
    //txt文件
    if (nFileType == 1)
    {
        fstream fs;
        fs.open(pFileName, ios::in);

        //先读取m_arrChessMan
        for (int i = 0; i < CHESSBOARD_ROW; i++)
        {
            for (int j = 0; j < CHESSBOARD_COLUMN; j++)
            {
                fs >> m_arrChessMan[i][j];
            }
        }
        fs.seekp(1, ios::cur);

        //再读取轮到谁走棋，棋局结果
        fs >> m_nCurrentTurn >> m_nGameResult >> m_nWhoIsDead;
        m_nCurrentSearchMoveTurn = m_nCurrentTurn;
        fs.close();
    }
    else
    {
        sqlite3 *db = NULL;
        int nRet = sqlite3_open(pFileName, &db);
        if (nRet != SQLITE_OK)
        {
            ::MessageBox(NULL, "无法打开文件！", "错误信息", MB_OK);
            return;
        }

        char szSQL[1024];
        sqlite3_stmt *stmt;

        sprintf(szSQL, "select nrow, ncolumn, nchesstype from chessman");        
        nRet = sqlite3_prepare_v2(db, szSQL, -1, &stmt, NULL);
        if (SQLITE_OK != nRet)
        {
            sqlite3_finalize(stmt);
            sqlite3_close(db);
            return;
        }

        while(sqlite3_step(stmt) == SQLITE_ROW)
        {
            int nRow = sqlite3_column_int(stmt, 0);
            int nColumn = sqlite3_column_int(stmt, 1);
            int nChessman = sqlite3_column_int(stmt, 2);

            m_arrChessMan[nRow][nColumn] = nChessman;
        }

        sqlite3_reset(stmt);
        sprintf(szSQL, "select ncurrentturn, ngameresult, nwhoisdead from gameinfo");        
        nRet = sqlite3_prepare_v2(db, szSQL, -1, &stmt, NULL);
        if (SQLITE_OK != nRet)
        {
            sqlite3_finalize(stmt);
            sqlite3_close(db);
            return;
        }

        while(sqlite3_step(stmt) == SQLITE_ROW)
        {
            m_nCurrentTurn = sqlite3_column_int(stmt, 0);
            m_nGameResult = sqlite3_column_int(stmt, 1);
            m_nWhoIsDead = sqlite3_column_int(stmt, 2);
            m_nCurrentSearchMoveTurn = m_nCurrentTurn;
        }

        sqlite3_finalize(stmt);
        sqlite3_close(db);
    }

    Notify(EVENT_LOAD_CHESSMAN);
    ResetMoveRoute(m_stCurrentMoveRoute);
    m_llCurrentStepStartTime = m_nGameResult == -1 ? ::timeGetTime() : 0;

    //发送棋盘同步消息
    if (g_GameSettings.m_nGameType == COMPITITOR_NETWORK)
    {
        SendChessboardSyncMsg();
    }
}

unsigned int __stdcall CGameHandle::SaveGameFunc( void *pParam )
{
    CGameHandle *pGameHandle = (CGameHandle *)pParam;

    while(1)
    {
        WaitForSingleObject(pGameHandle->m_hEventSaveGame, INFINITE);

        sqlite3 *db = NULL;
        int nRet = sqlite3_open(pGameHandle->m_szGameInfoSaveFile, &db);
        if (nRet != SQLITE_OK)
        {
            ::MessageBox(NULL, "无法创建文件！", "错误信息", MB_OK);
            return 0;
        }

        //创建表
        char szSQL[1024];
        char *pErrMsg = NULL;

        sprintf(szSQL, "DROP table if exists chessman");
        nRet = sqlite3_exec(db, szSQL, NULL, 0, &pErrMsg);
        if (nRet != SQLITE_OK)
        {
            ::MessageBox(NULL, "删除表chessman失败！", "错误信息", MB_OK);
            return 0;
        }

        sprintf(szSQL, "CREATE TABLE if not exists chessman (nrow INT, ncolumn INT, nchesstype INT)");
        nRet = sqlite3_exec(db, szSQL, NULL, 0, &pErrMsg);
        if (nRet != SQLITE_OK)
        {
            ::MessageBox(NULL, "创建表chessman失败！", "错误信息", MB_OK);
            return 0;
        }

        sprintf(szSQL, "DROP table if exists gameinfo");
        nRet = sqlite3_exec(db, szSQL, NULL, 0, &pErrMsg);
        if (nRet != SQLITE_OK)
        {
            ::MessageBox(NULL, "删除表gameinfo失败！", "错误信息", MB_OK);
            return 0;
        }

        sprintf(szSQL, "CREATE TABLE if not exists gameinfo (ncurrentturn INT, ngameresult INT, nwhoisdead INT)");
        nRet = sqlite3_exec(db, szSQL, NULL, 0, &pErrMsg);
        if (nRet != SQLITE_OK)
        {
            ::MessageBox(NULL, "创建表gameinfo失败！", "错误信息", MB_OK);
            sqlite3_close(db);
            return 0;
        }

        sprintf(szSQL, "DROP table if exists moveroute");
        nRet = sqlite3_exec(db, szSQL, NULL, 0, &pErrMsg);
        if (nRet != SQLITE_OK)
        {
            ::MessageBox(NULL, "删除表moveroute失败！", "错误信息", MB_OK);
            return 0;
        }

        sprintf(szSQL, "CREATE TABLE if not exists moveroute (id INT, nmovingchessman INT, nkilledchessman INT, battackgeneral INT, nfromrow INT, nfromcolumn INT, ntorow INT, ntocolumn INT, strmovestepalpha VARCHAR (10))");
        nRet = sqlite3_exec(db, szSQL, NULL, 0, &pErrMsg);
        if (nRet != SQLITE_OK)
        {
            ::MessageBox(NULL, "创建表moveroute失败！", "错误信息", MB_OK);
            sqlite3_close(db);
            return 0;
        }

        //插入数据
        sqlite3_stmt *stmt;

        sprintf(szSQL, "insert into chessman(nrow, ncolumn, nchesstype) values (:nrow, :ncolumn, :nchesstype)");
        nRet = sqlite3_prepare_v2(db, szSQL, -1, &stmt, NULL);
        if (SQLITE_OK != nRet)
        {
            sqlite3_finalize(stmt);
            sqlite3_close(db);
            return 0;
        }
        for (int i = 0; i < CHESSBOARD_ROW; i++)
        {
            for (int j = 0; j < CHESSBOARD_COLUMN; j++)
            {
                if (pGameHandle->m_arrChessMan[i][j] > 0)
                {
                    sqlite3_bind_int(stmt, 1, i);
                    sqlite3_bind_int(stmt, 2, j);
                    sqlite3_bind_int(stmt, 3, pGameHandle->m_arrChessMan[i][j]);
                    nRet = sqlite3_step(stmt);
                    if (nRet != SQLITE_DONE)
                    {
                        sqlite3_finalize(stmt);
                        sqlite3_close(db);
                        return 0;
                    }
                    sqlite3_reset(stmt);
                }
            }
        }

        sprintf(szSQL, "insert into gameinfo(ncurrentturn, ngameresult, nwhoisdead) values (:ncurrentturn, :ngameresult, :nwhoisdead)");
        nRet = sqlite3_prepare_v2(db, szSQL, -1, &stmt, NULL);
        if (SQLITE_OK != nRet)
        {
            sqlite3_finalize(stmt);
            sqlite3_close(db);
            return 0;
        }

        sqlite3_bind_int(stmt, 1, pGameHandle->m_nCurrentTurn);
        sqlite3_bind_int(stmt, 2, pGameHandle->m_nGameResult);
        sqlite3_bind_int(stmt, 3, pGameHandle->m_nWhoIsDead);
        nRet = sqlite3_step(stmt);
        if (nRet != SQLITE_DONE)
        {
            sqlite3_finalize(stmt);
            sqlite3_close(db);
            return 0;
        }
        sqlite3_reset(stmt);

        sprintf(szSQL, "insert into moveroute(id, nmovingchessman, nkilledchessman, battackgeneral, nfromrow, nfromcolumn, ntorow, ntocolumn, strmovestepalpha) values (?, ?, ?, ?, ?, ?, ?, ?, ?)");
        nRet = sqlite3_prepare_v2(db, szSQL, -1, &stmt, NULL);
        if (SQLITE_OK != nRet)
        {
            sqlite3_finalize(stmt);
            sqlite3_close(db);
            return 0;
        }

        list<MoveRoute>::iterator it = pGameHandle->m_lstMoveRoute.begin();
        int i = 1;
        for (; it != pGameHandle->m_lstMoveRoute.end(); ++it)
        {
            sqlite3_bind_int(stmt, 1, i);
            sqlite3_bind_int(stmt, 2, it->nMovingChessMan);
            sqlite3_bind_int(stmt, 3, it->nKilledChessMan);
            sqlite3_bind_int(stmt, 4, it->bAttackGeneral ? 1 : 0);
            sqlite3_bind_int(stmt, 5, it->stFromPos.nRow);
            sqlite3_bind_int(stmt, 6, it->stFromPos.nColumn);
            sqlite3_bind_int(stmt, 7, it->stToPos.nRow);
            sqlite3_bind_int(stmt, 8, it->stToPos.nColumn);
            sqlite3_bind_text(stmt, 9, it->szMoveStepAlpha, -1, NULL);
            nRet = sqlite3_step(stmt);
            if (nRet != SQLITE_DONE)
            {
                sqlite3_finalize(stmt);
                sqlite3_close(db);
                return 0;
            }  

            sqlite3_reset(stmt);
            i++;
        }

        sqlite3_finalize(stmt);
        sqlite3_close(db);

        ResetEvent(pGameHandle->m_hEventSaveGame);
        SetEvent(pGameHandle->m_hEventGameSaved);
    }

    return 0;
}

__int64 CGameHandle::GetCurrentStepStartTime()
{
    return m_llCurrentStepStartTime;
}

void CGameHandle::StepTimeOver()
{
    if (m_nGameResult == -1)
    {
        if (m_nCurrentTurn == BLACK)
        {
            m_nGameResult = RED;
        }
        else
        {
            m_nGameResult = BLACK;
        }

        m_llCurrentStepStartTime = 0;
        Notify(EVENT_GAME_RESULT);
    }
}

void CGameHandle::OnTie()
{
    if (m_nGameResult == -1)
    {
        m_nGameResult = TIE;

        m_llCurrentStepStartTime = 0;
        Notify(EVENT_GAME_RESULT);
    }
}

void CGameHandle::OnLose()
{
    if (m_nGameResult == -1)
    {
        if (g_GameSettings.m_nCompetitorSide == BLACK)
        {
            m_nGameResult = BLACK;
        }
        else
        {
            m_nGameResult = RED;
        }

        m_llCurrentStepStartTime = 0;
        Notify(EVENT_GAME_RESULT);
    }
}

void CGameHandle::ResetZobrist()
{
    m_clRC4.Init();
    m_clInitZobrist.Init(m_clRC4);
    for (int i = 0; i < CHESSMAN_TYPE_COUNT; i++)
    {
        for (int j = 0; j < CHESSBOARD_ROW; j++)
        {
            for (int k = 0; k < CHESSBOARD_COLUMN; k++)
            {
                m_arrZobristTable[i][j][k].Init(m_clRC4);
            }
        }
    }

    m_clCurrentZobrist.Reset();
}

void CGameHandle::Reset()
{
    m_nCurrentTurn = g_GameSettings.m_nAhead;
    m_nCurrentSearchMoveTurn = m_nCurrentTurn;
    m_nGameResult = -1;
    m_nWhoIsDead = 0;
    m_lstMoveRoute.clear();
    m_nBlackValue = 0;
    m_nRedValue = 0;
}

void CGameHandle::AddChessMan( int nChessMan, int nRow, int nColumn )
{
    assert(nChessMan >= BLACK_GENERAL && nChessMan <= RED_SOLDIER);
    assert(nRow >= 0 && nRow < CHESSBOARD_ROW);
    assert(nColumn >= 0 && nColumn < CHESSBOARD_COLUMN);

    m_arrChessMan[nRow][nColumn] = nChessMan;
    m_clCurrentZobrist.Xor(m_arrZobristTable[nChessMan - 1][nRow][nColumn]);

    if (IsBlackSide(nChessMan))
    {
        m_nBlackValue += CHESSMAN_VALUE[nChessMan - 1][nRow][nColumn];
    }
    else
    {
        m_nRedValue += CHESSMAN_VALUE[nChessMan - 1][nRow][nColumn];
    }
}

void CGameHandle::DelChessMan( int nChessMan, int nRow, int nColumn )
{
    assert(nChessMan >= BLACK_GENERAL && nChessMan <= RED_SOLDIER);
    assert(nRow >= 0 && nRow < CHESSBOARD_ROW);
    assert(nColumn >= 0 && nColumn < CHESSBOARD_COLUMN);

    m_arrChessMan[nRow][nColumn] = 0;
    m_clCurrentZobrist.Xor(m_arrZobristTable[nChessMan - 1][nRow][nColumn]);

    if (IsBlackSide(nChessMan))
    {
        m_nBlackValue -= CHESSMAN_VALUE[nChessMan - 1][nRow][nColumn];
    }
    else
    {
        m_nRedValue -= CHESSMAN_VALUE[nChessMan - 1][nRow][nColumn];
    }
}

int CGameHandle::MinMaxSearch( int nDepth, MoveRoute &stRoute )
{
    int nBestValue = IsMySide() ? MATE_VALUE : -MATE_VALUE;
    int nValue = 0;

    if (nDepth == 0)
    {
        return Evaluate();
    }

    list<MoveRoute> lstMoveRoute;
    m_clGenerator.GenerateAllMoveRoute(m_arrChessMan, m_nCurrentSearchMoveTurn, lstMoveRoute);

    for (list<MoveRoute>::iterator it = lstMoveRoute.begin(); it != lstMoveRoute.end(); ++it)
    {
        DoMakeMove(*it, false);
        nValue = MinMaxSearch(nDepth - 1, stRoute);
        UnDoMakeMove(*it);
        if (IsMySide())
        {
            if (nValue < nBestValue)
            {
                nBestValue = nValue;
                if (nDepth == MAX_SEARCH_DEPTH)
                {
                    stRoute = *it;
                }
            }
        }
        else
        {
            if (nValue > nBestValue)
            {
                nBestValue = nValue;
                if (nDepth == MAX_SEARCH_DEPTH)
                {
                    stRoute = *it;
                }
            }
        }
    }

    return nBestValue;
}

int CGameHandle::Evaluate()
{
    return g_GameSettings.m_nCompetitorSide == RED ? m_nRedValue - m_nBlackValue : m_nBlackValue - m_nRedValue;
}

void CGameHandle::DoMakeMove(MoveRoute &stMoveRoute, bool bRecord)
{
    int nFromRow = stMoveRoute.stFromPos.nRow;
    int nToRow = stMoveRoute.stToPos.nRow;
    int nFromColumn = stMoveRoute.stFromPos.nColumn;
    int nToColumn = stMoveRoute.stToPos.nColumn;
    int nKilledChessMan = stMoveRoute.nKilledChessMan;
    int nMovingChessMan = stMoveRoute.nMovingChessMan;

    if (nKilledChessMan > 0)
    {
        DelChessMan(nKilledChessMan, nToRow, nToColumn);
    }

    DelChessMan(nMovingChessMan, nFromRow, nFromColumn);
    AddChessMan(nMovingChessMan, nToRow, nToColumn);
    stMoveRoute.dwKey = m_clCurrentZobrist.m_dwKey;
    if (bRecord)
    {
        m_lstMoveRoute.push_back(stMoveRoute);
        m_nCurrentTurn = (m_nCurrentTurn == BLACK ? RED : BLACK);
    }
    m_nCurrentSearchMoveTurn = (m_nCurrentSearchMoveTurn == BLACK ? RED : BLACK);
    m_clCurrentZobrist.Xor(m_clInitZobrist);
}

bool CGameHandle::IsMySide()
{
    return !(g_GameSettings.m_nCompetitorSide == m_nCurrentSearchMoveTurn);
}

int CGameHandle::RepStatus( int nRecur )
{
    bool bSelfSide = true;
    bool bPerpCheck = true;
    bool bOppPerpCheck = true;

    list<MoveRoute>::reverse_iterator rit = m_lstMoveRoute.rbegin();

    while(rit != m_lstMoveRoute.rend() && rit->nKilledChessMan == 0)
    {
        if (bSelfSide)
        {
            bPerpCheck = bPerpCheck && rit->bAttackGeneral;
            if (rit->dwKey == m_stCurrentMoveRoute.dwKey)
            {
                nRecur--;
                if (nRecur == 0)
                {
                    return 1 + (bPerpCheck ? 2 : 0) + (bOppPerpCheck ? 4 : 0);
                }
            }
        }
        else
        {
            bOppPerpCheck = bOppPerpCheck && rit->bAttackGeneral;
        }
        bSelfSide = !bSelfSide;
        ++rit;
    }

    return 0;
}

int CGameHandle::RepValue( int nRepStatus )
{
    int nRetVal = 0;
    nRetVal = ((nRepStatus & 2) == 0 ? 0 :  - BAN_VALUE) + 
        ((nRepStatus & 4) == 0 ? 0 : BAN_VALUE);

    return nRetVal == 0 ? ((m_lstMoveRoute.size() & 1) == 0 ? -DRAW_VALUE : DRAW_VALUE) : nRetVal;
}

void CGameHandle::ProcessMessage( )
{
    BaseNetworkMsg *pMsg = NULL;
    while((pMsg = DequeMsg()) != NULL)
    {
        switch(pMsg->nMsgID)
        {
        case MSG_GAME_INFO:
            ProcessGameInfoMsg(pMsg);
            break;
        case MSG_NEW_GAME:
            ProcessNewGameMsg(pMsg);
            break;
        case MSG_CHESSBOARD_SYNC:
            ProcessChessboardSyncMsg(pMsg);
            break;
        case MSG_MOVE_ROUTE:
            ProcessMoveRouteMsg(pMsg);
            break;
        case MSG_TIP:
            ProcessTipMsg(pMsg);
            break;
        case MSG_TIP_REPLY:
            ProcessTipReplyMsg(pMsg);
            break;
        case MSG_DISCONNECT:
            ProcessDisconnectMsg(pMsg);
            break;
        default:
            break;
        }
    }
}

unsigned int __stdcall CGameHandle::RecvMsg( void *pParam )
{
    CGameHandle *pGameHandle = (CGameHandle *)pParam;
    if (g_GameSettings.m_nGameType == COMPITITOR_NETWORK)
    {
        char szMsg[MAX_MSG_SIZE];
        memset(szMsg, 0, MAX_MSG_SIZE);

        if (g_GameSettings.m_nServerOrClient == SERVER_SIDE)
        {
            while(1)
            {
                int nSize = pGameHandle->m_clServer.RecvMsg(szMsg);
                if (nSize > 0)
                {
                    pGameHandle->EnqueMsg((BaseNetworkMsg *)szMsg);
                }
                else
                {
                    MsgDisconnect *pMsgDisconnect = new MsgDisconnect;
                    pGameHandle->EnqueMsg((BaseNetworkMsg *)pMsgDisconnect);
                    break;
                }
            }
        }
        else
        {
            while(1)
            {
                int nSize = pGameHandle->m_clClient.RecvMsg(szMsg);
                if (nSize > 0)
                {
                    pGameHandle->EnqueMsg((BaseNetworkMsg *)szMsg);
                }
                else
                {
                    MsgDisconnect *pMsgDisconnect = new MsgDisconnect;
                    pGameHandle->EnqueMsg((BaseNetworkMsg *)pMsgDisconnect);
                    break;
                }
            }
        }
    }

    return 1;
}

void CGameHandle::OnStart()
{
    //如果是网络对战
    //服务器监听端口号，等待客户端连接
    //客户端连接服务器
    if (g_GameSettings.m_nGameType == COMPITITOR_NETWORK)
    {
        ::InitializeCriticalSection(&m_csMsgQue);
        if (g_GameSettings.m_nServerOrClient == SERVER_SIDE)
        {
            if(m_clServer.InitServer(g_GameSettings.m_szIpAddr, g_GameSettings.m_nPort))
            {
                if(m_clServer.WaitForClientConn() > 0)
                {
                    SendGameInfoMsg();
                    m_hThreadNetwork = (HANDLE)_beginthreadex(NULL, 0, RecvMsg, this, 0, NULL);
                }
            }

        }
        else
        {
            m_clClient.InitClient();
            m_clClient.ConnToServer(g_GameSettings.m_szIpAddr, g_GameSettings.m_nPort);
            m_hThreadNetwork = (HANDLE)_beginthreadex(NULL, 0, RecvMsg, this, 0, NULL);
        }
    }

}

void CGameHandle::Shutdown()
{
    WaitForSingleObject(m_hEventGameSaved, INFINITE);
    CloseHandle(m_hEventSaveGame);
    CloseHandle(m_hThreadSaveGame);
    CloseHandle(m_hEventGameSaved);

    if (g_GameSettings.m_nGameType == COMPITITOR_NETWORK)
    {
        if (g_GameSettings.m_nServerOrClient == SERVER_SIDE)
        {
            m_clServer.StopServer();
        }
        else
        {
            m_clClient.StopClient();
        }
        CloseHandle(m_hThreadNetwork);
        DeleteCriticalSection(&m_csMsgQue);
    }
    else if(g_GameSettings.m_nGameType == COMPITITOR_MACHINE)
    {
        CloseHandle(m_hEventComputerMove);
        CloseHandle(m_hThreadComputerMove);
    }
}

void CGameHandle::SendGameInfoMsg()
{
    MsgGameInfo stMsg;
    stMsg.nMsgID = MSG_GAME_INFO;
    stMsg.nMySide = g_GameSettings.m_nCompetitorSide;
    stMsg.nStepTime = g_GameSettings.m_nStepTime;
    stMsg.nAhead = g_GameSettings.m_nAhead;
    memcpy(stMsg.arrChessman, m_arrChessMan, sizeof(int) * CHESSBOARD_ROW * CHESSBOARD_COLUMN);
    m_clServer.SendMsg((char *)&stMsg, sizeof(stMsg));
}

void CGameHandle::ProcessGameInfoMsg( void *pMsg )
{
    MsgGameInfo *pMsgGameInfo = (MsgGameInfo *)pMsg;

    bool bNotify = g_GameSettings.m_nCompetitorSide == pMsgGameInfo->nMySide;
    g_GameSettings.m_nCompetitorSide = pMsgGameInfo->nMySide == BLACK ? RED : BLACK;
    g_GameSettings.m_nAhead = pMsgGameInfo->nAhead;
    g_GameSettings.m_nStepTime = pMsgGameInfo->nStepTime;
    memcpy(m_arrChessMan, pMsgGameInfo->arrChessman, sizeof(int) * CHESSBOARD_ROW * CHESSBOARD_COLUMN);

    if (bNotify)
    {
        Notify(EVENT_CHANGE_POS);
    }
}

void CGameHandle::ProcessNewGameMsg( void *pMsg )
{
    OnNewGame();
}

void CGameHandle::ProcessChessboardSyncMsg( void *pMsg )
{
    MsgChessboardSync *pMsgChessboardSync = (MsgChessboardSync *)pMsg;
    m_nCurrentTurn = pMsgChessboardSync->nCurrentTurn;
    m_nGameResult = pMsgChessboardSync->nGameResult;
    m_nWhoIsDead = pMsgChessboardSync->nWhoIsDead;
    memcpy(m_arrChessMan, pMsgChessboardSync->arrChessman, sizeof(int) * CHESSBOARD_ROW * CHESSBOARD_COLUMN);
    m_nCurrentSearchMoveTurn = m_nCurrentTurn;

    Notify(EVENT_LOAD_CHESSMAN);
    ResetMoveRoute(m_stCurrentMoveRoute);
    m_llCurrentStepStartTime = m_nGameResult == -1 ? ::timeGetTime() : 0;
}

void CGameHandle::ProcessMoveRouteMsg( void *pMsg )
{
    MsgMoveRoute *pMsgMoveRoute = (MsgMoveRoute *)pMsg;
    m_nCurrentTurn = pMsgMoveRoute->nCurrentTurn;
    m_nGameResult = pMsgMoveRoute->nGameResult;
    m_nWhoIsDead = pMsgMoveRoute->nWhoIsDead;
    m_stCurrentMoveRoute = pMsgMoveRoute->stMoveRoute;
    memcpy(m_arrChessMan, pMsgMoveRoute->arrChessman, sizeof(int) * CHESSBOARD_ROW * CHESSBOARD_COLUMN);

    Notify(EVENT_UPDATE_MOVE);
    if (IsCompleteMoveRoute(m_stCurrentMoveRoute))
    {
        m_lstMoveRoute.push_back(m_stCurrentMoveRoute);
        ResetMoveRoute(m_stCurrentMoveRoute);
        m_llCurrentStepStartTime = m_nGameResult == -1 ? ::timeGetTime() : 0;
    }
}

void CGameHandle::EnqueMsg( BaseNetworkMsg *pMsg )
{
    EnterCriticalSection(&m_csMsgQue);
    m_lstMsgQue.push_back(pMsg);
    LeaveCriticalSection(&m_csMsgQue);
}

BaseNetworkMsg * CGameHandle::DequeMsg()
{
    BaseNetworkMsg *pMsg = NULL;
    if (m_lstMsgQue.size())
    {
        EnterCriticalSection(&m_csMsgQue);
        pMsg = m_lstMsgQue.back();
        m_lstMsgQue.pop_back();
        LeaveCriticalSection(&m_csMsgQue);
    }

    return pMsg;
}

void CGameHandle::SendNewGameMsg()
{
    MsgNewGame stMsgNewGame;
    memcpy(stMsgNewGame.arrChessman, m_arrChessMan, sizeof(int) * CHESSBOARD_ROW * CHESSBOARD_COLUMN);
    SendMsg((char *)&stMsgNewGame, sizeof(MsgNewGame));
}

void CGameHandle::SendChessboardSyncMsg()
{
    MsgChessboardSync stMsgChessboardSync;
    stMsgChessboardSync.nCurrentTurn = m_nCurrentTurn;
    stMsgChessboardSync.nGameResult = m_nGameResult;
    stMsgChessboardSync.nWhoIsDead = m_nWhoIsDead;
    memcpy(stMsgChessboardSync.arrChessman, m_arrChessMan, sizeof(int) * CHESSBOARD_ROW * CHESSBOARD_COLUMN);
    SendMsg((char *)&stMsgChessboardSync, sizeof(MsgChessboardSync));
}

void CGameHandle::SendMoveRouteMsg()
{
    MsgMoveRoute stMsgMoveRoute;
    stMsgMoveRoute.nCurrentTurn = m_nCurrentTurn;
    stMsgMoveRoute.nGameResult = m_nGameResult;
    stMsgMoveRoute.nWhoIsDead = m_nWhoIsDead;
    memcpy(stMsgMoveRoute.arrChessman, m_arrChessMan, sizeof(int) * CHESSBOARD_ROW * CHESSBOARD_COLUMN);
    stMsgMoveRoute.stMoveRoute = m_stCurrentMoveRoute;
    SendMsg((char *)&stMsgMoveRoute, sizeof(MsgMoveRoute));
}

void CGameHandle::SendMsg( char *pMsg, int nMsgSize )
{
    if (g_GameSettings.m_nServerOrClient == SERVER_SIDE)
    {
        m_clServer.SendMsg(pMsg, nMsgSize);
    }
    else
    {
        m_clClient.SendMsg(pMsg, nMsgSize);
    }
}

void CGameHandle::OnWin()
{
    if (m_nGameResult == -1)
    {
        if (g_GameSettings.m_nCompetitorSide == BLACK)
        {
            m_nGameResult = RED;
        }
        else
        {
            m_nGameResult = BLACK;
        }

        m_llCurrentStepStartTime = 0;
        Notify(EVENT_GAME_RESULT);
    }
}

void CGameHandle::SendTipMsg( int nTipType )
{
    MsgTip stMsgTip;
    stMsgTip.nReqSide = g_GameSettings.m_nCompetitorSide;
    stMsgTip.nTipType = nTipType;

    SendMsg((char *)&stMsgTip, sizeof(MsgTip));
}

void CGameHandle::SendTipReplyMsg( int nTipType, int nResult )
{
    MsgTipReply stMsgTipReply;
    stMsgTipReply.nReqSide = g_GameSettings.m_nCompetitorSide;
    stMsgTipReply.nResult = nResult;
    stMsgTipReply.nTipType = nTipType;

    SendMsg((char *)&stMsgTipReply, sizeof(MsgTipReply));
}

void CGameHandle::ProcessTipMsg( void *pMsg )
{
    MsgTip *pMsgTip = (MsgTip *)pMsg;

    switch(pMsgTip->nTipType)
    {
    case TIP_REQ_FALLBACK:
        Notify(EVENT_REQ_FALLBACK);
        break;
    case TIP_REQ_TIE:
        Notify(EVENT_REQ_TIE);
        break;
    case TIP_REQ_LOSE:
        Notify(EVENT_REQ_LOSE);
        break;
    default:
        break;
    }
}

void CGameHandle::ProcessTipReplyMsg( void *pMsg )
{
    MsgTipReply *pMsgTipReply = (MsgTipReply *)pMsg;

    switch(pMsgTipReply->nTipType)
    {
    case TIP_REQ_FALLBACK:
        {
            Notify(EVENT_REQ_FALLBACK_REPLY, pMsg);
        }
        break;
    case TIP_REQ_TIE:
        {
            Notify(EVENT_REQ_TIE_REPLY, pMsg);
        }
        break;
    case TIP_REQ_LOSE:
        {
            Notify(EVENT_REQ_LOSE_REPLY, pMsg);
        }
        break;
    default:
        break;
    }
}

void CGameHandle::ProcessDisconnectMsg( void *pMsg )
{
    g_GameSettings.m_nGameType = COMPITITOR_HUMAN;
    if (pMsg)
    {
        delete pMsg;
        pMsg = NULL;
    }
    OnNewGame();
}
