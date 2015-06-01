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
 *  File:       GameHandle.h
 *  Author:     Richard Zou
 *  Created on: 2015-03-01
 */

#ifndef GAME_HANDLE_H
#define GAME_HANDLE_H

#include "NetworkMsgDef.h"
#include "Zobrist.h"
#include "Subject.h"
#include "MoveRouteGenerator.h"
#include "ClientNetwork.h"
#include "ServerNetwork.h"
#include <process.h>

#include <MMSystem.h>
using namespace std;

class CGameHandle : public CSubject
{
public:
    CGameHandle(void);
    virtual ~CGameHandle(void);
    void Init();
    void OnStart();
    void NewGame();

    void Reset();
    void ResetChessManLayout();
    void GetChessMan(int arrChessMan[][CHESSBOARD_COLUMN]);
    void DoMove(int nRow, int nColumn);

    void ApplyCompleteMove();

    void DoMakeMove(MoveRoute &stMoveRoute, bool bRecord = true);

    bool BlackDoMove(int nRow, int nColumn);
    bool RedDoMove(int nRow, int nColumn);
    void ComputerMove();
    const MoveRoute &GetCurrentMoveRoute();
    void ResetMoveRoute(MoveRoute &stRoute);
    void SetGameResult(int nGameResult);
    int GetGameResult() const;
    int GetCurrentTurn() const;
    void SetDeadOne(int nWhoIsDead);
    int GetDeadOne() const;
    list<MoveRoute> &GetLstMoveRoute();
    int GetGeneralPosition(int nChessMan, int &nRow, int &nColumn);
    void OnFallback();
    void UnDoMakeMove();
    void UnDoMakeMove(MoveRoute &stMoveRoute);
    void SaveToFile(const char *pFileName, int nFileType);
    void LoadFromFile(const char *pFileName, int nFileType);
    __int64 GetCurrentStepStartTime();
    void StepTimeOver();
    void OnTie();
    void OnLose();
    void OnWin();
    void ResetZobrist();
    void AddChessMan(int nChessMan, int nRow, int nColumn);
    void DelChessMan(int nChessMan, int nRow, int nColumn);
    int MinMaxSearch(int nDepth, MoveRoute &stRoute);
    int Evaluate();
    bool IsMySide();
    int RepStatus(int nRecur);              //重复局面检测
    int RepValue(int nRepStatus);

    //处理网络上的消息
    void ProcessMessage();
    void ProcessGameInfoMsg(void *pMsg);
    void ProcessNewGameMsg(void *pMsg);
    void ProcessChessboardSyncMsg(void *pMsg);
    void ProcessMoveRouteMsg(void *pMsg);
    void ProcessTipMsg(void *pMsg);
    void ProcessTipReplyMsg(void *pMsg);
    void SendGameInfoMsg();
    void SendNewGameMsg();
    void SendChessboardSyncMsg();
    void SendMoveRouteMsg();
    void SendTipMsg(int nTipType);
    void SendTipReplyMsg(int nTipType, int nResult);
    void SendMsg(char *pMsg, int nMsgSize);
    void Shutdown();
    void EnqueMsg(BaseNetworkMsg *pMsg);
    BaseNetworkMsg *DequeMsg();

    static unsigned int __stdcall SaveGameFunc(void *pParam);
    static unsigned int __stdcall ComputerMove(void *pParam);
    static unsigned int __stdcall RecvMsg(void *pParam);

private:
    int m_nCurrentTurn;
    int m_nCurrentSearchMoveTurn;
    int m_nGameResult;
    int m_nWhoIsDead;
    int m_arrChessMan[CHESSBOARD_ROW][CHESSBOARD_COLUMN];
    list<MoveRoute> m_lstMoveRoute;
    MoveRoute m_stCurrentMoveRoute;
    CMoveRouteGenerator m_clGenerator;
    char m_szGameInfoSaveFile[MAX_PATH];
    HANDLE m_hEventSaveGame;    //写数据库的Event
    HANDLE m_hEventGameSaved;
    HANDLE m_hThreadSaveGame;   //写数据库的线程
    HANDLE m_hThreadComputerMove;   //电脑走棋线程
    HANDLE m_hEventComputerMove;    //电脑走棋Event
    HANDLE m_hThreadNetwork;        //网络消息处理线程
    CRITICAL_SECTION m_csMsgQue;    //用于消息队列
    __int64 m_llCurrentStepStartTime;

    //子力值
    int m_nBlackValue;
    int m_nRedValue;

    //用于重复局面检测
    CRC4    m_clRC4;
    CZobrist m_clCurrentZobrist;
    CZobrist m_clInitZobrist;
    CZobrist m_arrZobristTable[CHESSMAN_TYPE_COUNT][CHESSBOARD_ROW][CHESSBOARD_COLUMN];

    //用于网络对战
    CServerNetwork m_clServer;
    CClientNetwork m_clClient;
    list<BaseNetworkMsg *>m_lstMsgQue;
};

extern CGameHandle g_GameHandle;

#endif
