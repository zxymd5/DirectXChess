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
 *  File:       NetworkMsgDef.h
 *  Author:     Richard Zou
 *  Created on: 2015-03-01
 */

#ifndef NETWORK_MSG_DEF_H
#define NETWORK_MSG_DEF_H
#include "CommDef.h"

const int MAX_MSG_SIZE = 1024;
const int MSG_GAME_INFO = 1;
const int MSG_NEW_GAME = 2;
const int MSG_CHESSBOARD_SYNC = 3;
const int MSG_FALLBACK = 4;
const int MSG_FALLBACK_REPLY = 5;
const int MSG_TIE = 6;
const int MSG_TIE_REPLY = 7;
const int MSG_LOSE = 8;
const int MSG_LOSE_REPLY = 9;
const int MSG_MOVE_ROUTE = 10;

struct BaseNetworkMsg
{
    int nMsgID;

    BaseNetworkMsg()
    {
        nMsgID = 0;
    }
};

struct MsgGameInfo : public BaseNetworkMsg
{
    int nMySide;
    int nAhead;
    int nStepTime;
    int arrChessman[CHESSBOARD_ROW][CHESSBOARD_COLUMN];

    MsgGameInfo()
    {
        nMsgID = MSG_GAME_INFO;
        nMySide = 0;
        nAhead = 0;
        nStepTime = 0;
        memset(arrChessman, 0, sizeof(int) * CHESSBOARD_ROW * CHESSBOARD_COLUMN);
    }
};

struct MsgNewGame : public BaseNetworkMsg
{
    int arrChessman[CHESSBOARD_ROW][CHESSBOARD_COLUMN];

    MsgNewGame()
    {
        nMsgID = MSG_NEW_GAME;
        memset(arrChessman, 0, sizeof(int) * CHESSBOARD_ROW * CHESSBOARD_COLUMN);
    }

};

struct MsgChessboardSync : public BaseNetworkMsg
{
    int nCurrentTurn; 
    int nGameResult; 
    int nWhoIsDead;
    int arrChessman[CHESSBOARD_ROW][CHESSBOARD_COLUMN];

    MsgChessboardSync()
    {
        nMsgID = MSG_CHESSBOARD_SYNC;
        nCurrentTurn = 0;
        nGameResult = -1;
        nWhoIsDead = 0;
        memset(arrChessman, 0, sizeof(int) * CHESSBOARD_ROW * CHESSBOARD_COLUMN);
    }
};

struct MsgFallback : public BaseNetworkMsg
{
    int nReqSide;

    MsgFallback()
    {
        nMsgID = MSG_FALLBACK;
        nReqSide = 0;
    }
};

struct MsgFallbackReply : public BaseNetworkMsg
{
    int nReqSide;
    int nResult;

    MsgFallbackReply()
    {
        nMsgID = MSG_FALLBACK_REPLY;
        nReqSide = 0;
        nResult = 0;
    }
};

struct MsgTie : public BaseNetworkMsg
{
    int nReqSide;

    MsgTie()
    {
        nMsgID = MSG_TIE;
        nReqSide = 0;
    }
};

struct MsgTieReply : public BaseNetworkMsg
{
    int nReqSide;
    int nResult;

    MsgTieReply()
    {
        nMsgID = MSG_TIE_REPLY;
        nReqSide = 0;
        nResult = 0;
    }
};

struct MsgLose : public BaseNetworkMsg
{
    int nReqSide;

    MsgLose()
    {
        nMsgID = MSG_LOSE;
        nReqSide = 0;
    }
};

struct MsgLoseReply : public BaseNetworkMsg
{
    int nReqSide;
    int nResult;

    MsgLoseReply()
    {
        nMsgID = MSG_LOSE_REPLY;
        nReqSide = 0;
        nResult = 0;
    }
};

struct MsgMoveRoute : public BaseNetworkMsg
{
    int nCurrentTurn; 
    int nGameResult; 
    int nWhoIsDead;
    int arrChessman[CHESSBOARD_ROW][CHESSBOARD_COLUMN];
    MoveRoute stMoveRoute;

    MsgMoveRoute()
    {
        nMsgID = MSG_MOVE_ROUTE;
        nCurrentTurn = 0;
        nGameResult = -1;
        nWhoIsDead = 0;
        memset(arrChessman, 0, sizeof(int) * CHESSBOARD_ROW * CHESSBOARD_COLUMN);
    }
};

#endif