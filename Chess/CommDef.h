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
 *  File:		CommDef.h
 *  Author:		Richard Zou
 *	Created on:	2015-03-01
 */

#ifndef COMM_DEF_H
#define COMM_DEF_H

#include <windows.h>
#include <stdio.h>
#include <string>

#define WM_SETTINGS (WM_USER + 0)
#define WM_OPENFILE (WM_USER + 1)
#define WM_SAVEFILE (WM_USER + 2)

static const int s_nWindowStartX = 0;
static const int s_nWindowStartY = 0;
static const int s_nChessBoardCellStartX = 271;
static const int s_nChessBoardCellStartY = 112;
static const int s_nChessManLength = 58;
static const int s_nChessBoardCellLength = 60;
static const int s_nChessBoardRow = 10;
static const int s_nChessBoardColumn = 9;
static const int s_nMaxMoveRouteHistory = 100;

static const char *s_pSettingsFile = "Config.ini";
static const char *s_pWidgetConfigFile = "Chess.ini";
static const char *s_pPictureFolder = "Pictures/";
static const char *s_pBlackDeadPicture = "Pictures/BlackGeneralDead.png";
static const char *s_pRedDeadPicture = "Pictures/RedGeneralDead.png";
static const char *s_pBlackIndicator = "Pictures/BlackIndicator.png";
static const char *s_pRedIndicator = "Pictures/RedIndicator.png";

static const char *s_pAudioNewGame = "Audio/NewGame.wav";
static const char *s_pAudioAttackKing = "Audio/AttackKing.wav";
static const char *s_pAudioChoose = "Audio/Choose.wav";
static const char *s_pAudioEat = "Audio/Eat.wav";
static const char *s_pAudioLoss = "Audio/Loss.wav";
static const char *s_pAudioMove = "Audio/Move.wav";
static const char *s_pAudioWin = "Audio/Win.wav";
static const char *s_pAudioTie = "Audio/Draw.wav";

static const int s_nTurnBlack = 1;
static const int s_nTurnRed = 2;
static const int s_nResultTie = 0;
static const int s_nResultBlackWin = 1;
static const int s_nResultRedWin = 2;
static const int s_nBlackDead = 1;
static const int s_nRedDead = 2;
static const int s_nBlackSide = 1;
static const int s_nRedSide = 2;
static const int s_nCompititorMachine = 1;
static const int s_nCompititorHuman = 2;

static const int s_nBlackGeneral = 1;  //黑将
static const int s_nBlackChariot = 2;  //黑车
static const int s_nBlackHorse = 3;    //黑马
static const int s_nBlackCannon = 4;   //黑炮
static const int s_nBlackAdvisor = 5;  //黑士
static const int s_nBlackMinister = 6;     //黑象
static const int s_nBlackSoldier = 7;  //黑卒

static const int s_nRedGeneral = 8;   //红帅
static const int s_nRedChariot = 9;    //红车
static const int s_nRedHorse = 10;      //红马
static const int s_nRedCannon = 11;    //红炮
static const int s_nRedAdvisor = 12;   //红仕
static const int s_nRedMinister = 13;  //红相
static const int s_nRedSoldier = 14;   //红兵

static const char *s_pChessManCode = "krncabpKRNCABP"; //将车马炮士象卒帅车马炮仕相兵

static const char *s_pszChessManName[] = 
{
    "BlackGeneral", "BlackChariot", "BlackHorse", "BlackCannon", 
    "BlackAdvisor", "BlackMinister", "BlackSoldier", "RedGeneral", 
    "RedChariot", "RedHorse", "RedCannon", "RedAdvisor", 
    "RedMinister", "RedSoldier"
};

static const int s_szDefaultChessManLayout[s_nChessBoardRow][s_nChessBoardColumn] = 
{
    {s_nBlackChariot, s_nBlackHorse, s_nBlackMinister, s_nBlackAdvisor, s_nBlackGeneral, s_nBlackAdvisor, s_nBlackMinister, s_nBlackHorse, s_nBlackChariot},
    {0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, s_nBlackCannon, 0, 0, 0, 0, 0, s_nBlackCannon, 0},
    {s_nBlackSoldier, 0, s_nBlackSoldier, 0, s_nBlackSoldier, 0, s_nBlackSoldier, 0, s_nBlackSoldier},
    {0, 0, 0, 0, 0, 0, 0, 0, 0},

    {0, 0, 0, 0, 0, 0, 0, 0, 0},
    {s_nRedSoldier, 0, s_nRedSoldier, 0, s_nRedSoldier, 0, s_nRedSoldier, 0, s_nRedSoldier},
    {0, s_nRedCannon, 0, 0, 0, 0, 0, s_nRedCannon, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0},
    {s_nRedChariot, s_nRedHorse, s_nRedMinister, s_nRedAdvisor, s_nRedGeneral, s_nRedAdvisor, s_nRedMinister, s_nRedHorse, s_nRedChariot}

};

static const int s_nChessManTypeCount = 14;    //棋子种类个数

static RECT GetChessManInitPos(int nRow, int nColumn, int nCompetitorSide)
{
    RECT Pos;
    nRow = nCompetitorSide == s_nBlackSide ? nRow : s_nChessBoardRow - nRow - 1;
    nColumn = nCompetitorSide == s_nBlackSide ? nColumn : s_nChessBoardColumn - nColumn - 1;
    Pos.left = s_nChessBoardCellStartX + nColumn * s_nChessBoardCellLength - s_nChessManLength / 2;
    Pos.top = s_nChessBoardCellStartY + nRow * s_nChessBoardCellLength - s_nChessManLength / 2;
    Pos.right = Pos.left + s_nChessManLength;
    Pos.bottom = Pos.top + s_nChessManLength;

    return Pos;
}

static RECT GetChessManSideInitPos(int nCompetitorSide, int nSide)
{
    RECT Pos;

    Pos.top = 55;
    Pos.bottom = Pos.top + s_nChessBoardCellLength;

    if (nSide == s_nBlackSide)
    {
        if (nCompetitorSide == s_nBlackSide)
        {
            Pos.left = 85;
        }
        else
        {
            Pos.left = 885;
        }
    }
    else
    {
        if (nCompetitorSide == s_nBlackSide)
        {
            Pos.left = 885;
        }
        else
        {
            Pos.left = 85;
        }
    }
    
    Pos.right = Pos.left + s_nChessBoardCellLength;

    return Pos;
}

static RECT GetChessManListViewInitPos(int nCompetitorSide, int nSide)
{
    RECT Pos;

    Pos.top = 130;
    Pos.bottom = Pos.top + 440;

    if (nSide == s_nBlackSide)
    {
        if (nCompetitorSide == s_nBlackSide)
        {
            Pos.left = 5;
        }
        else
        {
            Pos.left = 815;
        }
    }
    else
    {
        if (nCompetitorSide == s_nBlackSide)
        {
            Pos.left = 815;
        }
        else
        {
            Pos.left = 5;
        }
    }

    Pos.right = Pos.left + 200;

    return Pos;
}

static bool GetCoordinate (const POINT &stPt, int &nRow, int &nColumn, int nCompetitorSide)
{
    for (nRow = 0; nRow < s_nChessBoardRow; nRow++)
    {
        for (nColumn = 0; nColumn < s_nChessBoardColumn; nColumn++)
        {
            RECT Rect = GetChessManInitPos(nRow, nColumn, nCompetitorSide);
            if (PtInRect(&Rect, stPt))
            {
                return true;
            }
        }
    }

    nRow = -1;
    nColumn = -1;
    return false;
}

static void GetChessManPicture(char strPictureName[], int nChessMan, bool bSelected)
{
    if (!bSelected)
    {
        sprintf(strPictureName, "%s%s.png", s_pPictureFolder, s_pszChessManName[nChessMan - 1]);
    }
    else
    {
        sprintf(strPictureName, "%s%sSel.png", s_pPictureFolder, s_pszChessManName[nChessMan - 1]);
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
    char strMoveStepAlpha[5];

    MoveRoute()
    {
        nKilledChessMan = 0;
        nMovingChessMan = 0;
        bAttackGeneral = false;
        memset(strMoveStepAlpha, 0, 5);
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
    return (nChessManType >= s_nBlackGeneral) && (nChessManType <= s_nBlackSoldier);
}

static bool IsRedSide(int nChessManType)
{
    return (nChessManType >= s_nRedGeneral) && (nChessManType <= s_nRedSoldier);
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

static void CurrentTimeToStr(char *strTime)
{
    SYSTEMTIME sys;
    GetLocalTime( &sys );
    sprintf(strTime, "%02d:%02d:%02d", sys.wHour, sys.wMinute, sys.wSecond);
}

static void ConvertToTimeStr(int nSeconds, char *pSeconds)
{
    int nHour = nSeconds / 3600;
    int nMinute = (nSeconds - nHour * 3600) / 60;
    int nSecond = nSeconds - nHour * 3600 - nMinute * 60;
    sprintf(pSeconds, "%.2d:%.2d:%.2d", nHour, nMinute, nSecond);
}

static const int s_nEventUpdateChessMan = 1;
static const int s_nEventUpdateMove = 2;
static const int s_nEventFallback = 3;
static const int s_nEventLoadChessMan = 4;
static const int s_nEventGameResult = 5;
static const int s_nMoveStepPerPage = 10;

#endif