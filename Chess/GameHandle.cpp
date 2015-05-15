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
#include <assert.h>
#include <fstream>
#include "../Include/sqlite3.h"

CGameHandle g_GameHandle;

CGameHandle::CGameHandle(void)
{
    memset(m_arrChessMan, 0, sizeof(int) * CHESSBOARD_ROW * CHESSBOARD_COLUMN);
    m_nCurrentTurn = -1;
    m_nWhoIsDead = 0;
    memset(m_szGameInfoSaveFile, 0, MAX_PATH);

    m_hEventSaveGame = INVALID_HANDLE_VALUE;
    m_hEventGameSaved = INVALID_HANDLE_VALUE;
    m_hThreadSaveGame = INVALID_HANDLE_VALUE;
    m_llCurrentStepStartTime = 0;
}

CGameHandle::~CGameHandle(void)
{
    WaitForSingleObject(m_hEventGameSaved, INFINITE);
    CloseHandle(m_hEventSaveGame);
    CloseHandle(m_hThreadSaveGame);
    CloseHandle(m_hEventGameSaved);
}

void CGameHandle::Init()
{
    ResetChessManLayout();
    Notify(EVENT_INIT_BOARD);

    m_hEventSaveGame = CreateEvent(NULL, TRUE, FALSE, NULL); 
    m_hEventGameSaved = CreateEvent(NULL, TRUE, TRUE, NULL);  
    m_hThreadSaveGame = (HANDLE)_beginthreadex(NULL, 0, SaveGameFunc, this, 0, NULL);
}

void CGameHandle::NewGame()
{
    m_nCurrentTurn = g_GameSettings.m_nAhead;
    m_nGameResult = -1;
    m_nWhoIsDead = 0;
    m_lstMoveRoute.clear();
    ResetMoveRoute(m_stCurrentMoveRoute);
    ResetChessManLayout();
    Notify(EVENT_NEW_GAME);
    m_llCurrentStepStartTime = ::timeGetTime();
}


//置棋子为最初状态
void CGameHandle::ResetChessManLayout()
{
    memcpy(m_arrChessMan, DEFAULT_CHESSBOARD_LAYOUT, sizeof(int) * CHESSBOARD_ROW * CHESSBOARD_COLUMN);
}

void CGameHandle::GetChessMan( int arrChessMan[][CHESSBOARD_COLUMN] )
{
    memcpy(arrChessMan, m_arrChessMan, sizeof(int) * CHESSBOARD_ROW * CHESSBOARD_COLUMN);
}

//电脑走棋
void CGameHandle::ComputerMove()
{
    list<MoveRoute> lstMoveRoute;
    m_clGenerator.GenerateAllMoveRoute(m_arrChessMan, g_GameSettings.m_nCompetitorSide, lstMoveRoute);

    MoveRoute stMoveRoute = lstMoveRoute.back();
    m_stCurrentMoveRoute.stFromPos.nRow = stMoveRoute.stFromPos.nRow;
    m_stCurrentMoveRoute.stFromPos.nColumn = stMoveRoute.stFromPos.nColumn;
    m_stCurrentMoveRoute.nMovingChessMan = stMoveRoute.nMovingChessMan;

    Notify(EVENT_UPDATE_MOVE);

    m_stCurrentMoveRoute.stToPos.nRow = stMoveRoute.stToPos.nRow;
    m_stCurrentMoveRoute.stToPos.nColumn = stMoveRoute.stToPos.nColumn;
    m_stCurrentMoveRoute.nKilledChessMan = stMoveRoute.nKilledChessMan;
    m_stCurrentMoveRoute.bAttackGeneral = stMoveRoute.bAttackGeneral;
    m_clGenerator.GetChessManMoveStepAlpha(m_arrChessMan, m_stCurrentMoveRoute.stFromPos.nRow,
                                            m_stCurrentMoveRoute.stFromPos.nColumn, 
                                            m_stCurrentMoveRoute.stToPos.nRow, 
                                            m_stCurrentMoveRoute.stToPos.nColumn, 
                                            m_stCurrentMoveRoute.szMoveStepAlpha);

    ApplyCompleteMove();
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
    m_arrChessMan[m_stCurrentMoveRoute.stFromPos.nRow][m_stCurrentMoveRoute.stFromPos.nColumn] = 0;
    m_arrChessMan[m_stCurrentMoveRoute.stToPos.nRow][m_stCurrentMoveRoute.stToPos.nColumn] = m_stCurrentMoveRoute.nMovingChessMan;

    //判断是否将对方置于死地
    if (m_stCurrentMoveRoute.bAttackGeneral)
    {
        if (m_clGenerator.IsGeneralDead(m_arrChessMan, m_nCurrentTurn == BLACK ? RED : BLACK))
        {
            m_nWhoIsDead = m_nCurrentTurn == BLACK ? RED : BLACK;
            m_nGameResult = m_nCurrentTurn;
        }
    }

    m_lstMoveRoute.push_back(m_stCurrentMoveRoute);
    m_nCurrentTurn = (m_nCurrentTurn == BLACK ? RED : BLACK);
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

void CGameHandle::FallBack()
{
    if (m_lstMoveRoute.size() > 0)
    {
        if (m_lstMoveRoute.size() == 1)
        {
            //悔一步棋
            FallBackOneStep();
        }
        else
        {
            //悔两步棋
            FallBackOneStep();
            FallBackOneStep();
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

void CGameHandle::FallBackOneStep()
{
    MoveRoute stRoute = m_lstMoveRoute.back();
    m_arrChessMan[stRoute.stFromPos.nRow][stRoute.stFromPos.nColumn] = stRoute.nMovingChessMan;
    m_arrChessMan[stRoute.stToPos.nRow][stRoute.stToPos.nColumn] = stRoute.nKilledChessMan;
    m_lstMoveRoute.pop_back();
    m_nCurrentTurn = (m_nCurrentTurn == BLACK ? RED : BLACK);
    if (m_lstMoveRoute.size() > 0)
    {
        m_stCurrentMoveRoute = m_lstMoveRoute.back();
    }
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

        //最后保存走棋历史记录
        list<MoveRoute>::iterator it = m_lstMoveRoute.begin();
        for (; it != m_lstMoveRoute.end(); ++it)
        {
            fs << it->nMovingChessMan << '\t' 
                << it->nKilledChessMan << '\t' 
                << it->bAttackGeneral  << '\t' 
                << it->stFromPos.nRow << '\t' 
                << it->stFromPos.nColumn << '\t'
                << it->stToPos.nRow << '\t'
                << it->stToPos.nColumn << '\t'
                << it->szMoveStepAlpha << endl;
        }
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
        fs.seekp(1, ios::cur);

        //最后读取走棋历史记录
        MoveRoute stMoveRoute;
        while(!fs.eof())
        {
            fs >> stMoveRoute.nMovingChessMan
                >> stMoveRoute.nKilledChessMan
                >> stMoveRoute.bAttackGeneral
                >> stMoveRoute.stFromPos.nRow
                >> stMoveRoute.stFromPos.nColumn
                >> stMoveRoute.stToPos.nRow
                >> stMoveRoute.stToPos.nColumn
                >> stMoveRoute.szMoveStepAlpha;
            if (fs.eof())
            {
                break;
            }
            m_lstMoveRoute.push_back(stMoveRoute);
        }

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
        }

        sqlite3_reset(stmt);
        sprintf(szSQL, "select nmovingchessman, nkilledchessman, battackgeneral, nfromrow, nfromcolumn, ntorow, ntocolumn, strmovestepalpha from moveroute order by id");        
        nRet = sqlite3_prepare_v2(db, szSQL, -1, &stmt, NULL);
        if (SQLITE_OK != nRet)
        {
            sqlite3_finalize(stmt);
            sqlite3_close(db);
            return;
        }

        while(sqlite3_step(stmt) == SQLITE_ROW)
        {
            MoveRoute stRoute;
            stRoute.nMovingChessMan = sqlite3_column_int(stmt, 0);
            stRoute.nKilledChessMan = sqlite3_column_int(stmt, 1);
            stRoute.bAttackGeneral = sqlite3_column_int(stmt, 2);
            stRoute.stFromPos.nRow = sqlite3_column_int(stmt, 3);
            stRoute.stFromPos.nColumn = sqlite3_column_int(stmt, 4);
            stRoute.stToPos.nRow = sqlite3_column_int(stmt, 5);
            stRoute.stToPos.nColumn = sqlite3_column_int(stmt, 6);
            strcpy(stRoute.szMoveStepAlpha, (const char *)sqlite3_column_text(stmt, 7));

            m_lstMoveRoute.push_back(stRoute);
        }

        sqlite3_finalize(stmt);
        sqlite3_close(db);
    }

    if (m_lstMoveRoute.size())
    {
        m_stCurrentMoveRoute = m_lstMoveRoute.back();
    }
    
    Notify(EVENT_LOAD_CHESSMAN);
    ResetMoveRoute(m_stCurrentMoveRoute);
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
