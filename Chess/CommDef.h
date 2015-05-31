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
 *  File:       CommDef.h
 *  Author:     Richard Zou
 *  Created on: 2015-03-01
 */

#ifndef COMM_DEF_H
#define COMM_DEF_H

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <windows.h>
#include <stdio.h>
#include <string>

#define WM_SETTINGS (WM_USER + 0)
#define WM_OPENFILE (WM_USER + 1)
#define WM_SAVEFILE (WM_USER + 2)

static const int WINDOW_START_X = 0;
static const int WINDOW_START_Y = 0;
static const int CHESSBOARD_CELL_START_X = 271;
static const int CHESSBOARD_CELL_START_Y = 112;
static const int CHESSMAN_LEN = 58;
static const int CHESSBOARD_CELL_LEN = 60;
static const int CHESSBOARD_ROW = 10;
static const int CHESSBOARD_COLUMN = 9;
static const int MOVE_STEP_PER_PAGE = 10;

static const char *SETTINGS_FILE = "Config.ini";
static const char *WIDGET_CONFIG_FILE = "Chess.ini";
static const char *PICTURE_FOLDER = "Pictures/";
static const char *BLACK_DEAD_PICTURE = "Pictures/BlackGeneralDead.png";
static const char *RED_DEAD_PICTURE = "Pictures/RedGeneralDead.png";
static const char *BLACK_INDICATOR = "Pictures/BlackIndicator.png";
static const char *RED_INDICATOR = "Pictures/RedIndicator.png";

static const char *AUDIO_NEW_GAME = "Audio/NewGame.wav";
static const char *AUDIO_ATTACK_KING = "Audio/AttackKing.wav";
static const char *AUDIO_CHOOSE = "Audio/Choose.wav";
static const char *AUDIO_EAT = "Audio/Eat.wav";
static const char *AUDIO_LOSS = "Audio/Loss.wav";
static const char *AUDIO_MOVE = "Audio/Move.wav";
static const char *AUDIO_WIN = "Audio/Win.wav";
static const char *AUDIO_TIE = "Audio/Draw.wav";
static const char *AUDIO_ILLEGAL = "Audio/Forbid.wav";

static const int TIE = 0;
static const int BLACK = 1;
static const int RED = 2;
static const int COMPITITOR_MACHINE = 1;
static const int COMPITITOR_HUMAN = 2;
static const int COMPITITOR_NETWORK = 3;
static const int CLIENT_SIDE = 1;
static const int SERVER_SIDE = 2;

static const int BLACK_GENERAL = 1;     //黑将
static const int BLACK_CHARIOT = 2;     //黑车
static const int BLACK_HORSE = 3;       //黑马
static const int BLACK_CANNON = 4;       //黑炮
static const int BLACK_ADVISOR = 5;     //黑士
static const int BLACK_MINISTER = 6;    //黑象
static const int BLACK_SOLDIER = 7;     //黑卒

static const int RED_GENERAL = 8;       //红帅
static const int RED_CHARIOT = 9;       //红车
static const int RED_HORSE = 10;        //红马
static const int RED_CANNON = 11;       //红炮
static const int RED_ADVISOR = 12;      //红仕
static const int RED_MINISTER = 13;     //红相
static const int RED_SOLDIER = 14;      //红兵

static const char *CHESSMAN_CODE = "krncabpKRNCABP"; //将车马炮士象卒帅车马炮仕相兵
static const int MAX_SEARCH_DEPTH = 3;

static const char *ALL_CHESSMAN_NAME[] = 
{
    "BlackGeneral", "BlackChariot", "BlackHorse", "BlackCannon", 
    "BlackAdvisor", "BlackMinister", "BlackSoldier", "RedGeneral", 
    "RedChariot", "RedHorse", "RedCannon", "RedAdvisor", 
    "RedMinister", "RedSoldier"
};

static const int DEFAULT_CHESSBOARD_LAYOUT[CHESSBOARD_ROW][CHESSBOARD_COLUMN] = 
{
    {BLACK_CHARIOT, BLACK_HORSE, BLACK_MINISTER, BLACK_ADVISOR, BLACK_GENERAL, BLACK_ADVISOR, BLACK_MINISTER, BLACK_HORSE, BLACK_CHARIOT},
    {0,             0,           0,              0,             0,             0,             0,              0,           0            },
    {0,             BLACK_CANNON, 0,              0,             0,             0,             0,              BLACK_CANNON, 0            },
    {BLACK_SOLDIER, 0,           BLACK_SOLDIER,  0,             BLACK_SOLDIER, 0,             BLACK_SOLDIER,  0,           BLACK_SOLDIER},
    {0,             0,           0,              0,             0,             0,             0,              0,           0            },

    {0,             0,           0,              0,             0,             0,             0,              0,           0            },
    {RED_SOLDIER,   0,           RED_SOLDIER,    0,             RED_SOLDIER,   0,             RED_SOLDIER,    0,           RED_SOLDIER  },
    {0,             RED_CANNON,  0,              0,             0,             0,             0,              RED_CANNON,  0            },
    {0,             0,           0,              0,             0,             0,             0,              0,           0            },
    {RED_CHARIOT,   RED_HORSE,   RED_MINISTER,   RED_ADVISOR,   RED_GENERAL,   RED_ADVISOR,   RED_MINISTER,   RED_HORSE,   RED_CHARIOT  }

};

static const int CHESSMAN_TYPE_COUNT = 14;    //棋子种类个数

static RECT GetChessManInitPos(int nRow, int nColumn, int nCompetitorSide)
{
    RECT rc;
    nRow = nCompetitorSide == BLACK ? nRow : CHESSBOARD_ROW - nRow - 1;
    nColumn = nCompetitorSide == BLACK ? nColumn : CHESSBOARD_COLUMN - nColumn - 1;
    rc.left = CHESSBOARD_CELL_START_X + nColumn * CHESSBOARD_CELL_LEN - CHESSMAN_LEN / 2;
    rc.top = CHESSBOARD_CELL_START_Y + nRow * CHESSBOARD_CELL_LEN - CHESSMAN_LEN / 2;
    rc.right = rc.left + CHESSMAN_LEN;
    rc.bottom = rc.top + CHESSMAN_LEN;

    return rc;
}

static RECT GetChessManSideInitPos(int nCompetitorSide, int nSide)
{
    RECT rc;

    rc.top = 55;
    rc.bottom = rc.top + CHESSBOARD_CELL_LEN;

    if (nSide == BLACK)
    {
        if (nCompetitorSide == BLACK)
        {
            rc.left = 85;
        }
        else
        {
            rc.left = 885;
        }
    }
    else
    {
        if (nCompetitorSide == BLACK)
        {
            rc.left = 885;
        }
        else
        {
            rc.left = 85;
        }
    }
    
    rc.right = rc.left + CHESSBOARD_CELL_LEN;

    return rc;
}

static RECT GetChessManListViewInitPos(int nCompetitorSide, int nSide)
{
    RECT rc;

    rc.top = 130;
    rc.bottom = rc.top + 440;

    if (nSide == BLACK)
    {
        if (nCompetitorSide == BLACK)
        {
            rc.left = 5;
        }
        else
        {
            rc.left = 815;
        }
    }
    else
    {
        if (nCompetitorSide == BLACK)
        {
            rc.left = 815;
        }
        else
        {
            rc.left = 5;
        }
    }

    rc.right = rc.left + 200;

    return rc;
}

static bool GetCoordinate (const POINT &pt, int &nRow, int &nColumn, int nCompetitorSide)
{
    for (nRow = 0; nRow < CHESSBOARD_ROW; nRow++)
    {
        for (nColumn = 0; nColumn < CHESSBOARD_COLUMN; nColumn++)
        {
            RECT rc = GetChessManInitPos(nRow, nColumn, nCompetitorSide);
            if (PtInRect(&rc, pt))
            {
                return true;
            }
        }
    }

    nRow = -1;
    nColumn = -1;
    return false;
}

static void GetChessManPicture(char szPictureName[], int nChessMan, bool bSelected)
{
    if (!bSelected)
    {
        sprintf(szPictureName, "%s%s.png", PICTURE_FOLDER, ALL_CHESSMAN_NAME[nChessMan - 1]);
    }
    else
    {
        sprintf(szPictureName, "%s%sSel.png", PICTURE_FOLDER, ALL_CHESSMAN_NAME[nChessMan - 1]);
    }
}

struct ChessManPosition
{
    int nRow;
    int nColumn;

    ChessManPosition()
    {
        nRow = -1;
        nColumn = -1;
    }
};

struct MoveRoute
{
    int nKilledChessMan;
    int nMovingChessMan;
    bool bAttackGeneral;
    ChessManPosition stFromPos;
    ChessManPosition stToPos;
    char szMoveStepAlpha[5];
    DWORD dwKey;

    MoveRoute()
    {
        nKilledChessMan = 0;
        nMovingChessMan = 0;
        bAttackGeneral = false;
        memset(szMoveStepAlpha, 0, 5);
        dwKey = 0;
    }
};

struct ChineseMoveStep
{
    int nOrderNumber;
    char szMoveStepInfo[32];
    char szMoveStepTime[32];

    ChineseMoveStep()
    {
        nOrderNumber = 0;
        memset(szMoveStepInfo, 0, 32);
        memset(szMoveStepTime, 0, 32);
    }
};

static bool IsBlackSide(int nChessManType)
{
    return (nChessManType >= BLACK_GENERAL) && (nChessManType <= BLACK_SOLDIER);
}

static bool IsRedSide(int nChessManType)
{
    return (nChessManType >= RED_GENERAL) && (nChessManType <= RED_SOLDIER);
}

static bool IsSameSide(int nSrcChessManType, int nDstChessManType)
{
    return ((IsBlackSide(nSrcChessManType) && IsBlackSide(nDstChessManType))
            || (IsRedSide(nSrcChessManType) && IsRedSide(nDstChessManType)));
}

static bool IsCompleteMoveRoute(const MoveRoute &stRoute)
{
    return (stRoute.nMovingChessMan != 0 &&
        stRoute.stFromPos.nRow != -1 &&
        stRoute.stFromPos.nColumn != -1 &&
        stRoute.stToPos.nRow != -1 &&
        stRoute.stToPos.nColumn != -1);
}

static void CurrentTimeToStr(char *pTime)
{
    SYSTEMTIME sys;
    GetLocalTime( &sys );
    sprintf(pTime, "%02d:%02d:%02d", sys.wHour, sys.wMinute, sys.wSecond);
}

static void ConvertToTimeStr(int nSeconds, char *pSeconds)
{
    int nHour = nSeconds / 3600;
    int nMinute = (nSeconds - nHour * 3600) / 60;
    int nSecond = nSeconds - nHour * 3600 - nMinute * 60;
    sprintf(pSeconds, "%.2d:%.2d:%.2d", nHour, nMinute, nSecond);
}

static const int EVENT_INIT_BOARD = 1;
static const int EVENT_NEW_GAME = 2;
static const int EVENT_UPDATE_MOVE = 3;
static const int EVENT_FALLBACK = 4;
static const int EVENT_LOAD_CHESSMAN = 5;
static const int EVENT_GAME_RESULT = 6;
static const int EVENT_ILLEGAL_MOVE = 7;

#endif