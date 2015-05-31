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
        nMySide = 0;
        nAhead = 0;
        nStepTime = 0;
        memset(arrChessman, 0, sizeof(int) * CHESSBOARD_ROW * CHESSBOARD_COLUMN);
    }
};


#endif