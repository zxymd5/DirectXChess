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
 *  File:		GameHandle.cpp
 *  Author:		Richard Zou
 *	Created on:	2015-03-01
 */

#include "GameHandle.h"
#include "GameSettings.h"
#include <assert.h>
#include <fstream>

CGameHandle g_GameHandle;

CGameHandle::CGameHandle(void)
{
    memset(m_szChessMan, 0, sizeof(int) * s_nChessBoardRow * s_nChessBoardColumn);
    m_nCurrentTurn = -1;
    m_nWhoIsDead = 0;
}

CGameHandle::~CGameHandle(void)
{
}

void CGameHandle::Init()
{
    m_nCurrentTurn = g_GameSettings.m_nAhead;
    m_nGameResult = -1;
    m_nWhoIsDead = 0;
    m_lstMoveRoute.clear();
    ResetMoveRoute(m_stCurrentMoveRoute);
    ResetChessManLayout();
    Notify(s_nEventUpdateChessMan);
}

//置棋子为最初状态
void CGameHandle::ResetChessManLayout()
{
    memcpy(m_szChessMan, s_szDefaultChessManLayout, sizeof(int) * s_nChessBoardRow * s_nChessBoardColumn);
}

void CGameHandle::GetChessMan( int szChessMan[][s_nChessBoardColumn] )
{
    memcpy(szChessMan, m_szChessMan, sizeof(int) * s_nChessBoardRow * s_nChessBoardColumn);
}

//to do
void CGameHandle::SetCurrentMoveRoute(int nRow, int nColumn)
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

    int nChessMan = m_szChessMan[nRow][nColumn];
    int nMovingChessMan = m_stCurrentMoveRoute.nMovingChessMan;
    bool bNotifyView = false;
    
    if (m_nCurrentTurn == s_nTurnRed)
    {
        if (nMovingChessMan == 0)
        {
            if (IsRedSide(nChessMan))
            {
                m_stCurrentMoveRoute.nMovingChessMan = nChessMan;
                m_stCurrentMoveRoute.stFromPos.nRow = nRow;
                m_stCurrentMoveRoute.stFromPos.nColumn = nColumn;
                bNotifyView = true;
            }
        }
        else
        {
            if (IsRedSide(nChessMan))
            {
                m_stCurrentMoveRoute.nMovingChessMan = nChessMan;
                m_stCurrentMoveRoute.stFromPos.nRow = nRow;
                m_stCurrentMoveRoute.stFromPos.nColumn = nColumn;
                bNotifyView = true;
            }
            else
            {
                //判断走法是否合理
                if (m_clGenerator.ValidateMoveRoute(m_szChessMan, m_stCurrentMoveRoute.stFromPos.nRow, 
                    m_stCurrentMoveRoute.stFromPos.nColumn, nRow, nColumn))
                {
                    m_clGenerator.GetChessManMoveStepAlpha(m_szChessMan, m_stCurrentMoveRoute.stFromPos.nRow,
                                                            m_stCurrentMoveRoute.stFromPos.nColumn, nRow, nColumn, 
                                                            m_stCurrentMoveRoute.strMoveStepAlpha);
                    m_stCurrentMoveRoute.stToPos.nRow = nRow;
                    m_stCurrentMoveRoute.stToPos.nColumn = nColumn;
                    bNotifyView = true;
                }
            }
        }
    }
    else
    {
        if (nMovingChessMan == 0)
        {
            if (IsBlackSide(nChessMan))
            {
                m_stCurrentMoveRoute.nMovingChessMan = nChessMan;
                m_stCurrentMoveRoute.stFromPos.nRow = nRow;
                m_stCurrentMoveRoute.stFromPos.nColumn = nColumn;
                bNotifyView = true;
            }
        }
        else
        {
            if (IsBlackSide(nChessMan))
            {
                m_stCurrentMoveRoute.nMovingChessMan = nChessMan;
                m_stCurrentMoveRoute.stFromPos.nRow = nRow;
                m_stCurrentMoveRoute.stFromPos.nColumn = nColumn;
                bNotifyView = true;
            }
            else
            {
                //判断走法是否合理
                if (m_clGenerator.ValidateMoveRoute(m_szChessMan, m_stCurrentMoveRoute.stFromPos.nRow, 
                    m_stCurrentMoveRoute.stFromPos.nColumn, nRow, nColumn))
                {
                    m_clGenerator.GetChessManMoveStepAlpha(m_szChessMan, m_stCurrentMoveRoute.stFromPos.nRow,
                        m_stCurrentMoveRoute.stFromPos.nColumn, nRow, nColumn, 
                        m_stCurrentMoveRoute.strMoveStepAlpha);
                    m_stCurrentMoveRoute.stToPos.nRow = nRow;
                    m_stCurrentMoveRoute.stToPos.nColumn = nColumn;
                    bNotifyView = true;
                }
            }
        }
    }


    if (bNotifyView)
    {
        //如果走了完整的一步
        if (IsCompleteMoveRoute(m_stCurrentMoveRoute))
        {
            m_stCurrentMoveRoute.nKilledChessMan = m_szChessMan[nRow][nColumn];
            m_szChessMan[m_stCurrentMoveRoute.stFromPos.nRow][m_stCurrentMoveRoute.stFromPos.nColumn] = 0;
            m_szChessMan[nRow][nColumn] = m_stCurrentMoveRoute.nMovingChessMan;
            m_stCurrentMoveRoute.bAttackGeneral = m_clGenerator.IsAttackGeneral(m_szChessMan, m_nCurrentTurn == s_nTurnBlack ? s_nRedGeneral : s_nBlackGeneral);
            
            if (m_nCurrentTurn == s_nTurnBlack)
            {
                if (m_clGenerator.MeetWithGeneral(m_szChessMan, s_nBlackGeneral) ||
                    m_clGenerator.IsAttackGeneral(m_szChessMan, s_nBlackGeneral))
                {
                    m_nWhoIsDead = s_nBlackDead;
                    m_nGameResult = s_nResultRedWin;
                }
            }
            else
            {
                if (m_clGenerator.MeetWithGeneral(m_szChessMan, s_nRedGeneral) ||
                    m_clGenerator.IsAttackGeneral(m_szChessMan, s_nRedGeneral))
                {
                    m_nWhoIsDead = s_nRedDead;
                    m_nGameResult = s_nResultBlackWin;
                }
            }

            m_lstMoveRoute.push_back(m_stCurrentMoveRoute);
            m_nCurrentTurn = (m_nCurrentTurn == s_nTurnBlack ? s_nTurnRed : s_nTurnBlack);
            Notify(s_nEventUpdateCurrentChessMan);
            ResetMoveRoute(m_stCurrentMoveRoute);
        }
        else
        {
            Notify(s_nEventUpdateCurrentChessMan);
        }
    }
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
    memset(stRoute.strMoveStepAlpha, 0, 5);
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

void CGameHandle::SetDeadOne( int WhoIsDead )
{
    m_nWhoIsDead = WhoIsDead;
}

int CGameHandle::GetGeneralPosition( int nGeneral, int &nRow, int &nColumn )
{
    ChessManPosition szPos[5];
    int nChessManCount = m_clGenerator.GetChessManPosition(m_szChessMan, nGeneral, szPos);
    if (nChessManCount > 0)
    {
        nRow = szPos[0].nRow;
        nColumn = szPos[0].nColumn;
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

        Notify(s_nEventFallback);
        ResetMoveRoute(m_stCurrentMoveRoute);
    }
}

list<MoveRoute> & CGameHandle::GetLstMoveRoute()
{
    return m_lstMoveRoute;
}

void CGameHandle::FallBackOneStep()
{
    MoveRoute stRoute = m_lstMoveRoute.back();
    m_szChessMan[stRoute.stFromPos.nRow][stRoute.stFromPos.nColumn] = stRoute.nMovingChessMan;
    m_szChessMan[stRoute.stToPos.nRow][stRoute.stToPos.nColumn] = stRoute.nKilledChessMan;
    m_lstMoveRoute.pop_back();
    m_nCurrentTurn = (m_nCurrentTurn == s_nTurnBlack ? s_nTurnRed : s_nTurnBlack);
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
        //先保存m_szChessMan
        ofstream fs;
        fs.open(pFileName, ios::out | ios::trunc);
        for (int i = 0; i < s_nChessBoardRow; i++)
        {
            for (int j = 0; j < s_nChessBoardColumn - 1; j++)
            {
                fs << m_szChessMan[i][j] << '\t';
            }
            fs << m_szChessMan[i][s_nChessBoardColumn - 1] << endl;
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
                << it->strMoveStepAlpha << endl;
        }
        fs.close();
    }
    else
    {
  
    }
}

void CGameHandle::LoadFromFile( const char *pFileName, int nFileType)
{
    assert(pFileName != NULL);
    //txt文件
    if (nFileType == 1)
    {
        fstream fs;
        fs.open(pFileName, ios::in);

        //先读取m_szChessMan
        for (int i = 0; i < s_nChessBoardRow; i++)
        {
            for (int j = 0; j < s_nChessBoardColumn; j++)
            {
                fs >> m_szChessMan[i][j];
            }
        }
        fs.seekp(1, ios::cur);

        //再保存轮到谁走棋，棋局结果
        fs >> m_nCurrentTurn >> m_nGameResult >> m_nWhoIsDead;
        fs.seekp(1, ios::cur);

        //最后保存走棋历史记录
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
                >> stMoveRoute.strMoveStepAlpha;
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

    }

    if (m_lstMoveRoute.size())
    {
        m_stCurrentMoveRoute = m_lstMoveRoute.back();
    }
    
    Notify(s_nEventLoadChessMan);
    ResetMoveRoute(m_stCurrentMoveRoute);
}
