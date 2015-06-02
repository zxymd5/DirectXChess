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
const int MSG_MOVE_ROUTE = 4;
const int MSG_TIP = 5;
const int MSG_TIP_REPLY = 6;
const int MSG_DISCONNECT = 7;

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

struct MsgTip : public BaseNetworkMsg
{
    int nReqSide;
    int nTipType;

    MsgTip()
    {
        nMsgID = MSG_TIP;
        nReqSide = 0;
        nTipType = 0;
    }
};

struct MsgTipReply : public BaseNetworkMsg
{
    int nReqSide;
    int nResult;
    int nTipType;

    MsgTipReply()
    {
        nMsgID = MSG_TIP_REPLY;
        nReqSide = 0;
        nResult = 0;
        nTipType = 0;
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

struct MsgDisconnect : public BaseNetworkMsg
{
    MsgDisconnect()
    {
        nMsgID = MSG_DISCONNECT;
    }
};

#endif