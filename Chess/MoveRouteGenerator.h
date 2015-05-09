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
 *  File:		MoveRouteGenerator.h
 *  Author:		Richard Zou
 *	Created on:	2015-03-01
 */

#ifndef MOVE_ROUTE_GENERATOR_H
#define MOVE_ROUTE_GENERATOR_H

#include "CommDef.h"
#include <string>
#include <list>

using namespace std;

class CMoveRouteGenerator
{
public:
    CMoveRouteGenerator(void);
    ~CMoveRouteGenerator(void);
    bool ValidateMoveRoute(int szChessMan[][CHESSBOARD_COLUMN], int nFromRow, 
                        int nFromColumn, int nToRow, int nToColumn);
    bool ValidateBlackGeneralMoveRoute(int szChessMan[][CHESSBOARD_COLUMN], int nFromRow, 
                                        int nFromColumn, int nToRow, int nToColumn);
    bool ValidateRedGeneralMoveRoute(int szChessMan[][CHESSBOARD_COLUMN], int nFromRow, 
                                    int nFromColumn, int nToRow, int nToColumn);
    bool ValidateBlackAdvisorMoveRoute(int szChessMan[][CHESSBOARD_COLUMN], int nFromRow, 
                                        int nFromColumn, int nToRow, int nToColumn);
    bool ValidateRedAdvisorMoveRoute(int szChessMan[][CHESSBOARD_COLUMN], int nFromRow, 
                                    int nFromColumn, int nToRow, int nToColumn);
    bool ValidateBlackMinisterMoveRoute(int szChessMan[][CHESSBOARD_COLUMN], int nFromRow, 
                                        int nFromColumn, int nToRow, int nToColumn);
    bool ValidateRedMinisterMoveRoute(int szChessMan[][CHESSBOARD_COLUMN], int nFromRow, 
                                        int nFromColumn, int nToRow, int nToColumn);
    bool ValidateBlackSoldierMoveRoute(int szChessMan[][CHESSBOARD_COLUMN], int nFromRow, 
                                        int nFromColumn, int nToRow, int nToColumn);
    bool ValidateRedSoldierMoveRoute(int szChessMan[][CHESSBOARD_COLUMN], int nFromRow, 
                                    int nFromColumn, int nToRow, int nToColumn);
    bool ValidateChariotMoveRoute(int szChessMan[][CHESSBOARD_COLUMN], int nFromRow, 
                                int nFromColumn, int nToRow, int nToColumn);
    bool ValidateHorseMoveRoute(int szChessMan[][CHESSBOARD_COLUMN], int nFromRow, 
                                int nFromColumn, int nToRow, int nToColumn);
    bool ValidateCannonMoveRoute(int szChessMan[][CHESSBOARD_COLUMN], int nFromRow, 
                                int nFromColumn, int nToRow, int nToColumn);
    bool IsAttackGeneral( int szChessMan[][CHESSBOARD_COLUMN], int nGeneral); //是否将军
    bool AttackGeneral(int szChessMan[][CHESSBOARD_COLUMN], int nRow, int nColumn, int nSpecChessMan);

    int GetChessManPosition(int szChessMan[][CHESSBOARD_COLUMN], int nSpecChessMan, ChessManPosition szPos[] );

    //获取着法的数字坐标格式，如k5+1
    void GetChessManMoveStepAlpha(int szChessMan[][CHESSBOARD_COLUMN], int nFromRow, int nFromColumn, int nToRow, int nToColumn, char strAlphaFmt[]);
    void GetBlackGeneralMoveStepAlpha(int szChessMan[][CHESSBOARD_COLUMN], int nFromRow, int nFromColumn, int nToRow, int nToColumn, char strAlphaFmt[]);
    void GetBlackAdvisorMinisterMoveStepAlpha(int szChessMan[][CHESSBOARD_COLUMN], int nFromRow, int nFromColumn, int nToRow, int nToColumn, char strAlphaFmt[]);
    void GetBlackChariotCannonMoveStepAlpha(int szChessMan[][CHESSBOARD_COLUMN], int nFromRow, int nFromColumn, int nToRow, int nToColumn, char strAlphaFmt[], int nChessMan);
    void GetBlackHorseMoveStepAlpha(int szChessMan[][CHESSBOARD_COLUMN], int nFromRow, int nFromColumn, int nToRow, int nToColumn, char strAlphaFmt[]);
    void GetBlackSoldierMoveStepAlpha(int szChessMan[][CHESSBOARD_COLUMN], int nFromRow, int nFromColumn, int nToRow, int nToColumn, char strAlphaFmt[]);
    void GetRedGeneralMoveStepAlpha(int szChessMan[][CHESSBOARD_COLUMN], int nFromRow, int nFromColumn, int nToRow, int nToColumn, char strAlphaFmt[]);
    void GetRedAdvisorMinisterMoveStepAlpha(int szChessMan[][CHESSBOARD_COLUMN], int nFromRow, int nFromColumn, int nToRow, int nToColumn, char strAlphaFmt[]);
    void GetRedChariotCannonMoveStepAlpha(int szChessMan[][CHESSBOARD_COLUMN], int nFromRow, int nFromColumn, int nToRow, int nToColumn, char strAlphaFmt[], int nChessMan);
    void GetRedHorseMoveStepAlpha(int szChessMan[][CHESSBOARD_COLUMN], int nFromRow, int nFromColumn, int nToRow, int nToColumn, char strAlphaFmt[]);
    void GetRedSoldierMoveStepAlpha(int szChessMan[][CHESSBOARD_COLUMN], int nFromRow, int nFromColumn, int nToRow, int nToColumn, char strAlphaFmt[]);

    //将数字坐标格式转换成中文格式如将5进1
    static string DigitToChinese(char chDigit, bool bBlack);
    static string ChessManCodetoChinese(char chCode, bool bBlack);
    static string ActionToChinese(char chAction);
    static void AlphaFmtToChiness(const char *strAlphaFmt, char strChineseFmt[], bool bBlack);

    void GenerateAllMoveRoute(int szChessMan[][CHESSBOARD_COLUMN], int nSide, list<MoveRoute> &lstMoveRoute);
    void GenerateMoveRoute(int szChessMan[][CHESSBOARD_COLUMN], int nRow, int nColumn, list<MoveRoute> &lstMoveRoute);
    bool CanGenerateMove(int szChessMan[][CHESSBOARD_COLUMN], int nRow, int nColumn, int nGeneral);
    bool IsGeneralDead(int szChessMan[][CHESSBOARD_COLUMN], int nSide);
    
};

#endif
